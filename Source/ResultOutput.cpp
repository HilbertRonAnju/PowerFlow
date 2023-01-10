/***************************************************************
 * @file       ResultOutput.cpp
 * @brief      输出计算结果
 * @author     聂永欣
 * @date       2023年1月9日
 **************************************************************/

#include "PowerFlow.h"


void Result_Output(int IEEE_type)
{
	double ei, fi, ej, fj, Pij, Qij, Pji, Qji;
	double Branch_P_loss, Branch_Q_loss, P_Sum_loss = 0, Q_Sum_loss = 0;
	double P_Sum_generator = 0, Q_Sum_generator = 0, P_Sum_load = 0, Q_Sum_load = 0;//累计发电机总的有功和无功，负荷总的有功和无功
	int* Node_Oringin;
	Node_Oringin = new int[N + 1];
	for (i = 1; i <= N; i++)
	{
		for (j = 1; j <= N; j++)
		{
			if (Node_Sequence[j] == i)
			{
				Node_Oringin[i] = j;
			}
		}
	}
	string INPUT_PATH;
	INPUT_PATH = Get_Operation_Location() + "\\Result\\IEEE" + to_string(IEEE_type);

	//保存潮流计算结果
	ofstream Power_Flow_Savefile;
	remove((char*)(INPUT_PATH + "_Result.txt").c_str());
	Power_Flow_Savefile.open(INPUT_PATH + "_Result.txt", ios::out);

	if (Power_Flow_Savefile.is_open()) {
		cout << "开始输出计算结果..." << endl;
		Power_Flow_Savefile << "------------------------------------------------------------------------------------------------------" << endl;
		Power_Flow_Savefile << "********************************************原始数据信息**********************************************" << endl;
		Power_Flow_Savefile << "------------------------------------------------------------------------------------------------------" << endl << endl;
		Power_Flow_Savefile << "" << N << "节点系统的计算结果如下：" << endl;
		Power_Flow_Savefile << "最大允许迭代次数：" << MAXIMUM_ITERATION_TIME << endl;
		Power_Flow_Savefile << "计算精度：" << EPS << endl;
		Power_Flow_Savefile << "节点数   线路支路数   变压器支路数    发动机个数   负荷个数" << endl;
		Power_Flow_Savefile << setw(5) << N << setw(12) << Branch_Number << setw(13) << Transformer_Number << setw(15) << Generator_Number << setw(13) << Load_Number << endl;
		Power_Flow_Savefile << "<----------------------------------------------线路数据---------------------------------------------->" << endl << endl;
		Power_Flow_Savefile << setiosflags(ios::fixed) << setiosflags(ios::left) << setprecision(8);
		Power_Flow_Savefile << setw(5) << "i" << setw(5) << "j" << setw(18) << "R" << setw(18) << "X" << setw(18) << "B/2" << endl;

		for (i = 1; i <= Branch_Number; i++)
		{

			Power_Flow_Savefile << setw(5) << Branch[i].i << setw(5) << Branch[i].j << setw(15) << Branch[i].R << setw(15) << Branch[i].X << setw(5) << Branch[i].Y << endl;
		}
		Power_Flow_Savefile << endl;
		Power_Flow_Savefile << "<----------------------------------------------变压器数据---------------------------------------------->" << endl << endl;
		Power_Flow_Savefile << setw(5) << "i" << setw(20) << "j(非标准变比侧)" << setw(15) << "R" << setw(15) << "X" << setw(15) << "K" << endl;
		for (i = 1; i <= Transformer_Number; i++)
		{
			Power_Flow_Savefile << setw(5) << Transformer[i].i << setw(20) << Transformer[i].j << setw(15) << Transformer[i].R << setw(15) << Transformer[i].X << setw(15) << Transformer[i].K << endl;
		}
		Power_Flow_Savefile << endl;
		Power_Flow_Savefile << "<----------------------------------------------发电机数据---------------------------------------------->" << endl << endl;
		Power_Flow_Savefile << setw(5) << "i" << setw(8) << "type" << setw(15) << "p" << setw(15) << "Q" << setw(15) << "V" << endl;
		for (i = 1; i <= Generator_Number; i++)
		{
			Power_Flow_Savefile << setw(5) << Generator[i].i << setw(8) << Generator[i].type << setw(15) << Generator[i].P << setw(15) << Generator[i].Q << setw(15) << Generator[i].V << endl;
		}
		Power_Flow_Savefile << endl;
		Power_Flow_Savefile << "<-----------------------------------------------负荷数据----------------------------------------------->" << endl << endl;
		Power_Flow_Savefile << setw(5) << "i" << setw(8) << "type" << setw(15) << "p" << setw(15) << "Q" << setw(15) << "V" << endl;
		for (i = 1; i <= Load_Number; i++)
		{
			P_Sum_load = P_Sum_load + Load[i].P;
			Q_Sum_load = Q_Sum_load + Load[i].Q;
			Power_Flow_Savefile << setw(5) << Load[i].i << setw(8) << Load[i].type << setw(15) << Load[i].P << setw(15) << Load[i].Q << setw(15) << Load[i].V << endl;
		}
		Power_Flow_Savefile << endl;

		int Flag_A, Flag_B;
		int Double_T;
		int N_Ground, N_Line = 1;
		double Double_A, Double_B, Double_C, Double_D, Double_E;
		double** Generator_Power;
		Generator_Power = (double**)new double[Generator_Number + 1];
		for (i = 1; i <= Generator_Number; i++)
			Generator_Power[i] = new double[2];
		struct Load_Power_type
		{
			double Pg, Qg, Pl, Ql;
			int i;
		};
		Load_Power_type* Load_Power;
		Load_Power = new Load_Power_type[Load_Number + 1];

		for (i = 1; i <= N; i++)           //记录各个节点的Pg，Qg，Pl,Ql
		{
			Flag_A = 0;
			Flag_B = 0;
			for (j = 1; j <= Generator_Number; j++)
			{

				if (i == Generator[j].i && Flag_A != 1)
				{
					Flag_A = 1;
					N_Ground = j;
				}
			}
			for (j = 1; j <= Load_Number; j++)
			{
				if (i != Load[j].i && Flag_A == 1 && Flag_B != 1)
				{
					Flag_B = 1;
				}
				if (i == Load[j].i && Flag_A == 1)
				{
					Generator_Power[N_Ground][0] = Load[j].P;
					Generator_Power[N_Ground][1] = Load[j].Q;
					Flag_B = 0;
					break;
				}
				if (i == Load[j].i && Flag_A != 1)
				{
					Load_Power[N_Line].i = Load[j].i;
					Load_Power[N_Line].Pg = 0;
					Load_Power[N_Line].Qg = 0;
					Load_Power[N_Line].Pl = Load[j].P;
					Load_Power[N_Line].Ql = Load[j].Q;
					N_Line = N_Line + 1;
					Flag_B = 0;
					break;
				}
			}
			if (Flag_B == 1)
			{
				Generator_Power[N_Ground][0] = 0;
				Generator_Power[N_Ground][1] = 0;
			}
		}

		double V_min;
		int Node_V_min;
		V_min = Node_Voltage[1].V;
		for (i = 2; i <= N; i++)
		{
			if (Node_Voltage[i].V < V_min)
			{
				V_min = Node_Voltage[i].V;
				Node_V_min = Node_Oringin[i];
			}
		}
		Power_Flow_Savefile << endl << endl << endl;
		Power_Flow_Savefile << "------------------------------------------------------------------------------------------------------------" << endl;
		Power_Flow_Savefile << "********************************************牛顿法潮流计算结果**********************************************" << endl;
		Power_Flow_Savefile << "------------------------------------------------------------------------------------------------------------" << endl << endl;
		Power_Flow_Savefile << "节点数：" << N << endl;
		Power_Flow_Savefile << "发电机数：" << Generator_Number << endl;
		Power_Flow_Savefile << "负荷数：" << Load_Number << endl;
		Power_Flow_Savefile << "不接地线路数：" << Branch_Not_Ground << endl;
		Power_Flow_Savefile << "接地线路数：" << Branch_Number - Branch_Not_Ground << endl;
		Power_Flow_Savefile << "变压器数：" << Transformer_Number << endl;
		Power_Flow_Savefile << "平衡节点：" << Node_Oringin[Balance_Node_Order_Number] << endl;
		Power_Flow_Savefile << "迭代次数：" << Iteration_Time << endl;
		Power_Flow_Savefile << "最大失配量：" << MAXIMUM_ERROR << endl;
		Power_Flow_Savefile << "最低电压值：" << V_min << "      " << "最低电压节点：" << Node_V_min << endl << endl << endl;
		Power_Flow_Savefile << "<-----------------------------------------------各节点数据------------------------------------------------->" << endl << endl;
		Power_Flow_Savefile << setw(8) << "节点" << setw(15) << "电压幅值" << setw(15) << "电压相位" << setw(15) << "Pg" << setw(15) << "Qg" << setw(15) << "Pl" << setw(15) << "Ql" << endl;
		for (i = 1; i <= Generator_Number; i++)
		{
			m = Generator[i].i;
			Double_T = Node_Sequence[m];
			if (fabs(Generator_Power[i][0]) < 1e-8)
				Generator_Power[i][0] = 0;
			if (fabs(Generator_Power[i][1]) < 1e-8)
				Generator_Power[i][1] = 0;
			Power_Flow_Savefile << setw(8) << m << setw(15) << Node_Voltage[Double_T].V << setw(15) << Node_Voltage[Double_T].theta * 180 / 3.1415926 << setw(15) << Node_Power[Double_T].P + Generator_Power[i][0] << setw(15) << Node_Power[Double_T].Q + Generator_Power[i][1] << setw(15) << Generator_Power[i][0] << setw(15) << Generator_Power[i][1] << endl;
			P_Sum_generator = P_Sum_generator + Node_Power[Double_T].P + Generator_Power[i][0];
			Q_Sum_generator = Q_Sum_generator + Node_Power[Double_T].Q + Generator_Power[i][1];
		}
		for (i = 1; i < N_Line; i++)
		{
			m = Load_Power[i].i;
			Double_T = Node_Sequence[m];
			Power_Flow_Savefile << setw(8) << m << setw(15) << Node_Voltage[Double_T].V << setw(15) << Node_Voltage[Double_T].theta * 180 / 3.1415926 << setw(15) << Load_Power[i].Pg << setw(15) << Load_Power[i].Qg << setw(15) << Load_Power[i].Pl << setw(15) << Load_Power[i].Ql << endl;
		}
		Power_Flow_Savefile << endl << endl;
		Power_Flow_Savefile << "<------------------------------------------------支路功率------------------------------------------------->" << endl << endl;
		Power_Flow_Savefile << setw(10) << "节点号I" << setw(10) << "节点号J" << setw(28) << "支路功率I->J" << setw(27) << "支路功率J->I" << setw(15) << "支路有功损耗" << setw(15) << "支路无功损耗" << endl;
		for (i = 1; i <= Branch_Number; i++)
		{
			ei = Node_Voltage[Node_Sequence[Branch[i].i]].V * cos(Node_Voltage[Node_Sequence[Branch[i].i]].theta);
			fi = Node_Voltage[Node_Sequence[Branch[i].i]].V * sin(Node_Voltage[Node_Sequence[Branch[i].i]].theta);
			if (Branch[i].i == Branch[i].j)
			{
				ej = 0;
				fj = 0;
			}
			else
			{
				ej = Node_Voltage[Node_Sequence[Branch[i].j]].V * cos(Node_Voltage[Node_Sequence[Branch[i].j]].theta);
				fj = Node_Voltage[Node_Sequence[Branch[i].j]].V * sin(Node_Voltage[Node_Sequence[Branch[i].j]].theta);
			}
			Double_A = Branch[i].R * Branch[i].R + Branch[i].X * Branch[i].X;
			Double_B = ei - ej;
			Double_C = fi - fj;
			Double_D = (Double_B * Branch[i].R + Double_C * Branch[i].X) / Double_A;
			Double_E = (Double_C * Branch[i].R - Double_B * Branch[i].X) / Double_A;
			if (Branch[i].i == Branch[i].j)
			{
				Double_B = (ei * ei + fi * fi) * Branch[i].X;
				Pij = 0;
				Qij = -Double_B;
				Pji = 0;
				Qji = 0;

			}
			else
			{
				Double_B = (ei * ei + fi * fi) * Branch[i].Y;  //计算i侧接地电纳对支路的功率的影响
				Double_C = (ej * ej + fj * fj) * Branch[i].Y;  //计算j侧接地电纳对支路的功率的影响
				Pij = ei * Double_D + fi * Double_E;
				Qij = fi * Double_D - ei * Double_E - Double_B;
				Pji = -ej * Double_D - fj * Double_E;
				Qji = -fj * Double_D + ej * Double_E - Double_C;
			}
			P_Sum_loss = P_Sum_loss + Pij + Pji;
			Q_Sum_loss = Q_Sum_loss + Qij + Qji;
			Branch_P_loss = Pij + Pji;
			Branch_Q_loss = Qij + Qji;
			if (fabs(Pij) < 1e-8)
				Pij = 0;
			if (fabs(Qij) < 1e-8)
				Qij = 0;
			if (fabs(Pji) < 1e-8)
				Pji = 0;
			if (fabs(Qji) < 1e-8)
				Qji = 0;
			if (fabs(Branch_P_loss) < 1e-8)
				Branch_P_loss = 0;
			if (fabs(Branch_Q_loss) < 1e-8)
				Branch_Q_loss = 0;
			Power_Flow_Savefile << setw(10) << Branch[i].i << setw(10) << Branch[i].j << Pij << "+j" << setw(15) << Qij << Pji << "+j" << setw(15) << Qji << setw(15) << Branch_P_loss << setw(15) << Branch_Q_loss << endl;
		}
		Power_Flow_Savefile << endl << endl;
		Power_Flow_Savefile << "<------------------------------------------------变压器支路------------------------------------------------>" << endl << endl;
		for (i = 1; i <= Transformer_Number; i++)
		{
			ei = Node_Voltage[Node_Sequence[Transformer[i].i]].V * cos(Node_Voltage[Node_Sequence[Transformer[i].i]].theta);
			fi = Node_Voltage[Node_Sequence[Transformer[i].i]].V * sin(Node_Voltage[Node_Sequence[Transformer[i].i]].theta);
			ej = Node_Voltage[Node_Sequence[Transformer[i].j]].V * cos(Node_Voltage[Node_Sequence[Transformer[i].j]].theta) / Transformer[i].K;
			fj = Node_Voltage[Node_Sequence[Transformer[i].j]].V * sin(Node_Voltage[Node_Sequence[Transformer[i].j]].theta) / Transformer[i].K;

			Double_A = Transformer[i].R * Transformer[i].R + Transformer[i].X * Transformer[i].X;
			Double_B = ei - ej;
			Double_C = fi - fj;
			Double_D = (Double_B * Transformer[i].R + Double_C * Transformer[i].X) / Double_A;
			Double_E = (Double_C * Transformer[i].R - Double_B * Transformer[i].X) / Double_A;
			Pij = ei * Double_D + fi * Double_E;
			Qij = fi * Double_D - ei * Double_E;
			Pji = -ej * Double_D - fj * Double_E;
			Qji = -fj * Double_D + ej * Double_E;

			P_Sum_loss = P_Sum_loss + Pij + Pji;
			Q_Sum_loss = Q_Sum_loss + Qij + Qji;

			Branch_P_loss = Pij + Pji;
			Branch_Q_loss = Qij + Qji;
			if (fabs(Pij) < 1e-8)
				Pij = 0;
			if (fabs(Qij) < 1e-8)
				Qij = 0;
			if (fabs(Pji) < 1e-8)
				Pji = 0;
			if (fabs(Qji) < 1e-8)
				Qji = 0;
			if (fabs(Branch_P_loss) < 1e-8)
				Branch_P_loss = 0;
			if (fabs(Branch_Q_loss) < 1e-8)
				Branch_Q_loss = 0;
			Power_Flow_Savefile << setw(10) << Transformer[i].i << setw(10) << Transformer[i].j << Pij << "+j" << setw(15) << Qij << Pji << "+j" << setw(15) << Qji << setw(15) << Branch_P_loss << setw(15) << Branch_Q_loss << endl;
		}
		Power_Flow_Savefile << endl << endl;
		Power_Flow_Savefile << "------------------------------------------------------------------------------------------------------------" << endl << endl;;
		Power_Flow_Savefile << "发电机有功功率：" << P_Sum_generator << "         " << "负荷有功功率：" << P_Sum_load << "          " << "系统有功损耗：" << P_Sum_loss << endl;
		Power_Flow_Savefile << "发电机无功功率：" << Q_Sum_generator << "         " << "负荷无功功率：" << Q_Sum_load << "          " << "系统无功损耗：" << Q_Sum_loss << endl;

		cout << "潮流计算结果以输出至如下地址：" << endl;
		cout << INPUT_PATH << "_Result.txt" << endl;
		cout << "***********************************************" << endl;
	}
}