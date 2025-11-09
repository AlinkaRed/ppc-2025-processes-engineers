#include <gtest/gtest.h>
#include <mpi.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <vector>

#include "redkina_a_min_elem_vec/mpi/include/ops_mpi.hpp"
#include "redkina_a_min_elem_vec/seq/include/ops_seq.hpp"
#include "task/include/task.hpp"

namespace redkina_a_min_elem_vec {

static int MinElement(const std::vector<int> &vec) {  // NOLINT
  if (vec.empty()) {
    return 0;
  }
  int min_val = vec[0];
  for (size_t i = 1; i < vec.size(); i++) {
    min_val = std::min(vec[i], min_val);
  }
  return min_val;
}

TEST(redkina_a_min_elem_vec_mpi, test_pipeline_run) {  // NOLINT
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec(1000);
  for (size_t i = 0; i < vec.size(); i++) {
    vec[i] = static_cast<int>(vec.size() - i);
  }

  int result = MinElement(vec);

  std::vector<int> mpi_input = vec;
  RedkinaAMinElemVecMPI mpi_task(mpi_input);
  mpi_task.GetStateOfTesting() = ppc::task::StateOfTesting::kPerf;

  auto start_mpi = std::chrono::high_resolution_clock::now();
  bool mpi_success = mpi_task.Validation() && mpi_task.PreProcessing() && mpi_task.Run() && mpi_task.PostProcessing();
  auto end_mpi = std::chrono::high_resolution_clock::now();
  double mpi_time = std::chrono::duration<double>(end_mpi - start_mpi).count();

  ASSERT_TRUE(mpi_success) << "MPI pipeline failed";
  ASSERT_EQ(mpi_task.GetOutput(), result) << "MPI pipeline result incorrect";

  std::vector<int> seq_input = vec;
  RedkinaAMinElemVecSEQ seq_task(seq_input);
  seq_task.GetStateOfTesting() = ppc::task::StateOfTesting::kPerf;

  auto start_seq = std::chrono::high_resolution_clock::now();
  bool seq_success = seq_task.Validation() && seq_task.PreProcessing() && seq_task.Run() && seq_task.PostProcessing();
  auto end_seq = std::chrono::high_resolution_clock::now();
  double seq_time = std::chrono::duration<double>(end_seq - start_seq).count();

  ASSERT_TRUE(seq_success) << "SEQ pipeline failed";
  ASSERT_EQ(seq_task.GetOutput(), result) << "SEQ pipeline result incorrect";

  if (rank == 0) {
    std::cout << "redkina_a_min_elem_vec_seq_enabled:pipeline:" << seq_time << '\n';
    std::cout << "redkina_a_min_elem_vec_mpi_enabled:pipeline:" << mpi_time << '\n';
  }
}

TEST(redkina_a_min_elem_vec_mpi, test_task_run) {  // NOLINT
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> vec(1000);
  for (size_t i = 0; i < vec.size(); i++) {
    vec[i] = static_cast<int>(vec.size() - i);
  }

  int result = MinElement(vec);

  std::vector<int> mpi_input = vec;
  RedkinaAMinElemVecMPI mpi_task(mpi_input);
  mpi_task.GetStateOfTesting() = ppc::task::StateOfTesting::kPerf;

  ASSERT_TRUE(mpi_task.Validation() && mpi_task.PreProcessing());
  auto start_mpi = std::chrono::high_resolution_clock::now();
  bool mpi_run = mpi_task.Run();
  auto end_mpi = std::chrono::high_resolution_clock::now();
  double mpi_time = std::chrono::duration<double>(end_mpi - start_mpi).count();
  ASSERT_TRUE(mpi_run && mpi_task.PostProcessing());
  ASSERT_EQ(mpi_task.GetOutput(), result) << "MPI task run result incorrect";

  std::vector<int> seq_input = vec;
  RedkinaAMinElemVecSEQ seq_task(seq_input);
  seq_task.GetStateOfTesting() = ppc::task::StateOfTesting::kPerf;

  ASSERT_TRUE(seq_task.Validation() && seq_task.PreProcessing());
  auto start_seq = std::chrono::high_resolution_clock::now();
  bool seq_run = seq_task.Run();
  auto end_seq = std::chrono::high_resolution_clock::now();
  double seq_time = std::chrono::duration<double>(end_seq - start_seq).count();
  ASSERT_TRUE(seq_run && seq_task.PostProcessing());
  ASSERT_EQ(seq_task.GetOutput(), result) << "SEQ task run result incorrect";

  if (rank == 0) {
    std::cout << "redkina_a_min_elem_vec_seq_enabled:task_run:" << seq_time << '\n';
    std::cout << "redkina_a_min_elem_vec_mpi_enabled:task_run:" << mpi_time << '\n';
  }
}

}  // namespace redkina_a_min_elem_vec
