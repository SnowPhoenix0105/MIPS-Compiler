# CLMC

## 简介

C-like MIPS Compiler

用于北京航空航天大学2020年秋季编译技术课课程设计，将给定文法的C-like语言编译为MIPS汇编。

注意本项目采用了GPL协议。

请学弟学妹们不要抄袭。

作者：北京航空航天大学 高等理工学院 邓新宇

email：1595763343@qq.com

@雪凰SnowPhoenix

## 写在前面

申优文档的提交ddl是第一次竞速作业的ddl, 而不是第二次竞速作业ddl, 也就是说竞速结果还没有出来就得提交申优文档了. 我以为出了结果才提交文档, 所以错过了, 所以心态炸了, 很多写了一半的优化也没写完, 就这样了.

## 环境

使用 `Visual Studio 2019 Community` 开发，需要C++桌面开发相关组件，需要安装可选组件中的clang编译器支持, 采用 `Microsoft CppUnitTest` 进行单元测试和整体黑箱测试，整体黑箱测试依赖于MARS4.5，请从[MARS官网](http://courses.missouristate.edu/kenvollmar/mars/)下载 `Mars4_5.jar` 并将其置于 `Test\TestResource\blackbox_test` 文件夹下。(后改为课程组下发的MARS)

## 结构安排

* `MIPS-Compiler` 文件夹下为编译器主工程, 目标为可执行文件；
* `Develop` 文件夹下为工作项目，输出为lib；
* `Test` 文件夹下为 `Microsoft CppUnitTest` 项目，对Develop项目进行单元测试，包括单元测试和总体的黑箱测试，黑箱测试将通过MARS进行运行；
* `Documents` 文件夹下为设计文档，不包括课程组下发的指导手册；

`MIPS-Compiler`和`Develop`各有一份`global_control.h`用于控制全局的条件编译。

使用`prepare_to_release.py`将`Develop`下的源文件和头文件拷贝至`MIPS-Compiler`文件夹, 并打包zip至`Zips`文件夹.

`Develop`与`Test`两个项目采用`gbk`编码, 使用MSVC编译, `MIPS-Compiler`项目采用`utf-8`编码并使用clang编译, 二者之间通过`prepare_to_release.py`自动转码.

## 测试注意

注意测试工程中测试资源路径都是写死的, 请先进行修改.

