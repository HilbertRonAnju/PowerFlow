/***************************************************************
 * @file       NodeOptimization.cpp
 * @brief      �ڵ��Ż�(T-2����)
 * @author     ������
 * @date       2023��1��9��
 **************************************************************/

#include "PowerFlow.h"

void Node_Optimization()
{
	cout << "��ʼ�ڵ����Ż�" << endl;

	Branch_Not_Ground = 0;        //ͳ�Ʋ��ӵ���·��
	for (i = 1; i <= Branch_Number; i++)
	{
		if (Branch[i].i == Branch[i].j)
			continue;
		else	 Branch_Not_Ground = Branch_Not_Ground + 1;
	}

	Total_Branch_Number = Branch_Number + Transformer_Number;          //�ܵ�֧·���������ӵ���·���ͱ�ѹ��֧·��
	Total_Branch_Number_Not_Ground = Branch_Not_Ground + Transformer_Number;        //�ܵ�֧·�����������ӵ�֧·����
	Branch_Injection_Number = 0;           // ע��֧·��
	Node_Sequence = new int[N + 1];

	int* Node_Connection_Count;         //��¼���ڵ���߸���
	Node_Connection_Count = new int[N + 1];
	for (i = 1; i <= N; i++)     //��ʼ�������ڵ�ĳ�����
	{
		Node_Connection_Count[i] = 0;
	}
	int** Node_Connection_Matrix;    //��Ÿ����ڵ�����֧·�Զ˵Ľڵ�ţ���Ϊ�ڵ�ţ���Ϊ��Ӧ֧·�����ڵ��
	Node_Connection_Matrix = (int**)new int[N + 1]();
	for (i = 1; i <= N; i++)
	{
		Node_Connection_Matrix[i] = new int[50];    //����������Ϊ50
	}


	//ͳ�Ƹ��ڵ������
	for (i = 1; i <= N; i++)
	{
		k = 1;
		for (j = 1; j <= Branch_Number; j++)               //ͳ�Ƹ���·֧·��ÿ���ڵ������������
		{
			if (Branch[j].i == Branch[j].j) //�����ǽӵ�֧·�Ľڵ��������Ӱ��
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
		for (j = 1; j <= Transformer_Number; j++)                  //ͳ�Ƹ���ѹ��֧·��ÿ���ڵ������������
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

	int Mini_Connection_Count;          //�洢�ڵ����С������

	for (t = 1; t <= N; t++)           //���µĽڵ�Ų����Ժ���ȥ�����л������֧·д���µ�֧·����
	{
		Mini_Connection_Count = Node_Connection_Count[1];
		j = 1;
		for (i = 2; i <= N; i++)     //Ѱ����С�ڵ��,��С�������Ľڵ��Ϊj
		{
			if (Node_Connection_Count[i] < Mini_Connection_Count)
			{
				Mini_Connection_Count = Node_Connection_Count[i];
				j = i;
			}
		}
		Node_Sequence[j] = t;


		for (i = 1; i <= Mini_Connection_Count; i++)// ʹ��j�����������нڵ��������һ����ȡ��֧·�Զ˵Ľڵ�j
		{
			for (m = 1; m <= Node_Connection_Count[Node_Connection_Matrix[j][i]]; m++)
			{
				if (Node_Connection_Matrix[Node_Connection_Matrix[j][i]][m] == j)
				{
					Node_Connection_Matrix[Node_Connection_Matrix[j][i]][m] = Node_Connection_Matrix[Node_Connection_Matrix[j][i]][Node_Connection_Count[Node_Connection_Matrix[j][i]]];//��Putout_matrix������i�е����һ��Ԫ�طŵ�m��
					Node_Connection_Matrix[Node_Connection_Matrix[j][i]][Node_Connection_Count[Node_Connection_Matrix[j][i]]] = 0;
					Node_Connection_Count[Node_Connection_Matrix[j][i]] = Node_Connection_Count[Node_Connection_Matrix[j][i]] - 1;
				}

			}

		}

		for (i = 1; i <= Mini_Connection_Count; i++) //��ԭ����j�����Ľڵ���������֮��û��֧·������һ����֧·
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
					if (c == 0)//c=0������j�������������ڵ�֮��û��֧·
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
		Node_Connection_Count[j] = 50;//��ÿ���ڵ�ĳ��������ᳬ��50
	}
	cout << "�ڵ����Ż���ɣ�" << endl;
	cout << "***********************************************" << endl;
}