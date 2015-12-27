# How to use Xilinx Design Suite on Windows 8

## 1 Problem
* 使用windows 8系统的同学在使用Xilinx系列的开发套件（例如ISE，XPS，SDK等）之时，常常会遇到各种不兼容的现象，例如打开工程闪退，无法生成bitstream等。不过细心的同学可能已经找到了原因所在：是因为Xilinx开发套件的64bit版本和windows 8系统有冲突，从而引发了诸多问题。解决方法也很简单，只要打开相应开发套件的32bit版本就好了。

* 确实，到现在为止一切都很顺利。在大二大三的课程中，我们一直在使用Xilinx开发套件中的ISE软件，同时ISE软件是公开出了一个32bit的版本接口的，我们只要在开始菜单中选定32bit的ISE进行launch就可以了。

* 然而在本次课程中，我们使用的XPS和SDK软件就没有那么幸运了——Xilinx开发套件并没有在开始菜单中公开出一个XPS和SDK的32bit接口，所以我们不能简单地从开始菜单中找到32bit版本的XPS和SDK进行打开，google一番后也并无查获，只能自行尝试，下面列出我的solution。

## 2 Solution
在下面的叙述中，我使用`$XILINX_HOME`来代指你的xilinx系列软件安装根目录，用`$VERSION`来指代你的xilinx系列软件安装版本。

* 实际上，xilinx也是有开发出32bit版本的XPS和SDK版本的，那么我们怎么打开呢？很简单，我们直接从安装目录中的bin目录下执行可执行文件。

* 其中，32bit XPS的地址为`$XILINX_HOME/$VERSION/ISE_DS/EDK/bin/nt/xps.exe`，32bit SDK的地址为`$XILINX_HOME/$VERSION/ISE_DS/EDK/bin/nt/xsdk.exe`，直接执行即可。<br>
*（Note: 如果你仔细观察的话，会发现其实64bit XPS的地址为`$XILINX_HOME/$VERSION/ISE_DS/EDK/bin/nt64/xps.exe`，64it SDK的地址为`$XILINX_HOME/$VERSION/ISE_DS/EDK/bin/nt64/xsdk.exe`）*

* 当你打开上述的exe文件后，可能会遇到两个问题：**提示某个xilinx相关环境变量（PATH）缺失** 以及 **提示缺失特定lib文件**。<br>
其中环境变量缺失的问题网上已经有了很多solution，属于比较简单的问题，在此不加以赘述，可自行google。<br>
如果是提示缺失特定lib文件的话，可以去`$XILINX_HOME/$VERSION/ISE_DS/common/lib/nt`下进行查找，并把缺失的同名lib文件复制到`$XILINX_HOME/$VERSION/ISE_DS/EDK/lib/nt`下即可。

## 3 Check
* 在顺利打开XPS和SDK软件后，可通过打开`Help --> About...`来查看现在的软件版本，如果软件版本显示为nt的话则为成功打开为32位版本。（同理，nt64则为还是64位版本）

* Enjoy your coding.
