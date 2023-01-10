/***************************************************************
 * @file       Main.cpp
 * @brief      牛顿-拉夫逊法潮流计算程序
 * @author     聂永欣
 * @date       2023年1月9日
 **************************************************************/

#define GLOBALS
#include "PowerFlow.h"

int main()
{
	int IEEE_type;
	bool Result_Acquire = true;
	Initialization_Imformation();
	IEEE_type = IEEE_Type_Select();
	PowerFlow(IEEE_type);
	cin.get();
}

void Initialization_Imformation()
{
	cout << "***********************************************" << endl;
	cout << "牛顿-拉夫逊法潮流计算程序" << endl;
	cout << "作者：聂永欣" << endl;
	cout << "班级：硕2064" << endl;
	cout << "学号：3122104125" << endl;
	cout << "导师：刘俊 教授" << endl;
	cout << "***********************************************" << endl;
}

int IEEE_Type_Select()
{
	int IEEE_type;
	bool False_Input;
	cout << "请输入要计算的算例节点数:" << endl;
	cout << "输入格式:5，14，30，57，118，300" << endl;
	do
	{
		cin >> IEEE_type;
		False_Input = (
			IEEE_type != 5 && \
			IEEE_type != 14 && \
			IEEE_type != 30 && \
			IEEE_type != 57 && \
			IEEE_type != 118 && \
			IEEE_type != 300 \
			);
		if (False_Input)
		{
			cout << "输入错误，不存在IEEE" << IEEE_type << "节点算例数据文件!" << endl;
			cout << "请重新输入要计算的算例节点数：" << endl;
		}
	} while (False_Input);
	cout << "开始计算IEEE：" << IEEE_type << "节点算例" << endl;
	cout << "***********************************************" << endl;
	return IEEE_type;
}

void PowerFlow(int IEEE_type)
{
	clock_t begin, finish;
	bool File_Loaded = false; //判断数据文件是否正常打开
	// 开始计时
	begin = clock();

	File_Loaded = Data_Load(IEEE_type); // 导入系统数据
	if (File_Loaded)
	{
		Node_Optimization(); // 节点编号优化
		Conductance_Matrix_Generater(); // 导纳矩阵生成
		Newton_Iteration(); // 牛顿法潮流计算
		Result_Output(IEEE_type);
	}

	// 结束计时
	finish = clock();
	double endtime = (double)(finish - begin) / CLOCKS_PER_SEC;
	cout << "牛顿-拉夫逊法计算IEEE" << IEEE_type << "节点算例所花费的时间为：" << endtime << "s" << endl;
	system("pause");
}