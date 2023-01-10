/***************************************************************
 * @file       Main.cpp
 * @brief      ţ��-����ѷ�������������
 * @author     ������
 * @date       2023��1��9��
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
	cout << "ţ��-����ѷ�������������" << endl;
	cout << "���ߣ�������" << endl;
	cout << "�༶��˶2064" << endl;
	cout << "ѧ�ţ�3122104125" << endl;
	cout << "��ʦ������ ����" << endl;
	cout << "***********************************************" << endl;
}

int IEEE_Type_Select()
{
	int IEEE_type;
	bool False_Input;
	cout << "������Ҫ����������ڵ���:" << endl;
	cout << "�����ʽ:5��14��30��57��118��300" << endl;
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
			cout << "������󣬲�����IEEE" << IEEE_type << "�ڵ����������ļ�!" << endl;
			cout << "����������Ҫ����������ڵ�����" << endl;
		}
	} while (False_Input);
	cout << "��ʼ����IEEE��" << IEEE_type << "�ڵ�����" << endl;
	cout << "***********************************************" << endl;
	return IEEE_type;
}

void PowerFlow(int IEEE_type)
{
	clock_t begin, finish;
	bool File_Loaded = false; //�ж������ļ��Ƿ�������
	// ��ʼ��ʱ
	begin = clock();

	File_Loaded = Data_Load(IEEE_type); // ����ϵͳ����
	if (File_Loaded)
	{
		Node_Optimization(); // �ڵ����Ż�
		Conductance_Matrix_Generater(); // ���ɾ�������
		Newton_Iteration(); // ţ�ٷ���������
		Result_Output(IEEE_type);
	}

	// ������ʱ
	finish = clock();
	double endtime = (double)(finish - begin) / CLOCKS_PER_SEC;
	cout << "ţ��-����ѷ������IEEE" << IEEE_type << "�ڵ����������ѵ�ʱ��Ϊ��" << endtime << "s" << endl;
	system("pause");
}