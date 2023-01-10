/***************************************************************
 * @file       NodeOptimization.cpp
 * @brief      节点优化(T-2方案)
 * @author     聂永欣
 * @date       2023年1月9日
 **************************************************************/

#include "PowerFlow.h"

void Node_Optimization()
{
	cout << "开始节点编号优化" << endl;

	Branch_Not_Ground = 0;        //统计不接地线路数
	for (i = 1; i <= Branch_Number; i++)
	{
		if (Branch[i].i == Branch[i].j)
			continue;
		else	 Branch_Not_Ground = Branch_Not_Ground + 1;
	}

	Total_Branch_Number = Branch_Number + Transformer_Number;          //总的支路数，包括接地线路数和变压器支路数
	Total_Branch_Number_Not_Ground = Branch_Not_Ground + Transformer_Number;        //总的支路数，不包括接地支路数，
	Branch_Injection_Number = 0;           // 注入支路数
	Node_Sequence = new int[N + 1];

	int* Node_Connection_Count;         //记录各节点出线个数
	Node_Connection_Count = new int[N + 1];
	for (i = 1; i <= N; i++)     //初始化各个节点的出线数
	{
		Node_Connection_Count[i] = 0;
	}
	int** Node_Connection_Matrix;    //存放各个节点连接支路对端的节点号，行为节点号，列为对应支路所连节点号
	Node_Connection_Matrix = (int**)new int[N + 1]();
	for (i = 1; i <= N; i++)
	{
		Node_Connection_Matrix[i] = new int[50];    //最大出线数设为50
	}


	//统计各节点出线数
	for (i = 1; i <= N; i++)
	{
		k = 1;
		for (j = 1; j <= Branch_Number; j++)               //统计各线路支路对每个节点出线数的作用
		{
			if (Branch[j].i == Branch[j].j) //不考虑接地支路的节点出线数的影响
				continue;
			if (Branch[j].i == i || Branch[j].j == i)
			{
				Node_Connection_Count[i] = Node_Connection_Count[i] + 1;
				if (Branch[j].i == i)
					Node_Connection_Matrix[i][k] = Branch[j].j;
				else
					Node_Connection_Matrix[i][k] = Branch[j].i;
				k = k + 1;
			}
		}
		for (j = 1; j <= Transformer_Number; j++)                  //统计各变压器支路对每个节点出线数的作用
		{
			if (Transformer[j].i == i || Transformer[j].j == i)
			{
				Node_Connection_Count[i] = Node_Connection_Count[i] + 1;
				if (Transformer[j].i == i)
					Node_Connection_Matrix[i][k] = Transformer[j].j;
				else
					Node_Connection_Matrix[i][k] = Transformer[j].i;
				k = k + 1;
			}
		}
	}

	int Mini_Connection_Count;          //存储节点的最小出线数

	for (t = 1; t <= N; t++)           //求新的节点号并将以后削去过程中会产生的支路写入新的支路数组
	{
		Mini_Connection_Count = Node_Connection_Count[1];
		j = 1;
		for (i = 2; i <= N; i++)     //寻找最小节点号,最小出线数的节点号为j
		{
			if (Node_Connection_Count[i] < Mini_Connection_Count)
			{
				Mini_Connection_Count = Node_Connection_Count[i];
				j = i;
			}
		}
		Node_Sequence[j] = t;


		for (i = 1; i <= Mini_Connection_Count; i++)// 使与j点相连的所有节点出线数减一，并取掉支路对端的节点j
		{
			for (m = 1; m <= Node_Connection_Count[Node_Connection_Matrix[j][i]]; m++)
			{
				if (Node_Connection_Matrix[Node_Connection_Matrix[j][i]][m] == j)
				{
					Node_Connection_Matrix[Node_Connection_Matrix[j][i]][m] = Node_Connection_Matrix[Node_Connection_Matrix[j][i]][Node_Connection_Count[Node_Connection_Matrix[j][i]]];//将Putout_matrix矩阵中i行的最后一列元素放到m列
					Node_Connection_Matrix[Node_Connection_Matrix[j][i]][Node_Connection_Count[Node_Connection_Matrix[j][i]]] = 0;
					Node_Connection_Count[Node_Connection_Matrix[j][i]] = Node_Connection_Count[Node_Connection_Matrix[j][i]] - 1;
				}

			}

		}

		for (i = 1; i <= Mini_Connection_Count; i++) //若原来与j相连的节点中任两个之间没有支路，增加一条新支路
		{

			for (m = i + 1; m <= Mini_Connection_Count; m++)
			{
				if (Node_Connection_Matrix[j][i] == Node_Connection_Matrix[j][m])
					continue;
				else
				{
					c = 0;
					for (n = 1; n <= Node_Connection_Count[Node_Connection_Matrix[j][i]]; n++)
					{
						if (Node_Connection_Matrix[j][m] == Node_Connection_Matrix[Node_Connection_Matrix[j][i]][n])
							c = c + 1;
						else c = c;
					}
					if (c == 0)//c=0表明与j点相连的两个节点之间没有支路
					{
						Node_Connection_Count[Node_Connection_Matrix[j][i]] = Node_Connection_Count[Node_Connection_Matrix[j][i]] + 1;
						Node_Connection_Matrix[Node_Connection_Matrix[j][i]][Node_Connection_Count[Node_Connection_Matrix[j][i]]] = Node_Connection_Matrix[j][m];
						Node_Connection_Count[Node_Connection_Matrix[j][m]] = Node_Connection_Count[Node_Connection_Matrix[j][m]] + 1;
						Node_Connection_Matrix[Node_Connection_Matrix[j][m]][Node_Connection_Count[Node_Connection_Matrix[j][m]]] = Node_Connection_Matrix[j][i];
						Branch_Injection_Number = Branch_Injection_Number + 1;
						New_line[Branch_Injection_Number].i = Node_Connection_Matrix[j][i];
						New_line[Branch_Injection_Number].j = Node_Connection_Matrix[j][m];
					}
				}
			}
		}
		Node_Connection_Count[j] = 50;//设每个节点的出线数不会超过50
	}
	cout << "节点编号优化完成！" << endl;
	cout << "***********************************************" << endl;
}