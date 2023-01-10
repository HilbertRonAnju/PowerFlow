/***************************************************************
 * @file       ConductanceMatrixGenerater.cpp
 * @brief      输出计算结果
 * @author     聂永欣
 * @date       2023年1月9日
 **************************************************************/

#include "PowerFlow.h"

void Conductance_Matrix_Generater()
{
	double Magnitude;      //向量模值的平方
	OFF_Diagonal_Number = Branch_Injection_Number + Total_Branch_Number;    //优化后的所有非对角元素个数, 包括接地支路
	OFF_Diagonal_Number_Not_Ground = Branch_Injection_Number + Total_Branch_Number_Not_Ground;  //优化后的所有非对角元素个数,不包括接地支路
	Not_Zero_And_Off__Diagonal_Node_Number = 1;              //记录非零非对角元素个数 

	Off_Diag_Number_in_Row = new int[N + 1];             //各行非对角元素的个数
	Yii = new Yii_Type[N + 1];          //存储N个对角元素
	Yij = new Yij_Type[OFF_Diagonal_Number_Not_Ground + 1];//假定为N_all_line1个，实际上要少双回线的个数
	Branch_Col_Sub = new int[OFF_Diagonal_Number_Not_Ground + 1];      //存储每条支路的列下标
	Off_Diagonal_Element_Start_Address = new int[N + 1];                //存储每行第一个元素在Yij中的位置，即每行首地址
	Branch_Node = new Branch_Node_Type[OFF_Diagonal_Number_Not_Ground + 1];//存储优化后各个支路（不包括接地支路）的两端节点号
	Yii_Of_Temp_Branch_Type* Y_Of_Temp_Branch;
	Y_Of_Temp_Branch = new Yii_Of_Temp_Branch_Type[OFF_Diagonal_Number_Not_Ground + 1];

	for (i = 1; i <= N; i++)               //初始化对角元素
	{
		Yii[i].G = 0;
		Yii[i].B = 0;
	}
	Magnitude = 0;
	for (i = 1; i <= Branch_Number; i++)              //将每条线路支路读入到上面定义的临时数组中，其中包括接地支路
	{
		if (Branch[i].i == Branch[i].j) //此时是接地支路的情况
		{
			if (Branch[i].Y < 0)        //Branch[i].Y<0说明Branch[i].X是接地支路的电纳
			{
				Yii[Node_Sequence[Branch[i].i]].G = Yii[Node_Sequence[Branch[i].i]].G + 0;
				Yii[Node_Sequence[Branch[i].i]].B = Yii[Node_Sequence[Branch[i].i]].B + Branch[i].X;
			}
			else
			{
				Yii[Node_Sequence[Branch[i].i]].G = Yii[Node_Sequence[Branch[i].i]].G + 0;
				Yii[Node_Sequence[Branch[i].i]].B = Yii[Node_Sequence[Branch[i].i]].B - 1 / Branch[i].X;
			}
			continue;
		}
		if (Node_Sequence[Branch[i].i] < Node_Sequence[Branch[i].j])
		{
			Branch_Node[Not_Zero_And_Off__Diagonal_Node_Number].i = Node_Sequence[Branch[i].i];
			Branch_Node[Not_Zero_And_Off__Diagonal_Node_Number].j = Node_Sequence[Branch[i].j];
		}
		else
		{
			Branch_Node[Not_Zero_And_Off__Diagonal_Node_Number].i = Node_Sequence[Branch[i].j];
			Branch_Node[Not_Zero_And_Off__Diagonal_Node_Number].j = Node_Sequence[Branch[i].i];
		}
		Magnitude = Branch[i].R * Branch[i].R + Branch[i].X * Branch[i].X;
		Y_Of_Temp_Branch[Not_Zero_And_Off__Diagonal_Node_Number].G = -Branch[i].R / Magnitude;
		Y_Of_Temp_Branch[Not_Zero_And_Off__Diagonal_Node_Number].B = Branch[i].X / Magnitude;
		Yii[Node_Sequence[Branch[i].i]].G = Yii[Node_Sequence[Branch[i].i]].G + Branch[i].R / Magnitude;
		Yii[Node_Sequence[Branch[i].i]].B = Yii[Node_Sequence[Branch[i].i]].B + Branch[i].Y - Branch[i].X / Magnitude;
		Yii[Node_Sequence[Branch[i].j]].G = Yii[Node_Sequence[Branch[i].j]].G + Branch[i].R / Magnitude;
		Yii[Node_Sequence[Branch[i].j]].B = Yii[Node_Sequence[Branch[i].j]].B + Branch[i].Y - Branch[i].X / Magnitude;
		Not_Zero_And_Off__Diagonal_Node_Number = Not_Zero_And_Off__Diagonal_Node_Number + 1;
	}

	for (i = 1; i <= Transformer_Number; i++)             //将每条变压器支路继续读入到上面定义的临时数组中
	{
		if (Node_Sequence[Transformer[i].i] < Node_Sequence[Transformer[i].j])
		{
			Branch_Node[Not_Zero_And_Off__Diagonal_Node_Number].i = Node_Sequence[Transformer[i].i];
			Branch_Node[Not_Zero_And_Off__Diagonal_Node_Number].j = Node_Sequence[Transformer[i].j];
		}
		else
		{
			Branch_Node[Not_Zero_And_Off__Diagonal_Node_Number].i = Node_Sequence[Transformer[i].j];
			Branch_Node[Not_Zero_And_Off__Diagonal_Node_Number].j = Node_Sequence[Transformer[i].i];
		}
		Magnitude = Transformer[i].R * Transformer[i].R + Transformer[i].X * Transformer[i].X;
		Y_Of_Temp_Branch[Not_Zero_And_Off__Diagonal_Node_Number].G = -Transformer[i].R / (Transformer[i].K * Magnitude);//因为这是变压器支路，所以互导纳应该除以变比K
		Y_Of_Temp_Branch[Not_Zero_And_Off__Diagonal_Node_Number].B = Transformer[i].X / (Transformer[i].K * Magnitude);
		Yii[Node_Sequence[Transformer[i].i]].G = Yii[Node_Sequence[Transformer[i].i]].G + Transformer[i].R / Magnitude;//读入标准侧导纳
		Yii[Node_Sequence[Transformer[i].i]].B = Yii[Node_Sequence[Transformer[i].i]].B - Transformer[i].X / Magnitude;
		Yii[Node_Sequence[Transformer[i].j]].G = Yii[Node_Sequence[Transformer[i].j]].G + Transformer[i].R / (Transformer[i].K * Transformer[i].K * Magnitude);//读入非标准侧导纳，此时应该除以变比K的平方
		Yii[Node_Sequence[Transformer[i].j]].B = Yii[Node_Sequence[Transformer[i].j]].B - Transformer[i].X / (Transformer[i].K * Transformer[i].K * Magnitude);
		Not_Zero_And_Off__Diagonal_Node_Number = Not_Zero_And_Off__Diagonal_Node_Number + 1;
	}

	for (i = 1; i <= Branch_Injection_Number; i++)                      //考虑注入元素
	{
		Y_Of_Temp_Branch[Not_Zero_And_Off__Diagonal_Node_Number].G = 0;
		Y_Of_Temp_Branch[Not_Zero_And_Off__Diagonal_Node_Number].B = 0;
		if (Node_Sequence[New_line[i].i] < Node_Sequence[New_line[i].j])
		{
			Branch_Node[Not_Zero_And_Off__Diagonal_Node_Number].i = Node_Sequence[New_line[i].i];
			Branch_Node[Not_Zero_And_Off__Diagonal_Node_Number].j = Node_Sequence[New_line[i].j];
		}
		else
		{
			Branch_Node[Not_Zero_And_Off__Diagonal_Node_Number].i = Node_Sequence[New_line[i].j];
			Branch_Node[Not_Zero_And_Off__Diagonal_Node_Number].j = Node_Sequence[New_line[i].i];
		}
		Not_Zero_And_Off__Diagonal_Node_Number = Not_Zero_And_Off__Diagonal_Node_Number + 1;
	}
	Not_Zero_And_Off__Diagonal_Node_Number = Not_Zero_And_Off__Diagonal_Node_Number - 1;

	Yij_Of_Temp_Branch_Type* Yij_Of_Temp_Branch;
	Yij_Of_Temp_Branch = new Yij_Of_Temp_Branch_Type[OFF_Diagonal_Number_Not_Ground + 1];//另一个存储临时支路的导纳,作用将同一行的元素放在一起

	int* Off_Diag_Element_Col_Number = new int[OFF_Diagonal_Number_Not_Ground + 1];                           //存储各个非对角元素的列号
	int p, q = 1;
	for (i = 1; i <= N; i++)
		Off_Diagonal_Element_Start_Address[i] = 0;
	for (i = 1; i <= N; i++)                                                  //将同一行的元素放在一起
	{
		p = 0;
		for (j = 1; j <= Not_Zero_And_Off__Diagonal_Node_Number; j++)
		{
			if (j != Not_Zero_And_Off__Diagonal_Node_Number && Branch_Node[j].i == Branch_Node[j + 1].i && Branch_Node[j].j == Branch_Node[j + 1].j && Branch_Node[j].i == i)
			{
				Yij_Of_Temp_Branch[q].G = Y_Of_Temp_Branch[j].G + Y_Of_Temp_Branch[j + 1].G;
				Yij_Of_Temp_Branch[q].B = Y_Of_Temp_Branch[j].B + Y_Of_Temp_Branch[j + 1].B;
				Off_Diag_Element_Col_Number[q] = Branch_Node[j].j;
				p = p + 1;
				q = q + 1;
				j = j + 1;
			}
			else if (Branch_Node[j].i == i)
			{
				Yij_Of_Temp_Branch[q].G = Y_Of_Temp_Branch[j].G;
				Yij_Of_Temp_Branch[q].B = Y_Of_Temp_Branch[j].B;
				Off_Diag_Element_Col_Number[q] = Branch_Node[j].j;
				p = p + 1;
				q = q + 1;
			}
		}
		Off_Diag_Number_in_Row[i] = p;
	}

	Off_Diagonal_Element_Start_Address[1] = 1;              //设第一行在Yij中的首地址为1
	for (i = 2; i <= N; i++)
		Off_Diagonal_Element_Start_Address[i] = Off_Diagonal_Element_Start_Address[i - 1] + Off_Diag_Number_in_Row[i - 1];

	m = 1;
	for (i = 1; i <= N; i++)
	{
		for (j = Off_Diagonal_Element_Start_Address[i]; j < Off_Diagonal_Element_Start_Address[i + 1]; j++)
		{
			q = Off_Diag_Element_Col_Number[Off_Diagonal_Element_Start_Address[i]];
			p = Off_Diagonal_Element_Start_Address[i];
			for (n = Off_Diagonal_Element_Start_Address[i] + 1; n < Off_Diagonal_Element_Start_Address[i + 1]; n++)
			{
				if (Off_Diag_Element_Col_Number[n] < q)
				{
					q = Off_Diag_Element_Col_Number[n];
					p = n;
				}
			}
			Yij[m].G = Yij_Of_Temp_Branch[p].G;
			Yij[m].B = Yij_Of_Temp_Branch[p].B;
			Branch_Col_Sub[m] = q;
			Off_Diag_Element_Col_Number[p] = N + 1;
			m = m + 1;
		}
	}
	cout << "导纳矩阵已生成！" << endl;
}