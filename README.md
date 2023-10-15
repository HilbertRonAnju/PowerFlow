# 牛顿-拉夫逊法潮流计算程序设计

## 程序设计要求

1. 编程语言c++；
2. 使用稀疏矩阵技术和节点优化编号（T2方案）；
3. IEEE 5、14、30、57、118和300节点系统的计算及结果；
4. 提交程序源代码及编写说明，提交6个系统的计算结果

## 文件结构
项目文件结构如下：

```
C:\USERS\96547\DESKTOP\POWERFLOW
│  PowerFlow.exe
│  PowerFlow.sln
│  README.md
│  
├─Data
│      IEEE118.txt
│      IEEE14.txt
│      IEEE30.txt
│      IEEE300.txt
│      IEEE5.txt
│      IEEE57.txt
│      
├─Debug
│      PowerFlow.exe
│      PowerFlow.pdb
│      
├─Intermediates
│          
├─Result
│      IEEE118_Result.txt
│      IEEE14_Result.txt
│      IEEE300_Result.txt
│      IEEE30_Result.txt
│      IEEE57_Result.txt
│      
└─Source
        ConductanceMatrixGenerater.cpp
        DataLoader.cpp
        DataStruct.h
        Main.cpp
        NewtonMethod.cpp
        NodeOptimization.cpp
        PowerFlow.h
        ResultOutput.cpp
        Variables.h
```
```\\Data```文件夹存储算例数据

```\\Result ```文件夹存储结果数据

```\\Source ```文件夹存储源代码

```\\Intermediates ```文件夹存储编译产生的中间文件

```\\Debug ```文件夹存储输出的exe文件

## 项目环境

### 开发工具为visual studio 2022，编译平台为X86，编译选项为Debug
