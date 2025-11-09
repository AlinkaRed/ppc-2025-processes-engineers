#include "redkina_a_min_elem_vec/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <algorithm>
#include <climits>
#include <vector>

#include "redkina_a_min_elem_vec/common/include/common.hpp"

namespace redkina_a_min_elem_vec {

RedkinaAMinElemVecMPI::RedkinaAMinElemVecMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool RedkinaAMinElemVecMPI::ValidationImpl() {
  return (GetOutput() == 0);
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

  if (n == 0) {
    if (rank == 0) {
      GetOutput() = 0;
    }
    return true;
  }

  int local_size = n / size;
  int remainder = n % size;

  int start_idx = 0;
  int end_idx = 0;

  if (rank < remainder) {
    start_idx = rank * (local_size + 1);
    end_idx = start_idx + local_size + 1;
  } else {
    start_idx = (remainder * (local_size + 1)) + ((rank - remainder) * local_size);
    end_idx = start_idx + local_size;
  }

  int local_min = INT_MAX;
  for (int i = start_idx; i < end_idx && i < n; i++) {
    local_min = std::min(vec[i], local_min);
  }

  if (local_size == 0 && rank >= n) {
    local_min = INT_MAX;
  }

  int global_min = 0;
  MPI_Allreduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

  GetOutput() = global_min;

  return true;
}

bool RedkinaAMinElemVecMPI::PostProcessingImpl() {
  return true;
}

}  // namespace redkina_a_min_elem_vec
