#ifndef INCLUDE_MPL_COALESCE__HPP_C1E2DBDF02ECD6C4
#define INCLUDE_MPL_COALESCE__HPP_C1E2DBDF02ECD6C4

#include <type_traits>

namespace ense {
namespace mpl {

template<typename T, typename R>
using coalesce = typename std::conditional<std::is_void<T>::value, R, T>::type;

}
}

#endif /* INCLUDE_MPL_COALESCE__HPP_C1E2DBDF02ECD6C4 */
