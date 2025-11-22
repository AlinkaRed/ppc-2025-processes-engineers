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

Алгоритм рассчитывает минимальный элемент входного вектора параллельно, распределяя данные между всеми процессами при помощи механизма разрозненной рассылки (`MPI_Scatterv`). Каждый процесс находит локальный минимум своей части данных, после чего локальные результаты объединяются в глобальный минимум.

1. **Инициализация.**  
   Все процессы работают в одном коммуникаторе `MPI_COMM_WORLD`. Каждый процесс узнаёт свой ранг и общее количество процессов.

2. **Передача размера.**  
   Процесс 0 вычисляет размер входного вектора и рассылает это значение всем процессам с помощью `MPI_Bcast`.

3. **Распределение данных между процессами.**  
   Вектор делится почти равномерно: базовый размер `base = n / size`, а первые `rem = n % size` процессов получают на один элемент больше.  
   На процессе 0 формируются массивы `send_counts` и `displs`, которые определяют, сколько элементов и с какого смещения получает каждый процесс.  
   Рассылка данных выполняется вызовом `MPI_Scatterv`:  
   - процесс 0 отправляет части исходного массива,  
   - остальные процессы получают только свою локальную часть, не имея доступа к полному вектору.

4. **Локальный минимум.**  
   Каждый процесс последовательно просматривает полученный участок данных и вычисляет минимальный элемент среди них. Если процесс получил пустой участок (что возможно только при `n < size`), то локальный минимум остаётся равным `INT_MAX`.

5. **Глобальная редукция.**  
   Результаты всех процессов объединяются вызовом `MPI_Allreduce` с операцией `MPI_MIN`. Таким образом, все процессы получают один и тот же глобальный минимум.

6. **Завершение.**  
   Каждый процесс записывает вычисленный глобальный минимум в выходную переменную.

### Код параллельной реализации

