# 简介
C语言文本迷宫游戏.

# 依赖结构
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

# 计时功能
使用"unistd.h"中的`int usleep(useconds_t usec)`函数, 参考[CSDN文章: Linux中的休眠函数](https://blog.csdn.net/q28292929/article/details/127665877)

