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
  if (rank == 0) {
    n = static_cast<int>(GetInput().size());
  }
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int base = n / size;
  int rem = n % size;
  int local_size = (rank < rem) ? base + 1 : base;

  std::vector<int> local_vec(local_size);

  std::vector<int> send_counts;
  std::vector<int> displs;

  if (rank == 0) {
    send_counts.resize(size);
    displs.resize(size);
    for (int i = 0; i < size; i++) {
      send_counts[i] = (i < rem) ? base + 1 : base;
      if (i < rem) {
        displs[i] = i * (base + 1);
      } else {
        displs[i] = (rem * (base + 1)) + ((i - rem) * base);
      }
    }
  }

  if (rank == 0) {
    MPI_Scatterv(GetInput().data(), send_counts.data(), displs.data(), MPI_INT, local_vec.data(), local_size, MPI_INT,
                 0, MPI_COMM_WORLD);
  } else {
    MPI_Scatterv(nullptr, nullptr, nullptr, MPI_INT, local_vec.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);
  }

  int min_l = INT_MAX;
  for (int v : local_vec) {
    min_l = std::min(min_l, v);
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
