#define ENSE_STRUCT_EXTEND(reg) this->template extend<offsetof(struct_type, reg)>(this->target()->reg)
#define ENSE_STRUCT_EXTRACT(extended, reg) ense::detail::extract<offsetof(struct_type, reg)>(extended, this->target()->reg)
#define ENSE_STRUCT_UNPACK(...) __VA_ARGS__

#define ENSE_STRUCT_FORWARD_R(name, reg, name_in_reg, selector, params, args) \
	auto name(ENSE_STRUCT_UNPACK params) const \
		-> typename ense::mpl::result_type<decltype(ense::mpl::select_memfn ## selector(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type \
	{ \
		auto&& extended = ENSE_STRUCT_EXTEND(reg); \
		return ENSE_STRUCT_EXTRACT(extended, reg).name_in_reg(ENSE_STRUCT_UNPACK args); \
	}
#define ENSE_STRUCT_FORWARD_W(name, reg, name_in_reg, params, args) \
	auto name(ENSE_STRUCT_UNPACK params) \
		-> decltype(ENSE_STRUCT_EXTEND(reg)) \
	{ \
		auto&& extended = ENSE_STRUCT_EXTEND(reg); \
		ENSE_STRUCT_EXTRACT(extended, reg).name_in_reg(ENSE_STRUCT_UNPACK args); \
		return extended; \
	}
#define ENSE_STRUCT_FORWARD_TEMPLATES(name, reg, name_in_reg, params, args) \
	template<uint32_t... Items> \
	auto name ## _list(ENSE_STRUCT_UNPACK params) \
		-> decltype(ENSE_STRUCT_EXTEND(reg)) \
	{ \
		auto&& extended = ENSE_STRUCT_EXTEND(reg); \
		ENSE_STRUCT_EXTRACT(extended, reg).template name_in_reg ## _list<Items...>(ENSE_STRUCT_UNPACK args); \
		return extended; \
	} \
	template<uint32_t Bound1, uint32_t Bound2> \
	auto name ## _range(ENSE_STRUCT_UNPACK params) \
		-> decltype(ENSE_STRUCT_EXTEND(reg)) \
	{ \
		auto&& extended = ENSE_STRUCT_EXTEND(reg); \
		ENSE_STRUCT_EXTRACT(extended, reg).template name_in_reg ## _range<Bound1, Bound2>(ENSE_STRUCT_UNPACK args); \
		return extended; \
	}

#define ENSE_STRUCT_FORWARD1_W(name, reg, name_in_reg) \
	ENSE_STRUCT_FORWARD_W(name, reg, name_in_reg, \
		(typename ense::mpl::nth_arg<0, decltype(ense::mpl::select_memfn1(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type arg), \
		(arg))

#define STRUCT_BIT_R(name, reg, name_in_reg) STRUCT_FIELD_R(name, reg, name_in_reg)
#define STRUCT_BIT_W(name, reg, name_in_reg) STRUCT_FIELD_W(name, reg, name_in_reg)
#define STRUCT_BIT_C(name, reg, name_in_reg) ENSE_STRUCT_FORWARD_W(name, reg, name_in_reg, (), ())
#define STRUCT_BIT_T(name, reg, name_in_reg) ENSE_STRUCT_FORWARD_W(toggle_ ## name, reg, toggle_ ## name_in_reg, (), ())
#define STRUCT_BIT_RW(name, reg, name_in_reg) \
	STRUCT_BIT_R(name, reg, name_in_reg) \
	STRUCT_BIT_W(name, reg, name_in_reg) \
	STRUCT_BIT_T(name, reg, name_in_reg)

#define STRUCT_FIELD_R(name, reg, name_in_reg) ENSE_STRUCT_FORWARD_R(name, reg, name_in_reg, 0c, (), ())
#define STRUCT_FIELD_W(name, reg, name_in_reg) ENSE_STRUCT_FORWARD1_W(name, reg, name_in_reg)
#define STRUCT_FIELD_RW(name, reg, name_in_reg) \
	STRUCT_FIELD_R(name, reg, name_in_reg) \
	STRUCT_FIELD_W(name, reg, name_in_reg)

#define STRUCT_ARRAY_R(name, reg, name_in_reg) \
	ENSE_STRUCT_FORWARD_R(name, reg, name_in_reg, 1c, (uint32_t arg), (arg))
#define STRUCT_ARRAY_C(name, reg, name_in_reg) \
	ENSE_STRUCT_FORWARD1_W(name, reg, name_in_reg) \
	ENSE_STRUCT_FORWARD_W(name ## _list, reg, name_in_reg ## _list, (uint32_t arg), (arg)) \
	ENSE_STRUCT_FORWARD_TEMPLATES(name, reg, name_in_reg, (), ())
#define STRUCT_ARRAY_W(name, reg, name_in_reg) \
	ENSE_STRUCT_FORWARD_W(name, reg, name_in_reg, \
		(uint32_t arg0, \
		 typename ense::mpl::nth_arg<1, decltype(ense::mpl::select_memfn2(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type arg1), \
		(arg0, arg1)) \
	ENSE_STRUCT_FORWARD_TEMPLATES(name, reg, name_in_reg, \
		(typename ense::mpl::nth_arg<0, decltype(ense::mpl::select_memfn2(&decltype(std::declval<struct_type>().reg)::name_in_reg ## _list<>))>::type arg), \
		(arg))
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



#define STRUCT_CONFIGURE_SINGLE(target) \
	[[gnu::always_inline]] \
	auto configure(uint32_t idx, typename ense::mpl::nth_arg<1, decltype(ense::mpl::select_memfn2(&this_type::target))>::type arg) \
		-> decltype(this->target(idx, arg)) \
	{ \
		return this->target(idx, arg); \
	} \
	template<uint32_t... Items> \
	[[gnu::always_inline]] \
	auto configure_list(typename ense::mpl::nth_arg<1, decltype(ense::mpl::select_memfn2(&this_type::target))>::type arg) \
		-> decltype(this->target ## _list<Items...>(arg)) \
	{ \
		return this->target ## _list<Items...>(arg); \
	} \
	template<uint32_t Bound1, uint32_t Bound2> \
	[[gnu::always_inline]] \
	auto configure_range(typename ense::mpl::nth_arg<1, decltype(ense::mpl::select_memfn2(&this_type::target))>::type arg) \
		-> decltype(this->target ## _range<Bound1, Bound2>(arg)) \
	{ \
		return this->target ## _range<Bound1, Bound2>(arg); \
	}
#define STRUCT_CONFIGURE_MANY(...) \
	__VA_ARGS__ \
	template<typename First, typename... Rest> \
	[[gnu::always_inline]] \
	auto configure(uint32_t idx, First first, Rest... rest) \
		-> decltype(this->configure(idx, first).configure(idx, rest...)) \
	{ \
		return this->configure(idx, first).configure(idx, rest...); \
	} \
	template<uint32_t... Items, typename First, typename... Rest> \
	[[gnu::always_inline]] \
	auto configure_list(First first, Rest... rest) \
		-> decltype(this->configure_list<Items...>(first).template configure_list<Items...>(rest...)) \
	{ \
		return this->configure_list<Items...>(first).template configure_list<Items...>(rest...); \
	} \
	template<uint32_t Bound1, uint32_t Bound2, typename First, typename... Rest> \
	[[gnu::always_inline]] \
	auto configure_range(First first, Rest... rest) \
		-> decltype(this->configure_range<Bound1, Bound2>(first).template configure_range<Bound1, Bound2>(rest...)) \
	{ \
		return this->configure_range<Bound1, Bound2>(first).template configure_range<Bound1, Bound2>(rest...); \
	}
