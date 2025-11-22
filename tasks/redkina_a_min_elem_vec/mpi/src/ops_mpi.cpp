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
  return !GetInput().empty();
}

bool RedkinaAMinElemVecMPI::PreProcessingImpl() {
  return true;
}

bool RedkinaAMinElemVecMPI::RunImpl() {
  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int n = 0;
  std::vector<int> local_vec;

  if (rank == 0) {
    const auto &vec = GetInput();
    n = static_cast<int>(vec.size());
  }

  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int size_l = n / size;
  int remainder = n % size;

  int local_size = (rank < remainder) ? size_l + 1 : size_l;

  int displacement = 0;
  if (rank < remainder) {
    displacement = rank * (size_l + 1);
  } else {
    displacement = remainder * (size_l + 1) + (rank - remainder) * size_l;
  }

  local_vec.resize(local_size);

  if (rank == 0) {
    const auto &vec = GetInput();

    std::vector<int> send_counts(size);
    std::vector<int> displacements(size);

    for (int i = 0; i < size; ++i) {
      send_counts[i] = (i < remainder) ? size_l + 1 : size_l;
      if (i < remainder) {
        displacements[i] = i * (size_l + 1);
      } else {
        displacements[i] = remainder * (size_l + 1) + (i - remainder) * size_l;
      }
    }

    MPI_Scatterv(vec.data(), send_counts.data(), displacements.data(), MPI_INT, local_vec.data(), local_size, MPI_INT,
                 0, MPI_COMM_WORLD);
  } else {
    MPI_Scatterv(nullptr, nullptr, nullptr, MPI_INT, local_vec.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);
  }

  int min_l = INT_MAX;
  if (local_size > 0) {
    for (int val : local_vec) {
      min_l = std::min(min_l, val);
    }
  }

  if (local_size == 0) {
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
