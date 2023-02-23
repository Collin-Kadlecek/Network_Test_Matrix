#ifndef NEWHIHAPPEND_H   // To make sure you don't declare the function more than once by including the header multiple times.
#define NEWHIHAPPEND_H

#include <fstream>
#include <iostream>
//#include <stream>
#include <string>
#include <filesystem>
using namespace std;

struct header_NEW_HIH_Append;

struct orgDate_NEW_HIH_Append;

int createTemplate_NEW_HIH_Append(int fileCount);

void buildOut_NEW_HIH_Append(header_NEW_HIH_Append* temp, string data, bool* flightTrap, string bString, string buildString[], int* numElements, bool* pKTOE, bool* pRTrap);

void buildHeader_NEW_HIH_Append(string keyword, string location, string date, int Alt);

bool file_NEW_HIH_Append(WCHAR fName[], int* numOfElements, string outStr[], int pathLength, bool issa_flight);

bool ozoneCheck_NEW_HIH_Append(string fstring);

void buildVertical_NEW_HIH_Append(wstring fName, int* numOfElements, string outStr[], string dataType);

void buildF_NEW_HIH_Append(string outStr[], int* size, string keyword,bool first);

void clearOutput_NEW_HIH_Append(string keyword, wstring loc, LPWSTR outputF_date);

void clean_NEW_HIH_Append(string keyword);

void cleanFinal_NEW_HIH_Append(string keyword, string location, string date);

string buildDate_NEW_HIH_Append(int ac);

void normalizeNan_NEW_HIH_Append(string keyword, string outstr[], int size);

bool useTemplate_NEW_HIH_Append(string fName, string keyword, LPWSTR outputF_date, LPWSTR archive_rootPath, LPWSTR archive_date, wstring L_keywords, int filecount);

void organize_NEW_HIH_Append(string location, string date, int itemCount, string keyword, string filename);




#endif