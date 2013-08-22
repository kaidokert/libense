#ifndef INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A
#define INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A

#include <hw/config_struct.hpp>
#include <hw/platform_register_macros.hpp>
#include <hw/config_struct_macros.hpp>
#include <mpl/select_memfn.hpp>
#include <tuple>

namespace ense {
namespace platform {
namespace gpio {

enum class Function : uint32_t {
	input     = 0,
	output    = 1,
	alternate = 2,
	analog    = 3
};

template<bool Config = false>
struct MODER : ConfigurationRegister<void, Config, MODER> {
	REGISTER_SINGULAR_ARRAY_RW(Function[16], detail::bit::range<0, 31>, detail::bit::width<2>)
};

static_assert(traits::is_platform_register_valid<MODER<>>(), "");


enum class OutputType : uint32_t {
	push_pull  = 0,
	open_drain = 1
};

template<bool Config = false>
struct OTYPER : ConfigurationRegister<void, Config, OTYPER> {
	REGISTER_SINGULAR_ARRAY_RW(OutputType[16], detail::bit::range<0, 15>, detail::bit::width<1>)
};

static_assert(traits::is_platform_register_valid<OTYPER<>>(), "");


enum class Speed : uint32_t {
	low    = 0,
	medium = 1,
	fast   = 2,
	high   = 3
};

template<bool Config = false>
struct OSPEEDR : ConfigurationRegister<void, Config, OSPEEDR> {
	REGISTER_SINGULAR_ARRAY_RW(Speed[16], detail::bit::range<0, 31>, detail::bit::width<2>)
};

static_assert(traits::is_platform_register_valid<OSPEEDR<>>(), "");


enum class Pull : uint32_t {
	none = 0,
	up   = 1,
	down = 2
};

template<bool Config = false>
struct PUPDR : ConfigurationRegister<void, Config, PUPDR> {
	REGISTER_SINGULAR_ARRAY_RW(Pull[16], detail::bit::range<0, 31>, detail::bit::width<2>)
};

static_assert(traits::is_platform_register_valid<PUPDR<>>(), "");


template<bool Config = false>
struct IDR : ConfigurationRegister<void, Config, IDR> {
	REGISTER_SINGULAR_ARRAY_R(bool[16], detail::bit::range<0, 15>, detail::bit::width<1>)
	REGISTER_FIELD_R(uint16_t, raw, detail::bit::range<0, 15>)
};

static_assert(traits::is_platform_register_valid<IDR<>>(), "");


template<bool Config = false>
struct ODR : ConfigurationRegister<void, Config, ODR> {
	REGISTER_SINGULAR_ARRAY_RW(bool[16], detail::bit::range<0, 15>, detail::bit::width<1>)
	REGISTER_FIELD_RW(uint16_t, raw, detail::bit::range<0, 15>)

	ODR& operator=(const ODR&) = default;

	uint16_t operator=(uint16_t val)
	{
		raw(val);
		return val;
	}

	operator uint16_t() const { return raw(); }

#define OP_EQ(op) \
	uint16_t operator op ## =(uint16_t val) \
	{ \
		auto next = raw() op val; \
		raw(next); \
		return next; \
	}
	OP_EQ(+)
	OP_EQ(-)
	OP_EQ(*)
	OP_EQ(/)
	OP_EQ(%)
	OP_EQ(^)
	OP_EQ(&)
	OP_EQ(|)
	OP_EQ(>>)
	OP_EQ(<<)
#undef OP_EQ
};

static_assert(traits::is_platform_register_valid<ODR<>>(), "");


template<bool Config = false>
struct BSSR : ConfigurationRegister<void, Config, BSSR> {
	REGISTER_ARRAY_C(bool[16], set, detail::bit::range<0, 15>, detail::bit::width<1>)
	REGISTER_ARRAY_C(bool[16], reset, detail::bit::range<16, 31>, detail::bit::width<1>)
};

static_assert(traits::is_platform_register_valid<BSSR<>>(), "");


struct AF {
	const uint32_t af;

	constexpr explicit AF(uint32_t af) : af(af) {}

