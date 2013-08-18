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

			static type extend(ConfigurationStructFlight<Parts...>& csf, const next_part& next)
			{
				return std::conditional<
					extend_is_id,
					extend_id,
					extend_add>::type::extend(csf, next);
			}
	};



	template<typename NextPart, typename... Parts>
	auto extend(ConfigurationStructFlight<Parts...>& flight, const NextPart& next)
		-> typename extend_flight<ConfigurationStructFlight<Parts...>, NextPart>::type
	{
		return extend_flight<ConfigurationStructFlight<Parts...>, NextPart>::extend(flight, next);
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
		template<size_t Offset, typename Next> Derived<void>&       extend(Next&)       { return *target(); }
		template<size_t Offset, typename Next> Derived<void> const& extend(Next&) const { return *target(); }

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
