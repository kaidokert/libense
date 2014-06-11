#ifndef INCLUDE_HW_CONFIG__REGISTER__HPP_EF97A831B829FDCC
#define INCLUDE_HW_CONFIG__REGISTER__HPP_EF97A831B829FDCC

#include <stdint.h>

#include <mpl/coalesce.hpp>

#include <hw/platform_register.hpp>

namespace ense {

template<typename Bits, bool Config, template<bool> class RegisterName>
class ConfigurationRegister : public WritablePlatformRegister<RegisterName<true>, mpl::coalesce<Bits, uint32_t>> {
	friend class ConfigurationRegister<Bits, false, RegisterName>;

	private:
		RegisterName<false>* _target;
		mutable unsigned _status[ConfigurationRegister::words];

		enum {
			WORD_LOADED = 1,
			WORD_CHANGED = 2,
		};

	public:
		RegisterName<false>& commit()
		{
			for (uint32_t i = 0; i < ConfigurationRegister::words; i++)
				if (_status[i] == WORD_CHANGED)
					_target->word(i, this->word(i));
			return *_target;
		}

		typename ConfigurationRegister::word_type word(uint32_t idx) const
		{
			if (!_status[idx]) {
				const_cast<ConfigurationRegister*>(this)->_value[idx] = _target->word(idx);
				_status[idx] = WORD_LOADED;
			}
			return this->_value[idx];
		}

		RegisterName<true>& word(uint32_t i, typename ConfigurationRegister::word_type val)
		{
			_status[i] = WORD_CHANGED;
			this->_value[i] = val;
			return static_cast<RegisterName<true>&>(*this);
		}
};

template<typename Bits, template<bool> class RegisterName>
class ConfigurationRegister<Bits, false, RegisterName> : public WritablePlatformRegister<RegisterName<false>, volatile mpl::coalesce<Bits, uint32_t>> {
	public:
		typedef RegisterName<true> in_flight_type;

		RegisterName<true> begin()
		{
			auto result = RegisterName<true>();
			for (uint32_t i = 0; i < ConfigurationRegister::words; i++)
				result._status[i] = 0;
			result._target = static_cast<RegisterName<false>*>(this);
			return result;
		}
};

}

#endif /* INCLUDE_HW_CONFIG__REGISTER__HPP_EF97A831B829FDCC */
