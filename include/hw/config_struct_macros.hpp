#define STRUCT_OFFSETOF(reg) \
	offsetof(typename std::decay<decltype(*this)>::type::struct_type, reg)
#define STRUCT_EXTEND(reg) this->template extend<STRUCT_OFFSETOF(reg)>(this->target()->reg)
#define STRUCT_EXTENDED_TYPE(reg) decltype(STRUCT_EXTEND(reg))
#define STRUCT_EXTRACT(extended, reg) ense::detail::extract<STRUCT_OFFSETOF(reg)>(extended, this->target()->reg)

#define STRUCT_FORWARD0(name, reg, name_in_reg) \
	auto name() \
		-> STRUCT_EXTENDED_TYPE(reg) \
	{ \
		STRUCT_EXTENDED_TYPE(reg) extended = STRUCT_EXTEND(reg); \
		STRUCT_EXTRACT(extended, reg).name_in_reg(); \
		return extended; \
	}
#define STRUCT_FORWARD1(name, reg, name_in_reg) \
	auto name(typename ense::mpl::nth_arg<0, decltype(ense::detail::select_memfn1(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type arg) \
		-> STRUCT_EXTENDED_TYPE(reg) \
	{ \
		STRUCT_EXTENDED_TYPE(reg) extended = STRUCT_EXTEND(reg); \
		STRUCT_EXTRACT(extended, reg).name_in_reg(arg); \
		return extended; \
	}
#define STRUCT_FORWARD1_TMASK(name, index_type, reg, name_in_reg) \
	template<index_type Mask> \
	auto name ## _mask(typename ense::mpl::nth_arg<0, decltype(ense::detail::select_memfn1(&decltype(std::declval<struct_type>().reg)::name_in_reg ## _mask<Mask>))>::type arg) \
		-> STRUCT_EXTENDED_TYPE(reg) \
	{ \
		STRUCT_EXTENDED_TYPE(reg) extended = STRUCT_EXTEND(reg); \
		STRUCT_EXTRACT(extended, reg).template name_in_reg ## _mask<Mask>(arg); \
		return extended; \
	}
#define STRUCT_FORWARD1_TRANGE(name, index_type, reg, name_in_reg) \
	template<index_type Bound1, index_type Bound2> \
	auto name ## _range(typename ense::mpl::nth_arg<0, decltype(ense::detail::select_memfn1(&decltype(std::declval<struct_type>().reg)::name_in_reg ## _range<Bound1, Bound2>))>::type arg) \
		-> STRUCT_EXTENDED_TYPE(reg) \
	{ \
		STRUCT_EXTENDED_TYPE(reg) extended = STRUCT_EXTEND(reg); \
		STRUCT_EXTRACT(extended, reg).template name_in_reg ## _range<Bound1, Bound2>(arg); \
		return extended; \
	}
#define STRUCT_FORWARD2(name, reg, name_in_reg) \
	auto name( \
			typename ense::mpl::nth_arg<0, decltype(ense::detail::select_memfn2(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type arg0, \
			typename ense::mpl::nth_arg<1, decltype(ense::detail::select_memfn2(&decltype(std::declval<struct_type>().reg)::name_in_reg))>::type arg1) \
		-> STRUCT_EXTENDED_TYPE(reg) \
	{ \
		STRUCT_EXTENDED_TYPE(reg) extended = STRUCT_EXTEND(reg); \
		STRUCT_EXTRACT(extended, reg).name_in_reg(arg0, arg1); \
		return extended; \
	}

#define STRUCT_BIT_R STRUCT_FORWARD0
#define STRUCT_BIT_W STRUCT_FORWARD1
#define STRUCT_BIT_C STRUCT_FORWARD0
#define STRUCT_BIT_T(name, reg, name_in_reg) STRUCT_FORWARD0(toggle_ ## name, reg, toggle_ ## name_in_reg)
#define STRUCT_BIT_RW(name, reg, name_in_reg) \
	STRUCT_BIT_R(name, reg, name_in_reg) \
	STRUCT_BIT_W(name, reg, name_in_reg) \
	STRUCT_BIT_T(name, reg, name_in_reg)

#define STRUCT_FIELD_R STRUCT_FORWARD0
#define STRUCT_FIELD_W STRUCT_FORWARD1
#define STRUCT_FIELD_RW(name, reg, name_in_reg) \
	STRUCT_FIELD_R(name, reg, name_in_reg) \
	STRUCT_FIELD_W(name, reg, name_in_reg)

#define STRUCT_INT_R STRUCT_FORWARD0
#define STRUCT_INT_W STRUCT_FORWARD1
#define STRUCT_INT_RW(name, reg, name_in_reg) \
	STRUCT_INT_R(name, reg, name_in_reg) \
	STRUCT_INT_W(name, reg, name_in_reg)

#define STRUCT_ARRAY_R STRUCT_FORWARD1
#define STRUCT_ARRAY_W(name, index_type, reg, name_in_reg) \
	STRUCT_FORWARD2(name, reg, name_in_reg) \
	/* FIXME: find a way to fix this, or drop it altogether // STRUCT_FORWARD2(name ## _mask, reg, name_in_reg ## _mask)*/ \
	STRUCT_FORWARD1_TMASK(name, index_type, reg, name_in_reg) \
	STRUCT_FORWARD1_TRANGE(name, index_type, reg, name_in_reg)
#define STRUCT_ARRAY_RW(name, index_type, reg, name_in_reg) \
	STRUCT_ARRAY_R(name, reg, name_in_reg) \
	STRUCT_ARRAY_W(name, index_type, reg, name_in_reg)
