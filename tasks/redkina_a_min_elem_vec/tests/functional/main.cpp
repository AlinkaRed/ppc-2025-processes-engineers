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

// Специальные тесты для покрытия конкретных непокрытых строк
TEST(redkina_a_min_elem_vec_mpi, single_element_many_processes_edge_case) {  // NOLINT
  // Покрывает строки: if (local_size == 0 && rank >= n) { local_min = INT_MAX; }
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

TEST(redkina_a_min_elem_vec_mpi, loop_boundary_condition_coverage) {  // NOLINT
  // Создаем случай где end_idx может быть больше чем n (теоретически)
  // Используем просто большой вектор чтобы покрыть разные распределения
  InType vec(17);  // Простое число для неравномерного распределения
  for (size_t i = 0; i < vec.size(); i++) {
    vec[i] = static_cast<int>(vec.size() - i);
  }
  vec[16] = -1;  // Минимальный элемент в конце

  RedkinaAMinElemVecMPI task(vec);
  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  EXPECT_EQ(task.GetOutput(), -1);
}

// Тест для покрытия всех возможных путей в условии валидации
TEST(redkina_a_min_elem_vec_validation, mpi_validation_condition_coverage) {  // NOLINT
  // Тест на непустой вектор и output = 0 (валидация успешна)
  InType vec = {1, 2, 3};
  RedkinaAMinElemVecMPI task(vec);
  EXPECT_TRUE(task.Validation());

  // Тест на пустой вектор (валидация неуспешна)
  InType empty_vec = {};
  RedkinaAMinElemVecMPI empty_task(empty_vec);
  EXPECT_FALSE(empty_task.Validation());
}

// Специфические тесты для принудительного покрытия веток MPI

// Тест для принудительного покрытия ветки: if (local_size == 0 && rank >= n)
// Создаем ситуацию, когда процессов больше чем элементов
TEST(redkina_a_min_elem_vec_mpi, force_coverage_excess_processes) {  // NOLINT
  // Используем минимально возможный вектор чтобы максимизировать шансы
  // попасть в ветку с local_size == 0
  InType vec = {100};
  RedkinaAMinElemVecMPI task(vec);

  // Принудительно вызываем все методы чтобы покрыть все ветки
  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());

  // Проверяем что результат корректен несмотря на избыток процессов
  EXPECT_EQ(task.GetOutput(), 100);
}

// Тест для покрытия специфического распределения с remainder > 0
// где некоторые процессы получают на 1 элемент больше
TEST(redkina_a_min_elem_vec_mpi, force_coverage_uneven_distribution) {  // NOLINT
  // Размер вектора выбран так чтобы был ненулевой remainder
  // Например: для 4 процессов: 7 элементов, remainder = 3
  InType vec = {50, 30, 80, 10, 60, 20, 40};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());

  EXPECT_EQ(task.GetOutput(), 10);
}

// Тест для покрытия случая когда все процессы имеют local_size > 0
// но распределение неравномерное
TEST(redkina_a_min_elem_vec_mpi, force_coverage_all_processes_have_work) {  // NOLINT
  // Для 4 процессов: 9 элементов, remainder = 1
  // rank 0: 3 элемента, rank 1-3: по 2 элемента
  InType vec = {15, 25, 5, 35, 45, 10, 55, 20, 30};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());

  EXPECT_EQ(task.GetOutput(), 5);
}

// Тест для принудительного покрытия условия в цикле когда vec[i] >= local_min
TEST(redkina_a_min_elem_vec_mpi, force_coverage_condition_false) {  // NOLINT
  // Вектор где минимальный элемент в начале,
  // чтобы на остальных итерациях условие было false
  InType vec = {1, 2, 3, 4, 5, 6, 7, 8};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());

  EXPECT_EQ(task.GetOutput(), 1);
}

// Тест для покрытия граничного условия i < n в цикле
TEST(redkina_a_min_elem_vec_mpi, force_coverage_loop_boundary) {  // NOLINT
  // Вектор с размером, который на границе распределения
  InType vec = {10, 20, 30, 40, 50};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());

  EXPECT_EQ(task.GetOutput(), 10);
}

// Тест для покрытия случая когда start_idx и end_idx вычисляются
// по разным формулам в зависимости от rank и remainder
TEST(redkina_a_min_elem_vec_mpi, force_coverage_index_calculation_both_paths) {  // NOLINT
  // Размер вектора выбран чтобы покрыть обе ветки вычисления индексов
  InType vec = {8, 3, 6, 1, 9, 4, 2, 7, 5, 0};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());

  EXPECT_EQ(task.GetOutput(), 0);
}

