# shellcode_generate-test_framework

非原创，来自 https://www.freebuf.com/articles/web/231663.html

本次工程创建是win32空项目，使用的是VS2015编译relase/x86，编译之前进行如下设置：

1. 创建工程时关闭SDL检查

2. 属性->C/C++->代码生成->运行库->多线程 (/MT)如果是debug则设置成MTD

3. 属性->常规->平台工具集->设置为Visual Studio 2015- Windows XP (v140_xp)，如果没有则可以去安装上对应的兼容xp的组件

4. 属性->C/C++->代码生成->禁用安全检查GS

5. 关闭生成清单 属性->链接器->清单文件->生成清单 选择否

6. C/C++ -> 语言 -> 禁用语言扩展 选择否

7. C/C++ -> 语言 -> 符合模式 选择否
