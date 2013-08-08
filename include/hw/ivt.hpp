#ifndef INCLUDE_HW_IVT__HPP_28B4CDD0E4A16DF7
#define INCLUDE_HW_IVT__HPP_28B4CDD0E4A16DF7

#include <type_traits>

#include <hw/interrupt.hpp>
#include <hw/nvic.hpp>

#include <mpl/list.hpp>
#include <mpl/max.hpp>

namespace ense {

template<void (*Vector)()>
struct reset_handler {
	static constexpr uint32_t slot = 0;
	typedef std::integral_constant<void (*)(), Vector> vector_t;
};

template<void (*Vector)()>
struct nmi_handler {
	static constexpr uint32_t slot = 1;
	typedef std::integral_constant<void (*)(), Vector> vector_t;
};

template<void (*Vector)()>
struct hard_fault_handler {
	static constexpr uint32_t slot = 2;
	typedef std::integral_constant<void (*)(), Vector> vector_t;
};

template<SystemHandlerName Handler, void (*Vector)()>
struct system_handler {
	static constexpr uint32_t slot = static_cast<uint32_t>(Handler) - 1;
	typedef std::integral_constant<void (*)(), Vector> vector_t;
};

template<ExternalInterrupt Interrupt, void (*Vector)()>
struct external_interrupt {
	static constexpr uint32_t slot = static_cast<uint32_t>(Interrupt) + 15;
	typedef std::integral_constant<void (*)(), Vector> vector_t;
};

namespace detail {

	extern "C" {
		extern void _start();
	}

	template<typename>
	struct ignore {};

	template<uint32_t Slot, typename... Handlers>
	struct select_handler :
		std::conditional<
			Handlers::slot == Slot,
			typename Handlers::vector_t,
			ignore<Handlers>
			>::type... {
	};



	template<typename... Handlers>
	struct definer_mask : std::integral_constant<uint32_t, Handlers::slot>... {};

	template<uint32_t Slot, typename... Handlers>
	struct defines_slot : std::is_base_of<std::integral_constant<uint32_t, Slot>, definer_mask<Handlers...>> {};



	template<typename Range, typename... Handlers>
	struct ivt_handler_list;

	template<uint32_t... Slots, typename... Handlers>
	struct ivt_handler_list<mpl::list<std::integral_constant<uint32_t, Slots>...>, Handlers...> {
		void (*const vectors[sizeof...(Slots)])();

		constexpr ivt_handler_list()
			: vectors{
				std::conditional<
					defines_slot<Slots, Handlers...>::value,
					select_handler<Slots, Handlers...>,
					std::integral_constant<void (*)(), nullptr>
					>::type::value...
			}
		{
		}
	};

	template<typename... Handlers>
	struct interrupt_vector_table : detail::ivt_handler_list<
		mpl::make_range<uint32_t, 0, mpl::max(Handlers::slot...)>,
		Handlers...> {
		constexpr interrupt_vector_table()
		{
		}
	};

}

template<typename... Handlers>
class IVT : std::conditional<
		detail::defines_slot<reset_handler<nullptr>::slot, Handlers...>::value,
		detail::interrupt_vector_table<Handlers...>,
		detail::interrupt_vector_table<Handlers..., reset_handler<detail::_start>>>::type {

	public:
		constexpr IVT()
		{
		}
};

}

#endif /* INCLUDE_HW_IVT__HPP_28B4CDD0E4A16DF7 */
