/**************************************************************
 * @file       DataLoader.cpp
 * @brief      ��ȡ�����ļ�
 * @author     ������
 * @date       2023��1��9��
**************************************************************/

#include "PowerFlow.h"


/***************************************************************
�����Ŀ���Ե�ַ
**************************************************************/
string Get_Operation_Location()
{
	//�����Ŀ���Ե�ַ
	char* ProjectPath = nullptr;
	ProjectPath = _getcwd(nullptr, 1);
	string RootPath(ProjectPath);
	delete ProjectPath;
	ProjectPath = nullptr;
	return RootPath;
}

/**************************************************************
����������ݵ�ַ
��������λ����Ŀ�ļ��е�data�ļ�����
**************************************************************/
string Get_Operation_Location(int IEEE_type)
{
	string RootPath;
	RootPath = Get_Operation_Location();
	string INPUT_PATH = RootPath + "\\Data\\IEEE" + to_string(IEEE_type) + ".txt";
	cout << "IEEE��" << IEEE_type << "�ڵ����������ļ���ַΪ��" << endl;
	cout << INPUT_PATH << endl;
	cout << "***********************************************" << endl;
	return INPUT_PATH;
}


/**************************************************************
��ȡ��������
**************************************************************/
bool Data_Load(int IEEE_type)
{
	string INPUT_PATH = Get_Operation_Location(IEEE_type);
	ifstream Input_File;
	Input_File.open(INPUT_PATH, ios::in);

	if (Input_File.is_open()) {
		//������������������ȣ��ڵ�����֧·������ѹ����������������������
		Input_File >> Iteration_Time >> EPS >> N >> Branch_Number >> Transformer_Number >> Generator_Number >> Load_Number;  

		MAXIMUM_ITERATION_TIME = Iteration_Time;
		Branch = new Branch_Type[Branch_Number + 1];
		Transformer = new Transformer_Type[Transformer_Number + 1];
		Generator = new Generator_Type[Generator_Number + 1];
		Load = new Load_Type[Load_Number + 1];

		for (i = 1; i <= Branch_Number; i++)
		{
			Input_File >> Branch[i].i >> Branch[i].j >> Branch[i].R >> Branch[i].X >> Branch[i].Y;
		}
		for (i = 1; i <= Transformer_Number; i++)
		{
			Input_File >> Transformer[i].i >> Transformer[i].j >> Transformer[i].R >> Transformer[i].X >> Transformer[i].K;
		}
		for (i = 1; i <= Generator_Number; i++)
		{
			Input_File >> Generator[i].i >> Generator[i].type >> Generator[i].P >> Generator[i].Q >> Generator[i].V;
		}
		for (i = 1; i <= Load_Number; i++)
		{
			Input_File >> Load[i].i >> Load[i].type >> Load[i].P >> Load[i].Q >> Load[i].V;
		}

		Input_File.close();
		cout << "IEEE��" << IEEE_type << "�ڵ����������ļ�����ɹ�" << endl;
		cout << "***********************************************" << endl;
		return true;
	}
	else 
	{
		cout << "�ļ���ʧ�ܣ���ȷ���ļ����ã�" << endl;
		return false;
	}

	return true;
}