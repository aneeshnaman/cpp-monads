#include "monads.h"

namespace constraints {

using State = std::vector<int>;

template <class T>
using PlanResult = std::vector<std::pair<T, State>>;

template <class T>
using Plan = std::function<PlanResult<T>(State)>;

template <class T>
PlanResult<T> run_plan(Plan<T> plan, State state) {
  return plan(state);
}

template <class T>
std::vector<T> eval_plan(Plan<T> plan, State state) {
  std::vector<T> results;
  for (const auto& res_and_state : run_plan(plan, state)) {
    results.push_back(res_and_state.first);
  }
  return results;
}

template <class T, class U, class F>
auto mbind(Plan<T> plan, F f) -> decltype(f(plan(State()).begin()->first)) {
  static_assert(std::is_convertible<
      F, std::function<Plan<U>(T)>>::value, "bad continuation type");
  return [=](State state) {
    PlanResult<T> result = plan(state);
    std::vector<PlanResult<U>> mapped;
    for (auto r : result) {
      Plan<U> next_plan = f(r.first);
      mapped.push_back(run_plan(next_plan, r.second));
    }
    return monads::concat_all(mapped);
  };
}

template <class T, class U, class F>
auto mthen(Plan<T> plan, F f) -> decltype(f()) {
  static_assert(std::is_convertible<
      F, std::function<Plan<U>()>>::value, "bad continuation type");
  return [=](State state) {
    PlanResult<T> result = plan(state);
    std::vector<PlanResult<U>> mapped;
    for (auto r : result) {
      Plan<U> next_plan = f();
      mapped.push_back(run_plan(next_plan, r.second));
    }
    return monads::concat_all(mapped);
  };
}

template <class T>
Plan<T> mreturn(T t) {
  return [t](State state) {
    return PlanResult<T>{std::make_pair(t, state)};
  };
}

template <class T>
Plan<T> mzero() {
  return [](State state) {
    return PlanResult<T>{};
  };
}

Plan<void*> mguard(bool condition) {
  if (condition) {
    return [](State state) {
      return PlanResult<void*>{std::make_pair(nullptr, state)};
    };
  } else {
    return mzero<void*>();
  }
}

template <class T>
std::vector<T> nth_removed(const std::vector<T>& in, int n) {
  if (n < 0 || n >= in.size()) throw "Out of bounds";
  std::vector<T> out = in;
  out.erase(out.begin() + n);
  return out;
}

template <class T>
Plan<T> select_one() {
  return [](State state) {
    PlanResult<T> result;
    for (int i = 0; i < state.size(); ++i) {
      result.push_back(std::make_pair(state[i], nth_removed(state, i)));
    }
    return result;
  };
}

template <class T>
Plan<std::pair<T, T>> pair_with_sum(T total) {
  return mbind<T, std::pair<T, T>>(select_one<T>(), [=](T first) {
    return mbind<T, std::pair<T, T>>(select_one<T>(), [=](T second) {
      return mthen<void*, std::pair<T, T>>(mguard(first + second == total), [=]() {
        return mreturn(std::make_pair(first, second));
      });
    });
  });
}

template <class T>
T sum(std::vector<T> in) {
  T s = 0;
  for (auto t : in) { s += t; }
  return s;
}

template <class T, class F>
Plan<std::vector<T>> mbind_repeat(int n, Plan<T> plan, std::vector<T> list, F f) {
  if (n == 0) return f(list);
  return mbind<T, std::vector<T>>(plan, [=](T selected) {
    std::vector<T> new_list = list;
    new_list.push_back(selected);
    return mbind_repeat(n-1, plan, new_list, f);
  });
}

template <class T>
Plan<std::vector<T>> vector_with_sum(int length, T total) {
  return mbind_repeat(length, select_one<T>(), {}, [=](std::vector<T> selected) {
    return mthen<void*, std::vector<T>>(mguard(sum(selected) == total), [=]() {
      return mreturn(selected);
    });
  });
}

}  // namespace constraints
