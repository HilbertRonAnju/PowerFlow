/***************************************************************
 * @file       NewtonMethod.cpp
 * @brief      牛顿法解潮流
 * @author     聂永欣
 * @date       2023年1月9日
 **************************************************************/

#include "PowerFlow.h"

void Newton_Iteration()
{
	//由导纳矩阵形成消去规格化后的雅可比矩阵并存储其非对角元素
	struct Nodal_Power_Reckon_Type
	{
		double P, Q;
	};
	Nodal_Power_Reckon_Type* Nodal_Reckon_Power;
	Nodal_Reckon_Power = new Nodal_Power_Reckon_Type[N + 1]; //存放节点的计算有功功率和计算无功功率

	struct Node_Power_Injection_Type
	{
		double P, Q;
	};
	Node_Power_Injection_Type* Node_Power_Injection;
	Node_Power_Injection = new Node_Power_Injection_Type[N + 1];  //存放节点的注入有功功率和注入无功功率

	Node_Power = new Node_Power_Type[N + 1];      //存放节点的中间变量（有功功率和无功功率）
	Node_Voltage = new Node_Voltage_Type[N + 1];           //存放节点电压和相角

	struct Jacobion_Single_Branch_Type
	{
		double H, N, J, L;
		int j;
	};
	Jacobion_Single_Branch_Type* Jacobion_Single_Branch;
	Jacobion_Single_Branch = new Jacobion_Single_Branch_Type[N + 1];//存放雅可比矩阵一行的非零元素和元素的列号，且最多为N个

	struct Jacobion_Reckoned_Type
	{
		double H, N, J, L;
		int j;
	};
	Jacobion_Reckoned_Type* Jacobion_Reckoned;
	Jacobion_Reckoned = new Jacobion_Reckoned_Type[OFF_Diagonal_Number_Not_Ground + 1];//存放雅可比矩阵消去规格化后的上三角非对角非零元素

	struct Temp_Param
	{
		double tem1, tem2;
		int tem3, tem4;
	};
	Temp_Param* tem_para;
	tem_para = new Temp_Param[N + 1];//

	int* NYsum_reckoned;
	NYsum_reckoned = new int[N + 1];        //存放消去规格化之后的各行非零非对角元素的个数

	struct Power_Type
	{
		double P, Q;
	};
	Power_Type* P_Q_power;
	P_Q_power = new Power_Type[N + 1]; //存放N个节点的功率偏差

	double* PP;
	PP = new double[N + 1];

	double* Flag_PV;
	Flag_PV = new double[N + 1];         //PV节点的标志符号

	int Iteration_Count;         //迭代数

	int I_Count_A, I_Count_D;
	double P_Count_C, Q_Count_C, P_Count_D, Q_Count_D;
	double V_Start = 1.0000;
	double A_Count_D, B_Count_D, C_Count_D, D_Count_D, A_Count_F, B_Count_F, C_Count_F, D_Count_F;
	int Ig;
	double Zz;
	int M_Count_A, I_Count_B, I_Count_C, K_Count_F;
	int Temp;

	for (i = 1; i <= N; i++) //平启动
	{
		Node_Voltage[i].V = V_Start;
		Node_Voltage[i].theta = 0;
	}
	for (i = 1; i <= N; i++)
	{
		Node_Power_Injection[i].P = 0;
		Node_Power_Injection[i].Q = 0;
		Flag_PV[i] = 0;
	}
	for (i = 1; i <= Generator_Number; i++) //将发动机类型中各节点初值赋给这点 
	{
		if (Generator[i].type == 0)
		{
			Balance_Node_Order_Number = Node_Sequence[Generator[i].i];
			Node_Voltage[Balance_Node_Order_Number].V = Generator[i].V;     //赋平衡节点的电压
			Node_Voltage[Balance_Node_Order_Number].theta = Generator[i].P; //赋平衡节点的相角
		}
		if (Generator[i].type == -1)
		{
			Node_Power_Injection[Node_Sequence[Generator[i].i]].P = Generator[i].P;
			Node_Power_Injection[Node_Sequence[Generator[i].i]].Q = 0;
			Node_Voltage[Node_Sequence[Generator[i].i]].V = Generator[i].V;
			Flag_PV[Node_Sequence[Generator[i].i]] = 1;
		}
		if (Generator[i].type == 1)
		{
			Node_Power_Injection[Node_Sequence[Generator[i].i]].P = -Generator[i].P;
			Node_Power_Injection[Node_Sequence[Generator[i].i]].Q = -Generator[i].Q;
		}

	}
	for (i = 1; i <= Load_Number; i++)//将负荷类型中各节点P，Q初值赋给这点的注入功率数组，即减去这些点的功率
	{
		if (Load[i].type == 1)
		{
			Node_Power_Injection[Node_Sequence[Load[i].i]].P = Node_Power_Injection[Node_Sequence[Load[i].i]].P - Load[i].P;
			Node_Power_Injection[Node_Sequence[Load[i].i]].Q = Node_Power_Injection[Node_Sequence[Load[i].i]].Q - Load[i].Q;
		}
	}

	//开始消去和规格化
	Iteration_Count = 0;                       //填写修正方程式的矩阵和向量
	int K_Count_A;
	int W_Count;
	int J_Count_F;                     //雅可比矩阵每一行的非零元素记数，包括对角元素
	while (Iteration_Count <= Iteration_Time)
	{
		Iteration_Count = Iteration_Count + 1;
		MAXIMUM_ERROR = 0;
		K_Count_A = 1;               //导纳非零矩阵记数
		for (i = 1; i <= N; i++)  //填写修正方程式的矩阵和向量
		{
			A_Count_D = Node_Voltage[i].V * Yii[i].G;
			B_Count_D = Node_Voltage[i].V * (-Yii[i].B);
			J_Count_F = 1;
			Temp = 1;
			if (i == 1)
			{
				for (K_Count_A = 1; K_Count_A <= Off_Diag_Number_in_Row[1]; K_Count_A++)
				{
					W_Count = Branch_Col_Sub[K_Count_A];  //用于记录列号
					A_Count_D = A_Count_D + Node_Voltage[W_Count].V * (Yij[K_Count_A].G * cos(Node_Voltage[i].theta - Node_Voltage[W_Count].theta) + Yij[K_Count_A].B * sin(Node_Voltage[i].theta - Node_Voltage[W_Count].theta));
					B_Count_D = B_Count_D + Node_Voltage[W_Count].V * (Yij[K_Count_A].G * sin(Node_Voltage[i].theta - Node_Voltage[W_Count].theta) - Yij[K_Count_A].B * cos(Node_Voltage[i].theta - Node_Voltage[W_Count].theta));
					if (i != Balance_Node_Order_Number && W_Count != Balance_Node_Order_Number)
					{
						J_Count_F = J_Count_F + 1;
						Jacobion_Single_Branch[J_Count_F].j = W_Count;
						Jacobion_Single_Branch[J_Count_F].H = Node_Voltage[i].V * Node_Voltage[W_Count].V * (Yij[K_Count_A].G * sin(Node_Voltage[i].theta - Node_Voltage[W_Count].theta) - Yij[K_Count_A].B * cos(Node_Voltage[i].theta - Node_Voltage[W_Count].theta));
						Jacobion_Single_Branch[J_Count_F].N = Node_Voltage[i].V * Node_Voltage[W_Count].V * (Yij[K_Count_A].G * cos(Node_Voltage[i].theta - Node_Voltage[W_Count].theta) + Yij[K_Count_A].B * sin(Node_Voltage[i].theta - Node_Voltage[W_Count].theta));
						Jacobion_Single_Branch[J_Count_F].J = -Jacobion_Single_Branch[J_Count_F].N;
						Jacobion_Single_Branch[J_Count_F].L = Jacobion_Single_Branch[J_Count_F].H;
						if (Flag_PV[W_Count] == 1)
						{
							Jacobion_Single_Branch[J_Count_F].N = 0;
							Jacobion_Single_Branch[J_Count_F].L = 0;
						}

					}

				}
			}
			else
			{
				for (j = 1; j < K_Count_A; j++)
				{
					M_Count_A = 1;
					if (Branch_Col_Sub[j] == i)
					{
						tem_para[Temp].tem1 = Yij[j].G;
						tem_para[Temp].tem2 = Yij[j].B;
						tem_para[Temp].tem3 = Branch_Col_Sub[j];
						while (j >= Off_Diagonal_Element_Start_Address[M_Count_A])
						{
							M_Count_A = M_Count_A + 1;
						}
						tem_para[Temp].tem4 = M_Count_A - 1;
						Temp = Temp + 1;
					}
				}
				for (j = 1; j < Temp; j++)
				{
					W_Count = tem_para[j].tem4;
					A_Count_D = A_Count_D + Node_Voltage[W_Count].V * (tem_para[j].tem1 * cos(Node_Voltage[i].theta - Node_Voltage[W_Count].theta) + tem_para[j].tem2 * sin(Node_Voltage[i].theta - Node_Voltage[W_Count].theta));
					B_Count_D = B_Count_D + Node_Voltage[W_Count].V * (tem_para[j].tem1 * sin(Node_Voltage[i].theta - Node_Voltage[W_Count].theta) - tem_para[j].tem2 * cos(Node_Voltage[i].theta - Node_Voltage[W_Count].theta));
					if (i != Balance_Node_Order_Number && W_Count != Balance_Node_Order_Number)
					{
						J_Count_F = J_Count_F + 1;
						Jacobion_Single_Branch[J_Count_F].j = W_Count;
						Jacobion_Single_Branch[J_Count_F].H = Node_Voltage[i].V * Node_Voltage[W_Count].V * (tem_para[j].tem1 * sin(Node_Voltage[i].theta - Node_Voltage[W_Count].theta) - tem_para[j].tem2 * cos(Node_Voltage[i].theta - Node_Voltage[W_Count].theta));
						Jacobion_Single_Branch[J_Count_F].N = Node_Voltage[i].V * Node_Voltage[W_Count].V * (tem_para[j].tem1 * cos(Node_Voltage[i].theta - Node_Voltage[W_Count].theta) + tem_para[j].tem2 * sin(Node_Voltage[i].theta - Node_Voltage[W_Count].theta));
						Jacobion_Single_Branch[J_Count_F].J = -Jacobion_Single_Branch[J_Count_F].N;
						Jacobion_Single_Branch[J_Count_F].L = Jacobion_Single_Branch[J_Count_F].H;
						if (Flag_PV[W_Count] == 1)
						{
							Jacobion_Single_Branch[J_Count_F].N = 0;
							Jacobion_Single_Branch[J_Count_F].L = 0;
						}

					}
				}
				for (j = 1; j <= Off_Diag_Number_in_Row[i]; j++)
				{
					W_Count = Branch_Col_Sub[K_Count_A];
					A_Count_D = A_Count_D + Node_Voltage[W_Count].V * (Yij[K_Count_A].G * cos(Node_Voltage[i].theta - Node_Voltage[W_Count].theta) + Yij[K_Count_A].B * sin(Node_Voltage[i].theta - Node_Voltage[W_Count].theta));
					B_Count_D = B_Count_D + Node_Voltage[W_Count].V * (Yij[K_Count_A].G * sin(Node_Voltage[i].theta - Node_Voltage[W_Count].theta) - Yij[K_Count_A].B * cos(Node_Voltage[i].theta - Node_Voltage[W_Count].theta));

					if (i != Balance_Node_Order_Number && W_Count != Balance_Node_Order_Number)
					{
						J_Count_F = J_Count_F + 1;
						Jacobion_Single_Branch[J_Count_F].j = W_Count;
						Jacobion_Single_Branch[J_Count_F].H = Node_Voltage[i].V * Node_Voltage[W_Count].V * (Yij[K_Count_A].G * sin(Node_Voltage[i].theta - Node_Voltage[W_Count].theta) - Yij[K_Count_A].B * cos(Node_Voltage[i].theta - Node_Voltage[W_Count].theta));
						Jacobion_Single_Branch[J_Count_F].N = Node_Voltage[i].V * Node_Voltage[W_Count].V * (Yij[K_Count_A].G * cos(Node_Voltage[i].theta - Node_Voltage[W_Count].theta) + Yij[K_Count_A].B * sin(Node_Voltage[i].theta - Node_Voltage[W_Count].theta));
						Jacobion_Single_Branch[J_Count_F].J = -Jacobion_Single_Branch[J_Count_F].N;
						Jacobion_Single_Branch[J_Count_F].L = Jacobion_Single_Branch[J_Count_F].H;
						if (Flag_PV[W_Count] == 1)
						{
							Jacobion_Single_Branch[J_Count_F].N = 0;
							Jacobion_Single_Branch[J_Count_F].L = 0;
						}

					}
					K_Count_A = K_Count_A + 1;
				}
			}

			//填写功率误差
			if (i == Balance_Node_Order_Number)//如果是平衡节点
			{
				Node_Power[i].P = Node_Voltage[i].V * A_Count_D;
				Node_Power[i].Q = Node_Voltage[i].V * B_Count_D;
				P_Q_power[i].P = 0;
				P_Q_power[i].Q = 0;
				NYsum_reckoned[i] = 0;
			}
			else
			{
				P_Count_C = Node_Voltage[i].V * A_Count_D;
				Q_Count_C = Node_Voltage[i].V * B_Count_D;
				Node_Power[i].P = P_Count_C;
				Node_Power[i].Q = Q_Count_C;
				P_Count_C = Node_Power_Injection[i].P - P_Count_C;
				Q_Count_C = Node_Power_Injection[i].Q - Q_Count_C;
				P_Count_D = P_Count_C;
				Q_Count_D = Q_Count_C;
				if (Flag_PV[i] == 1)
					Q_Count_D = 0;
				Jacobion_Single_Branch[1].H = -(Node_Voltage[i].V * Node_Voltage[i].V * Yii[i].B + Node_Voltage[i].V * B_Count_D);
				Jacobion_Single_Branch[1].N = Node_Voltage[i].V * Node_Voltage[i].V * Yii[i].G + Node_Voltage[i].V * A_Count_D;
				Jacobion_Single_Branch[1].j = i;
				P_Q_power[i].P = P_Count_C;
				if (Flag_PV[i] == 1)
				{
					PP[i] = Q_Count_C;
					for (j = 2; j <= J_Count_F; j++)
					{
						Jacobion_Single_Branch[j].J = 0;
						Jacobion_Single_Branch[j].L = 0;
					}
					Jacobion_Single_Branch[1].N = 0;
					Jacobion_Single_Branch[1].J = 0;
					Jacobion_Single_Branch[1].L = 1;
					Q_Count_C = 0;
					P_Q_power[i].Q = Q_Count_C;
				}
				else
				{
					Jacobion_Single_Branch[1].J = -(Node_Voltage[i].V * Node_Voltage[i].V * Yii[i].G - Node_Voltage[i].V * A_Count_D);
					Jacobion_Single_Branch[1].L = -(Node_Voltage[i].V * Node_Voltage[i].V * Yii[i].B - Node_Voltage[i].V * B_Count_D);
					P_Q_power[i].Q = Q_Count_C;
				}
				C_Count_F = fabs(P_Count_D);
				D_Count_F = fabs(Q_Count_D);

				if (C_Count_F > MAXIMUM_ERROR)
				{
					I_Count_A = i + 1;
					MAXIMUM_ERROR = C_Count_F;
				}
				if (D_Count_F > MAXIMUM_ERROR)
				{
					I_Count_A = i + 1;
					MAXIMUM_ERROR = D_Count_F;
				}

				//开始进行消去
				k = 1;      //用来做雅可比矩阵消去规格化后上三角非对角非零元素的指针
				if (i != 1)  //第一行不用消去，直接进行规格化
				{
					for (I_Count_B = 1; I_Count_B < i; I_Count_B++)
					{
						I_Count_D = 2;
						while (Jacobion_Single_Branch[I_Count_D].j != I_Count_B && I_Count_D <= J_Count_F)
						{
							I_Count_D = I_Count_D + 1;
						}
						if (I_Count_D > J_Count_F)     //说明没有找到
						{
							k = k + NYsum_reckoned[I_Count_B];
						}
						else
						{
							if (NYsum_reckoned[I_Count_B] != 0)
							{
								for (Ig = 1; Ig <= NYsum_reckoned[I_Count_B]; Ig++)
								{
									I_Count_C = 1;
									while (Jacobion_Single_Branch[I_Count_C].j != Jacobion_Reckoned[k].j && I_Count_C <= J_Count_F)
										I_Count_C = I_Count_C + 1;
									if (I_Count_C > J_Count_F)
									{
										J_Count_F = J_Count_F + 1;
										Jacobion_Single_Branch[J_Count_F].H = -Jacobion_Single_Branch[I_Count_D].H * Jacobion_Reckoned[k].H - Jacobion_Single_Branch[I_Count_D].N * Jacobion_Reckoned[k].J;
										Jacobion_Single_Branch[J_Count_F].N = -Jacobion_Single_Branch[I_Count_D].H * Jacobion_Reckoned[k].N - Jacobion_Single_Branch[I_Count_D].N * Jacobion_Reckoned[k].L;
										Jacobion_Single_Branch[J_Count_F].J = -Jacobion_Single_Branch[I_Count_D].J * Jacobion_Reckoned[k].H - Jacobion_Single_Branch[I_Count_D].L * Jacobion_Reckoned[k].J;
										Jacobion_Single_Branch[J_Count_F].L = -Jacobion_Single_Branch[I_Count_D].J * Jacobion_Reckoned[k].N - Jacobion_Single_Branch[I_Count_D].L * Jacobion_Reckoned[k].L;
									}
									else
									{
										Jacobion_Single_Branch[I_Count_C].H = Jacobion_Single_Branch[I_Count_C].H - Jacobion_Single_Branch[I_Count_D].H * Jacobion_Reckoned[k].H - Jacobion_Single_Branch[I_Count_D].N * Jacobion_Reckoned[k].J;
										Jacobion_Single_Branch[I_Count_C].N = Jacobion_Single_Branch[I_Count_C].N - Jacobion_Single_Branch[I_Count_D].H * Jacobion_Reckoned[k].N - Jacobion_Single_Branch[I_Count_D].N * Jacobion_Reckoned[k].L;
										Jacobion_Single_Branch[I_Count_C].J = Jacobion_Single_Branch[I_Count_C].J - Jacobion_Single_Branch[I_Count_D].J * Jacobion_Reckoned[k].H - Jacobion_Single_Branch[I_Count_D].L * Jacobion_Reckoned[k].J;
										Jacobion_Single_Branch[I_Count_C].L = Jacobion_Single_Branch[I_Count_C].L - Jacobion_Single_Branch[I_Count_D].J * Jacobion_Reckoned[k].N - Jacobion_Single_Branch[I_Count_D].L * Jacobion_Reckoned[k].L;
									}
									k = k + 1;
								}
								P_Q_power[i].P = P_Q_power[i].P - Jacobion_Single_Branch[I_Count_D].H * P_Q_power[I_Count_B].P - Jacobion_Single_Branch[I_Count_D].N * P_Q_power[I_Count_B].Q;
								P_Q_power[i].Q = P_Q_power[i].Q - Jacobion_Single_Branch[I_Count_D].J * P_Q_power[I_Count_B].P - Jacobion_Single_Branch[I_Count_D].L * P_Q_power[I_Count_B].Q;
							}
						}

					}
				}
				//开始规格化 
				K_Count_F = k;
				Zz = Jacobion_Single_Branch[1].H * Jacobion_Single_Branch[1].L - Jacobion_Single_Branch[1].N * Jacobion_Single_Branch[1].J;
				if (Zz == 0)
				{
					P_Q_power[i].P = 0;
					P_Q_power[i].Q = 0;
					NYsum_reckoned[i] = 0;
				}
				else
				{
					A_Count_D = Jacobion_Single_Branch[1].L / Zz;
					B_Count_D = -Jacobion_Single_Branch[1].N / Zz;
					C_Count_D = -Jacobion_Single_Branch[1].J / Zz;
					D_Count_D = Jacobion_Single_Branch[1].H / Zz;
					for (j = 2; j <= J_Count_F; j++)
					{
						if (Jacobion_Single_Branch[j].j > i)
						{
							Jacobion_Reckoned[k].j = Jacobion_Single_Branch[j].j;
							Jacobion_Reckoned[k].H = A_Count_D * Jacobion_Single_Branch[j].H + B_Count_D * Jacobion_Single_Branch[j].J;
							Jacobion_Reckoned[k].N = A_Count_D * Jacobion_Single_Branch[j].N + B_Count_D * Jacobion_Single_Branch[j].L;
							Jacobion_Reckoned[k].J = C_Count_D * Jacobion_Single_Branch[j].H + D_Count_D * Jacobion_Single_Branch[j].J;
							Jacobion_Reckoned[k].L = C_Count_D * Jacobion_Single_Branch[j].N + D_Count_D * Jacobion_Single_Branch[j].L;
							k = k + 1;
						}
					}
					A_Count_F = P_Q_power[i].Q;
					B_Count_F = P_Q_power[i].P;
					P_Q_power[i].P = A_Count_D * B_Count_F + B_Count_D * A_Count_F;
					P_Q_power[i].Q = C_Count_D * B_Count_F + D_Count_D * A_Count_F;
					NYsum_reckoned[i] = k - K_Count_F;
				}
			}
		}

		//解修正方程回代运算
		if (MAXIMUM_ERROR >= 1e-5 && Iteration_Count <= Iteration_Time)//判断是否收敛
		{
			for (i = N - 1; i >= 1; i--)
			{
				if (NYsum_reckoned[i] != 0)
				{
					for (Ig = 1; Ig <= NYsum_reckoned[i]; Ig++)
					{
						k = k - 1;
						j = Jacobion_Reckoned[k].j;
						P_Q_power[i].P = P_Q_power[i].P - Jacobion_Reckoned[k].H * P_Q_power[j].P - Jacobion_Reckoned[k].N * P_Q_power[j].Q;
						P_Q_power[i].Q = P_Q_power[i].Q - Jacobion_Reckoned[k].J * P_Q_power[j].P - Jacobion_Reckoned[k].L * P_Q_power[j].Q;
					}
				}
			}

			//算节点新的电压值
			for (i = 1; i <= N; i++)
			{
				Node_Voltage[i].V = Node_Voltage[i].V + Node_Voltage[i].V * P_Q_power[i].Q; //电压修正
				Node_Voltage[i].theta = Node_Voltage[i].theta + P_Q_power[i].P;           //相角修正
			}
			for (i = 1; i <= Generator_Number; i++)//恢复PV节点的电压和相角
			{
				if (Generator[i].type == -1)
				{
					j = Node_Sequence[Generator[i].i];
					Node_Voltage[j].V = Generator[i].V;
					Node_Voltage[j].theta = Node_Voltage[j].theta;
				}
			}

		}
		else
		{
			c = Iteration_Time;
			Iteration_Time = Iteration_Count;
			Iteration_Count = c + 1;
		}
	}
	cout << "牛顿-拉夫逊法潮流迭代计算已完成!" << endl;
	cout << "***********************************************" << endl;
}