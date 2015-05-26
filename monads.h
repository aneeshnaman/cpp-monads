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

template <class T /* output type */, class State>
using PlanResult = std::pair<T, State>;

// A Plan<T> takes a state and returns a T and modified state.
template <class T /* output type */, class State>
using Plan = std::function<PlanResult<T, State>(State)>;

// Runs the given plan with the state
template <class T, class State>
PlanResult<T, State> run_plan(Plan<T, State> plan, State state) {
  return plan(state);
}

// Runs the given plan with the state. Returns only the result.
template <class T, class State>
T eval_plan(Plan<T, State> plan, State state) {
  return plan(state).first;
}

// Binds a Plan to a continuation (a plan creator)
template <class T, class State, class F>
auto mbind(Plan<T, State> plan, F f) -> decltype(f(plan(State()).first)) {
  using U = decltype(f(plan(State()).first)(State()).first);
  static_assert(std::is_convertible<
      F, std::function<Plan<U, State>(T)>>::value,
      "Input function type not correct");
  return [plan, f](State state) {
    auto result = run_plan(plan, state);
    auto new_plan = f(result.first);
    return run_plan(new_plan, result.second);
  };
}

// Like mbind, but continuation doesn't use the result of the first plan.
template <class T, class State, class F>
auto mthen(Plan<T, State> plan, F f) -> decltype(f()) {
  return [plan, f](State state) {
    auto result = run_plan(plan);
    auto new_plan = f();
    return run_plan(new_plan, result.second);
  };
}

template <class T, class State>
Plan<T, State> mreturn(T t) {
  return [t](State state) { return std::make_pair(t, state); };
}

template <class State>
Plan<State, State> get_state() {
  return [](State state) { return std::make_pair(state, state); };
}

template <class State>
Plan<void*, State> set_state(State new_state) {
  return [=](State state) { return std::make_pair(nullptr, new_state); };
}

template <class T>
using ListState = std::vector<T>;

// Plan that selects the first element from the list
template <class T>
PlanResult<T, ListState<T>> select_first(ListState<T> state) {
  auto new_state = ListState<T>(state.begin() + 1, state.end());
  return std::make_pair(*state.begin(), new_state);
}

// Plan that selects first two elements (as pair) from the list
template <class T>
PlanResult<std::pair<T, T>, ListState<T>> select_two(ListState<T> state) {
  return mbind<T, ListState<T>>(select_first<T>, [=](T first) {
    return mbind<T, ListState<T>>(select_first<T>, [=](T second) {
      return mreturn<std::pair<T, T>, ListState<T>>(
          std::make_pair(first, second));
    });
  })(state);
}

}  // namespace monads
