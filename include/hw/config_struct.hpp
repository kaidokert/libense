#ifndef INCLUDE_HW_CONFIG__STRUCT__HPP_B1B3A97CF51F2814
#define INCLUDE_HW_CONFIG__STRUCT__HPP_B1B3A97CF51F2814

#include <type_traits>
#include <cstddef>

#include <hw/config_register.hpp>
#include <mpl/list.hpp>
#include <mpl/any.hpp>
#include <mpl/nth_arg.hpp>
#include <mpl/result_type.hpp>
#include <mpl/select_memfn.hpp>

namespace ense {

namespace detail {

	template<size_t Offset, typename ConfigRegister>
	struct ConfigurationStructFlightPart {
		ConfigRegister _register;

		void commit()
		{
			_register.commit();
		}
	};

	template<typename... Parts>
	struct ConfigurationStructFlight : Parts... {
		ConfigurationStructFlight() = default;

		ConfigurationStructFlight(const Parts&... parts)
			: Parts(parts)...
		{
		}

		template<typename Only>
		static void commit_all(Only& only)
		{
			only.commit();
		}

		template<typename First, typename... Rest>
		static void commit_all(First& first, Rest&... rest)
		{
			first.commit();
			commit_all(rest...);
		}

		void commit()
		{
			commit_all(static_cast<Parts&>(*this)...);
		}
	};

	template<>
	struct ConfigurationStructFlight<> {
		void commit()
		{
		}
	};



	template<typename Flight, typename... Extension>
	struct extend_flight;

	template<typename... Parts, size_t... Offsets>
	struct extend_flight<void, ConfigurationStructFlightPart<Offsets, Parts>...> {
		typedef void type;
	};

	template<size_t... Offsets, typename... Registers>
	struct extend_flight<ConfigurationStructFlight<ConfigurationStructFlightPart<Offsets, Registers>...>> {
		typedef ConfigurationStructFlight<ConfigurationStructFlightPart<Offsets, Registers>...> type;

		static type extend(type& csf)
		{
			return csf;
		}
	};

	template<
		typename... Parts,
		size_t Offset, typename Register,
		typename... Rest>
	struct extend_flight<
		ConfigurationStructFlight<Parts...>,
		ConfigurationStructFlightPart<Offset, Register>,
		Rest...> {

		private:
			typedef ConfigurationStructFlightPart<Offset, Register> next_part;

			static constexpr bool extend_is_id = ense::mpl::any(std::is_same<Parts, next_part>::value...);

			struct extend_id {
				static ConfigurationStructFlight<Parts...> extend(ConfigurationStructFlight<Parts...>& csf, const next_part&)
				{
					return csf;
				}
			};

			struct extend_add {
				static ConfigurationStructFlight<Parts..., next_part> extend(ConfigurationStructFlight<Parts...>& csf, const next_part& next)
				{
					return ConfigurationStructFlight<Parts..., next_part>(static_cast<Parts&>(csf)..., next);
				}
			};

			typedef typename std::conditional<
				extend_is_id,
				ConfigurationStructFlight<Parts...>,
				ConfigurationStructFlight<Parts..., next_part>
				>::type single_extended_type;

		public:
			typedef typename extend_flight<single_extended_type, Rest...>::type type;

			static type extend(ConfigurationStructFlight<Parts...>& csf, const next_part& next, const Rest&... rest)
			{
				single_extended_type single = std::conditional<
					extend_is_id,
					extend_id,
					extend_add>::type::extend(csf, next);
				return extend_flight<single_extended_type, Rest...>::extend(single, rest...);
			}
	};

	template<typename Flight, typename Register, size_t... Offsets>
	struct extend_flight_multipart : extend_flight<Flight, ConfigurationStructFlightPart<Offsets, Register>...> {
	};



	template<typename... Parts, typename... NextParts>
	auto extend(ConfigurationStructFlight<Parts...>& flight, const NextParts&... next)
		-> typename extend_flight<ConfigurationStructFlight<Parts...>, NextParts...>::type
	{
		return extend_flight<ConfigurationStructFlight<Parts...>, NextParts...>::extend(flight, next...);
	}

	template<size_t Offset, typename CS, typename Part>
	auto extract(CS& cs, Part& part)
		-> decltype(cs.template extract<Offset>(part))
	{
		return cs.template extract<Offset>(part);
	}

}

template<template<typename> class Derived, typename Struct, typename Flight>
class ConfigurationStruct {
	template<template<typename> class, typename, typename>
	friend class ConfigurationStruct;

	template<size_t Offset, typename CS, typename Part>
	friend auto detail::extract(CS& cs, Part& part) -> decltype(cs.template extract<Offset>(part));

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
			auto single_extended = detail::extend(_flight, detail::ConfigurationStructFlightPart<FirstOffset, typename NextPart::in_flight_type>{ next.begin() });
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
			typedef std::integral_constant<
				bool,
				std::is_base_of<
					detail::ConfigurationStructFlightPart<FirstOffset, typename NextPart::in_flight_type>,
					Flight>::value> already_extended;

			return extend_step<FirstOffset>(next, already_extended());
		}

		template<size_t FirstOffset, size_t... RestOffsets, typename NextPart, typename... RestParts>
		auto extend(NextPart& next, RestParts&... rest)
			-> Derived<typename detail::extend_flight<
					Flight,
					detail::ConfigurationStructFlightPart<FirstOffset, typename NextPart::in_flight_type>,
					detail::ConfigurationStructFlightPart<RestOffsets, typename RestParts::in_flight_type>...>::type>
		{
			return extend<FirstOffset>(next).template extend<RestOffsets...>(rest...);
		}

		template<size_t Offset, typename Part>
		typename Part::in_flight_type& extract(Part&)
		{
			return static_cast<detail::ConfigurationStructFlightPart<Offset, typename Part::in_flight_type>&>(_flight)._register;
		}

	public:
		Derived<void>& commit()
		{
			_flight.commit();
			return *_target;
		}
};


template<template<typename> class Derived, typename Struct>
class ConfigurationStruct<Derived, Struct, void> : public Struct {
	template<size_t Offset, typename CS, typename Part>
	friend auto detail::extract(CS& cs, Part& part) -> decltype(cs.template extract<Offset>(part));

	protected:
		template<size_t... Offsets, typename... NextParts> Derived<void>&       extend(NextParts&...)       { return *target(); }
		template<size_t... Offsets, typename... NextParts> Derived<void> const& extend(NextParts&...) const { return *target(); }

		Derived<void>*       target()       { return static_cast<Derived<void>*>(this); }
		Derived<void> const* target() const { return static_cast<const Derived<void>*>(this); }

		template<size_t Offset, typename Part> Part&       extract(Part& part)       { return part; }
		template<size_t Offset, typename Part> Part const& extract(Part& part) const { return part; }

	public:
		Derived<detail::ConfigurationStructFlight<>> begin()
		{
			Derived<detail::ConfigurationStructFlight<>> result;
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

		static_assert(alignof(Struct<void>) <= sizeof(typename Struct<void>::struct_type), "Register has weird alignment requirements");

		return true;
	}

}

template<template<typename> class Struct>
using linker_placed_struct = typename std::conditional<traits::is_config_struct_valid<Struct>(), Struct<void>, void>::type;

}

#endif /* INCLUDE_HW_CONFIG__STRUCT__HPP_B1B3A97CF51F2814 */
