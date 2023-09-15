# Spinning Cube

This small project display a spinning cube on the LCD without any lib such as opengl. 

The code runs on the imx6ull embedfire development board, and may also works on alientec board. 

main.c initializes controls the LCD and spinning.c calculates the coordinate. 

***

# 旋转立方体

这个小项目的目标是在不使用 opengl 的情况下在 LCD 上显示一个旋转的立方体。

使用的硬件平台是野火的 imx6ull 开发板，因为野火和正点原子的都是基于 NXP 的 imx6ull 开发板，所以应该也能在正点原子的开发板上跑。

文件结构很简单，main.c 初始化了 LCD，包括打开 fb0、将其映射到内存空间等，spinning.c 包含立方体的坐标计算等。

直接 make 应该就能用。
