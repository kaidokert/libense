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

	public:
		RegisterName<false>& commit()
		{
			for (uint32_t i = 0; i < ConfigurationRegister::words; i++)
				_target->word(i, this->word(i));
			return *_target;
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
				result.word(i, this->word(i));
			result._target = static_cast<RegisterName<false>*>(this);
			return result;
		}
};

}

#endif /* INCLUDE_HW_CONFIG__REGISTER__HPP_EF97A831B829FDCC */
