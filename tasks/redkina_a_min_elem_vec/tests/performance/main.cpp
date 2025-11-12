#include <gtest/gtest.h>
#include <mpi.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <random>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "redkina_a_min_elem_vec/common/include/common.hpp"
#include "redkina_a_min_elem_vec/mpi/include/ops_mpi.hpp"
#include "redkina_a_min_elem_vec/seq/include/ops_seq.hpp"

namespace redkina_a_min_elem_vec {

static int MinElement(const std::vector<int> &vec) {
  if (vec.empty()) {
    return 0;
  }
  int min_val = vec[0];
  for (size_t i = 1; i < vec.size(); ++i) {
    if (vec[i] < min_val) {
      min_val = vec[i];
    }
  }
  return min_val;
}

class RedkinaAMinElemVecPerfTests : public ::testing::TestWithParam<size_t> {
 protected:
  void SetUp() override {
    vec_size = GetParam();
    vec = GenerateRandomVector(vec_size);
  }

  static std::vector<int> GenerateRandomVector(size_t size) {
    std::vector<int> result(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(-1'000'000, 1'000'000);
    for (size_t i = 0; i < size; ++i) {
      result[i] = dis(gen);
    }
    if (size > 0) {
      result[size / 2] = -2000000;
    }
    return result;
  }

  std::vector<int> vec;
  size_t vec_size = 0;
};

static std::string PrintTestParam(const testing::TestParamInfo<size_t> &info) {
  return "size_" + std::to_string(info.param);
}

TEST_P(RedkinaAMinElemVecPerfTests, TestPipelineRun) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int expected = MinElement(vec);

  InType mpi_input = vec;
  RedkinaAMinElemVecMPI mpi_task(mpi_input);
  mpi_task.GetStateOfTesting() = ppc::task::StateOfTesting::kPerf;

  const auto start_mpi = std::chrono::high_resolution_clock::now();
  const bool mpi_success =
      mpi_task.Validation() && mpi_task.PreProcessing() && mpi_task.Run() && mpi_task.PostProcessing();
  const auto end_mpi = std::chrono::high_resolution_clock::now();
  const double mpi_time = std::chrono::duration<double>(end_mpi - start_mpi).count();

  ASSERT_TRUE(mpi_success) << "MPI pipeline failed";
  ASSERT_EQ(mpi_task.GetOutput(), expected) << "MPI pipeline result incorrect";

  InType seq_input = vec;
  RedkinaAMinElemVecSEQ seq_task(seq_input);
  seq_task.GetStateOfTesting() = ppc::task::StateOfTesting::kPerf;

  const auto start_seq = std::chrono::high_resolution_clock::now();
  const bool seq_success =
      seq_task.Validation() && seq_task.PreProcessing() && seq_task.Run() && seq_task.PostProcessing();
  const auto end_seq = std::chrono::high_resolution_clock::now();
  const double seq_time = std::chrono::duration<double>(end_seq - start_seq).count();

  ASSERT_TRUE(seq_success) << "SEQ pipeline failed";
  ASSERT_EQ(seq_task.GetOutput(), expected) << "SEQ pipeline result incorrect";

  if (rank == 0) {
    std::cout << "redkina_a_min_elem_vec_seq_enabled:pipeline:" << seq_time << '\n';
    std::cout << "redkina_a_min_elem_vec_mpi_enabled:pipeline:" << mpi_time << '\n';
  }
}

TEST_P(RedkinaAMinElemVecPerfTests, TestTaskRun) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int expected = MinElement(vec);

  InType mpi_input = vec;
  RedkinaAMinElemVecMPI mpi_task(mpi_input);
  mpi_task.GetStateOfTesting() = ppc::task::StateOfTesting::kPerf;
  ASSERT_TRUE(mpi_task.Validation() && mpi_task.PreProcessing());

  const auto start_mpi = std::chrono::high_resolution_clock::now();
  const bool mpi_run = mpi_task.Run();
  const auto end_mpi = std::chrono::high_resolution_clock::now();
  const double mpi_time = std::chrono::duration<double>(end_mpi - start_mpi).count();

  ASSERT_TRUE(mpi_run && mpi_task.PostProcessing());
  ASSERT_EQ(mpi_task.GetOutput(), expected) << "MPI task run result incorrect";

  InType seq_input = vec;
  RedkinaAMinElemVecSEQ seq_task(seq_input);
  seq_task.GetStateOfTesting() = ppc::task::StateOfTesting::kPerf;
  ASSERT_TRUE(seq_task.Validation() && seq_task.PreProcessing());

  const auto start_seq = std::chrono::high_resolution_clock::now();
  const bool seq_run = seq_task.Run();
  const auto end_seq = std::chrono::high_resolution_clock::now();
  const double seq_time = std::chrono::duration<double>(end_seq - start_seq).count();

  ASSERT_TRUE(seq_run && seq_task.PostProcessing());
  ASSERT_EQ(seq_task.GetOutput(), expected) << "SEQ task run result incorrect";

  if (rank == 0) {
    std::cout << "redkina_a_min_elem_vec_seq_enabled:task_run:" << seq_time << '\n';
    std::cout << "redkina_a_min_elem_vec_mpi_enabled:task_run:" << mpi_time << '\n';
  }
}

const std::vector<size_t> kTestParams = {100000000};

INSTANTIATE_TEST_SUITE_P(redkina_a_min_elem_vec, RedkinaAMinElemVecPerfTests, ::testing::ValuesIn(kTestParams),
                         PrintTestParam);

}  // namespace redkina_a_min_elem_vec
