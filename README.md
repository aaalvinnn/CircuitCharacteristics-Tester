# CircuitCharacteristics-Tester
a simple circuit characteristics tester which can provide Rin, Rout, amp-fre and fault-detection.  
通过软件SPI方式控制信号发生器ad9833输出可变频正弦波
# 涉及知识点
* 封装好了的AD采样函数（都采用全局变量传参）;
* 带数字电位器的DDS驱动: ad9833与mcp41010
* 通过串口屏进行输入和输出， 中断方式单片机接收串口输入并执行相应服务函数框架
* 扫频