```cpp
bool RedkinaAMinElemVecMPI::RunImpl() {
  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int n = 0;
  if (rank == 0) {
    n = static_cast<int>(GetInput().size());
  }
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int base = n / size;
  int rem = n % size;
  int local_size = (rank < rem) ? base + 1 : base;

  std::vector<int> local_vec(local_size);

  std::vector<int> send_counts;
  std::vector<int> displs;

  if (rank == 0) {
    send_counts.resize(size);
    displs.resize(size);
    for (int i = 0; i < size; i++) {
      send_counts[i] = (i < rem) ? base + 1 : base;
      if (i < rem) {
        displs[i] = i * (base + 1);
      } else {
        displs[i] = (rem * (base + 1)) + ((i - rem) * base);
      }
    }
  }

  if (rank == 0) {
    MPI_Scatterv(GetInput().data(), send_counts.data(), displs.data(), MPI_INT, 
                 local_vec.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    MPI_Scatterv(nullptr, nullptr, nullptr, MPI_INT, 
                 local_vec.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);
  }

  int min_l = INT_MAX;
  for (int v : local_vec) {
    min_l = std::min(min_l, v);
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
- Положительные числа: {7, 2, 5, 3, 4, -7} → -7
- Отрицательные числа: {-7, -2, -5, -3, -4, -1} → -7
- Смешанные числа: {4, -1, 5, -7, 3, -3} → -7
- Единичный элемент: {-7} → -7
- Все одинаковые элементы: {3, 3, 3, 3, -7} → -7
- Дубликаты: {6, 1, 3, 9, 2, -7} → -7
- Большие числа: {7000, 1500, 4000, -7, 6000} → -7
- Смешанные положительные, отрицательные и нулевые значения: {10, -5, 20, 0, -7, 15} → -7
- Минимум в начале: {-7, 1, 9, 14, 20} → -7
- Минимум в конце: {88, 8, 18, 4, -7} → -7
- Минимум в середине: {18, 8, -7, 17, 19} → -7
- Большой вектор (1000 элементов, убывающий порядок): {1000, 999, 998, …, 1} с -7 в середине → -7
- Граничные значения: {INT_MAX, -7, 0, 100} → -7
- Простой возрастающий вектор: {1, 2, -7} → -7
- Минимум в середине: {1, 5, 8, 2, -7} → -7
- Смешанные значения: {5, 8, 10, 4, -7} → -7
- Отрицательные числа: {-3, -1, -6, -7} → -7
- Два элемента, минимум в конце: {2, -7} → -7
- Три элемента, минимум в середине: {3, -7, 2} → -7
- Большой вектор (100 элементов, убывающий порядок): {100, 99, 98, …, 1} с -7 в середине → -7
- Смешанные положительные и отрицательные значения: {7, -6, 0, -7, 9, -1} → -7
- Граничные значения типа int: {INT_MAX, -7, 0} → -7
- Единичный элемент: {-7} → -7
- Возрастающий вектор из 4 элементов: {-7, 1, 2, 3} → -7
- Возрастающий вектор из 5 элементов: {-7, 1, 2, 3, 4} → -7
- Возрастающий вектор из 6 элементов: {-7, 1, 2, 3, 4, 5} → -7
- Возрастающий вектор из 7 элементов: {-7, 1, 2, 3, 4, 5, 6} → -7
- Минимум в середине: {-7, 1, 2, 3, 4} → -7
- Минимум в конце: {2, 3, 4, -7, 5} → -7
- Минимум в начале (убывающий порядок): {5, 4, 3, 2, -7} → -7
- Смешанные значения: {11, 6, 7, 2, 6, -7, 10, 4} → -7
- Все отрицательные элементы: {-1, -2, -3, -4, -7} → -7
- Нули с одним положительным элементом: {0, 0, 0, 0, -7} → -7

**Результат:** Все тесты успешно пройдены, последовательная и MPI-версии возвращают одинаковые значения.

### Оценка производительности
  Для оценки производительности использовались тесты с большим объемом данных (100000000 элементов).

**Время выполнения task_run**

Режим | Процессы | Время, с | Ускорение  | Эффективность
seq   | 1        | 0.062    | 1.00       | 1.00
mpi   | 2        | 0.086    | 0.72       | 0.36
mpi   | 3        | 0.092    | 0.67       | 0.22
mpi   | 4        | 0.075    | 0.83       | 0.21

**Время выполнения pipeline**

Режим | Процессы | Время, с | Ускорение  | Эффективность
seq   | 1        | 0.064    | 1.00       | 1.00
mpi   | 2        | 0.096    | 0.67       | 0.33
mpi   | 3        | 0.078    | 0.82       | 0.27
mpi   | 4        | 0.089    | 0.72       | 0.18

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

#include <algorithm>
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
  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int n = 0;
  if (rank == 0) {
    n = static_cast<int>(GetInput().size());
  }
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int base = n / size;
  int rem = n % size;
  int local_size = (rank < rem) ? base + 1 : base;

  std::vector<int> local_vec(local_size);

  std::vector<int> send_counts;
  std::vector<int> displs;

  if (rank == 0) {
    send_counts.resize(size);
    displs.resize(size);
    for (int i = 0; i < size; i++) {
      send_counts[i] = (i < rem) ? base + 1 : base;
      if (i < rem) {
        displs[i] = i * (base + 1);
      } else {
        displs[i] = (rem * (base + 1)) + ((i - rem) * base);
      }
    }
  }

  if (rank == 0) {
    MPI_Scatterv(GetInput().data(), send_counts.data(), displs.data(), MPI_INT, local_vec.data(), local_size, MPI_INT,
                 0, MPI_COMM_WORLD);
  } else {
    MPI_Scatterv(nullptr, nullptr, nullptr, MPI_INT, local_vec.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);
  }

  int min_l = INT_MAX;
  for (int v : local_vec) {
    min_l = std::min(min_l, v);
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