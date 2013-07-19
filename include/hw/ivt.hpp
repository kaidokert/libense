#ifndef INCLUDE_HW_IVT__HPP_
#define INCLUDE_HW_IVT__HPP_

#include <type_traits>

#include <hw/interrupt.hpp>
#include <hw/nvic.hpp>

#include <mpl/list.hpp>
#include <mpl/values.hpp>

namespace ense {

template<void (*Vector)()>
struct reset_handler {
	static constexpr uint32_t slot = 0;
	static constexpr void (*vector)() = Vector;
	typedef std::integral_constant<void (*)(), Vector> vector_t;
};

template<void (*Vector)()>
struct nmi_handler {
	static constexpr uint32_t slot = 1;
	static constexpr void (*vector)() = Vector;
	typedef std::integral_constant<void (*)(), Vector> vector_t;
};

template<void (*Vector)()>
struct hard_fault_handler {
	static constexpr uint32_t slot = 2;
	static constexpr void (*vector)() = Vector;
	typedef std::integral_constant<void (*)(), Vector> vector_t;
};

template<SystemHandlerName Handler, void (*Vector)()>
struct system_handler {
	static constexpr uint32_t slot = static_cast<uint32_t>(Handler) - 1;
	static constexpr void (*vector)() = Vector;
	typedef std::integral_constant<void (*)(), Vector> vector_t;
};

template<ExternalInterrupt Interrupt, void (*Vector)()>
struct external_interrupt {
	static constexpr uint32_t slot = static_cast<uint32_t>(Interrupt) + 15;
	static constexpr void (*vector)() = Vector;
	typedef std::integral_constant<void (*)(), Vector> vector_t;
};

namespace detail {

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
			static_assert(defines_slot<0, Handlers...>::value, "");
			static_assert(defines_slot<1, Handlers...>::value, "");
		}
	};

}

template<typename... Handlers>
class IVT : detail::ivt_handler_list<
		mpl::make_range<uint32_t, 0, mpl::max(Handlers::slot...)>,
		Handlers...> {

	public:
		constexpr IVT()
		{
		}
};

}

#endif /* INCLUDE_HW_IVT__HPP_ */
