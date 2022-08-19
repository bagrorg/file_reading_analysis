# Installation
```console
$ mkdir build
$ cd build
$ cmake ..
$ make -j 4
$ cd tests
$ ./test
```

### Running
```console
$ ./file_reading_analysis -m {method} -i {input} -o {output} -b {batch size} -v -r {iterations}
```
- `method` -- method to run (`ifstream`/`mmap`/`read`)
- `input` -- input file
- `output` -- `.csv` report file
- `batch size` -- batch size for file processing. Equals file size if not set
- `iterations` -- count of iterations

`-v` -- verbose (optional)

# Report
## Designations
- `ms` -- microseconds
- `s` -- seconds 
- `b` bytes
- `mb` megabytes
## Сharacteristics

- OS: `Ubuntu 20.04.4 LTS`
- Linux Kernel: `5.13.0-39-generic`
- Processor: `Intel(R) Core(TM) i7-10700 CPU @ 2.90GHz`
- Memory device type: `SSD`
- File system: `ext4`
- I/O scheduler: `[mq-deadline] none`

## Different batch sizes
Let's look at how the size of the batch affects the time of our work. Let's start the analysis by considering the dependence of time on the size of the batch on a fixed file size.

<p float="left">
  <img src="https://github.com/bagrorg/file_reading_analysis/blob/development/report_process/Batches%20experiment%2C%20size%20%3D%201024.png" width="500" height="500">
  <img src="https://github.com/bagrorg/file_reading_analysis/blob/development/report_process/Batches%20experiment%2C%20size%20%3D%2065536.png" width="500" height="500">
  <img src="https://github.com/bagrorg/file_reading_analysis/blob/development/report_process/Batches%20experiment%2C%20size%20%3D%201048576.png" width="500" height="500">
  <img src="https://github.com/bagrorg/file_reading_analysis/blob/development/report_process/Batches%20experiment%2C%20size%20%3D%2016777216.png" width="500" height="500">
</p>

You can notice that basically `ifstream` is inferior to `read` in performance, except for the first picture. However, the first graph shows a rather small file size, and if we pay attention to the `y` axis, we will see that the difference is imperceptible.

It is also seen that, as expected, on large sizes, a small batch size can be fatal for the performance. Indeed, too frequent access to the system call begins to give a larger overhead than using a larger buffer size.

On most graphs the optimum is not on the right side - it is somewhere in the middle between the small size and the size of the file itself. This shows that the use of these functions can be improved by piecemeal reading.

Now let's consider the dependence of the speed of work on the file size for different sizes of batches

<p float="left">
  <img src="https://github.com/bagrorg/file_reading_analysis/blob/development/report_process/Batches%20experiment%2C%20method%20%3D%20read.png" width="500" height="500">
  <img src="https://github.com/bagrorg/file_reading_analysis/blob/development/report_process/Batches%20experiment%2C%20method%20%3D%20ifstream.png" width="500" height="500">
</p>

As expected, the size of the `128` batch proved to be significantly worse than the rest on big data. This trend has been discussed above. With the growth of the buffer size, the work on average becomes better and it is difficult to see the difference on large sizes, but the size of `131072` proved to be better than `2097152'. We will use it for comparison with `mmap'.


## General comparison
Let's fix the batch sizes for `read` and `ifstream` and compare them with `mmap`. The results can be seen in the following graphs:

<img src="https://github.com/bagrorg/file_reading_analysis/blob/development/report_process/full.png">

Watching the markers on the graph lines, it can be seen that `mmap` shows itself a little better than the others, which makes it a good choice for working with files - it is fast and does not require separate study with batches.


However, you can see an interesting phenomenon on the graph: about `256` megabytes of `mmap` and `ifstream` make a performance jump in the negative direction, while `read` does not react in any way. Perhaps this can be explained from a hardware point of view.


It is also worth noting that such a small difference may arise due to the overhead obtained from counting byte statistics and using various utility tools during measurements. To complete the picture, we can look at the results from a slightly different angle (see the following paragraph)


### Kernel time
Consider the running time in 'kernel space'. To do this, use the `time` utility.

| size      | method   | kernel_time, s |
|------------|----------|-------------|
| 128 b        | ifstream | 0.0         |
|            | mmap     | 0.0         |
|            | read     | 0.0         |
| 256 b       | ifstream | 0.005       |
|         | mmap     | 0.0         |
|         | read     | 0.0         |
| 512 b        | ifstream | 0.001       |
|         | mmap     | 0.0         |
|         | read     | 0.0         |
| 1 kb      | ifstream | 0.001       |
|        | mmap     | 0.0         |
|        | read     | 0.0         |
| 2 kb       | ifstream | 0.005       |
|        | mmap     | 0.002       |
|        | read     | 0.0         |
| 4 kb       | ifstream | 0.004       |
|        | mmap     | 0.0         |
|        | read     | 0.0         |
| 8 kb       | ifstream | 0.004       |
|        | mmap     | 0.0         |
|        | read     | 0.0         |
| 16 kb      | ifstream | 0.001       |
|       | mmap     | 0.0         |
|       | read     | 0.0         |
| 32 kb      | ifstream | 0.004       |
|       | mmap     | 0.001       |
|       | read     | 0.0         |
| 64 kb      | ifstream | 0.004       |
|       | mmap     | 0.0         |
|       | read     | 0.0         |
| 128 kb     | ifstream | 0.005       |
|      | mmap     | 0.0         |
|      | read     | 0.0         |
| 256 kb     | ifstream | 0.0         |
|      | mmap     | 0.0         |
|      | read     | 0.0         |
| 512 kb     | ifstream | 0.0         |
|      | mmap     | 0.0         |
|      | read     | 0.0         |
| 1 mb    | ifstream | 0.0         |
|     | mmap     | 0.0         |
|     | read     | 0.0         |
| 2 mb    | ifstream | 0.0         |
|     | mmap     | 0.0         |
|     | read     | 0.0         |
| 4 mb    | ifstream | 0.0         |
|     | mmap     | 0.004       |
|     | read     | 0.0         |
| 8 mb    | ifstream | 0.004       |
|     | mmap     | 0.0         |
|     | read     | 0.004       |
| 16 mb   | ifstream | 0.0         |
|    | mmap     | 0.004       |
|    | read     | 0.0         |
| 32 mb   | ifstream | 0.0         |
|    | mmap     | 0.004       |
|    | read     | 0.004       |
| 64 mb   | ifstream | 0.004       |
|    | mmap     | 0.0         |
|    | read     | 0.012       |
| 128 mb  | ifstream | 0.02        |
|   | mmap     | 0.012       |
|   | read     | 0.016       |
| 256 mb  | ifstream | 0.056       |
|   | mmap     | 0.012       |
|   | read     | 0.056       |
| 512 mb  | ifstream | 0.06        |
|   | mmap     | 0.048       |
|   | read     | 0.048       |
| 1 gb | ifstream | 0.084       |
|  | mmap     | 0.068       |
|  | read     | 0.121       |
| 2 gb | ifstream | 0.196       |
|  | mmap     | 0.088       |
|  | read     | 0.204       |


As we can see, mapping files directly into memory usually spends less time in the `kernel space`, although it is clear that sometimes competitors achieve similar results (they even overtake on small sizes).

From the data, we can also conclude that with the growth of the file, the trend is such that the `mmap` time in the `kernel space` may grow slower than the rest.

## Conclusions
- When using `read`/`ifstream` can give a better performance approach with reading by butch
- Using a small batch size on a large file can result in a strong performance loss
- `mmap` spends less time in `kernel space`
