/***************************************************************
 * @file       ConductanceMatrixGenerater.cpp
 * @brief      ���������
 * @author     ������
 * @date       2023��1��9��
 **************************************************************/

#include "PowerFlow.h"

void Conductance_Matrix_Generater()
{
	double Magnitude;      //����ģֵ��ƽ��
	OFF_Diagonal_Number = Branch_Injection_Number + Total_Branch_Number;    //�Ż�������зǶԽ�Ԫ�ظ���, �����ӵ�֧·
	OFF_Diagonal_Number_Not_Ground = Branch_Injection_Number + Total_Branch_Number_Not_Ground;  //�Ż�������зǶԽ�Ԫ�ظ���,�������ӵ�֧·
	Not_Zero_And_Off__Diagonal_Node_Number = 1;              //��¼����ǶԽ�Ԫ�ظ��� 

	Off_Diag_Number_in_Row = new int[N + 1];             //���зǶԽ�Ԫ�صĸ���
	Yii = new Yii_Type[N + 1];          //�洢N���Խ�Ԫ��
	Yij = new Yij_Type[OFF_Diagonal_Number_Not_Ground + 1];//�ٶ�ΪN_all_line1����ʵ����Ҫ��˫���ߵĸ���
	Branch_Col_Sub = new int[OFF_Diagonal_Number_Not_Ground + 1];      //�洢ÿ��֧·�����±�
	Off_Diagonal_Element_Start_Address = new int[N + 1];                //�洢ÿ�е�һ��Ԫ����Yij�е�λ�ã���ÿ���׵�ַ
	Branch_Node = new Branch_Node_Type[OFF_Diagonal_Number_Not_Ground + 1];//�洢�Ż������֧·���������ӵ�֧·�������˽ڵ��
	Yii_Of_Temp_Branch_Type* Y_Of_Temp_Branch;
	Y_Of_Temp_Branch = new Yii_Of_Temp_Branch_Type[OFF_Diagonal_Number_Not_Ground + 1];

	for (i = 1; i <= N; i++)               //��ʼ���Խ�Ԫ��
	{
		Yii[i].G = 0;
		Yii[i].B = 0;
	}
	Magnitude = 0;
	for (i = 1; i <= Branch_Number; i++)              //��ÿ����·֧·���뵽���涨�����ʱ�����У����а����ӵ�֧·
	{
		if (Branch[i].i == Branch[i].j) //��ʱ�ǽӵ�֧·�����
		{
			if (Branch[i].Y < 0)        //Branch[i].Y<0˵��Branch[i].X�ǽӵ�֧·�ĵ���
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

	for (i = 1; i <= Transformer_Number; i++)             //��ÿ����ѹ��֧·�������뵽���涨�����ʱ������
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
		Y_Of_Temp_Branch[Not_Zero_And_Off__Diagonal_Node_Number].G = -Transformer[i].R / (Transformer[i].K * Magnitude);//��Ϊ���Ǳ�ѹ��֧·�����Ի�����Ӧ�ó��Ա��K
		Y_Of_Temp_Branch[Not_Zero_And_Off__Diagonal_Node_Number].B = Transformer[i].X / (Transformer[i].K * Magnitude);
		Yii[Node_Sequence[Transformer[i].i]].G = Yii[Node_Sequence[Transformer[i].i]].G + Transformer[i].R / Magnitude;//�����׼�ർ��
		Yii[Node_Sequence[Transformer[i].i]].B = Yii[Node_Sequence[Transformer[i].i]].B - Transformer[i].X / Magnitude;
		Yii[Node_Sequence[Transformer[i].j]].G = Yii[Node_Sequence[Transformer[i].j]].G + Transformer[i].R / (Transformer[i].K * Transformer[i].K * Magnitude);//����Ǳ�׼�ർ�ɣ���ʱӦ�ó��Ա��K��ƽ��
		Yii[Node_Sequence[Transformer[i].j]].B = Yii[Node_Sequence[Transformer[i].j]].B - Transformer[i].X / (Transformer[i].K * Transformer[i].K * Magnitude);
		Not_Zero_And_Off__Diagonal_Node_Number = Not_Zero_And_Off__Diagonal_Node_Number + 1;
	}

	for (i = 1; i <= Branch_Injection_Number; i++)                      //����ע��Ԫ��
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
	Yij_Of_Temp_Branch = new Yij_Of_Temp_Branch_Type[OFF_Diagonal_Number_Not_Ground + 1];//��һ���洢��ʱ֧·�ĵ���,���ý�ͬһ�е�Ԫ�ط���һ��

	int* Off_Diag_Element_Col_Number = new int[OFF_Diagonal_Number_Not_Ground + 1];                           //�洢�����ǶԽ�Ԫ�ص��к�
	int p, q = 1;
	for (i = 1; i <= N; i++)
		Off_Diagonal_Element_Start_Address[i] = 0;
	for (i = 1; i <= N; i++)                                                  //��ͬһ�е�Ԫ�ط���һ��
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

	Off_Diagonal_Element_Start_Address[1] = 1;              //���һ����Yij�е��׵�ַΪ1
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
	cout << "���ɾ��������ɣ�" << endl;
}