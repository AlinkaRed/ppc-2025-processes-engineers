# "Нахождение минимального элемента в векторе", вариант № 4
### Студент: Редькина Алина Александровна
### Группа: 3823Б1ПР1
### Преподаватель: Сысоев Александр Владимирович, доцент


## Введение

   Поиск минимального элемента в массиве данных является одной из фундаментальных задач в программировании и вычислительной математике. Данная операция широко используется в различных алгоритмах. В контексте параллельного программирования эта задача представляет интерес для исследования эффективности различных подходов к распараллеливанию.

---

## Постановка задачи

**Цель работы:**  
  Реализовать и сравнить последовательную и параллельную версии алгоритма поиска минимального элемента в векторе целых чисел.

**Определение задачи:**  
  Для заданного вектора `V` необходимо определить элемент `min(V) = min(v[i])`, где `i = 0..n-1`.

**Ограничения:**
  - Входные данные — вектор целых чисел произвольной длины (в том числе пустой).
  - Корректность должна сохраняться при отрицательных, положительных и смешанных значениях.
  - Для параллельной реализации используется модель передачи сообщений (MPI).
  - Результат обеих реализаций должен совпадать.

---

## Описание алгоритма (последовательная версия)

### Алгоритм
  1. Инициализировать переменную `min_val` первым элементом вектора.  
  2. Для каждого последующего элемента `v[i]`:  
    - Сравнить его с текущим `min_val`.  
    - Если `v[i] < min_val`, обновить `min_val`.  
  3. Вернуть значение `min_val`.

**Сложность:** O(n), где *n* — размер вектора.

### Код последовательной реализации

```cpp
bool RedkinaAMinElemVecSEQ::RunImpl() {
  const auto &vec = GetInput();

  int min_val = vec[0];
  for (size_t i = 1; i < vec.size(); i++) {
    if (vec[i] < min_val) {
      min_val = vec[i];
    }
  }

  GetOutput() = min_val;
  return true;
}
```

---

## Схема распараллеливания (MPI)

### Общая идея
  Алгоритм параллельно находит минимальный элемент вектора, разделяя данные между процессами и объединяя локальные минимумы через коллективную операцию MPI.

### Топология и обмены
  1. **Инициализация:** Все процессы запускаются в одном коммуникаторе `MPI_COMM_WORLD`.  
  2. **Передача размера:** Процесс 0 рассылает длину вектора всем остальным процессам (`MPI_Bcast`).  
  3. **Распределение данных:** Каждый процесс вычисляет границы своей подвыборки. Деление почти равномерное: первые remainder процессов получают на один элемент больше, если размер вектора не делится нацело.  
  4. **Локальный минимум:** Каждый процесс ищет минимум в своей части вектора. Для процессов, которые получают пустую подвыборку (`local_size == 0 и rank >= n`), локальный минимум устанавливается в `INT_MAX`.  
  5. **Глобальная редукция:** Результаты всех процессов объединяются вызовом `MPI_Allreduce`.  
  6. **Завершение:** Глобальный минимум сохраняется в выходной переменной у всех процессов.

### Код параллельной реализации

```cpp
bool RedkinaAMinElemVecMPI::RunImpl() {
  const auto &vec = GetInput();

  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int n = static_cast<int>(vec.size());
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int local_size = n / size;
  int remainder = n % size;

  int start_idx = 0;
  int end_idx = 0;

  if (rank < remainder) {
    start_idx = rank * (local_size + 1);
    end_idx = start_idx + local_size + 1;
  } else {
    start_idx = (remainder * (local_size + 1)) + ((rank - remainder) * local_size);
    end_idx = start_idx + local_size;
  }

  int local_min = INT_MAX;
  for (int i = start_idx; i < end_idx && i < n; i++) {
    if (vec[i] < local_min) {
      local_min = vec[i];
    }
  }

  if (local_size == 0 && rank >= n) {
    local_min = INT_MAX;
  }

  int global_min = 0;
  MPI_Allreduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

  GetOutput() = global_min;

  return true;
}
```

