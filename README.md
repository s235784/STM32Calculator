[![GitHub license][1]][2]

[1]:https://img.shields.io/badge/license-Apache%202-blue.svg
[2]:https://github.com/hegaojian/JetpackMvvm/blob/master/LICENSE

# STM32Calculator
 一个用STM32F407和串口屏制作的简单计算器，能够进行四则运算、三角函数运算、指对数运算，且有正确的计算优先级。
 
 
 ## 硬件
 单片机：STM32F407VET6
 
 串口屏：淘晶驰 TJC4827X343_011R
 
 ## 连接方式
 串口屏RX接单片机PA9/TXD1
 
 串口屏TX接单片机PA10/RXD1
 
 
 ## 效果图
 
 <img src="https://raw.githubusercontent.com/s235784/STM32Calculator/main/document/ui.png" alt="ui" width="400">
 
 ## 参考
 
 算法部分参考自 [一个用C实现的科学计算器 - Designer_DRK](https://blog.csdn.net/Designer_DRK/article/details/52749016)
 
 UI部分参考自 [简易计算器 - USART HMI 资料中心](http://wiki.tjc1688.com/doku.php?id=7.%E5%B7%A5%E7%A8%8B%E6%A0%B7%E4%BE%8B:3.%E7%BD%91%E5%8F%8B%E6%8F%90%E4%BE%9B%E5%BA%94%E7%94%A8%E6%A0%B7%E4%BE%8B)


## License
``` license
 Copyright 2021, NuoTian       
  
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at 
 
       http://www.apache.org/licenses/LICENSE-2.0 

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
```