// Тест для покрытия всех возможных путей в конструкторе
TEST(redkina_a_min_elem_vec_mpi, force_coverage_constructor_paths) {  // NOLINT
  // Множество различных векторов для покрытия разных путей в конструкторе
  std::vector<std::vector<int>> test_vectors = {{1}, {1, 2}, {1, 2, 3}, {3, 2, 1}, {5, 5, 5}, {-1, -2, -3}};

  for (const auto &vec : test_vectors) {
    RedkinaAMinElemVecMPI task(vec);
    EXPECT_TRUE(task.Validation());
    EXPECT_TRUE(task.PreProcessing());
    EXPECT_TRUE(task.Run());
    EXPECT_TRUE(task.PostProcessing());
    CheckMinElementResult(vec, task.GetOutput());
  }
}

// Тест для принудительного покрытия всех методов PreProcessingImpl и PostProcessingImpl
TEST(redkina_a_min_elem_vec_mpi, force_coverage_pre_post_processing) {  // NOLINT
  InType vec = {42};
  RedkinaAMinElemVecMPI task(vec);

  // Явно проверяем что PreProcessing и PostProcessing всегда возвращают true
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.PostProcessing());
}

// Специфические тесты для SEQ версии

// Тест для принудительного покрытия случая когда цикл не выполняется
TEST(redkina_a_min_elem_vec_seq, force_coverage_no_loop_execution) {  // NOLINT
  InType vec = {999};
  RedkinaAMinElemVecSEQ task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());

  EXPECT_EQ(task.GetOutput(), 999);
}

// Тест для покрытия случая когда условие в цикле всегда истинно
TEST(redkina_a_min_elem_vec_seq, force_coverage_condition_always_true) {  // NOLINT
  // Строго убывающая последовательность
  InType vec = {5, 4, 3, 2, 1, 0, -1, -2};
  RedkinaAMinElemVecSEQ task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());

  EXPECT_EQ(task.GetOutput(), -2);
}

// Тест для покрытия случая когда условие в цикле всегда ложно
TEST(redkina_a_min_elem_vec_seq, force_coverage_condition_always_false) {  // NOLINT
  // Строго возрастающая последовательность
  InType vec = {-5, -4, -3, -2, -1, 0, 1, 2};
  RedkinaAMinElemVecSEQ task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());

  EXPECT_EQ(task.GetOutput(), -5);
}

// Тест для покрытия случая когда условие в цикле иногда истинно, иногда ложно
TEST(redkina_a_min_elem_vec_seq, force_coverage_condition_mixed) {  // NOLINT
  // Неупорядоченная последовательность
  InType vec = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0};
  RedkinaAMinElemVecSEQ task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());

  EXPECT_EQ(task.GetOutput(), 0);
}

// Тест для принудительного покрытия всех путей в валидации
TEST(redkina_a_min_elem_vec_validation, force_coverage_validation_paths) {  // NOLINT
  // Тест успешной валидации
  {
    InType vec = {1, 2, 3};
    RedkinaAMinElemVecMPI task(vec);
    EXPECT_TRUE(task.Validation());
  }

  // Тест неуспешной валидации - пустой вектор
  {
    InType vec = {};
    RedkinaAMinElemVecMPI task(vec);
    EXPECT_FALSE(task.Validation());
  }

  // Тест неуспешной валидации - output != 0 (хотя в конструкторе он всегда 0)
  // Этот случай сложно воспроизвести, так как конструктор всегда устанавливает output = 0
}

// Комплексный тест для покрытия всех основных веток в одном тесте
TEST(redkina_a_min_elem_vec_mpi, force_coverage_comprehensive) {  // NOLINT
  // Тестируем различные комбинации которые могут повлиять на покрытие веток
  std::vector<std::pair<std::vector<int>, std::string>> test_cases = {
      {{1}, "single_element"},         {{1, 2}, "two_elements"},           {{2, 1}, "two_elements_reversed"},
      {{3, 1, 2}, "three_elements"},   {{5, 3, 8, 2, 7}, "five_elements"}, {{1, 1, 1, 1, 1}, "all_identical"},
      {{-1, -2, -3}, "negative_only"}, {{-1, 0, 1}, "mixed_signs"},        {{INT_MAX, INT_MIN, 0}, "boundary_values"}};

  for (const auto &[vec, description] : test_cases) {
    RedkinaAMinElemVecMPI task(vec);
    EXPECT_TRUE(task.Validation()) << "Failed validation for: " << description;
    EXPECT_TRUE(task.PreProcessing()) << "Failed pre-processing for: " << description;
    EXPECT_TRUE(task.Run()) << "Failed run for: " << description;
    EXPECT_TRUE(task.PostProcessing()) << "Failed post-processing for: " << description;
    CheckMinElementResult(vec, task.GetOutput()) << "Failed check for: " << description;
  }
}

}  // namespace redkina_a_min_elem_vec
