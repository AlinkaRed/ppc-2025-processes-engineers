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

TEST(redkina_a_min_elem_vec_coverage, seq_constructor_initialization) {  // NOLINT
  InType vec = {1, 2, 3};
  RedkinaAMinElemVecSEQ task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 1);
}

TEST(redkina_a_min_elem_vec_coverage, seq_run_impl_empty_vector) {  // NOLINT
  InType vec = {};
  RedkinaAMinElemVecSEQ task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 0);
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

TEST(redkina_a_min_elem_vec_coverage, mpi_run_impl_empty_vector) {  // NOLINT
  InType vec = {};
  RedkinaAMinElemVecMPI task(vec);

  EXPECT_TRUE(task.Validation());
  EXPECT_TRUE(task.PreProcessing());
  EXPECT_TRUE(task.Run());
  EXPECT_TRUE(task.PostProcessing());
  EXPECT_EQ(task.GetOutput(), 0);
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

TEST(redkina_a_min_elem_vec_mpi, empty_vector_rank_zero_branch) {  // NOLINT
  // Покрывает строки 38-42: if (n == 0) { if (rank == 0) { GetOutput() = 0; } return true; }
  InType vec = {};
  RedkinaAMinElemVecMPI task(vec);

  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  EXPECT_EQ(task.GetOutput(), 0);
}

TEST(redkina_a_min_elem_vec_mpi, single_element_many_processes_edge_case) {  // NOLINT
  // Покрывает строки 65-67: if (local_size == 0 && rank >= n) { local_min = INT_MAX; }
  // Этот случай возникает когда процессов больше чем элементов
  InType vec = {42};  // Всего 1 элемент
  RedkinaAMinElemVecMPI task(vec);

  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  EXPECT_EQ(task.GetOutput(), 42);
}

TEST(redkina_a_min_elem_vec_mpi, two_elements_excess_processes_coverage) {  // NOLINT
  // Еще один тест для покрытия строк 65-67
  InType vec = {5, 3};  // 2 элемента
  RedkinaAMinElemVecMPI task(vec);

  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  EXPECT_EQ(task.GetOutput(), 3);
}

TEST(redkina_a_min_elem_vec_seq, empty_vector_direct_branch) {  // NOLINT
  // Покрывает строки 28-30: if (vec.empty()) { GetOutput() = 0; return true; }
  InType vec = {};
  RedkinaAMinElemVecSEQ task(vec);

  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  EXPECT_EQ(task.GetOutput(), 0);
}

TEST(redkina_a_min_elem_vec_seq, single_element_no_loop_coverage) {  // NOLINT
  // Покрывает случай когда вектор имеет только один элемент (цикл не выполняется)
  InType vec = {7};
  RedkinaAMinElemVecSEQ task(vec);

  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  EXPECT_EQ(task.GetOutput(), 7);
}

TEST(redkina_a_min_elem_vec_mpi, minimal_elements_max_processes) {  // NOLINT
  // Тест для максимального покрытия условия local_size == 0 && rank >= n
  InType vec = {1};  // Минимальное количество элементов
  RedkinaAMinElemVecMPI task(vec);

  bool success = task.Validation() && task.PreProcessing() && task.Run() && task.PostProcessing();
  ASSERT_TRUE(success);
  EXPECT_EQ(task.GetOutput(), 1);
}

TEST(redkina_a_min_elem_vec_mpi, all_ranks_empty_vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  InType vec = {};
  RedkinaAMinElemVecMPI task(vec);

  // Каждый процесс должен выполнить эти методы
  bool validation = task.Validation();
  bool pre_processing = task.PreProcessing();
  bool run = task.Run();
  bool post_processing = task.PostProcessing();
  int output = task.GetOutput();

  // Синхронизируем все процессы
  MPI_Barrier(MPI_COMM_WORLD);

  // Проверяем что все вернули успех
  EXPECT_TRUE(validation);
  EXPECT_TRUE(pre_processing);
  EXPECT_TRUE(run);
  EXPECT_TRUE(post_processing);

  // Проверяем результат (должен быть 0 для пустого вектора)
  EXPECT_EQ(output, 0);
}

TEST(redkina_a_min_elem_vec_mpi, all_ranks_single_element) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  InType vec = {42};
  RedkinaAMinElemVecMPI task(vec);

  bool validation = task.Validation();
  bool pre_processing = task.PreProcessing();
  bool run = task.Run();
  bool post_processing = task.PostProcessing();
  int output = task.GetOutput();

  MPI_Barrier(MPI_COMM_WORLD);

  EXPECT_TRUE(validation);
  EXPECT_TRUE(pre_processing);
  EXPECT_TRUE(run);
  EXPECT_TRUE(post_processing);

  // Все процессы должны получить одинаковый результат
  EXPECT_EQ(output, 42);
}

TEST(redkina_a_min_elem_vec_mpi, all_ranks_minimal_coverage) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Используем разное количество элементов в зависимости от количества процессов
  InType vec;
  if (size == 1) {
    vec = {1};  // Один элемент для одного процесса
  } else if (size == 2) {
    vec = {1, 2};  // Два элемента для двух процессов
  } else {
    vec = {1, 2, 3};  // Три элемента для трех и более процессов
  }

  RedkinaAMinElemVecMPI task(vec);

  bool validation = task.Validation();
  bool pre_processing = task.PreProcessing();
  bool run = task.Run();
  bool post_processing = task.PostProcessing();
  int output = task.GetOutput();

  MPI_Barrier(MPI_COMM_WORLD);

  EXPECT_TRUE(validation);
  EXPECT_TRUE(pre_processing);
  EXPECT_TRUE(run);
  EXPECT_TRUE(post_processing);

  // Проверяем корректность результата
  int expected_min = vec[0];
  for (size_t i = 1; i < vec.size(); i++) {
    if (vec[i] < expected_min) {
      expected_min = vec[i];
    }
  }
  EXPECT_EQ(output, expected_min);
}

TEST(redkina_a_min_elem_vec_seq, forced_empty_coverage) {
  InType vec = {};
  RedkinaAMinElemVecSEQ task(vec);

  // Многократный вызов для гарантии покрытия
  for (int i = 0; i < 10; i++) {
    bool validation = task.Validation();
    bool pre_processing = task.PreProcessing();
    bool run = task.Run();
    bool post_processing = task.PostProcessing();
    int output = task.GetOutput();

    EXPECT_TRUE(validation);
    EXPECT_TRUE(pre_processing);
    EXPECT_TRUE(run);
    EXPECT_TRUE(post_processing);
    EXPECT_EQ(output, 0);

    // Пересоздаем задачу для чистого теста
    if (i < 9) {
      task = RedkinaAMinElemVecSEQ(vec);
    }
  }
}

}  // namespace redkina_a_min_elem_vec
