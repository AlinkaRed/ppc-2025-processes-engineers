#include <gtest/gtest.h>
#include <mpi.h>

#include <cstddef>
#include <limits>
#include <vector>

#include "redkina_a_min_elem_vec/common/include/common.hpp"
#include "redkina_a_min_elem_vec/mpi/include/ops_mpi.hpp"
#include "redkina_a_min_elem_vec/seq/include/ops_seq.hpp"

namespace redkina_a_min_elem_vec {

// Вспомогательная функция для проверки результата
static void CheckMinElementResult(const std::vector<int> &vec, int result) {  // NOLINT
  int expected_result = vec[0];
  for (size_t i = 1; i < vec.size(); i++) {
    if (vec[i] < expected_result) {  // NOLINT
      expected_result = vec[i];
    }
  }

  EXPECT_EQ(result, expected_result) << "Failed for vector size: " << vec.size();
}

// Тесты для MPI версии
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

// Тесты для SEQ версии
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

// Тесты на валидацию пустого вектора
TEST(redkina_a_min_elem_vec_validation, mpi_empty_vector_validation_fails) {  // NOLINT
  InType vec = {};
  RedkinaAMinElemVecMPI task(vec);
  EXPECT_FALSE(task.Validation());
}

TEST(redkina_a_min_elem_vec_validation, seq_empty_vector_validation_fails) {  // NOLINT
  InType vec = {};
  RedkinaAMinElemVecSEQ task(vec);
  EXPECT_FALSE(task.Validation());
}

// Coverage тесты
TEST(redkina_a_min_elem_vec_coverage, seq_constructor_initialization) {  // NOLINT
  InType vec = {1, 2, 3};
  RedkinaAMinElemVecSEQ task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 1);
}

TEST(redkina_a_min_elem_vec_coverage, seq_run_impl_single_element) {  // NOLINT
  InType vec = {42};
  RedkinaAMinElemVecSEQ task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 42);
}

TEST(redkina_a_min_elem_vec_coverage, seq_run_impl_multiple_elements_first_min) {  // NOLINT
  InType vec = {1, 5, 8, 2, 9};
  RedkinaAMinElemVecSEQ task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 1);
}

TEST(redkina_a_min_elem_vec_coverage, seq_run_impl_multiple_elements_middle_min) {  // NOLINT
  InType vec = {5, 8, 1, 9, 2};
  RedkinaAMinElemVecSEQ task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 1);
}

TEST(redkina_a_min_elem_vec_coverage, seq_run_impl_multiple_elements_last_min) {  // NOLINT
  InType vec = {5, 8, 9, 2, 1};
  RedkinaAMinElemVecSEQ task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 1);
}

TEST(redkina_a_min_elem_vec_coverage, seq_run_impl_negative_numbers) {  // NOLINT
  InType vec = {-3, -1, -5, -2};
  RedkinaAMinElemVecSEQ task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), -5);
}

TEST(redkina_a_min_elem_vec_coverage, seq_run_impl_all_equal) {  // NOLINT
  InType vec = {7, 7, 7, 7};
  RedkinaAMinElemVecSEQ task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 7);
}

TEST(redkina_a_min_elem_vec_coverage, mpi_constructor_initialization) {  // NOLINT
  InType vec = {1, 2, 3};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 1);
}

TEST(redkina_a_min_elem_vec_coverage, mpi_run_impl_single_element) {  // NOLINT
  InType vec = {42};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 42);
}

TEST(redkina_a_min_elem_vec_coverage, mpi_run_impl_single_negative) {  // NOLINT
  InType vec = {-5};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), -5);
}

TEST(redkina_a_min_elem_vec_coverage, mpi_run_impl_two_elements) {  // NOLINT
  InType vec = {2, 1};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 1);
}

TEST(redkina_a_min_elem_vec_coverage, mpi_run_impl_three_elements) {  // NOLINT
  InType vec = {3, 1, 2};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 1);
}