	explicit operator uint32_t() { return af; }
};

template<bool Config = false>
struct AFR : ConfigurationRegister<void, Config, AFR> {
	REGISTER_SINGULAR_ARRAY_RW(AF[8], detail::bit::range<0, 31>, detail::bit::width<4>)
};

static_assert(traits::is_platform_register_valid<AFR<>>(), "");




namespace detail {

	struct layout {
		MODER<> moder;
		OTYPER<> otyper;
		OSPEEDR<> ospeedr;
		PUPDR<> pupdr;
		IDR<> idr;
		ODR<> odr;
		BSSR<> bssr;
		volatile uint32_t lckr;
		AFR<> afrl;
		AFR<> afrh;
	};

}

template<typename Flight = void>
struct GPIO : ConfigurationStruct<GPIO, detail::layout, Flight> {
	template<typename>
	friend struct GPIO;

	public:
		typedef detail::layout struct_type;
	private:
		typedef GPIO this_type;
		template<typename Next>
		using next_flight = GPIO<Next>;

	public:
		STRUCT_SINGULAR_ARRAY_RW(mode, uint32_t, moder)
		STRUCT_SINGULAR_ARRAY_RW(output_type, uint32_t, otyper)
		STRUCT_SINGULAR_ARRAY_RW(output_speed, uint32_t, ospeedr)
		STRUCT_SINGULAR_ARRAY_RW(pull, uint32_t, pupdr)
		STRUCT_SINGULAR_ARRAY_R(input, uint32_t, idr)
		STRUCT_SINGULAR_ARRAY_RW(output, uint32_t, odr)
		STRUCT_ARRAY_C(set, uint32_t, bssr, set)
		STRUCT_ARRAY_C(reset, uint32_t, bssr, reset)

#define STRUCT_MULTIARRAY_IMPL(name, regtype, getter, setter, SNAME, STYPE, offsets) \
	private: \
		template<size_t... Offsets> \
		struct SNAME { \
			static constexpr bool is_config = !std::is_same<Flight, void>::value; \
			\
			typedef next_flight<typename ense::detail::extend_flight_multipart<Flight, typename regtype::in_flight_type, Offsets...>::type> next_type; \
			typedef typename std::conditional<is_config, next_type, next_flight<void>&>::type extended_type; \
			typedef typename ense::mpl::nth_arg<1, decltype(ense::mpl::select_memfn2(&regtype::setter))>::type arg_type; \
			\
			template<size_t Offset> \
			static regtype& reg_at_offset(this_type& self) \
			{ \
				return *reinterpret_cast<regtype*>(reinterpret_cast<uint8_t*>(self.target()) + Offset); \
			} \
			\
			template<typename T> \
			static extended_type begin_apply(T& self) \
			{ \
				return self.template extend<Offsets...>(reg_at_offset<Offsets>(self)...); \
			} \
			static extended_type begin_apply(extended_type& self) \
			{ \
				return self; \
			} \
			\
			template<uint32_t Idx, uint32_t Mask> \
			static extended_type apply(extended_type& self, arg_type arg, std::integral_constant<uint32_t, Mask>) \
			{ \
				if (Mask & 1) { \
					extended_type next = self.name(Idx, arg); \
					return apply<Idx + 1>(next, arg, std::integral_constant<uint32_t, (Mask >> 1)>()); \
				} else { \
					return apply<Idx + 1>(self, arg, std::integral_constant<uint32_t, (Mask >> 1)>()); \
				} \
			} \
			template<uint32_t Idx> \
			static extended_type apply(extended_type& self, arg_type arg, std::integral_constant<uint32_t, 0>) \
			{ \
				return self.name(Idx, arg); \
			} \
			\
			static auto make_tuple(this_type& self) -> \
				decltype(std::make_tuple(std::ref(reg_at_offset<Offsets>(self))...)) \
			{ \
				return std::make_tuple(std::ref(reg_at_offset<Offsets>(self))...); \
			} \
			\
			template<size_t Idx> \
			using nth_offset = ense::mpl::nth<ense::mpl::list<std::integral_constant<uint32_t, Offsets>...>, Idx>; \
			\
			static constexpr size_t part_extent = regtype::getter ## _extent; \
			static constexpr size_t extent = sizeof...(Offsets) * regtype::getter ## _extent; \
		}; \
		template<uint32_t Element, typename Tuple> \
		auto apply_ ## name ## _step(uint32_t idx, typename STYPE::arg_type arg, const Tuple& targets, std::integral_constant<uint32_t, Element>) -> \
			typename STYPE::extended_type \
		{ \
			if (idx >= Element * STYPE::part_extent) { \
				static constexpr uint32_t offset = Element * STYPE::part_extent; \
				STRUCT_EXTRACT_SPECIFIC(*this, (STYPE::template nth_offset<Element>::value), std::get<Element>(targets)).setter(idx - offset, arg); \
				return *this; \
			} else { \
				return apply_ ## name ## _step(idx, arg, targets, std::integral_constant<uint32_t, Element - 1>()); \
			} \
		} \
		template<typename Tuple> \
		auto apply_ ## name ## _step(uint32_t idx, typename STYPE::arg_type arg, const Tuple& targets, std::integral_constant<uint32_t, 0>) -> \
			typename STYPE::extended_type \
		{ \
			STRUCT_EXTRACT_SPECIFIC(*this, (STYPE::template nth_offset<0>::value), std::get<0>(targets)).setter(idx, arg); \
			return *this; \
		} \
	public: \
		auto name(uint32_t idx, typename STYPE::arg_type arg) -> \
			typename STYPE::extended_type \
		{ \
			typename STYPE::extended_type extended = STYPE::begin_apply(*this); \
			auto targets = STYPE::make_tuple(*this); \
			extended.apply_ ## name ## _step(idx, arg, targets, std::integral_constant<uint32_t, std::tuple_size<decltype(targets)>::value - 1>()); \
			return extended; \
		} \
		auto name ## _mask(uint32_t mask, typename STYPE::arg_type arg) -> \
			typename STYPE::extended_type \
		{ \
			typename STYPE::extended_type extended = STYPE::begin_apply(*this); \
			for (uint32_t i = 0; i < STYPE::extent; i++) { \
				if (mask & (1ULL << i)) { \
					extended.name(i, arg); \
				} \
			} \
			return extended; \
		} \
		template<uint32_t Mask> \
		auto name ## _mask(typename STYPE::arg_type arg) -> \
			typename STYPE::extended_type \
		{ \
			static_assert(Mask <= (1ULL << STYPE::extent), "Mask too large"); \
			typename STYPE::extended_type extended = STYPE::begin_apply(*this); \
			return STYPE::template apply<0>(extended, arg, std::integral_constant<uint32_t, Mask>()); \
		} \
		template<uint32_t Bound1, uint32_t Bound2> \
		auto name ## _range(typename STYPE::arg_type arg) -> \
			typename STYPE::extended_type \
		{ \
			typedef ense::detail::bit::expand<ense::detail::bit::range<Bound1, Bound2>> range; \
			static_assert(range::end < STYPE::extent, "Index out or range"); \
			return name ## _mask<range::field_mask>(arg); \
		}

#define STRUCT_MULTIARRAY_IMPL1(name, regtype, getter, setter, SNAME, offsets) \
		STRUCT_MULTIARRAY_IMPL(name, regtype, getter, setter, SNAME, SNAME<STRUCT_UNPACK offsets>, offsets)
#define STRUCT_MULTIARRAY(name, regtype, getter, setter, offsets) \
	STRUCT_MULTIARRAY_IMPL1(name, regtype, getter, setter, _apply_ ## name ## _impl, offsets)
#define STRUCT_SINGULAR_MULTIARRAY(name, regtype, offsets) \
		STRUCT_MULTIARRAY(name, regtype, get, set, offsets)
		STRUCT_SINGULAR_MULTIARRAY(alternate_function, AFR<>, (STRUCT_OFFSETOF(afrl), STRUCT_OFFSETOF(afrh)))

#define CONFIG_ONE(target) \
		auto configure(uint32_t pin, typename ense::mpl::nth_arg<1, decltype(ense::mpl::select_memfn2(&GPIO::target))>::type arg) \
			-> decltype(this->target(pin, arg)) \
		{ \
			return this->target(pin, arg); \
		} \
		template<uint32_t Mask> \
		auto configure_mask(typename ense::mpl::nth_arg<1, decltype(ense::mpl::select_memfn2(&GPIO::target))>::type arg) \
			-> decltype(this->target ## _mask<Mask>(arg)) \
		{ \
			return this->target ## _mask<Mask>(arg); \
		} \
		template<uint32_t Bound1, uint32_t Bound2> \
		auto configure_range(typename ense::mpl::nth_arg<1, decltype(ense::mpl::select_memfn2(&GPIO::target))>::type arg) \
			-> decltype(this->target ## _range<Bound1, Bound2>(arg)) \
		{ \
			return this->target ## _range<Bound1, Bound2>(arg); \
		}
		CONFIG_ONE(mode)
		CONFIG_ONE(output_type)
		CONFIG_ONE(output_speed)
		CONFIG_ONE(pull)
		CONFIG_ONE(alternate_function)
#undef CONFIG_ONE

		template<typename First, typename... Rest>
		auto configure(uint32_t pin, First first, Rest... rest)
			-> decltype(this->configure(pin, first).configure(pin, rest...))
		{
			return this->configure(pin, first).configure(pin, rest...);
		}
		template<uint32_t Mask, typename First, typename... Rest>
		auto configure_mask(First first, Rest... rest)
			-> decltype(this->configure_mask<Mask>(first).template configure_mask<Mask>(rest...))
		{
			return this->configure_mask<Mask>(first).template configure_mask<Mask>(rest...);
		}
		template<uint32_t Bound1, uint32_t Bound2, typename First, typename... Rest>
		auto configure_range(First first, Rest... rest)
			-> decltype(this->configure_range<Bound1, Bound2>(first).template configure_range<Bound1, Bound2>(rest...))
		{
			return this->configure_range<Bound1, Bound2>(first).template configure_range<Bound1, Bound2>(rest...);
		}

		template<typename = void>
		GPIO& lock(uint16_t bits)
		{
			static_assert(std::is_same<Flight, void>::value, "Cannot lock configuration flights");
			asm volatile (
				"str %1, %0\n"
				"str %2, %0\n"
				"str %1, %0\n"
				"ldr r2, %0"
				: "+m" (this->lckr) : "r"(0x10000 | bits), "r"(bits) : "r2", "memory");
			return *this;
		}

		uint32_t lock() const { return this->lckr; }

		uint16_t in() const { return this->idr.raw(); }

		ODR<>& out() { return this->odr; }
};

extern linker_placed_struct<GPIO> gpioA [[gnu::weak, gnu::alias(".GPIO_PORTA")]];
extern linker_placed_struct<GPIO> gpioB [[gnu::weak, gnu::alias(".GPIO_PORTB")]];
extern linker_placed_struct<GPIO> gpioC [[gnu::weak, gnu::alias(".GPIO_PORTC")]];
extern linker_placed_struct<GPIO> gpioD [[gnu::weak, gnu::alias(".GPIO_PORTD")]];
extern linker_placed_struct<GPIO> gpioE [[gnu::weak, gnu::alias(".GPIO_PORTE")]];
extern linker_placed_struct<GPIO> gpioF [[gnu::weak, gnu::alias(".GPIO_PORTF")]];
extern linker_placed_struct<GPIO> gpioG [[gnu::weak, gnu::alias(".GPIO_PORTG")]];
extern linker_placed_struct<GPIO> gpioH [[gnu::weak, gnu::alias(".GPIO_PORTH")]];
extern linker_placed_struct<GPIO> gpioI [[gnu::weak, gnu::alias(".GPIO_PORTI")]];

}
}
}

#include <hw/platform_register_macros_clear.hpp>
#include <hw/config_struct_macros_clear.hpp>

#endif /* INCLUDE_HW_STM32F4_GPIO__HPP_1F2CE1E9C788264A */
