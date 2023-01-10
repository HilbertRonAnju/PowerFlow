#pragma once

#ifndef DataStruct_H__
#define DataStruct_H__

struct Branch_Type  //����֧·�ṹ��
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
struct Injection_Line_Type    //ע��֧·
{
	int i, j;
};
struct Branch_Node_Type             //�洢�Ż������֧·�����˽ڵ��,�������ӵ�֧·
{
	int i, j;
};
struct Node_Power_Type   //���N���ڵ���м�������й����ʺ��޹����ʣ�
{
	double P, Q;
};
struct Yii_Type             //�洢N���Խ�Ԫ��
{
	double G, B;
};
struct Yij_Type            //�洢�ǶԽ�Ԫ��
{
	double G, B;
};
struct Node_Voltage_Type   //���ڵ�ĵ�ѹ�����
{
	double V, theta;
};
struct Yii_Of_Temp_Branch_Type //�洢��ʱ֧·�ĵ���
{
	double G, B;
};
struct Yij_Of_Temp_Branch_Type
{
	double G, B;
};

#endif
