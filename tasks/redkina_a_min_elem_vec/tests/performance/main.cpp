#include <gtest/gtest.h>

#include <cstddef>
#include <random>
#include <vector>

#include "redkina_a_min_elem_vec/common/include/common.hpp"
#include "redkina_a_min_elem_vec/mpi/include/ops_mpi.hpp"
#include "redkina_a_min_elem_vec/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace redkina_a_min_elem_vec {

class RedkinaAMinElemVecRunPerfTests : public ppc::util::BaseRunPerfTests<InType, OutType> {
  const size_t kCount_ = 100000000;
  InType input_data_{};
  int expected_result_ = 0;

  void SetUp() override {
    input_data_ = GenerateRandomVector(kCount_);
    expected_result_ = -2000;
  }

  static std::vector<int> GenerateRandomVector(size_t size) {
    std::vector<int> result(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(-1000, 1000);

    for (size_t i = 0; i < size; ++i) {
      result[i] = dis(gen);
    }

    std::uniform_int_distribution<size_t> pos_dis(0, size - 1);
    size_t min_pos = pos_dis(gen);
    result[min_pos] = -2000;

    return result;
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return output_data == expected_result_;
  }

  InType GetTestInputData() final {
    return input_data_;
  }
};

TEST_P(RedkinaAMinElemVecRunPerfTests, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks = ppc::util::MakeAllPerfTasks<InType, RedkinaAMinElemVecMPI, RedkinaAMinElemVecSEQ>(
    PPC_SETTINGS_redkina_a_min_elem_vec);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = RedkinaAMinElemVecRunPerfTests::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, RedkinaAMinElemVecRunPerfTests, kGtestValues, kPerfTestName);

}  // namespace redkina_a_min_elem_vec
