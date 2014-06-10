#ifndef INCLUDE_HW_NVIC__HPP_6E44CE169E4DF4A3
#define INCLUDE_HW_NVIC__HPP_6E44CE169E4DF4A3

#include <hw/platform_register.hpp>
#include <hw/platform_register_macros.hpp>

#include <type_traits>
#include <mpl/list.hpp>

namespace ense {

enum class ExternalInterrupt : uint32_t;

class NVIC {
	private:
		typedef volatile uint32_t bitfield_t[16];

	private:
		bitfield_t iser;
		bitfield_t __iser_padding [[gnu::unused]];
		bitfield_t icer;
		bitfield_t __icer_padding [[gnu::unused]];
		bitfield_t ispr;
		bitfield_t __ispr_padding [[gnu::unused]];
		bitfield_t icpr;
		bitfield_t __icpr_padding [[gnu::unused]];
		bitfield_t iabr;
		bitfield_t __iabr_padding [[gnu::unused]];
		bitfield_t __reserved [[gnu::unused]];
		bitfield_t __reserved_padding [[gnu::unused]];
		volatile uint8_t ipr[496];

		template<uint32_t... Flags>
		static void assign(bitfield_t&, std::integral_constant<uint32_t, std::extent<bitfield_t>::value>)
		{
		}

		template<uint32_t... Flags, uint32_t Idx>
		static void assign(bitfield_t& target, std::integral_constant<uint32_t, Idx>)
		{
			constexpr uint32_t mask = detail::bit::make_mask_fragment(Idx, 8 * sizeof(target[0]), Flags...);

			if (mask)
				target[Idx] = mask;

			assign<Flags...>(target, std::integral_constant<uint32_t, Idx + 1>());
		}

		template<ExternalInterrupt... Interrupts>
		static void assign(bitfield_t& target)
		{
			assign<static_cast<uint32_t>(Interrupts)...>(target, std::integral_constant<uint32_t, 0>());
		}

		static void set_flag(bitfield_t& target, ExternalInterrupt interrupt)
		{
			uint32_t value = static_cast<uint32_t>(interrupt);
			target[value >> 5] = 1U << (value & 0x1F);
		}

	public:
		NVIC& enabled(ExternalInterrupt interrupt, bool enable)
		{
			set_flag(enable ? iser : icer, interrupt);
			return *this;
		}

		bool enabled(ExternalInterrupt interrupt) const
		{
			uint32_t value = static_cast<uint32_t>(interrupt);
			return iser[value >> 5] & (1U << (value & 0x1F));
		}

		NVIC& pending(ExternalInterrupt interrupt, bool pending)
		{
			set_flag(pending ? ispr : icpr, interrupt);
			return *this;
		}

		bool pending(ExternalInterrupt interrupt) const
		{
			uint32_t value = static_cast<uint32_t>(interrupt);
			return ispr[value >> 5] & (1U << (value & 0x1F));
		}

		bool active(ExternalInterrupt interrupt) const
		{
			uint32_t value = static_cast<uint32_t>(interrupt);
			return iabr[value >> 5] & (1U << (value & 0x1F));
		}

		NVIC& priority(ExternalInterrupt interrupt, uint8_t priority)
		{
			ipr[static_cast<uint32_t>(interrupt)] = priority;
			return *this;
		}

		uint8_t priority(ExternalInterrupt interrupt) const
		{
			return ipr[static_cast<uint32_t>(interrupt)];
		}



		template<ExternalInterrupt... Interrupts>
		NVIC& enable()
		{
			assign<Interrupts...>(iser);
			return *this;
		}

		template<ExternalInterrupt... Interrupts>
		NVIC& disable()
		{
			assign<Interrupts...>(icer);
			return *this;
		}

		template<ExternalInterrupt... Interrupts>
		NVIC& set()
		{
			assign<Interrupts...>(ispr);
			return *this;
		}

		template<ExternalInterrupt... Interrupts>
		NVIC& clear()
		{
			assign<Interrupts...>(icpr);
			return *this;
		}
};

static NVIC nvic [[gnu::weakref(".SCS_NVIC")]];

}

#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_NVIC__HPP_6E44CE169E4DF4A3 */
