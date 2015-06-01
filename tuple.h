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
struct TupleDef<T, 0> {
  typedef std::tuple<> type;
};

///////////////////////////////////////////////////////////////////////////////

template <class T, std::size_t size, std::size_t pos>
class Sum {
 public:
  static T sum(typename TupleDef<T, size>::type list) {
    return std::get<pos>(list) + Sum<T, size, pos+1>::sum(list);
  }
};

template <class T, std::size_t size>
class Sum<T, size, size> {
 public:
  static T sum(typename TupleDef<T, size>::type list) {
    return 0;
  }
};

template <class T, std::size_t size>
T sum(typename TupleDef<T, size>::type list) {
  return Sum<T, size, 0>::sum(list);
}

///////////////////////////////////////////////////////////////////////////////

template <class T, std::size_t size, std::size_t pos>
class Printer {
 public:
  static std::string print(typename TupleDef<T, size>::type list) {
    return std::to_string(std::get<pos>(list)) + ", " + Printer<T, size, pos+1>::print(list);
  }
};

template <class T, std::size_t size>
class Printer<T, size, size> {
 public:
  static std::string print(typename TupleDef<T, size>::type list) {
    return "";
  }
};

template <class T, std::size_t size>
std::string to_string(typename TupleDef<T, size>::type list) {
  return Printer<T, size, 0>::print(list);
}


}  // namespace tuple

#endif  // _TUPLE_H_
