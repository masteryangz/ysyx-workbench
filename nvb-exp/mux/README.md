# 示例工程

先设置环境变量`NVBOARD_HOME`为NVBoard项目的路径, 然后执行`make run`.

该示例的演示效果如下:
1. LED0 and LED1 are outputs of 41mux with input sw[9:2] as 4 2bit data and sw[1:0] as key
1. 8个数码管流水显示数字0-7
1. 窗口左下角为VGA输出, 将会展示一张图片
1. 敲击键盘, 终端将会输出按键的扫描码
1. 窗口右侧中部为UART终端
   * 鼠标选中后光标变为粉红色, 此时的按键输入将被UART RX端捕捉
   * UART RX端发送的输入将通过回环连接从UART TX端输出, 并在UART终端上显示
   * 鼠标点击UART终端以外的其他位置, 可取消选中UART终端, 光标变为黑色, 后续按键输入将被PS/2键盘捕捉
