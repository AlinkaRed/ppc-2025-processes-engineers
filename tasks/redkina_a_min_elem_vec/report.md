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

  1. Инициализировать переменную `minimum` первым элементом вектора.  
  2. Для каждого последующего элемента `v[i]`:  
    - Выбрать минимальное между `minimum` и `v[i]` и записать в `minimum`.
  3. Вернуть значение `minimum`.

**Сложность:** O(n), где *n* — размер вектора.

### Код последовательной реализации

```cpp
bool RedkinaAMinElemVecSEQ::RunImpl() {
  const auto &vec = GetInput();

  int minimum = vec[0];
  for (size_t i = 1; i < vec.size(); i++) {
    minimum = std::min(minimum, vec[i]);
  }

  GetOutput() = minimum;
  return true;
}
```

---

## Схема распараллеливания (MPI)

  Алгоритм параллельно находит минимальный элемент вектора, разделяя данные между процессами и объединяя локальные минимумы через коллективную операцию MPI.

  1. **Инициализация:** Все процессы запускаются в одном коммуникаторе `MPI_COMM_WORLD`.  
  2. **Передача размера:** Процесс 0 рассылает длину вектора всем остальным процессам (`MPI_Bcast`).  
  3. **Распределение данных:** Каждый процесс вычисляет границы своей подвыборки. Деление почти равномерное: первые remainder процессов получают на один элемент больше, если размер вектора не делится нацело.  
  4. **Локальный минимум:** Каждый процесс ищет минимум в своей части вектора. Для процессов, которые получают пустую подвыборку (`size_l == 0 и rank >= n`), локальный минимум устанавливается в `INT_MAX`.  
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

  int size_l = n / size;
  int remainder = n % size;

  int start_idx = 0;
  int end_idx = 0;

  if (rank < remainder) {
    start_idx = rank * (size_l + 1);
    end_idx = start_idx + size_l + 1;
  } else {
    start_idx = (remainder * (size_l + 1)) + ((rank - remainder) * size_l);
    end_idx = start_idx + size_l;
  }

  int min_l = INT_MAX;
  for (int i = start_idx; i < end_idx && i < n; i++) {
    min_l = std::min(min_l, vec[i]);
  }

  if (size_l == 0 && rank >= n) {
    min_l = INT_MAX;
  }

  int min_g = 0;
  MPI_Allreduce(&min_l, &min_g, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

  GetOutput() = min_g;

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
- Положительные числа: {7, 2, 5, 3, 4, 1}
- Отрицательные числа: {-7, -2, -5, -3, -4, -1}
- Смешанные числа: {4, -1, 5, -9, 3, -3}
- Единичный элемент: {38}
- Все одинаковые элементы: {3, 3, 3, 3, 3}
- Дубликаты: {6, 1, 3, 9, 2, 1}
- Большие числа: {7000, 1500, 4000, 200, 6000}
- Смешанные положительные, отрицательные и нулевые значения: {10, -5, 20, 0, -8, 15}
- Минимум в начале: {-10, 1, 9, 14, 20}
- Минимум в конце: {88, 8, 18, 4, -8}
- Минимум в середине: {18, 8, -8, 17, 19}
- Большой вектор (1000 элементов, убывающий порядок): {1000, 999, 998, …, 1}
- Граничные значения: {INT_MAX, INT_MIN, 0, -100, 100}
- Простой возрастающий вектор: {1, 2, 3}
- Минимум в середине: {1, 5, 8, 2, 9}
- Смешанные значения: {5, 8, 10, 4, 2}
- Отрицательные числа: {-3, -1, -6, -2}
- Два элемента, минимум в конце: {2, 1}
- Три элемента, минимум в середине: {3, 1, 2}
- Большой вектор (100 элементов, убывающий порядок): {100, 99, 98, …, 1}
- Смешанные положительные и отрицательные значения: {7, -6, 0, -9, 9, -1}
- Граничные значения типа int: {INT_MAX, INT_MIN, 0}
- Единичный элемент: {15}
- Возрастающий вектор из 4 элементов: {1, 2, 3, 4}
- Возрастающий вектор из 5 элементов: {1, 2, 3, 4, 5}
- Возрастающий вектор из 6 элементов: {1, 2, 3, 4, 5, 6}
- Возрастающий вектор из 7 элементов: {1, 2, 3, 4, 5, 6, 7}
- Минимум в середине: {1, 2, 3, 4, 5}
- Минимум в конце: {2, 3, 4, 1, 5}
- Минимум в начале (убывающий порядок): {5, 4, 3, 2, 1}
- Смешанные значения: {11, 6, 7, 2, 6, 1, 10, 4}
- Все отрицательные элементы: {-1, -2, -3, -4, -5}
- Нули с одним положительным элементом: {0, 0, 0, 0, 1}

**Результат:** Все тесты успешно пройдены, последовательная и MPI-версии возвращают одинаковые значения.

### Оценка производительности
  Для оценки производительности использовались тесты с большим объемом данных (100000000 элементов).

**Время выполнения task_run**

| Режим | Процессы | Время, с | 
|-------|----------|----------|
| seq   | 1        | 0.045    | 
| mpi   | 2        | 0.032    | 
| mpi   | 3        | 0.026    | 
| mpi   | 4        | 0.027    | 

**Время выполнения pipeline**

| Режим | Процессы | Время, с | 
|-------|----------|----------|
| seq   | 1        | 0.060    | 
| mpi   | 2        | 0.059    | 
| mpi   | 3        | 0.060    | 
| mpi   | 4        | 0.060    | 

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
  
  int minimum = vec[0];
  for (size_t i = 1; i < vec.size(); i++) {
    minimum = std::min(minimum, vec[i]);
  }

  GetOutput() = minimum;
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

  int size_l = n / size;
  int remainder = n % size;

  int start_idx = 0;
  int end_idx = 0;

  if (rank < remainder) {
    start_idx = rank * (size_l + 1);
    end_idx = start_idx + size_l + 1;
  } else {
    start_idx = (remainder * (size_l + 1)) + ((rank - remainder) * size_l);
    end_idx = start_idx + size_l;
  }

  int min_l = INT_MAX;
  for (int i = start_idx; i < end_idx && i < n; i++) {
    min_l = std::min(min_l, vec[i]);
  }

  if (size_l == 0 && rank >= n) {
    min_l = INT_MAX;
  }

  int min_g = 0;
  MPI_Allreduce(&min_l, &min_g, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

  GetOutput() = min_g;

  return true;
}

bool RedkinaAMinElemVecMPI::PostProcessingImpl() {
  return true;
}

}  // namespace redkina_a_min_elem_vec
```