#ifndef NEWLDRAPPEND_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define NEWLDRAPPEND_H

#include <fstream>
#include <iostream>
//#include <stream>
#include <string>
#include <filesystem>
using namespace std;

struct header_NEW_LDR_Append;

struct orgDate_NEW_LDR_Append;

void buildOut_NEW_LDR_Append(header_NEW_LDR_Append* temp, string data, bool* flightTrap, string bString, string buildString[], int* numElements, bool* pKTOE, bool* pRTrap);

void buildHeader_NEW_LDR_Append(string keyword, string location, string date, int Alt);

bool file_NEW_LDR_Append(WCHAR fName[], int* numOfElements, string outStr[], int pathLength, bool issa_flight,bool first);

void buildVertical_NEW_LDR_Append(WCHAR fName[], int* numOfElements, string outStr[], string dataType, int pathLength);

void buildF_NEW_LDR_Append(string outStr[], int* size, string keyword, bool first);

void clearOutput_NEW_LDR_Append(string keyword, wstring L_keywords, wstring outputF_date);

void clean_NEW_LDR_Append(string keyword);

void cleanFinal_NEW_LDR_Append(string keyword, string location, string date);

void normalizeNan_NEW_LDR_Append(string keyword, string outstr[], int size);

bool useTemplate_NEW_LDR_Append(string fName, string keyword, LPWSTR outputF_date, LPWSTR archive_rootPath, LPWSTR archive_date, wstring L_keywords, int filecount);

void organize_NEW_LDR_Append(string location, string date, int itemCount, string keyword);

int createTemplate_NEW_LDR_Append(int fileCount);






#endif