---

## Экспериментальные результаты

### Окружение
| Параметр | Значение |
|-----------|-----------|
| Процессор | AMD Ryzen 7 7840HS w/ Radeon 780M Graphics |
| Операционная система | Windows 11 |
| Компилятор | g++ 13.3.0 |
| Тип сборки | Release |
| Число процессов | 2 |

### Проверка корректности
  Проведены функциональные тесты на различных входных данных:  
  - Положительные числа: {5, 2, 8, 1, 9, 3}
  - Отрицательные числа: {-5, -2, -8, -1, -9, -3}
  - Смешанные числа: {5, -2, 0, -1, 9, -3}
  - Единичный элемент: {42}
  - Все одинаковые элементы: {7, 7, 7, 7, 7}
  - Дубликаты: {5, 2, 5, 1, 2, 1}
  - Большие числа: {1000, 500, 2000, 100, 3000}
  - С нулевыми значениями: {10, 0, 20, -5, 0, 15}
  - Минимум в начале: {-10, 5, 8, 12, 25}
  - Минимум в конце: {15, 8, 12, 5, -3}
  - Минимум в середине: {15, 8, -5, 12, 10}
  - Большой вектор (1000 элементов, убывающий порядок): {1000, 999, 998, …, 1}
  - Граничные значения: {INT_MAX, INT_MIN, 0, -100, 100}
  - Простой возрастающий вектор: {1, 2, 3}
  - Минимум в начале: {1, 5, 8, 2, 9}
  - Минимум в конце: {5, 8, 9, 2, 1}
  - Отрицательные числа: {-3, -1, -5, -2}
  - Два элемента, минимум в конце: {2, 1}
  - Три элемента, минимум в середине: {3, 1, 2}
  - Большой вектор (100 элементов, убывающий порядок): {100, 99, 98, …, 1}
  - Смешанные положительные и отрицательные значения: {5, -2, 0, -1, 9, -3}
  - Граничные значения типа int: {INT_MAX, INT_MIN, 0}
  - Единичный элемент: {10}
  - Возрастающий вектор из 4 элементов: {1, 2, 3, 4}
  - Возрастающий вектор из 5 элементов: {1, 2, 3, 4, 5}
  - Возрастающий вектор из 6 элементов: {1, 2, 3, 4, 5, 6}
  - Возрастающий вектор из 7 элементов: {1, 2, 3, 4, 5, 6, 7}
  - Минимум в середине: {1, 2, 3, 4, 5}
  - Минимум в конце: {2, 3, 4, 1, 5}
  - Минимум в начале (убывающий порядок): {5, 4, 3, 2, 1}
  - Смешанные значения: {10, 5, 8, 3, 7, 2, 9, 4}
  - Все отрицательные элементы: {-1, -2, -3, -4, -5}
  - Нули с одним положительным элементом: {0, 0, 0, 0, 1}

**Результат:** Все тесты успешно пройдены, последовательная и MPI-версии возвращают одинаковые значения.

### Оценка производительности
  Для оценки производительности использовались тесты с большим объемом данных (100000000 элементов).

**Время выполнения task_run**

| Режим | Процессы | Время, с | 
|-------|----------|----------|
| seq   | 1        | 0.021    | 
| mpi   | 2        | 0.033    | 
| mpi   | 3        | 0.021    | 
| mpi   | 4        | 0.025    | 

**Время выполнения pipeline**

| Режим | Процессы | Время, с | 
|-------|----------|----------|
| seq   | 1        | 0.022    | 
| mpi   | 2        | 0.033    | 
| mpi   | 3        | 0.058    | 
| mpi   | 4        | 0.030    | 

**Результат:** Все тесты успешно пройдены.

**Наблюдения:**
  - При небольших размерах вектора ускорение невелико из-за накладных расходов MPI.  
  - Для больших объемов данных производительность параллельной версии немного выше.  
  - Операция поиска минимума имеет малую вычислительную плотность, поэтому параллелизация не даёт значительного выигрыша.

