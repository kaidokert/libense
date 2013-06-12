#ifndef INCLUDE_HW_CONFIG__REGISTER__HPP_
#define INCLUDE_HW_CONFIG__REGISTER__HPP_

#include <stdint.h>

namespace ense {

template<typename Bits, bool Config, template<bool> class RegisterName>
class ConfigurationRegister {
	friend class ConfigurationRegister<Bits, false, RegisterName>;

	public:
		typedef Bits bits_type;

	private:
		uint32_t _value;
		ConfigurationRegister<Bits, false, RegisterName>* _target;

	public:
		uint32_t value() const { return _value; }
		void value(uint32_t val) { _value = val; }

		bool get(Bits bit) const { return _value & static_cast<uint32_t>(bit); }

		void commit() {
			_target->value(_value);
		}

		template<typename... Flags>
		ConfigurationRegister& set(Flags... flags) {
			_value = ConfigurationRegister<Bits, false, RegisterName>::set_bits(_value, flags...);
			return *this;
		}

		template<typename... Flags>
		ConfigurationRegister& clear(Flags... flags) {
			_value = ConfigurationRegister<Bits, false, RegisterName>::clear_bits(_value, flags...);
			return *this;
		}
};

template<typename Bits, template<bool> class RegisterName>
class ConfigurationRegister<Bits, false, RegisterName> {
	friend class ConfigurationRegister<Bits, true, RegisterName>;

	public:
		typedef Bits bits_type;

	private:
		volatile uint32_t _value;

		static uint32_t set_bits(uint32_t value, Bits bit) {
			return value | static_cast<uint32_t>(bit);
		}
		template<typename... Rest>
		static uint32_t set_bits(uint32_t value, Bits bit, Rest... rest) {
			return set_bits(set_bits(value, bit), rest...);
		}

		static uint32_t clear_bits(uint32_t value, Bits bit) {
			return value & ~static_cast<uint32_t>(bit);
		}
		template<typename... Rest>
		static uint32_t clear_bits(uint32_t value, Bits bit, Rest... rest) {
			return clear_bits(clear_bits(value, bit), rest...);
		}

	public:
		uint32_t value() const { return _value; }
		void value(uint32_t val) { _value = val; }

		bool get(Bits bit) const { return _value & static_cast<uint32_t>(bit); }

		RegisterName<true> begin() {
			auto result = RegisterName<true>();
			result._value = _value;
			result._target = this;
			return result;
		}

		template<typename... Flags>
		ConfigurationRegister& set(Flags... flags) {
			_value = set_bits(_value, flags...);
			return *this;
		}

		template<typename... Flags>
		ConfigurationRegister& clear(Flags... flags) {
			_value = clear_bits(_value, flags...);
			return *this;
		}
};

#define REGISTER_BIT(name) \
		bool name() const { return this->get(this_type::bits_type::name); } \
		this_type& name(bool enable) { \
			if (enable) \
				this->set(this_type::bits_type::name); \
			else \
			   this->clear(this_type::bits_type::name); \
			return *this; \
		}

}

#endif /* INCLUDE_HW_CONFIG__REGISTER__HPP_ */
