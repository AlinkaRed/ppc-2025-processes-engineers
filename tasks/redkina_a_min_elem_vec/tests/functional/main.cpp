#include <gtest/gtest.h>

#include <algorithm>
#include <cstddef>
#include <limits>
#include <vector>

#include "redkina_a_min_elem_vec/common/include/common.hpp"
#include "redkina_a_min_elem_vec/mpi/include/ops_mpi.hpp"
#include "redkina_a_min_elem_vec/seq/include/ops_seq.hpp"

namespace redkina_a_min_elem_vec {
static void CheckMinElementResult(const std::vector<int> &vec, int result) {  // NOLINT
  if (vec.empty()) {
    EXPECT_EQ(result, 0) << "Failed for empty vector";
    return;
  }

  int expected_result = vec[0];
  for (size_t i = 1; i < vec.size(); i++) {
    expected_result = std::min(vec[i], expected_result);
  }

  EXPECT_EQ(result, expected_result) << "Failed for vector size: " << vec.size();
}

TEST(redkina_a_min_elem_vec_mpi, positive_numbers) {  // NOLINT
  InType vec = {5, 2, 8, 1, 9, 3};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, negative_numbers) {  // NOLINT
  InType vec = {-5, -2, -8, -1, -9, -3};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, mixed_numbers) {  // NOLINT
  InType vec = {5, -2, 0, -1, 9, -3};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, single_element) {  // NOLINT
  InType vec = {42};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, empty_vector) {  // NOLINT
  InType vec = {};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, all_identical) {  // NOLINT
  InType vec = {7, 7, 7, 7, 7};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, with_duplicates) {  // NOLINT
  InType vec = {5, 2, 5, 1, 2, 1};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, large_numbers) {  // NOLINT
  InType vec = {1000, 500, 2000, 100, 3000};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, with_zero) {  // NOLINT
  InType vec = {10, 0, 20, -5, 0, 15};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, min_at_beginning) {  // NOLINT
  InType vec = {-10, 5, 8, 12, 25};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, min_at_end) {  // NOLINT
  InType vec = {15, 8, 12, 5, -3};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, min_in_middle) {  // NOLINT
  InType vec = {15, 8, -5, 12, 10};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, large_vector) {  // NOLINT
  InType vec(1000);
  for (size_t i = 0; i < vec.size(); i++) {
    vec[i] = static_cast<int>(vec.size() - i);
  }
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, boundary_values) {  // NOLINT
  InType vec = {std::numeric_limits<int>::max(), std::numeric_limits<int>::min(), 0, -100, 100};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, positive_numbers) {  // NOLINT
  InType vec = {5, 2, 8, 1, 9, 3};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, negative_numbers) {  // NOLINT
  InType vec = {-5, -2, -8, -1, -9, -3};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, mixed_numbers) {  // NOLINT
  InType vec = {5, -2, 0, -1, 9, -3};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, single_element) {  // NOLINT
  InType vec = {42};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, empty_vector) {  // NOLINT
  InType vec = {};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, all_identical) {  // NOLINT
  InType vec = {7, 7, 7, 7, 7};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, with_duplicates) {  // NOLINT
  InType vec = {5, 2, 5, 1, 2, 1};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, large_numbers) {  // NOLINT
  InType vec = {1000, 500, 2000, 100, 3000};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, with_zero) {  // NOLINT
  InType vec = {10, 0, 20, -5, 0, 15};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, min_at_beginning) {  // NOLINT
  InType vec = {-10, 5, 8, 12, 25};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, min_at_end) {  // NOLINT
  InType vec = {15, 8, 12, 5, -3};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, min_in_middle) {  // NOLINT
  InType vec = {15, 8, -5, 12, 10};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, large_vector) {  // NOLINT
  InType vec(1000);
  for (size_t i = 0; i < vec.size(); i++) {
    vec[i] = static_cast<int>(vec.size() - i);
  }
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, boundary_values) {  // NOLINT
  InType vec = {std::numeric_limits<int>::max(), std::numeric_limits<int>::min(), 0, -100, 100};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, coverage_single_negative) {  // NOLINT
  InType vec = {-5};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, coverage_single_negative) {  // NOLINT
  InType vec = {-5};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, alternating_signs) {  // NOLINT
  InType vec = {10, -10, 20, -20, 30, -30};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, alternating_signs) {  // NOLINT
  InType vec = {10, -10, 20, -20, 30, -30};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, repeated_minimums) {  // NOLINT
  InType vec = {1, 3, 1, 5, 1};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, repeated_minimums) {  // NOLINT
  InType vec = {1, 3, 1, 5, 1};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

}  // namespace redkina_a_min_elem_vec
