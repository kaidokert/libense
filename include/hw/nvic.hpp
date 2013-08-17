#ifndef INCLUDE_HW_NVIC__HPP_6E44CE169E4DF4A3
#define INCLUDE_HW_NVIC__HPP_6E44CE169E4DF4A3

#include <hw/platform_register.hpp>
#include <hw/platform_register_macros.hpp>

#include <type_traits>
#include <mpl/list.hpp>

namespace ense {

enum class ExternalInterrupt { a, b, c, d, x = 179 };

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
		const bitfield_t iabr;
		bitfield_t __iabr_padding [[gnu::unused]];
		volatile uint8_t ipr[496];

		template<size_t Idx>
		struct partition_match {
			template<typename Arg>
			struct apply : std::integral_constant<bool, Arg::value < Idx * 32> {};
		};

		template<typename Acc, typename Arg>
		struct to_bit : std::integral_constant<uint32_t, Acc::value | (1U << (Arg::value & 0x1F))> {
		};

		template<typename List>
		static void assign(bitfield_t& target, std::integral_constant<size_t, 0>)
		{
			typedef typename mpl::partition<partition_match<0>::template apply, List>::right flags;
			typedef mpl::fold_left<
				to_bit,
				std::integral_constant<uint32_t, 0>,
				flags> value;

			if (value::value != 0)
				target[0] = value::value;
		}

		template<typename List, size_t Idx>
		static void assign(bitfield_t& target, std::integral_constant<size_t, Idx>)
		{
			typedef mpl::partition<partition_match<Idx>::template apply, List> parts;
			typedef mpl::fold_left<
				to_bit,
				std::integral_constant<uint32_t, 0>,
				typename parts::right> value;

			assign<typename parts::left>(target, std::integral_constant<size_t, Idx - 1>());

			if (value::value != 0)
				target[Idx] = value::value;
		}

		template<ExternalInterrupt... Interrupts>
		static void assign(bitfield_t& target)
		{
			typedef mpl::list<std::integral_constant<uint32_t, static_cast<uint32_t>(Interrupts)>...> flags;
			assign<flags>(target, std::integral_constant<size_t, 15>());
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

extern NVIC nvic [[gnu::weak, gnu::alias(".SCS_NVIC")]];

}

#include <hw/platform_register_macros_clear.hpp>

#endif /* INCLUDE_HW_NVIC__HPP_6E44CE169E4DF4A3 */
