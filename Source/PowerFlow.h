#pragma once

#ifndef PowerFlow_H__
#define PowerFlow_H__

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>    
#include <cmath>
#include <cstdio>
#include <string>
#include <ctime> 
#include <direct.h>

#include "DataStruct.h"
#include "Variables.h"

using namespace std;

void Initialization_Imformation();
int IEEE_Type_Select();
extern bool Data_Load(int IEEE_type);
extern void Node_Optimization();
extern void Conductance_Matrix_Generater();
extern void Newton_Iteration();
extern void PowerFlow(int IEEE_type);
extern void Result_Output(int IEEE_type);
extern string Get_Operation_Location();

#endif