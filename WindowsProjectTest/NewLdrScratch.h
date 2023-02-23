#ifndef NEWLDRSCRATCH_H
#define NEWLDRSCRATCH_H



#include <fstream>
#include <iostream>
#include <string>
#include "string"

using namespace std;

struct header_NEW_LDR_Scratch;

struct orgDate_NEW_LDR_Scratch;

void buildOut_NEW_LDR_Scratch(header_NEW_LDR_Scratch* temp, string data, bool* flightTrap, string bString, string buildString[], int* numElements, bool* pKTOE, bool* pRTrap);

void buildHeader_NEW_LDR_Scratch(string keyword, string location, string date, int Alt);

bool file_NEW_LDR_Scratch(string fName, int* numOfElements, string outStr[]);

bool ozoneCheck_NEW_LDR_Scratch(string fstring);

void buildVertical_NEW_LDR_Scratch(string fName, int* numOfElements, string outStr[], string dataType);

void buildF_NEW_LDR_Scratch(string outStr[], int* size, string keyword);

void clearOutput_NEW_LDR_Scratch(string keyword, string loc, string date);

void clean_NEW_LDR_Scratch(string keyword);

void cleanFinal_NEW_LDR_Scratch(string keyword, string location, string date);

string buildDate_NEW_LDR_Scratch(int ac);

void normalizeNan_NEW_LDR_Scratch(string keyword, string outstr[], int size);

void useTemplate_NEW_LDR_Scratch(string fName, string keyword, string date, int filecount);

void organize_NEW_LDR_Scratch(string location, string date, int itemCount, string keyword);

void createTemplate_NEW_LDR_Scratch();

//void main_NEW_LDR_Scratch(LPWSTR fullPath);






#endif