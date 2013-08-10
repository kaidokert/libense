#ifndef INCLUDE_HW_CONFIG__REGISTER__HPP_EF97A831B829FDCC
#define INCLUDE_HW_CONFIG__REGISTER__HPP_EF97A831B829FDCC

#include <stdint.h>

#include <hw/platform_register.hpp>

namespace ense {

template<typename Bits, bool Config, template<bool> class RegisterName>
class ConfigurationRegister : public WritablePlatformRegister<Bits, RegisterName<true>, uint32_t> {
	friend class ConfigurationRegister<Bits, false, RegisterName>;

	private:
		RegisterName<false>* _target;

	public:
		RegisterName<false>& commit()
		{
			_target->value(this->_value);
			return *_target;
		}
};

template<typename Bits, template<bool> class RegisterName>
class ConfigurationRegister<Bits, false, RegisterName> : public WritablePlatformRegister<Bits, RegisterName<false>, volatile uint32_t> {
	public:
		typedef RegisterName<true> in_flight_type;

		RegisterName<true> begin()
		{
			auto result = RegisterName<true>();
			result._value = this->_value;
			result._target = static_cast<RegisterName<false>*>(this);
			return result;
		}
};

}

#endif /* INCLUDE_HW_CONFIG__REGISTER__HPP_EF97A831B829FDCC */
