/**************************************************************
 * @file       DataLoader.cpp
 * @brief      读取算例文件
 * @author     聂永欣
 * @date       2023年1月9日
**************************************************************/

#include "PowerFlow.h"


/***************************************************************
获得项目绝对地址
**************************************************************/
string Get_Operation_Location()
{
	//获得项目绝对地址
	char* ProjectPath = nullptr;
	ProjectPath = _getcwd(nullptr, 1);
	string RootPath(ProjectPath);
	delete ProjectPath;
	ProjectPath = nullptr;
	return RootPath;
}

/**************************************************************
获得算例数据地址
算例数据位于项目文件夹的data文件夹中
**************************************************************/
string Get_Operation_Location(int IEEE_type)
{
	string RootPath;
	RootPath = Get_Operation_Location();
	string INPUT_PATH = RootPath + "\\Data\\IEEE" + to_string(IEEE_type) + ".txt";
	cout << "IEEE：" << IEEE_type << "节点算例数据文件地址为：" << endl;
	cout << INPUT_PATH << endl;
	cout << "***********************************************" << endl;
	return INPUT_PATH;
}


/**************************************************************
读取算例数据
**************************************************************/
bool Data_Load(int IEEE_type)
{
	string INPUT_PATH = Get_Operation_Location(IEEE_type);
	ifstream Input_File;
	Input_File.open(INPUT_PATH, ios::in);

	if (Input_File.is_open()) {
		//读入迭代数，收敛精度，节点数，支路数，变压器数，发动机数，负荷数
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
		cout << "IEEE：" << IEEE_type << "节点算例数据文件读入成功" << endl;
		cout << "***********************************************" << endl;
		return true;
	}
	else 
	{
		cout << "文件打开失败！请确保文件可用！" << endl;
		return false;
	}

	return true;
}