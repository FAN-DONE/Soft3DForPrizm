# Soft3DForPrizm

3D软光栅-C新手的图形学尝试<br>
An attempt on the Computer Graphics using C<br> 

这是什么？
=========
一个简单的软件3D引擎,使用C语言实现(有极少量的C++).如名,Soft3D是为[Casio Prizm](https://www.casio.com.cn/cal/graphic/fx-CG50/index.html)系列计算器设计的,但也提供了PC版本(使用SDL2库显示).<br>
Soft3D是个烂大街的名字,可能会和其他库名字重合.注意,两者不是一个东西.<br>

特性
===
* 无依赖:库的本身无任何依赖,包括标准库,因此易于移植.当然,显示、窗口、malloc这种依赖于操作系统的除外
* 轻量:但为了提高速度过多地使用了static inline
* 傻瓜式:我只是个萌新
* 优化:大量的定点数运算,但在PC上没什么效果

实现
===
* 变换:平移 缩放 旋转 简单透视
* 漫反射光照
* OBJ模型加载

目前
===
**施工中**

额外的信息
=========
Casio Prizm系列计算器搭载Casio设计的微小系统,允许通过USB安装插件(ADD-IN).<br>
该系统提供了一组系统调用(SYSCALL),插件拥有很高的权限.<br>
CPU架构SH4A,正常频率55MHZ.RAM有2MB,其中162KB为显存(主屏幕尺寸384\*216,像素颜色使用RGB565编码,占2字节).<br>
ROM为32MB,使用FAT16文件系统.<br>
[PrizmWiKi](https://prizm.cemetech.net/index.php?title=Main_Page)提供了包括Prizm编程的很多有用信息.<br>
