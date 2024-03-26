# 简介
C语言文本迷宫游戏.

## 依赖结构
```
main.c
├ maze_gen.c : 迷宫的生成, 移动, 迷宫相关的类型
├ keys_input.c : 处理用户按键输入, 计时功能(或许)
│ ├ [maze_gen]
│ └ [rendering]
├ rendering.c : 迷宫的绘制, 界面的绘制
│ └ [maze_gen]
...
```

# 界面设计
```
┌───────┬─────────────────┐
│ ┌─┬─┐ │time: 00: 00: 00 │
│ ├─┼─┤ ├─────────────────┤
│ ├─┼─┤ │press [any] key. │
│ ├─┼─┤ ├─────────────────┤
│ └─┴─┘ │ [Q/q]: quit.    │
└───────┴─────────────────┘
```

# 功能设计
## 计时功能
1. 获取时间
   1. "time.h"库中的`clock_t clock()`函数
      ```C
      int main () {
        clock_t start, end;
        long time_ms; // 64位平台下, int 4byte, long 8byte.
        start = clock();
        // 做某些事情
        end = clock();
        time_ms = (end - start) * 1000 / CLOCKS_PER_SEC; 
      }
      ```
      问题: 不精确
   2. "sys/time.h"库中的`gettimeofday`函数(过时)
   3. 使用"time.h"库中的`clock_gettime`函数: 更好的精度和可移植性
2. 使用"unistd.h"中的`int usleep(useconds_t usec)`函数, 参考[CSDN文章: Linux中的休眠函数](https://blog.csdn.net/q28292929/article/details/127665877)
3. 使用"pthread.h"开一个线程, 使用`pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)`跟`pthread_cancel(pthread_t thread)`函数
   ```C
   pthread_t tid;
   pthread_create(&tid, NULL, start_routine, NULL);
   // 做了某事
   pthread_cancel(start_routine);
   pthread_join(start_routine, NULL);
   ```

## 游戏重置功能
1. 将要在init_gameboard中判断窗口是否已经存在，若已经存在，则先销毁（以及清空）后新建
2. 在游戏循环中添加`p/P`暂停键功能，暂停期间可以设置迷宫参数
   - 迷宫参数保存在变量（结构体）内; 
   - 默认设置作为全局变量保存，设置参数保存在相同类型变量中
   - 当确认更改并重启游戏时，重新配置全局变量，并初始化迷宫
3. 关于控制游戏流程的方案
   1. 定义枚举体`enum GAME_STATE = {exit_state, default_on_state, customed_on_state};`, 保存自定义参数的变量同样为全局变量（静态变量）
   2. 将配置参数保存在临时分配结构体中，如果以默认参数重新开始游戏，则复制全局变量指针(危险，有误释放的风险)

## 信息功能
1. 将信息保存为字符串常量数组`const massages = ["任意键开始", "往右下角冲", "你赢了，新游戏?"];`
2. 将与信息对应的阶段保存在枚举体中`enum MSG_STATE = {ready_msg, game_on_msg, restart_msg}'`