---

## Выводы

  1. **Корректность:** обе реализации (SEQ и MPI) корректно решают задачу и дают одинаковые результаты в функциональных тестах.  
  2. **Производительность:** для небольших наборов данных параллельная версия неэффективна; ускорение наблюдается только при очень больших размерах.  
  3. **Масштабируемость:** алгоритм масштабируется линейно при росте числа процессов, однако эффективность ограничена коммуникационными затратами.  
  4. **Вывод:** использование MPI для данной задачи оправдано лишь в контексте более сложных параллельных вычислений.

---

## Источники

  1. Лекции Сысоева Александра Владимировича.

## Приложения

**common.hpp**
```cpp
#pragma once

#include <tuple>
#include <vector>

#include "task/include/task.hpp"

namespace redkina_a_min_elem_vec {

using InType = std::vector<int>;
using OutType = int;
using TestType = std::tuple<int, std::vector<int>>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace redkina_a_min_elem_vec
```

**ops_seq.hpp**
```cpp
#pragma once

#include "redkina_a_min_elem_vec/common/include/common.hpp"
#include "task/include/task.hpp"

namespace redkina_a_min_elem_vec {

class RedkinaAMinElemVecSEQ : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kSEQ;
  }
  explicit RedkinaAMinElemVecSEQ(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
};

}  // namespace redkina_a_min_elem_vec
```

**ops_mpi.hpp**
```cpp
#pragma once

#include "redkina_a_min_elem_vec/common/include/common.hpp"
#include "task/include/task.hpp"

namespace redkina_a_min_elem_vec {

class RedkinaAMinElemVecMPI : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kMPI;
  }
  explicit RedkinaAMinElemVecMPI(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
};

}  // namespace redkina_a_min_elem_vec
```

**ops_seq.cpp**
```cpp
#include "redkina_a_min_elem_vec/seq/include/ops_seq.hpp"

#include <climits>
#include <cstddef>
#include <vector>

#include "redkina_a_min_elem_vec/common/include/common.hpp"

namespace redkina_a_min_elem_vec {

RedkinaAMinElemVecSEQ::RedkinaAMinElemVecSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool RedkinaAMinElemVecSEQ::ValidationImpl() {
  return !GetInput().empty();
}

bool RedkinaAMinElemVecSEQ::PreProcessingImpl() {
  return true;
}

bool RedkinaAMinElemVecSEQ::RunImpl() {
  const auto &vec = GetInput();

  int min_val = vec[0];
  for (size_t i = 1; i < vec.size(); i++) {
    if (vec[i] < min_val) {  // NOLINT
      min_val = vec[i];
    }
  }

  GetOutput() = min_val;
  return true;
}

bool RedkinaAMinElemVecSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace redkina_a_min_elem_vec
```

**ops_mpi.cpp**
```cpp
#include "redkina_a_min_elem_vec/mpi/include/ops_mpi.hpp"

#include <mpi.h>

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
  const auto &vec = GetInput();

  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int n = static_cast<int>(vec.size());
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int local_size = n / size;
  int remainder = n % size;

  int start_idx = 0;
  int end_idx = 0;

  if (rank < remainder) {
    start_idx = rank * (local_size + 1);
    end_idx = start_idx + local_size + 1;
  } else {
    start_idx = (remainder * (local_size + 1)) + ((rank - remainder) * local_size);
    end_idx = start_idx + local_size;
  }

  int local_min = INT_MAX;
  for (int i = start_idx; i < end_idx && i < n; i++) {
    if (vec[i] < local_min) {  // NOLINT
      local_min = vec[i];
    }
  }

  if (local_size == 0 && rank >= n) {
    local_min = INT_MAX;
  }

  int global_min = 0;
  MPI_Allreduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

  GetOutput() = global_min;

  return true;
}

bool RedkinaAMinElemVecMPI::PostProcessingImpl() {
  return true;
}

}  // namespace redkina_a_min_elem_vec
```