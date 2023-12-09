# 操作系统第六章作业
## 6.9

```c
typedef struct {
    int available;
} lock;

// test_and_set定义
int test_and_set(int *target) {
    int original = *target;
    *target = 1;  // 设置为1表示锁被占用
    return original;
}

// compare_and_swap定义
int compare_and_swap(int *target, int expected, int new_value) {
    int original = *target;
    if (original == expected) {
        *target = new_value;
    }
    return original;
}

void init_lock(lock *mutex) {
    mutex->available = 0;  // 初始化为可用状态
}
```
使用 test_and_set() 实现 acquire 函数
```c
void acquire(lock *mutex) {
    while (test_and_set(&mutex->available) == 1) {
        // 锁已被占用，等待
    }
}
```
使用 compare_and_swap() 实现 acquire 函数
```c
void acquire(lock *mutex) {
    while (compare_and_swap(&mutex->available, 0, 1) == 1) {
        // 锁已被占用，等待
    }
}
```
release()函数
```c
void release(lock *mutex) {
    mutex->available = 0;  // 释放锁，设置为可用状态
}
```

## 6.10
1. **等待队列的管理**： 为每个互斥锁维护一个等待队列，将等待获取锁的进程加入队列。
2. **阻塞**： 当一个进程尝试获取已被其他进程持有的锁时，将其放入等待队列并阻塞该进程。
3. **唤醒**： 当持有锁的进程释放锁时，从等待队列中选择一个进程唤醒，使其可以尝试获取锁。

## 6.11
1. **用锁的时间很短**：应该采用**自旋锁**。它会快速尝试获取锁，而不是立即进入睡眠状态。这有助于减少由于频繁获取和释放锁而引起的线程切换开销。

2. **用锁的时间很长**：应该采用**互斥锁**。互斥锁可以使等待的线程进入睡眠状态，而不是一直处于自旋状态，避免了不必要的资源消耗。


3. **线程在拥有锁时可能处于睡眠**：应该使用**互斥锁**。如果线程在拥有锁时可能处于睡眠状态，说明可能会有长时间的等待。当线程持有锁时，其他线程会被阻塞而进入睡眠状态，等待锁释放。

## 6.19
可以用管程代码实现信号量机制：
```c
monitor file_access{
    // 当前已经占用文件的资源总和
    int curr_sum = 0;
    // 文件的最大资源容量
    int n;
    // 在访问文件时进行条件等待
    condition c;

    void access_file(int my_num){
        // 如果当前已占用的资源总和加上线程要求的资源量超过了文件的最大容量,则等待
        while (curr_sum + my_num >= n){
            c.wait();
        }
        curr_sum += my_num;
    }

    // 完成访问，释放资源
    void finish_access(int my_num){
        curr_sum -= my_num;
        c.broadcast();
    }
}
```

## 请用PV操作协调3个进程的同步关系
```c
// 假设缓冲区大小为1，设置两个缓冲区的信号量
semaphore empty_1 = 1, empty_2 = 1, full_1 = 0, full_2 = 0;
// 设置互斥锁
mutex mtx = 1;

void get()
{
    while(true)
    {
        wait(empty_1);
        wait(mtx);
        // 读取数据并放入buffer1
        Get_data(Buffer1);
        signal(mtx);
        signal(full_1);
    }
}

void copy()
{
    while(true)
    {
        wait(full_1);
        wait(mtx);
        // 拷贝数据并放入Buffer2
        Copy_data(Buffer2);
        signal(mtx);
        signal(full_2);
    }
}

void put()
{
    while(true)
    {
        wait(full_2);
        wait(mtx);
        // 打印数据
        Put_data(Buffer3);
        signal(mtx);
        signal(empty_2);
    }
}


```
