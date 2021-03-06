#ifndef INCLUDE_HW_CONFIG__STRUCT__HPP_B1B3A97CF51F2814
#define INCLUDE_HW_CONFIG__STRUCT__HPP_B1B3A97CF51F2814

#include <type_traits>
#include <cstddef>

#include <hw/config_register.hpp>
#include <mpl/nth_arg.hpp>
#include <mpl/result_type.hpp>
#include <mpl/select_memfn.hpp>

namespace ense {

namespace detail {

	template<size_t Offset, typename ConfigRegister>
	struct ConfigurationStructFlightPart {
		ConfigRegister _register;
	};

	template<bool Reset, typename... Parts>
	struct ConfigurationStructFlight : Parts... {
		static constexpr bool is_reset_flight = Reset;

		ConfigurationStructFlight() = default;

		ConfigurationStructFlight(int, const Parts&... parts)
			: Parts(parts)...
		{
		}

		template<typename... T> void ignore(T...) {}

		void commit()
		{
			ignore((static_cast<Parts&>(*this)._register.commit(), 1)...);
		}
	};



	template<typename Flight, typename Extension>
	struct extend_flight;

	template<bool Reset, typename... Parts, size_t Offset, typename Register>
	struct extend_flight<
		ConfigurationStructFlight<Reset, Parts...>,
		ConfigurationStructFlightPart<Offset, Register>> {

		private:
			typedef ConfigurationStructFlightPart<Offset, Register> next_part;

			typedef ConfigurationStructFlight<Reset, Parts...> current_flight;
			typedef ConfigurationStructFlight<Reset, Parts..., next_part> extended_flight;

			static constexpr bool extend_is_id = std::is_base_of<next_part, current_flight>::value;

			static current_flight extend(current_flight& csf, const next_part&, std::true_type)
			{
				return csf;
			}

			static extended_flight extend(current_flight& csf, const next_part& next, std::false_type)
			{
				return extended_flight(1, static_cast<Parts&>(csf)..., next);
			}

		public:
			typedef typename std::conditional<extend_is_id, current_flight, extended_flight>::type type;

			static type extend(current_flight& csf, const next_part& next)
			{
				return extend(csf, next, std::integral_constant<bool, extend_is_id>());
			}
	};



	template<bool Reset, typename... Parts, typename NextPart>
	auto extend(ConfigurationStructFlight<Reset, Parts...>& flight, const NextPart& next)
		-> typename extend_flight<ConfigurationStructFlight<Reset, Parts...>, NextPart>::type
	{
		return extend_flight<ConfigurationStructFlight<Reset, Parts...>, NextPart>::extend(flight, next);
	}

}

template<template<typename> class Derived, typename Struct, typename Flight>
class ConfigurationStruct {
	template<template<typename> class, typename, typename>
	friend class ConfigurationStruct;

	private:
		template<size_t FirstOffset, typename NextPart>
		auto extend_step(NextPart&, std::true_type)
			-> Derived<Flight>&
		{
			return static_cast<Derived<Flight>&>(*this);
		}

		template<size_t FirstOffset, typename NextPart>
		auto extend_step(NextPart& next, std::false_type)
			-> Derived<typename detail::extend_flight<Flight, detail::ConfigurationStructFlightPart<FirstOffset, typename NextPart::in_flight_type>>::type>
		{
			auto next_flight = Flight::is_reset_flight ? next.begin_reset() : next.begin();
			auto single_extended = detail::extend(_flight, detail::ConfigurationStructFlightPart<FirstOffset, typename NextPart::in_flight_type>{ next_flight });
			Derived<decltype(single_extended)> result;
			result._target = _target;
			result._flight = single_extended;
			return result;
		}

	protected:
		Derived<void>* _target;
		Flight _flight;

		Derived<void>* target()
		{
			return _target;
		}

		template<size_t FirstOffset, typename NextPart>
		auto extend(NextPart& next)
			-> Derived<typename detail::extend_flight<Flight, detail::ConfigurationStructFlightPart<FirstOffset, typename NextPart::in_flight_type>>::type>
		{
			typedef std::is_base_of<
				detail::ConfigurationStructFlightPart<FirstOffset, typename NextPart::in_flight_type>,
				Flight> already_extended;

			return extend_step<FirstOffset>(next, already_extended());
		}

	public:
		template<size_t Offset, typename Part>
		typename Part::in_flight_type& _load_part(Part&)
		{
			return static_cast<detail::ConfigurationStructFlightPart<Offset, typename Part::in_flight_type>&>(_flight)._register;
		}

		Derived<void>& commit()
		{
			_flight.commit();
			return *_target;
		}
};


template<template<typename> class Derived, typename Struct>
class ConfigurationStruct<Derived, Struct, void> : public Struct {
	protected:
		Derived<void>*       target()       { return static_cast<Derived<void>*>(this); }
		Derived<void> const* target() const { return static_cast<const Derived<void>*>(this); }

		template<size_t Offset, typename NextPart> Derived<void>&       extend(NextPart&)       { return *target(); }
		template<size_t Offset, typename NextPart> Derived<void> const& extend(NextPart&) const { return *target(); }

	public:
		template<size_t Offset, typename Part> Part&       _load_part(Part& part)       { return part; }
		template<size_t Offset, typename Part> Part const& _load_part(Part& part) const { return part; }

		Derived<detail::ConfigurationStructFlight<false>> begin()
		{
			Derived<detail::ConfigurationStructFlight<false>> result;
			result._target = target();
			return result;
		}

		Derived<detail::ConfigurationStructFlight<true>> begin_reset()
		{
			Derived<detail::ConfigurationStructFlight<true>> result;
			result._target = target();
			return result;
		}
};

namespace traits {

	template<template<typename> class Struct>
	constexpr bool is_config_struct_valid()
	{
		static_assert(
			std::is_base_of<
				ense::ConfigurationStruct<Struct, typename Struct<void>::struct_type, void>,
				Struct<void>>::value,
			"Struct must inherit ConfigurationStruct<Struct, _, _> somewhere");

		static_assert(std::is_standard_layout<Struct<void>>::value, "Struct not standard-layout");

		static_assert(sizeof(Struct<void>) == sizeof(typename Struct<void>::struct_type), "Struct contains superfluous fields");

		return true;
	}

}

template<template<typename> class Struct>
using linker_placed_struct = typename std::conditional<traits::is_config_struct_valid<Struct>(), Struct<void>, void>::type;

}

#endif /* INCLUDE_HW_CONFIG__STRUCT__HPP_B1B3A97CF51F2814 */
