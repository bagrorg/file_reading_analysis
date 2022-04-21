# Отчет
## Обозначения
На графиках `ms` означает микросекунды, `b` означает байты.
## Характеристики

...

## Различные батчи
Рассмотрим, как размер батча влияет на время нашей работы. Начнем анализ с рассмотрения зависимости времени от размера батча на фиксированном размере файла.

<p float="left">
  <img src="https://github.com/bagrorg/file_reading_analysis/blob/development/report_process/Batches%20experiment%2C%20size%20%3D%201024.png" width="500" height="500">
  <img src="https://github.com/bagrorg/file_reading_analysis/blob/development/report_process/Batches%20experiment%2C%20size%20%3D%2065536.png" width="500" height="500">
  <img src="https://github.com/bagrorg/file_reading_analysis/blob/development/report_process/Batches%20experiment%2C%20size%20%3D%201048576.png" width="500" height="500">
  <img src="https://github.com/bagrorg/file_reading_analysis/blob/development/report_process/Batches%20experiment%2C%20size%20%3D%2016777216.png" width="500" height="500">
</p>




базар



Теперь рассмотрим зависимость скорости работы от размера файла для разных размеров батчей

....

<p float="left">
  <img src="https://github.com/bagrorg/file_reading_analysis/blob/development/report_process/Batches%20experiment%2C%20method%20%3D%20read.png" width="500" height="500">
  <img src="https://github.com/bagrorg/file_reading_analysis/blob/development/report_process/Batches%20experiment%2C%20method%20%3D%20ifstream.png" width="500" height="500">
</p>


## Общее сравнение
Зафиксируем размеры батчей для `read` и `ifstream` и сравним их с `mmap`. Результаты можно видеть на следующих графиках:

<img src="https://github.com/bagrorg/file_reading_analysis/blob/development/report_process/full.png">

базар


### Kernel time
Также нас может заинтересовать время работы в `kernel space`. Для этого используем утилиту `time`. 

| size       | method   | kernel_time |
|------------|----------|-------------|
| 128        | ifstream | 0.0         |
|            | mmap     | 0.0         |
|            | read     | 0.0         |
| 256        | ifstream | 0.005       |
|         | mmap     | 0.0         |
|         | read     | 0.0         |
| 512        | ifstream | 0.001       |
|         | mmap     | 0.0         |
|         | read     | 0.0         |
| 1024       | ifstream | 0.001       |
|        | mmap     | 0.0         |
|        | read     | 0.0         |
| 2048       | ifstream | 0.005       |
|        | mmap     | 0.002       |
|        | read     | 0.0         |
| 4096       | ifstream | 0.004       |
|        | mmap     | 0.0         |
|        | read     | 0.0         |
| 8192       | ifstream | 0.004       |
|        | mmap     | 0.0         |
|        | read     | 0.0         |
| 16384      | ifstream | 0.001       |
|       | mmap     | 0.0         |
|       | read     | 0.0         |
| 32768      | ifstream | 0.004       |
|       | mmap     | 0.001       |
|       | read     | 0.0         |
| 65536      | ifstream | 0.004       |
|       | mmap     | 0.0         |
|       | read     | 0.0         |
| 131072     | ifstream | 0.005       |
|      | mmap     | 0.0         |
|      | read     | 0.0         |
| 262144     | ifstream | 0.0         |
|      | mmap     | 0.0         |
|      | read     | 0.0         |
| 524288     | ifstream | 0.0         |
|      | mmap     | 0.0         |
|      | read     | 0.0         |
| 1048576    | ifstream | 0.0         |
|     | mmap     | 0.0         |
|     | read     | 0.0         |
| 2097152    | ifstream | 0.0         |
|     | mmap     | 0.0         |
|     | read     | 0.0         |
| 4194304    | ifstream | 0.0         |
|     | mmap     | 0.004       |
|     | read     | 0.0         |
| 8388608    | ifstream | 0.004       |
|     | mmap     | 0.0         |
|     | read     | 0.004       |
| 16777216   | ifstream | 0.0         |
|    | mmap     | 0.004       |
|    | read     | 0.0         |
| 33554432   | ifstream | 0.0         |
|    | mmap     | 0.004       |
|    | read     | 0.004       |
| 67108864   | ifstream | 0.004       |
|    | mmap     | 0.0         |
|    | read     | 0.012       |
| 134217728  | ifstream | 0.02        |
|   | mmap     | 0.012       |
|   | read     | 0.016       |
| 268435456  | ifstream | 0.056       |
|   | mmap     | 0.012       |
|   | read     | 0.056       |
| 536870912  | ifstream | 0.06        |
|   | mmap     | 0.048       |
|   | read     | 0.048       |
| 1073741824 | ifstream | 0.084       |
|  | mmap     | 0.068       |
|  | read     | 0.121       |
| 2147483648 | ifstream | 0.196       |
|  | mmap     | 0.088       |
|  | read     | 0.204       |


Как мы видим, мапинг файлов напрямую в память как правило проводит меньше времени в `kernel space`, хотя видно, что иногда конкуренты достигают похожих результатов.
