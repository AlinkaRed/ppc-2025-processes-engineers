#include <gtest/gtest.h>

#include <cstddef>
#include <limits>
#include <vector>

#include "redkina_a_min_elem_vec/common/include/common.hpp"
#include "redkina_a_min_elem_vec/mpi/include/ops_mpi.hpp"
#include "redkina_a_min_elem_vec/seq/include/ops_seq.hpp"

namespace redkina_a_min_elem_vec {

// Вспомогательная функция для проверки результата
static void CheckMinElementResult(const std::vector<int> &vec, int result) {  // NOLINT
  if (vec.empty()) {
    EXPECT_EQ(result, 0) << "Failed for empty vector";
    return;
  }

  int expected_result = vec[0];
  for (size_t i = 1; i < vec.size(); i++) {
    if (vec[i] < expected_result) {  // NOLINT
      expected_result = vec[i];
    }
  }

  EXPECT_EQ(result, expected_result) << "Failed for vector size: " << vec.size();
}

// Базовые тесты для MPI
TEST(redkina_a_min_elem_vec_mpi, positive_numbers) {
  InType vec = {5, 2, 8, 1, 9, 3};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, negative_numbers) {
  InType vec = {-5, -2, -8, -1, -9, -3};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, mixed_numbers) {
  InType vec = {5, -2, 0, -1, 9, -3};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, single_element) {
  InType vec = {42};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, empty_vector) {
  InType vec = {};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, all_identical) {
  InType vec = {7, 7, 7, 7, 7};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, with_duplicates) {
  InType vec = {5, 2, 5, 1, 2, 1};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, large_numbers) {
  InType vec = {1000, 500, 2000, 100, 3000};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, with_zero) {
  InType vec = {10, 0, 20, -5, 0, 15};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, min_at_beginning) {
  InType vec = {-10, 5, 8, 12, 25};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, min_at_end) {
  InType vec = {15, 8, 12, 5, -3};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, min_in_middle) {
  InType vec = {15, 8, -5, 12, 10};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, large_vector) {
  InType vec(1000);
  for (size_t i = 0; i < vec.size(); i++) {
    vec[i] = static_cast<int>(vec.size() - i);
  }
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_mpi, boundary_values) {
  InType vec = {std::numeric_limits<int>::max(), std::numeric_limits<int>::min(), 0, -100, 100};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

// Базовые тесты для SEQ
TEST(redkina_a_min_elem_vec_seq, positive_numbers) {
  InType vec = {5, 2, 8, 1, 9, 3};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, negative_numbers) {
  InType vec = {-5, -2, -8, -1, -9, -3};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, mixed_numbers) {
  InType vec = {5, -2, 0, -1, 9, -3};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, single_element) {
  InType vec = {42};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, empty_vector) {
  InType vec = {};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, all_identical) {
  InType vec = {7, 7, 7, 7, 7};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, with_duplicates) {
  InType vec = {5, 2, 5, 1, 2, 1};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, large_numbers) {
  InType vec = {1000, 500, 2000, 100, 3000};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, with_zero) {
  InType vec = {10, 0, 20, -5, 0, 15};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, min_at_beginning) {
  InType vec = {-10, 5, 8, 12, 25};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, min_at_end) {
  InType vec = {15, 8, 12, 5, -3};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, min_in_middle) {
  InType vec = {15, 8, -5, 12, 10};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, large_vector) {
  InType vec(1000);
  for (size_t i = 0; i < vec.size(); i++) {
    vec[i] = static_cast<int>(vec.size() - i);
  }
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_seq, boundary_values) {
  InType vec = {std::numeric_limits<int>::max(), std::numeric_limits<int>::min(), 0, -100, 100};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

// Дополнительные тесты для покрытия специфических случаев
TEST(redkina_a_min_elem_vec_coverage, mpi_single_negative) {
  InType vec = {-5};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_coverage, mpi_two_elements) {
  InType vec = {2, 1};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_coverage, mpi_three_elements) {
  InType vec = {3, 1, 2};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_coverage, mpi_with_remainder) {
  InType vec = {1, 2, 3, 4, 5, 6, 7};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_coverage, mpi_medium_vector) {
  InType vec(100);
  for (size_t i = 0; i < vec.size(); i++) {
    vec[i] = static_cast<int>(vec.size() - i);
  }
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_coverage, mpi_min_in_different_positions) {
  InType vec = {10, 20, 5, 15, 25, 3, 30};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_coverage, mpi_more_processes_than_elements) {
  InType vec = {1};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_coverage, mpi_empty_vector_rank_zero) {
  InType vec = {};
  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_coverage, seq_single_negative) {
  InType vec = {-5};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

TEST(redkina_a_min_elem_vec_coverage, seq_all_same_elements) {
  InType vec = {3, 3, 3, 3, 3};
  RedkinaAMinElemVecSEQ task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  CheckMinElementResult(vec, task.GetOutput());
}

}  // namespace redkina_a_min_elem_vec
