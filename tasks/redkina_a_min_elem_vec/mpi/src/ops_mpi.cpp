#include "redkina_a_min_elem_vec/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <algorithm>
#include <vector>

#include "redkina_a_min_elem_vec/common/include/common.hpp"

namespace redkina_a_min_elem_vec {

RedkinaAMinElemVecMPI::RedkinaAMinElemVecMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool RedkinaAMinElemVecMPI::ValidationImpl() {
  return !GetInput().empty();
}

bool RedkinaAMinElemVecMPI::PreProcessingImpl() {
  return true;
}

bool RedkinaAMinElemVecMPI::RunImpl() {
  const auto &vec = GetInput();

  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int n = static_cast<int>(vec.size());
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int size_l = n / size;
  int remainder = n % size;

  int start_idx = 0;
  int end_idx = 0;

  if (rank < remainder) {
    start_idx = rank * (size_l + 1);
    end_idx = start_idx + size_l + 1;
  } else {
    start_idx = (remainder * (size_l + 1)) + ((rank - remainder) * size_l);
    end_idx = start_idx + size_l;
  }

  int min_l = INT_MAX;
  for (int i = start_idx; i < end_idx && i < n; i++) {
    min_l = std::min(min_l, vec[i]);
  }

  if (size_l == 0 && rank >= n) {
    min_l = INT_MAX;
  }

  int min_g = 0;
  MPI_Allreduce(&min_l, &min_g, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

  GetOutput() = min_g;

  return true;
}

bool RedkinaAMinElemVecMPI::PostProcessingImpl() {
  return true;
}

}  // namespace redkina_a_min_elem_vec
