# CLMC

## 简介

C-like MIPS Compiler

用于北京航空航天大学2020年秋季编译技术课课程设计，将给定文法的C-like语言编译为MIPS汇编。

注意本项目采用了GPL协议。

请学弟学妹们不要抄袭。

作者：北京航空航天大学 高等理工学院 邓新宇

email：1595763343@qq.com

@雪凰SnowPhoenix

## 环境

使用 `Visual Studio 2019 Community` 开发，需要C++开发相关组件，采用 `Microsoft CppUnitTest` 进行单元测试和整体黑箱测试，整体黑箱测试依赖于MARS4.5，请从[MARS官网](http://courses.missouristate.edu/kenvollmar/mars/)下载 `Mars4_5.jar` 并将其置于 `BlackboxTest\TestResource` 文件夹下。由于课程OJ平台采用Clang编译器，所以姑且认为Clang可以正确编译CLMC工程下的内容。

## 结构安排

* `CLMC` 文件夹下为编译器主工程CLMC；
* `UnitTest` 文件夹下为 `Microsoft CppUnitTest` 项目，用于单元测试；
* `BlackboxTest` 文件夹下为黑箱测试，使用CLMC编译后用MARS进行汇编并模拟执行，将执行结果与期望输出进行对比；
* `Documents` 文件夹下为设计文档，不包括课程组下发的指导手册；



