#define STRUCT_OFFSETOF(reg) offsetof(struct_type, reg)
#define STRUCT_EXTEND(reg) this->template extend<STRUCT_OFFSETOF(reg)>(this->target()->reg)
#define STRUCT_EXTENDED_TYPE(reg) decltype(STRUCT_EXTEND(reg))
#define STRUCT_EXTRACT_SPECIFIC(extended, offset, reg) ense::detail::extract<offset>(extended, reg)
#define STRUCT_EXTRACT(extended, reg) STRUCT_EXTRACT_SPECIFIC(extended, STRUCT_OFFSETOF(reg), this->target()->reg)

#define STRUCT_UNPACK(...) __VA_ARGS__

#define STRUCT_FORWARD_R(name, reg, name_in_reg, selector, params, args) \
	auto name(STRUCT_UNPACK params) const \
		-> typename ense::mpl::result_type<decltype(ense::mpl::select_memfn ## selector(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type \
	{ \
		STRUCT_EXTENDED_TYPE(reg) extended = STRUCT_EXTEND(reg); \
		return STRUCT_EXTRACT(extended, reg).name_in_reg(STRUCT_UNPACK args); \
	}
#define STRUCT_FORWARD_W(name, reg, name_in_reg, params, args) \
	auto name(STRUCT_UNPACK params) \
		-> STRUCT_EXTENDED_TYPE(reg) \
	{ \
		STRUCT_EXTENDED_TYPE(reg) extended = STRUCT_EXTEND(reg); \
		STRUCT_EXTRACT(extended, reg).name_in_reg(STRUCT_UNPACK args); \
		return extended; \
	}
#define STRUCT_FORWARD_TMASK(name, index_type, reg, name_in_reg, params, args) \
	template<STRUCT_UNPACK index_type Mask> \
	auto name ## _mask(STRUCT_UNPACK params) \
		-> STRUCT_EXTENDED_TYPE(reg) \
	{ \
		STRUCT_EXTENDED_TYPE(reg) extended = STRUCT_EXTEND(reg); \
		STRUCT_EXTRACT(extended, reg).template name_in_reg ## _mask<Mask>(STRUCT_UNPACK args); \
		return extended; \
	}
#define STRUCT_FORWARD_TRANGE(name, index_type, reg, name_in_reg, params, args) \
	template<STRUCT_UNPACK index_type Bound1, STRUCT_UNPACK index_type Bound2> \
	auto name ## _range(STRUCT_UNPACK params) \
		-> STRUCT_EXTENDED_TYPE(reg) \
	{ \
		STRUCT_EXTENDED_TYPE(reg) extended = STRUCT_EXTEND(reg); \
		STRUCT_EXTRACT(extended, reg).template name_in_reg ## _range<Bound1, Bound2>(STRUCT_UNPACK args); \
		return extended; \
	}

#define STRUCT_FORWARD0_R(name, reg, name_in_reg) STRUCT_FORWARD_R(name, reg, name_in_reg, 0c, (), ())
#define STRUCT_FORWARD0_W(name, reg, name_in_reg) STRUCT_FORWARD_W(name, reg, name_in_reg, (), ())
#define STRUCT_FORWARD0_TMASK(name, index_type, reg, name_in_reg) STRUCT_FORWARD_TMASK(name, index_type, reg, name_in_reg, (), ())
#define STRUCT_FORWARD0_TRANGE(name, index_type, reg, name_in_reg) STRUCT_FORWARD_TRANGE(name, index_type, reg, name_in_reg, (), ())

#define STRUCT_FORWARD1_R(name, reg, name_in_reg) \
	STRUCT_FORWARD_R(name, reg, name_in_reg, 1c, \
		(typename ense::mpl::nth_arg<0, decltype(ense::mpl::select_memfn1c(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type arg), \
		(arg))
#define STRUCT_FORWARD1_W(name, reg, name_in_reg) \
	STRUCT_FORWARD_W(name, reg, name_in_reg, \
		(typename ense::mpl::nth_arg<0, decltype(ense::mpl::select_memfn1(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type arg), \
		(arg))
#define STRUCT_FORWARD1_TMASK(name, index_type, reg, name_in_reg) \
	STRUCT_FORWARD_TMASK(name, index_type, reg, name_in_reg, \
		(typename ense::mpl::nth_arg<0, decltype(ense::mpl::select_memfn1(&decltype(std::declval<struct_type>().reg)::name_in_reg ## _mask<Mask>))>::type arg), \
		(arg))
#define STRUCT_FORWARD1_TRANGE(name, index_type, reg, name_in_reg) \
	STRUCT_FORWARD_TRANGE(name, index_type, reg, name_in_reg, \
		(typename ense::mpl::nth_arg<0, decltype(ense::mpl::select_memfn1(&decltype(std::declval<struct_type>().reg)::name_in_reg ## _range<Bound1, Bound2>))>::type arg), \
		(arg))

#define STRUCT_FORWARD2_W(name, reg, name_in_reg) \
	STRUCT_FORWARD_W(name, reg, name_in_reg, \
		(typename ense::mpl::nth_arg<0, decltype(ense::mpl::select_memfn2(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type arg0, \
		 typename ense::mpl::nth_arg<1, decltype(ense::mpl::select_memfn2(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type arg1), \
		(arg0, arg1))

#define STRUCT_BIT_R STRUCT_FORWARD0_R
#define STRUCT_BIT_W STRUCT_FORWARD1_W
#define STRUCT_BIT_C STRUCT_FORWARD0_W
#define STRUCT_BIT_T(name, reg, name_in_reg) STRUCT_FORWARD0_W(toggle_ ## name, reg, toggle_ ## name_in_reg)
#define STRUCT_BIT_RW(name, reg, name_in_reg) \
	STRUCT_BIT_R(name, reg, name_in_reg) \
	STRUCT_BIT_W(name, reg, name_in_reg) \
	STRUCT_BIT_T(name, reg, name_in_reg)

#define STRUCT_FIELD_R STRUCT_FORWARD0_R
#define STRUCT_FIELD_W STRUCT_FORWARD1_W
#define STRUCT_FIELD_RW(name, reg, name_in_reg) \
	STRUCT_FIELD_R(name, reg, name_in_reg) \
	STRUCT_FIELD_W(name, reg, name_in_reg)

#define STRUCT_INT_R STRUCT_FORWARD0_R
#define STRUCT_INT_W STRUCT_FORWARD1_W
#define STRUCT_INT_RW(name, reg, name_in_reg) \
	STRUCT_INT_R(name, reg, name_in_reg) \
	STRUCT_INT_W(name, reg, name_in_reg)

#define STRUCT_ARRAY_R(name, reg, name_in_reg) \
	STRUCT_FORWARD1_R(name, reg, name_in_reg)
#define STRUCT_ARRAY_C(name, reg, name_in_reg) \
	STRUCT_FORWARD1_W(name, reg, name_in_reg) \
	STRUCT_FORWARD_W(name ## _mask, reg, name_in_reg ## _mask, \
		(typename ense::mpl::nth_arg<0, decltype(ense::mpl::select_memfn1(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type arg), \
		(arg)) \
	STRUCT_FORWARD0_TMASK(name, (typename ense::mpl::nth_arg<0, decltype(ense::mpl::select_memfn1(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type), reg, name_in_reg) \
	STRUCT_FORWARD0_TRANGE(name, (typename ense::mpl::nth_arg<0, decltype(ense::mpl::select_memfn1(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type), reg, name_in_reg)
#define STRUCT_ARRAY_W(name, reg, name_in_reg) \
	STRUCT_FORWARD2_W(name, reg, name_in_reg) \
	STRUCT_FORWARD_W(name ## _mask, reg, name_in_reg ## _mask, \
		(typename ense::mpl::nth_arg<0, decltype(ense::mpl::select_memfn2(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type arg0, \
		 typename ense::mpl::nth_arg<1, decltype(ense::mpl::select_memfn2(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type arg1), \
		(arg0, arg1)) \
	STRUCT_FORWARD1_TMASK(name, (typename ense::mpl::nth_arg<0, decltype(ense::mpl::select_memfn2(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type), reg, name_in_reg) \
	STRUCT_FORWARD1_TRANGE(name, (typename ense::mpl::nth_arg<0, decltype(ense::mpl::select_memfn2(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type), reg, name_in_reg)
#define STRUCT_ARRAY_RW(name, reg, name_in_reg) \
	STRUCT_ARRAY_R(name, reg, name_in_reg) \
	STRUCT_ARRAY_W(name, reg, name_in_reg)

#define STRUCT_SINGULAR_ARRAY_R(name, reg) \
	STRUCT_ARRAY_R(name, reg, get)
#define STRUCT_SINGULAR_ARRAY_C(name, reg) \
	STRUCT_ARRAY_C(name, reg, clear)
#define STRUCT_SINGULAR_ARRAY_W(name, reg) \
	STRUCT_ARRAY_W(name, reg, set)
#define STRUCT_SINGULAR_ARRAY_RW(name, reg) \
	STRUCT_SINGULAR_ARRAY_R(name, reg) \
	STRUCT_SINGULAR_ARRAY_W(name, reg)

#define STRUCT_MULTIARRAY_IMPL_CLASS(name, regtype, getter, setter, arg_type_tpl, SNAME, STYPE) \
	private: \
		template<size_t... Offsets> \
		struct SNAME { \
			static constexpr bool is_config = !std::is_same<flight_type, void>::value; \
			\
			typedef this_template<typename ense::detail::extend_flight_multipart<flight_type, typename regtype::in_flight_type, Offsets...>::type> next_type; \
			typedef typename std::conditional<is_config, next_type, this_template<void>&>::type extended_type; \
			typedef STRUCT_UNPACK arg_type_tpl arg_type; \
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
			static extended_type& begin_apply(extended_type& self) \
			{ \
				return self; \
			} \
			\
			template<uint32_t Idx, uint32_t Mask> \
			static void apply(extended_type& self, arg_type arg, std::integral_constant<uint32_t, Mask>) \
			{ \
				if (Mask & 1) { \
					self.name(Idx, arg); \
				} \
				apply<Idx + 1>(self, arg, std::integral_constant<uint32_t, (Mask >> 1)>()); \
			} \
			template<uint32_t Idx> \
			static void apply(extended_type& self, arg_type arg, std::integral_constant<uint32_t, 0>) \
			{ \
				if (Idx < extent) { \
					self.name(Idx, arg); \
				} \
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
		auto get_ ## name ## _step(uint32_t idx, const Tuple& targets, std::integral_constant<uint32_t, Element>) -> \
			typename STYPE::arg_type \
		{ \
			if (idx >= Element * STYPE::part_extent) { \
				static constexpr uint32_t offset = Element * STYPE::part_extent; \
				return STRUCT_EXTRACT_SPECIFIC(*this, (STYPE::template nth_offset<Element>::value), std::get<Element>(targets)).getter(idx - offset); \
			} else { \
				return get_ ## name ## _step(idx, targets, std::integral_constant<uint32_t, Element - 1>()); \
			} \
		} \
		template<typename Tuple> \
		auto get_ ## name ## _step(uint32_t idx, const Tuple& targets, std::integral_constant<uint32_t, 0>) -> \
			typename STYPE::arg_type \
		{ \
			return STRUCT_EXTRACT_SPECIFIC(*this, (STYPE::template nth_offset<0>::value), std::get<0>(targets)).getter(idx); \
		} \
		template<uint32_t Element, typename Tuple> \
		auto apply_ ## name ## _step(uint32_t idx, typename STYPE::arg_type arg, const Tuple& targets, std::integral_constant<uint32_t, Element>) -> \
			typename STYPE::extended_type \
		{ \
			if (idx >= Element * STYPE::part_extent) { \
				static constexpr uint32_t offset = Element * STYPE::part_extent; \
				STRUCT_EXTRACT_SPECIFIC(*this, (STYPE::template nth_offset<Element>::value), std::get<Element>(targets)).setter(idx - offset, arg); \
			} else { \
				apply_ ## name ## _step(idx, arg, targets, std::integral_constant<uint32_t, Element - 1>()); \
			} \
			return *this; \
		} \
		template<typename Tuple> \
		auto apply_ ## name ## _step(uint32_t idx, typename STYPE::arg_type arg, const Tuple& targets, std::integral_constant<uint32_t, 0>) -> \
			typename STYPE::extended_type \
		{ \
			STRUCT_EXTRACT_SPECIFIC(*this, (STYPE::template nth_offset<0>::value), std::get<0>(targets)).setter(idx, arg); \
			return *this; \
		}
#define STRUCT_MULTIARRAY_IMPL_GETTERS(name, STYPE) \
	public: \
		auto name(uint32_t idx) -> \
			decltype(STYPE::begin_apply(*this).get_ ## name ## _step( \
				idx, \
				STYPE::make_tuple(*this), \
				std::integral_constant<uint32_t, std::tuple_size<decltype(STYPE::make_tuple(*this))>::value - 1>())) \
		{ \
			auto&& extended = STYPE::begin_apply(*this); \
			auto targets = STYPE::make_tuple(*this); \
			return extended.get_ ## name ## _step(idx, targets, std::integral_constant<uint32_t, std::tuple_size<decltype(targets)>::value - 1>()); \
		}
#define STRUCT_MULTIARRAY_IMPL_SETTERS(name, STYPE) \
	public: \
		auto name(uint32_t idx, typename STYPE::arg_type arg) -> \
			typename STYPE::extended_type \
		{ \
			auto&& extended = STYPE::begin_apply(*this); \
			auto targets = STYPE::make_tuple(*this); \
			extended.apply_ ## name ## _step(idx, arg, targets, std::integral_constant<uint32_t, std::tuple_size<decltype(targets)>::value - 1>()); \
			return extended; \
		} \
		auto name ## _mask(uint32_t mask, typename STYPE::arg_type arg) -> \
			typename STYPE::extended_type \
		{ \
			auto&& extended = STYPE::begin_apply(*this); \
			for (uint32_t i = 0; i < STYPE::extent; i++) { \
				if (mask & (1ULL << i)) { \
					extended.name(i, arg); \
				} \
			} \
			return extended; \
		} \
	private: \
		template<uint32_t Mask> \
		auto name ## _mask(typename STYPE::arg_type arg, std::true_type) -> \
			typename STYPE::extended_type \
		{ \
			typename STYPE::extended_type extended = STYPE::begin_apply(*this); \
			STYPE::template apply<0>(extended, arg, std::integral_constant<uint32_t, Mask>()); \
			return extended; \
		} \
		template<uint32_t Mask> \
		auto name ## _mask(typename STYPE::arg_type arg, std::false_type) -> \
			typename STYPE::extended_type \
		{ \
			return this->begin() \
				.template name ## _mask<Mask>(arg, std::true_type()) \
				.commit(); \
		} \
	public: \
		template<uint32_t Mask> \
		auto name ## _mask(typename STYPE::arg_type arg) -> \
			typename STYPE::extended_type \
		{ \
			static_assert(Mask <= (1ULL << STYPE::extent), "Mask too large"); \
			return name ## _mask<Mask>(arg, std::integral_constant<bool, STYPE::is_config>()); \
		} \
		template<uint32_t Bound1, uint32_t Bound2> \
		auto name ## _range(typename STYPE::arg_type arg) -> \
			typename STYPE::extended_type \
		{ \
			typedef ense::detail::bit::expand<ense::detail::bit::range<Bound1, Bound2>> range; \
			static_assert(range::end < STYPE::extent, "Index out or range"); \
			return name ## _mask<range::field_mask>(arg); \
		}

#define STRUCT_MULTIARRAY_IMPL1_R(name, regtype, getter, setter, SNAME, ...) \
	STRUCT_MULTIARRAY_IMPL_CLASS(name, \
		regtype, \
		getter, \
		setter, \
		(typename ense::mpl::result_type<decltype(&regtype::getter)>::type), \
		SNAME, \
		SNAME<STRUCT_UNPACK(__VA_ARGS__)>) \
	STRUCT_MULTIARRAY_IMPL_GETTERS(name, SNAME<STRUCT_UNPACK(__VA_ARGS__)>)
#define STRUCT_MULTIARRAY_IMPL1_W(name, regtype, getter, setter, SNAME, ...) \
	STRUCT_MULTIARRAY_IMPL_CLASS(name, \
		regtype, \
		getter, \
		setter, \
		(typename ense::mpl::nth_arg<1, decltype(ense::mpl::select_memfn2(&regtype::setter))>::type), \
		SNAME, \
		SNAME<STRUCT_UNPACK(__VA_ARGS__)>) \
	STRUCT_MULTIARRAY_IMPL_SETTERS(name, SNAME<STRUCT_UNPACK(__VA_ARGS__)>)
#define STRUCT_MULTIARRAY_IMPL1_RW(name, regtype, getter, setter, SNAME, ...) \
	STRUCT_MULTIARRAY_IMPL_CLASS(name, \
		regtype, \
		getter, \
		setter, \
		(typename ense::mpl::nth_arg<1, decltype(ense::mpl::select_memfn2(&regtype::setter))>::type), \
		SNAME, \
		SNAME<STRUCT_UNPACK(__VA_ARGS__)>) \
	STRUCT_MULTIARRAY_IMPL_GETTERS(name, SNAME<STRUCT_UNPACK(__VA_ARGS__)>) \
	STRUCT_MULTIARRAY_IMPL_SETTERS(name, SNAME<STRUCT_UNPACK(__VA_ARGS__)>)
#define STRUCT_MULTIARRAY_R(name, regtype, getter, setter, ...) \
	STRUCT_MULTIARRAY_IMPL1_R(name, regtype, getter, setter, ense_apply_ ## name ## _impl, __VA_ARGS__)
#define STRUCT_MULTIARRAY_W(name, regtype, getter, setter, ...) \
	STRUCT_MULTIARRAY_IMPL1_W(name, regtype, getter, setter, ense_apply_ ## name ## _impl, __VA_ARGS__)
#define STRUCT_MULTIARRAY_RW(name, regtype, getter, setter, ...) \
	STRUCT_MULTIARRAY_IMPL1_RW(name, regtype, getter, setter, ense_apply_ ## name ## _impl, __VA_ARGS__)
#define STRUCT_SINGULAR_MULTIARRAY_R(name, regtype, ...) \
	STRUCT_MULTIARRAY_R(name, regtype, get, set, __VA_ARGS__)
#define STRUCT_SINGULAR_MULTIARRAY_W(name, regtype, ...) \
	STRUCT_MULTIARRAY_W(name, regtype, set, set, __VA_ARGS__)
#define STRUCT_SINGULAR_MULTIARRAY_RW(name, regtype, ...) \
	STRUCT_MULTIARRAY_RW(name, regtype, get, set, __VA_ARGS__)

#define STRUCT_CONFIGURE_SINGLE(target) \
		auto configure(uint32_t idx, typename ense::mpl::nth_arg<1, decltype(ense::mpl::select_memfn2(&this_type::target))>::type arg) \
			-> decltype(this->target(idx, arg)) \
		{ \
			return this->target(idx, arg); \
		} \
		template<uint32_t Mask> \
		auto configure_mask(typename ense::mpl::nth_arg<1, decltype(ense::mpl::select_memfn2(&this_type::target))>::type arg) \
			-> decltype(this->target ## _mask<Mask>(arg)) \
		{ \
			return this->target ## _mask<Mask>(arg); \
		} \
		template<uint32_t Bound1, uint32_t Bound2> \
		auto configure_range(typename ense::mpl::nth_arg<1, decltype(ense::mpl::select_memfn2(&this_type::target))>::type arg) \
			-> decltype(this->target ## _range<Bound1, Bound2>(arg)) \
		{ \
			return this->target ## _range<Bound1, Bound2>(arg); \
		}
#define STRUCT_CONFIGURE_MANY(...) \
		__VA_ARGS__ \
		template<typename First, typename... Rest> \
		auto configure(uint32_t idx, First first, Rest... rest) \
			-> decltype(this->configure(idx, first).configure(idx, rest...)) \
		{ \
			return this->configure(idx, first).configure(idx, rest...); \
		} \
		template<uint32_t Mask, typename First, typename... Rest> \
		auto configure_mask(First first, Rest... rest) \
			-> decltype(this->configure_mask<Mask>(first).template configure_mask<Mask>(rest...)) \
		{ \
			return this->configure_mask<Mask>(first).template configure_mask<Mask>(rest...); \
		} \
		template<uint32_t Bound1, uint32_t Bound2, typename First, typename... Rest> \
		auto configure_range(First first, Rest... rest) \
			-> decltype(this->configure_range<Bound1, Bound2>(first).template configure_range<Bound1, Bound2>(rest...)) \
		{ \
			return this->configure_range<Bound1, Bound2>(first).template configure_range<Bound1, Bound2>(rest...); \
		}
