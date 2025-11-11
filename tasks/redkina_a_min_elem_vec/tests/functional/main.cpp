#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <limits>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "redkina_a_min_elem_vec/common/include/common.hpp"
#include "redkina_a_min_elem_vec/mpi/include/ops_mpi.hpp"
#include "redkina_a_min_elem_vec/seq/include/ops_seq.hpp"
#include "util/include/func_test_util.hpp"
#include "util/include/util.hpp"

namespace redkina_a_min_elem_vec {

using TestType = std::tuple<int, std::vector<int>>;

class RedkinaAMinElemVecFuncTests : public ppc::util::BaseRunFuncTests<InType, OutType, TestType> {
 public:
  static std::string PrintTestParam(const TestType &test_param) {
    return std::to_string(std::get<0>(test_param));
  }

 protected:
  void SetUp() override {
    TestType params = std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());
    test_vector_ = std::get<1>(params);
  }

  bool CheckTestOutputData(OutType &output_data) final {
    if (test_vector_.empty()) {
      return false;
    }

    int expected_result = test_vector_[0];
    for (size_t i = 1; i < test_vector_.size(); i++) {
      if (test_vector_[i] < expected_result) {
        expected_result = test_vector_[i];
      }
    }
    return output_data == expected_result;
  }

  InType GetTestInputData() final {
    return test_vector_;
  }

 private:
  std::vector<int> test_vector_;
};

