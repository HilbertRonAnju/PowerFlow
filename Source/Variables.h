#pragma once
#pragma once

#ifndef VARIABLES_H__
#define VARIABLES_H__


#ifdef GLOBALS
int MAXIMUM_ITERATION_TIME;     //最大迭代次数
double EPS;        //收敛精度
double MAXIMUM_ERROR;   //最大功率误差
int N, Branch_Number, Transformer_Number, Generator_Number, Load_Number; //系统节点总数，线路支路数，变压器数，发动机节点数，负荷节点数
int i, j, k, t, m, n, c;
int Branch_Not_Ground;           //不接地线路数
int Total_Branch_Number;        //总的支路数，包括接地线路数和变压器支路数
int Total_Branch_Number_Not_Ground;       //总的支路数，不包括接地支路数
int Branch_Injection_Number;     // 注入支路数
int OFF_Diagonal_Number;    //优化后的所有非对角元素个数, 包括接地支路
int OFF_Diagonal_Number_Not_Ground;   //优化后的所有非对角元素个数,不包括接地支路
int* Node_Sequence;  //优化后的节点顺序号
int Not_Zero_And_Off__Diagonal_Node_Number;          //非零非对角元素个数
int* Branch_Col_Sub;       //存储每条支路的列下标
int* Off_Diagonal_Element_Start_Address;         //存储每行非对角元素的首地址
int* Off_Diag_Number_in_Row;         //各行非对角元素的个数
int Balance_Node_Order_Number;   //平衡节点优化后的点号
int Iteration_Time;        //迭代次数

struct Branch_Type* Branch;
struct Transformer_Type* Transformer;
struct Generator_Type* Generator;
struct Load_Type* Load;
struct Injection_Line_Type New_line[500];
struct Node_Voltage_Type* Node_Voltage;
struct Yij_Type* Yij;
struct Yii_Type* Yii;
struct Branch_Node_Type* Branch_Node;
struct Node_Power_Type* Node_Power;

#else
extern int MAXIMUM_ITERATION_TIME;     //最大迭代次数
extern double EPS;        //收敛精度
extern double MAXIMUM_ERROR;   //最大功率误差
extern int N, Branch_Number, Transformer_Number, Generator_Number, Load_Number; //系统节点总数，线路支路数，变压器数，发动机节点数，负荷节点数
extern int i, j, k, t, m, n, c;
extern int Branch_Not_Ground;           //不接地线路数
extern int Total_Branch_Number;        //总的支路数，包括接地线路数和变压器支路数
extern int Total_Branch_Number_Not_Ground;       //总的支路数，不包括接地支路数
extern int Branch_Injection_Number;     // 注入支路数
extern int OFF_Diagonal_Number;    //优化后的所有非对角元素个数, 包括接地支路
extern int OFF_Diagonal_Number_Not_Ground;   //优化后的所有非对角元素个数,不包括接地支路
extern int* Node_Sequence;  //优化后的节点顺序号
extern int Not_Zero_And_Off__Diagonal_Node_Number;          //非零非对角元素个数
extern int* Branch_Col_Sub;       //存储每条支路的列下标
extern int* Off_Diagonal_Element_Start_Address;         //存储每行非对角元素的首地址
extern int* Off_Diag_Number_in_Row;         //各行非对角元素的个数
extern int Balance_Node_Order_Number;   //平衡节点优化后的点号
extern int Iteration_Time;        //迭代次数

extern struct Branch_Type* Branch;
extern struct Transformer_Type* Transformer;
extern struct Generator_Type* Generator;
extern struct Load_Type* Load;
extern struct Injection_Line_Type New_line[500];
extern struct Node_Voltage_Type* Node_Voltage;
extern struct Yij_Type* Yij;
extern struct Yii_Type* Yii;
extern struct Branch_Node_Type* Branch_Node;
extern struct Node_Power_Type* Node_Power;
#endif 

#endif