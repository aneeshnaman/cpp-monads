#ifndef _TUPLE_H_
#define _TUPLE_H_

#include <tuple>

namespace tuple {

template <class T, std::size_t size>
struct TupleDef {
  typedef decltype(std::tuple_cat(typename TupleDef<T, size-1>::type(), std::tuple<T>()))
      type;
};

template <class T>
struct TupleDef<T, 1> {
  typedef std::tuple<T> type;
};

}  // namespace tuple

#endif  // _TUPLE_H_