namespace {

// Functional Tests
TEST_P(RedkinaAMinElemVecFuncTests, FunctionalTests) {
  ExecuteTest(GetParam());
}

// Coverage Tests
TEST_P(RedkinaAMinElemVecFuncTests, CoverageTests) {
  ExecuteTest(GetParam());
}

// Functional Tests Data
const std::array<TestType, 13> kFunctionalTests = {
    // Basic positive numbers
    std::make_tuple(1, std::vector<int>{5, 2, 8, 1, 9, 3}),
    std::make_tuple(2, std::vector<int>{-5, -2, -8, -1, -9, -3}),
    std::make_tuple(3, std::vector<int>{5, -2, 0, -1, 9, -3}),
    
    // Edge cases: single element and identical values
    std::make_tuple(4, std::vector<int>{42}),
    std::make_tuple(5, std::vector<int>{7, 7, 7, 7, 7}),
    std::make_tuple(6, std::vector<int>{5, 2, 5, 1, 2, 1}),
    
    // Large numbers and zeros
    std::make_tuple(7, std::vector<int>{1000, 500, 2000, 100, 3000}),
    std::make_tuple(8, std::vector<int>{10, 0, 20, -5, 0, 15}),
    
    // Min element position variations
    std::make_tuple(9, std::vector<int>{-10, 5, 8, 12, 25}),
    std::make_tuple(10, std::vector<int>{15, 8, 12, 5, -3}),
    std::make_tuple(11, std::vector<int>{15, 8, -5, 12, 10}),
    
    // Large vector and boundary values
    std::make_tuple(12, []() {
      std::vector<int> vec(1000);
      for (size_t i = 0; i < vec.size(); i++) {
        vec[i] = static_cast<int>(vec.size() - i);
      }
      return vec;
    }()),
    std::make_tuple(13, std::vector<int>{
        std::numeric_limits<int>::max(),
        std::numeric_limits<int>::min(),
        0, -100, 100
    })
};

// Coverage Tests Data
const std::array<TestType, 22> kCoverageTests = {
    // Basic sequences
    std::make_tuple(14, std::vector<int>{1, 2, 3}),
    std::make_tuple(15, std::vector<int>{42}),
    
    // Min element at different positions
    std::make_tuple(16, std::vector<int>{1, 5, 8, 2, 9}),
    std::make_tuple(17, std::vector<int>{5, 8, 1, 9, 2}),
    std::make_tuple(18, std::vector<int>{5, 8, 9, 2, 1}),
    
    // Negative numbers
    std::make_tuple(19, std::vector<int>{-3, -1, -5, -2}),
    std::make_tuple(20, std::vector<int>{7, 7, 7, 7}),
    std::make_tuple(21, std::vector<int>{-5}),
    
    // Small arrays
    std::make_tuple(22, std::vector<int>{2, 1}),
    std::make_tuple(23, std::vector<int>{3, 1, 2}),
    std::make_tuple(24, std::vector<int>{1, 2, 3, 4, 5, 6, 7}),
    
    // Generated arrays
    std::make_tuple(25, []() {
      std::vector<int> vec(100);
      for (size_t i = 0; i < vec.size(); i++) {
        vec[i] = static_cast<int>(vec.size() - i);
      }
      return vec;
    }()),
    std::make_tuple(26, std::vector<int>{5, 5, 5, 5, 5}),
    
    // Mixed cases
    std::make_tuple(27, std::vector<int>{-1, -5, -3, -2}),
    std::make_tuple(28, std::vector<int>{5, -2, 0, -1, 9, -3}),
    
    // Boundary values
    std::make_tuple(29, std::vector<int>{
        std::numeric_limits<int>::max(),
        std::numeric_limits<int>::min(),
        0
    }),
    
    // MPI-specific cases
    std::make_tuple(30, std::vector<int>{1, 2}),
    std::make_tuple(31, std::vector<int>{10, 20, 5, 15, 25, 3, 30}),
    
    // Single element edge cases
    std::make_tuple(32, std::vector<int>{42}),
    std::make_tuple(33, std::vector<int>{5, 3}),
    std::make_tuple(34, std::vector<int>{7}),
    std::make_tuple(35, std::vector<int>{1})
};

const auto kFunctionalTasksList =
    std::tuple_cat(ppc::util::AddFuncTask<redkina_a_min_elem_vec::RedkinaAMinElemVecMPI, InType>(
                       kFunctionalTests, PPC_SETTINGS_redkina_a_min_elem_vec),
                   ppc::util::AddFuncTask<redkina_a_min_elem_vec::RedkinaAMinElemVecSEQ, InType>(
                       kFunctionalTests, PPC_SETTINGS_redkina_a_min_elem_vec));

const auto kCoverageTasksList =
    std::tuple_cat(ppc::util::AddFuncTask<redkina_a_min_elem_vec::RedkinaAMinElemVecMPI, InType>(
                       kCoverageTests, PPC_SETTINGS_redkina_a_min_elem_vec),
                   ppc::util::AddFuncTask<redkina_a_min_elem_vec::RedkinaAMinElemVecSEQ, InType>(
                       kCoverageTests, PPC_SETTINGS_redkina_a_min_elem_vec));

inline const auto kFunctionalGtestValues = ppc::util::ExpandToValues(kFunctionalTasksList);
inline const auto kCoverageGtestValues = ppc::util::ExpandToValues(kCoverageTasksList);

inline const auto kPerfTestName = RedkinaAMinElemVecFuncTests::PrintFuncTestName<RedkinaAMinElemVecFuncTests>;

INSTANTIATE_TEST_SUITE_P(Functional, RedkinaAMinElemVecFuncTests, kFunctionalGtestValues, kPerfTestName);
INSTANTIATE_TEST_SUITE_P(Coverage, RedkinaAMinElemVecFuncTests, kCoverageGtestValues, kPerfTestName);

// Validation tests for empty vector
TEST(redkina_a_min_elem_vec_validation, mpi_empty_vector_validation_fails) {
  InType vec = {};
  RedkinaAMinElemVecMPI task(vec);
  EXPECT_FALSE(task.Validation());
}

TEST(redkina_a_min_elem_vec_validation, seq_empty_vector_validation_fails) {
  InType vec = {};
  RedkinaAMinElemVecSEQ task(vec);
  EXPECT_FALSE(task.Validation());
}

}  // namespace

}  // namespace redkina_a_min_elem_vec
