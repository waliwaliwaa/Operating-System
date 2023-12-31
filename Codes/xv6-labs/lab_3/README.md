# xv6-labs：lab 3

### 作业内容
#### 实验背景
在本次实验中，你将探索页表并修改它，以检测被访问的内存页。

在开始编码之前，阅读xv6书籍的第3章，以及相关文件：

+ 'kernel/memlayout.h'，其中包含了内存布局的信息。
+ 'kernel/vm.c'，其中包含大部分虚拟内存（VM）代码。
+ 'kernel/kalloc.c'，其中包含用于分配和释放物理内存的代码。


#### 实验内容 1: 打印页表 （🍰🍰🍰）
编写函数用来打印进程页表的内容，定义一个名为'vmprint()'的函数。该函数接受一个'pagetable_t'参数，并以下面描述的格式打印该页表。在'exec.c'中，在'return argc'之前插入'if(p->pid==1) vmprint(p->pagetable)'，以打印第一个进程的页表。

##### 提示
+ 你可以将'vmprint()'放在'kernel/vm.c'中。
+ 使用位于'kernel/riscv.h'文件末尾的宏。
+ 函数'freewalk'可能会给你一些启发。
+ 在'kernel/defs.h'中定义'vmprint'的原型，以便你可以从'exec.c'中调用它。
+ 在你的'printf'调用中使用'%p'，以打印完整的64位十六进制PTE和地址。

##### 结果自测
启动xv6时，它应该输出类似下面的内容，描述了在执行init的exec()操作完成时，第一个进程的页表：
```
page table 0x0000000087f6b000
 ..0: pte 0x0000000021fd9c01 pa 0x0000000087f67000
 .. ..0: pte 0x0000000021fd9801 pa 0x0000000087f66000
 .. .. ..0: pte 0x0000000021fda01b pa 0x0000000087f68000
 .. .. ..1: pte 0x0000000021fd9417 pa 0x0000000087f65000
 .. .. ..2: pte 0x0000000021fd9007 pa 0x0000000087f64000
 .. .. ..3: pte 0x0000000021fd8c17 pa 0x0000000087f63000
 ..255: pte 0x0000000021fda801 pa 0x0000000087f6a000
 .. ..511: pte 0x0000000021fda401 pa 0x0000000087f69000
 .. .. ..509: pte 0x0000000021fdcc13 pa 0x0000000087f73000
 .. .. ..510: pte 0x0000000021fdd007 pa 0x0000000087f74000
 .. .. ..511: pte 0x0000000020001c0b pa 0x0000000080007000
init: starting sh
```
第一行显示了vmprint的参数。之后，每个PTE都有一行，包括指向树中更深层的页表页的PTE。每个PTE行都有一定数量的" .."缩进，指示其在树中的深度。每个PTE行显示了其页表页中的PTE索引、pte位以及从PTE中提取的物理地址。不要打印无效的PTE。在上面的示例中，顶层页表页有条目0和255的映射。下一个层级的条目0仅映射索引0，该索引0的底层页表页映射了条目0、1和2。

你的代码可能会产生与上面示例中不同的物理地址。但是，条目数和虚拟地址应该是相同的。

#### 实验内容 2：检测被访问的内存页 （🍰🍰🍰🍰）
某些垃圾回收器可以受益于关于哪些页面已被访问（读或写）的信息。因此需要你向xv6添加一个新功能，通过检查RISC-V页表中的访问位，检测并向用户空间报告这些信息。每当RISC-V硬件解析TLB未命中时，它会在PTE中标记相应的位。你的任务是实现系统调用'pgaccess()'，以报告哪些页面已经被访问。这个系统调用有三个参数，第一个参数是要检查的第一个用户页面的起始虚拟地址；第二个参数是要检查的页面数量；第三个参数是一个用户地址，用于将结果存储到一个位掩码中（一种数据结构，其中每个页面使用一个位表示，第一个页面对应最低有效位）。

##### 提示
+ 在Makefile中的UPROGS中添加"$U/_pgtbltest"。
+ 阅读'user/pgtbltest.c'中的'pgaccess_test()'，以了解如何使用'pgaccess'。
+ 首先在'kernel/sysproc.c'中实现'sys_pgaccess()'。
+ 你需要使用'argaddr()'和'argint()'来解析参数。
+ 对于输出的位掩码，最好在内核中存储一个临时缓冲区，并在填充正确的位后将其复制到用户空间（通过'copyout()'）。
+ 可以适当设置一个页面扫描的上限。
+ 'kernel/vm.c'中的'walk()'对于找到正确的PTE非常有用。
+ 你需要在'kernel/riscv.h'中定义PTE_A，即访问位。请查阅RISC-V特权架构手册以确定其值。
+ 确保在检查PTE_A是否被设置后清除它。否则，将无法确定自上次调用'pgaccess()'以来是否访问了该页面（即该位将永远被设置）。
+ 用于调试页表的'vmprint()'可能会有所帮助。

##### 结果自测
运行'pgtbltest'时'pgaccess'测试用例通过，则你完成了该实验。
