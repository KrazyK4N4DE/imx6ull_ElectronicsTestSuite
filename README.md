# 电子设备模块测试与烧写工具

这是一个应用层软件项目，项目原名为《电子产品量产工具》([百问网](https://www.100ask.net/p/t_pc/course_pc_detail/column/p_5f04515ce4b036f1c0cf4254?product_id=p_5f04515ce4b036f1c0cf4254)，[B站](https://www.bilibili.com/video/BV1it4y1Q75z/?vd_source=11fdf4ddf6712b19eba94ccd395578f0))，作者为韦东山老师。本人基于学习的需要与兴趣，对项目进行复现，并增加了一点点的改进。

该软件用于实际生产中，能够对电子设备各模块(也可以自定义)自动进行测试、烧写系统，并将结果显示在屏幕。

- 通过配置文件gui.conf自定义测试项。
- 每个测试项可以添加command，即测试项的按钮状态发生变化时，运行对应的脚本。
- 每个测试项的测试程序及command脚本本项目不涉及，需自己提供。

![整体](./docs/pics/%E6%95%B4%E4%BD%93.jpg) | ![效果](./docs/pics/%E6%95%88%E6%9E%9C.jpg)
---|---

## 涉及知识

- C基础以及程序结构设计的思维
- framebuffer编程
- tslib与freetype库的使用
- socket(udp)编程
- 多线程编程

## 设备及环境

硬件：imx6ull_pro，LCD屏幕
环境：Ubuntu 18.04.1
交叉编译工具链：arm-buildroot-linux-gnueabihf-gcc v7.5.0
第三方库：tslib，freetype

## 使用

进入build目录，进行编译：

```sh
cmake ..
make
arm-buildroot-linux-gnueabihf-gcc ../unittest/client.c -o client
```

编译结束后，在开发板挂载目录，进入build目录，运行程序：

```sh
./main ../simsun.ttc 
```

## 目录结构

.
├── build  # 编译好的文件
├── docs  # 文档与材料
├── etc  # 配置文件或其他
├── include  # 头文件
├── src  # 源文件代码
└── unittest  # 单元测试与测试程序

## TODO / 改进

- [x] 在程序启动时自动清理背景、结束时清屏 (加入memset和signal函数实现)
- [x] CMake替代Makefile
