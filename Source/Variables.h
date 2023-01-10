#pragma once
#pragma once

#ifndef VARIABLES_H__
#define VARIABLES_H__


#ifdef GLOBALS
int MAXIMUM_ITERATION_TIME;     //����������
double EPS;        //��������
double MAXIMUM_ERROR;   //��������
int N, Branch_Number, Transformer_Number, Generator_Number, Load_Number; //ϵͳ�ڵ���������·֧·������ѹ�������������ڵ��������ɽڵ���
int i, j, k, t, m, n, c;
int Branch_Not_Ground;           //���ӵ���·��
int Total_Branch_Number;        //�ܵ�֧·���������ӵ���·���ͱ�ѹ��֧·��
int Total_Branch_Number_Not_Ground;       //�ܵ�֧·�����������ӵ�֧·��
int Branch_Injection_Number;     // ע��֧·��
int OFF_Diagonal_Number;    //�Ż�������зǶԽ�Ԫ�ظ���, �����ӵ�֧·
int OFF_Diagonal_Number_Not_Ground;   //�Ż�������зǶԽ�Ԫ�ظ���,�������ӵ�֧·
int* Node_Sequence;  //�Ż���Ľڵ�˳���
int Not_Zero_And_Off__Diagonal_Node_Number;          //����ǶԽ�Ԫ�ظ���
int* Branch_Col_Sub;       //�洢ÿ��֧·�����±�
int* Off_Diagonal_Element_Start_Address;         //�洢ÿ�зǶԽ�Ԫ�ص��׵�ַ
int* Off_Diag_Number_in_Row;         //���зǶԽ�Ԫ�صĸ���
int Balance_Node_Order_Number;   //ƽ��ڵ��Ż���ĵ��
int Iteration_Time;        //��������

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
extern int MAXIMUM_ITERATION_TIME;     //����������
extern double EPS;        //��������
extern double MAXIMUM_ERROR;   //��������
extern int N, Branch_Number, Transformer_Number, Generator_Number, Load_Number; //ϵͳ�ڵ���������·֧·������ѹ�������������ڵ��������ɽڵ���
extern int i, j, k, t, m, n, c;
extern int Branch_Not_Ground;           //���ӵ���·��
extern int Total_Branch_Number;        //�ܵ�֧·���������ӵ���·���ͱ�ѹ��֧·��
extern int Total_Branch_Number_Not_Ground;       //�ܵ�֧·�����������ӵ�֧·��
extern int Branch_Injection_Number;     // ע��֧·��
extern int OFF_Diagonal_Number;    //�Ż�������зǶԽ�Ԫ�ظ���, �����ӵ�֧·
extern int OFF_Diagonal_Number_Not_Ground;   //�Ż�������зǶԽ�Ԫ�ظ���,�������ӵ�֧·
extern int* Node_Sequence;  //�Ż���Ľڵ�˳���
extern int Not_Zero_And_Off__Diagonal_Node_Number;          //����ǶԽ�Ԫ�ظ���
extern int* Branch_Col_Sub;       //�洢ÿ��֧·�����±�
extern int* Off_Diagonal_Element_Start_Address;         //�洢ÿ�зǶԽ�Ԫ�ص��׵�ַ
extern int* Off_Diag_Number_in_Row;         //���зǶԽ�Ԫ�صĸ���
extern int Balance_Node_Order_Number;   //ƽ��ڵ��Ż���ĵ��
extern int Iteration_Time;        //��������

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