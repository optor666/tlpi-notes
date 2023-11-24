Downloads/812fa92405f19d7333cfbdccd53d724b.pdf
P1334 -> 非标准模式
1. 使用 Java、Python、C 写一个程序，测试非标准模式

# 终端
## 概念
1. 现在已经无法体验到早些年真正的终端啦，现在的硬件设备（打印机、显示器）都比较高级啦，操作系统提供的终端都是伪终端，也可以称之为终端模拟器。

# 终端驱动
1. 不论是真实终端、还是终端模拟器，都需要驱动程序处理终端设备的输入、输出。
2. 终端驱动通常提供两种模式：标准模式、非标准模式。
3. 终端驱动通常会操作两个队列：
    - 第一个：用于接收终端设备的输入，并将其传输给读取中的进程
    - 第二个：用于接收进程的输出，并将其发送给终端设备
    - 也就是说，有两个完全独立的线程处理上面两个队列的操作。
4. 
## 模式
### 标准模式
1. 按行读取。上层软件执行 read 操作时，会阻塞，直到用户按下回车键。
2. 在标准模式下，终端驱动通常会解释一系列特殊字符，例如：interrupt character (Control-C), the end-of-file character (normally Control-D)。终端驱动会将这些特殊字符解释为对前台进程组的信号或者正在从终端读取输入的输入条件。
### 非标准模式
1. 实时读取。使用这种模式的程序有：vi、more、less
2. 在非标准模式下，终端驱动通常不会解释特殊字符。

# 关联章节
1. 64. Pseudoterminals P1419

# 参考程序
1. vi
2. more
3. less

# 参考书籍
1. Strang, J. 1986. Programming with Curses. O’Reilly, Sebastopol, California.
2. Strang, J., Mui, L., and O’Reilly, T. 1988. Termcap & Terminfo (3rd edition). O’Reilly, Sebastopol, California.

