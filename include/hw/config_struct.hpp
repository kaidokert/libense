#ifndef INCLUDE_HW_CONFIG__STRUCT__HPP_B1B3A97CF51F2814
#define INCLUDE_HW_CONFIG__STRUCT__HPP_B1B3A97CF51F2814

#include <type_traits>
#include <cstddef>

#include <hw/config_register.hpp>
#include <mpl/list.hpp>
#include <mpl/any.hpp>
#include <mpl/nth_arg.hpp>

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

	template<typename NextPart, typename... Parts>
	struct extended_flight {
		static constexpr bool extend_is_id = ense::mpl::any(std::is_same<Parts, NextPart>::value...);

		struct extend_id {
			static ConfigurationStructFlight<Parts...> extend(ConfigurationStructFlight<Parts...>& csf, const NextPart&)
			{
				return csf;
			}
		};

		struct extend_add {
			static ConfigurationStructFlight<Parts..., NextPart> extend(ConfigurationStructFlight<Parts...>& csf, const NextPart& next)
			{
				return ConfigurationStructFlight<Parts..., NextPart>(static_cast<Parts&>(csf)..., next);
			}
		};

		typedef typename std::conditional<
			extend_is_id,
			ConfigurationStructFlight<Parts...>,
			ConfigurationStructFlight<Parts..., NextPart>
			>::type type;

		static type extend(ConfigurationStructFlight<Parts...>& csf, const NextPart& next)
		{
			return std::conditional<
				extend_is_id,
				extend_id,
				extend_add>::type::extend(csf, next);
		}
	};

	template<typename NextPart, typename Flight>
	struct extend_flight_type;
	template<typename NextPart, typename... Parts>
	struct extend_flight_type<NextPart, ConfigurationStructFlight<Parts...>> {
		typedef typename extended_flight<NextPart, Parts...>::type type;
	};

	template<typename NextPart, typename... Parts>
	auto extend(ConfigurationStructFlight<Parts...>& flight, const NextPart& next)
		-> typename extended_flight<NextPart, Parts...>::type
	{
		return extended_flight<NextPart, Parts...>::extend(flight, next);
	}

	template<size_t Offset, typename CS, typename Part>
	auto extract(CS& cs, Part& part)
		-> decltype(cs.template extract<Offset>(part))
	{
		return cs.template extract<Offset>(part);
	}

	template<typename Result, typename Class, typename Arg1>
	constexpr auto select_memfn1(Result (Class::*fn)(Arg1))
		-> Result (Class::*)(Arg1)
	{
		return fn;
	}

	template<typename Result, typename Class, typename Arg1>
	constexpr auto select_memfn1(Result (Class::*fn)(Arg1) const)
		-> Result (Class::*)(Arg1) const
	{
		return fn;
	}

	template<typename Result, typename Class, typename Arg1, typename Arg2>
	constexpr auto select_memfn2(Result (Class::*fn)(Arg1, Arg2))
		-> Result (Class::*)(Arg1, Arg2)
	{
		return fn;
	}

}

template<template<typename> class Derived, typename Struct, typename Flight>
class ConfigurationStruct {
	template<template<typename> class, typename, typename>
	friend class ConfigurationStruct;

	template<size_t Offset, typename CS, typename Part>
	friend auto detail::extract(CS& cs, Part& part) -> decltype(cs.template extract<Offset>(part));

	protected:
		Derived<void>* _target;
		Flight _flight;

		Derived<void>* target()
		{
			return _target;
		}

		template<size_t Offset, typename Next>
		auto extend(Next& next)
			-> Derived<decltype(detail::extend(_flight, detail::ConfigurationStructFlightPart<Offset, typename Next::in_flight_type>{ next.begin() }))>
		{
			auto next_flight = detail::extend(_flight, detail::ConfigurationStructFlightPart<Offset, typename Next::in_flight_type>{ next.begin() });
			Derived<decltype(next_flight)> result;
			result._target = _target;
			result._flight = next_flight;
			return result;
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
		template<size_t Offset, typename Next>
		Derived<void>& extend(Next&)
		{
			return *target();
		}

		Derived<void>* target()
		{
			return static_cast<Derived<void>*>(this);
		}

		template<size_t Offset, typename Part>
		Part& extract(Part& part)
		{
			return part;
		}

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
