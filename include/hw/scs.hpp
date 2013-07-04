#ifndef INCLUDE_HW_SCS__HPP_
#define INCLUDE_HW_SCS__HPP_

#include <type_traits>

namespace ense {

class InterruptControllerType {
	private:
		volatile uint32_t value;

	public:
		uint32_t lines() const
		{
			uint32_t line_code = value & 0xF;
			return line_code == 0xf ? 496 : 32 * (line_code + 1);
		}
};

static_assert(std::is_standard_layout<InterruptControllerType>::value, "");
static_assert(sizeof(InterruptControllerType) == sizeof(uint32_t), "");

extern InterruptControllerType ict __attribute__((__weak__, __alias__(".SCS_ICTR")));

extern volatile uint32_t actlr __attribute__((__weak__, __alias__(".SCS_ACTLR")));

inline void trigger_interrupt(uint32_t num)
{
	extern volatile uint32_t stir __attribute__((__weak__, __alias__(".SCS_STIR")));
	stir = num;
}

}

#endif /* INCLUDE_HW_SCS__HPP_ */
