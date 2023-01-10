#pragma once

#ifndef DataStruct_H__
#define DataStruct_H__

struct Branch_Type  //定义支路结构体
{
	int i, j;
	double R, X, Y;
};
struct Transformer_Type
{
	int i, j;
	double R, X, K;
};
struct Generator_Type
{
	int i, type;
	double P, Q, V;
};
struct Load_Type
{
	int i, type;
	double P, Q, V;
};
struct Injection_Line_Type    //注入支路
{
	int i, j;
};
struct Branch_Node_Type             //存储优化后各个支路的两端节点号,不包括接地支路
{
	int i, j;
};
struct Node_Power_Type   //存放N个节点的中间变量（有功功率和无功功率）
{
	double P, Q;
};
struct Yii_Type             //存储N个对角元素
{
	double G, B;
};
struct Yij_Type            //存储非对角元素
{
	double G, B;
};
struct Node_Voltage_Type   //各节点的电压和相角
{
	double V, theta;
};
struct Yii_Of_Temp_Branch_Type //存储临时支路的导纳
{
	double G, B;
};
struct Yij_Of_Temp_Branch_Type
{
	double G, B;
};

#endif