TEST(redkina_a_min_elem_vec_coverage, mpi_run_impl_with_remainder) {  // NOLINT
  InType vec = {1, 2, 3, 4, 5, 6, 7};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 1);
}

TEST(redkina_a_min_elem_vec_coverage, mpi_run_impl_large_vector) {  // NOLINT
  InType vec(100);
  for (size_t i = 0; i < vec.size(); i++) {
    vec[i] = static_cast<int>(vec.size() - i);
  }
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 1);
}

TEST(redkina_a_min_elem_vec_coverage, mpi_run_impl_all_identical) {  // NOLINT
  InType vec = {5, 5, 5, 5, 5};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 5);
}

TEST(redkina_a_min_elem_vec_coverage, mpi_run_impl_negative_numbers) {  // NOLINT
  InType vec = {-1, -5, -3, -2};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), -5);
}

TEST(redkina_a_min_elem_vec_coverage, mpi_run_impl_mixed_numbers) {  // NOLINT
  InType vec = {5, -2, 0, -1, 9, -3};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), -3);
}

TEST(redkina_a_min_elem_vec_coverage, mpi_run_impl_boundary_values) {  // NOLINT
  InType vec = {std::numeric_limits<int>::max(), std::numeric_limits<int>::min(), 0};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), std::numeric_limits<int>::min());
}

TEST(redkina_a_min_elem_vec_coverage, mpi_run_impl_more_processes_than_elements_2) {  // NOLINT
  InType vec = {1, 2};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 1);
}

TEST(redkina_a_min_elem_vec_coverage, mpi_run_impl_min_in_different_positions) {  // NOLINT
  InType vec = {10, 20, 5, 15, 25, 3, 30};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 3);
}

TEST(redkina_a_min_elem_vec_mpi, single_element_many_processes_edge_case) {  // NOLINT

  InType vec = {42};
  RedkinaAMinElemVecMPI task(vec);

  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  EXPECT_EQ(task.GetOutput(), 42);
}

TEST(redkina_a_min_elem_vec_mpi, two_elements_excess_processes_coverage) {  // NOLINT
  InType vec = {5, 3};
  RedkinaAMinElemVecMPI task(vec);

  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  EXPECT_EQ(task.GetOutput(), 3);
}

TEST(redkina_a_min_elem_vec_seq, single_element_no_loop_coverage) {  // NOLINT
  InType vec = {7};
  RedkinaAMinElemVecSEQ task(vec);

  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  EXPECT_EQ(task.GetOutput(), 7);
}

TEST(redkina_a_min_elem_vec_mpi, minimal_elements_max_processes) {  // NOLINT
  InType vec = {1};
  RedkinaAMinElemVecMPI task(vec);

  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  EXPECT_EQ(task.GetOutput(), 1);
}

// -------------------- MPI ValidationImpl() --------------------

// Покрытие ветки: пустой вектор (ValidationImpl() вернет false)
TEST(redkina_a_min_elem_vec_mpi_branches_extra, validation_empty_vector) {
  InType vec = {};
  RedkinaAMinElemVecMPI task(vec);
  EXPECT_FALSE(task.Validation());
}

// Покрытие ветки: local_size == 0 && rank >= n
// Нужно запускать >=3 процессов для 2 элементов
TEST(redkina_a_min_elem_vec_mpi_branches_extra, local_size_zero_rank_exceeds_n) {
  InType vec = {10, 20};  // 2 элемента
  RedkinaAMinElemVecMPI task(vec);

  // Валидация и запуск
  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 10);
}

// -------------------- SEQ ValidationImpl() --------------------

// Покрытие ветки: пустой вектор
TEST(redkina_a_min_elem_vec_seq_branches_extra, validation_empty_vector) {
  InType vec = {};
  RedkinaAMinElemVecSEQ task(vec);
  EXPECT_FALSE(task.Validation());
}

}  // namespace redkina_a_min_elem_vec
