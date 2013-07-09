#ifndef INCLUDE_HW_CONFIG__REGISTER__HPP_
#define INCLUDE_HW_CONFIG__REGISTER__HPP_

#include <stdint.h>

#define __REGISTERS_NODEF
#include <hw/platform_register.hpp>

namespace ense {

template<typename Bits, bool Config, template<bool> class RegisterName>
class ConfigurationRegister : public WritablePlatformRegister<Bits, RegisterName<true>, uint32_t> {
	friend class ConfigurationRegister<Bits, false, RegisterName>;

	private:
		RegisterName<false>* _target;

	public:
		void commit() {
			_target->value(this->_value);
		}
};

template<typename Bits, template<bool> class RegisterName>
class ConfigurationRegister<Bits, false, RegisterName> : public WritablePlatformRegister<Bits, RegisterName<false>, volatile uint32_t> {
	public:
		RegisterName<true> begin() {
			auto result = RegisterName<true>();
			result._value = this->_value;
			result._target = static_cast<RegisterName<false>*>(this);
			return result;
		}
};

}

#endif /* INCLUDE_HW_CONFIG__REGISTER__HPP_ */
