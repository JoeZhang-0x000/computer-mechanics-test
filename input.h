#ifndef __INPUT_H__
#define __INPUT_H__
#include"TotalNode.h"
#include<stdio.h>	
#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include <stdlib.h>
using namespace std;
struct POINT{
    int index;
    double x;
    double y;
};
struct ELEMENT{
    int index;
    int node1;
    int node2;
    int node3;
};
TotalNode* readFile(string filename);
void split(const std::string& , 
           std::vector<std::string>& , 
           const char);
#endif
