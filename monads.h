#include <algorithm>
#include <type_traits>
#include <vector>

namespace monads {

// Returns a list with all the input lists concatenated
template <class T /* element type */>
std::vector<T> concat_all(const std::vector<std::vector<T>>& listList) {
  std::vector<T> result;
  for (const auto& list : listList) {
    std::copy(list.begin(), list.end(),
              std::back_inserter(result));
  }
  return result;
}

// Applies the given function to each element in the list.
// Input function should be of type T -> vector<U>.
template <class T /* input element type*/, class F /* function */ >
auto for_each(const std::vector<T>& list, F f) -> decltype(f(list.front())) {
  using U = typename std::decay<decltype(f(list.front()).front())>::type;
  static_assert(std::is_convertible<
      F, std::function<std::vector<U>(T)>>::value,
      "Input function type not correct");

  std::vector<std::vector<U>> transformed;
  std::transform(list.begin(), list.end(),
                 std::back_inserter(transformed), f);
  return concat_all(transformed);
}

}  // namespace monads
