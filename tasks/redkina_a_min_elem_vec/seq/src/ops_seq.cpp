#include "redkina_a_min_elem_vec/seq/include/ops_seq.hpp"

#include <algorithm>
#include <climits>
#include <cstddef>
#include <vector>

#include "redkina_a_min_elem_vec/common/include/common.hpp"

namespace redkina_a_min_elem_vec {

RedkinaAMinElemVecSEQ::RedkinaAMinElemVecSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool RedkinaAMinElemVecSEQ::ValidationImpl() {
  return (GetOutput() == 0);
}

bool RedkinaAMinElemVecSEQ::PreProcessingImpl() {
  return true;
}

bool RedkinaAMinElemVecSEQ::RunImpl() {
  const auto &vec = GetInput();
  if (vec.empty()) {
    GetOutput() = 0;
    return true;
  }

  int min_val = vec[0];
  for (size_t i = 1; i < vec.size(); i++) {
    min_val = std::min(vec[i], min_val);
  }

  GetOutput() = min_val;
  return true;
}

bool RedkinaAMinElemVecSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace redkina_a_min_elem_vec
