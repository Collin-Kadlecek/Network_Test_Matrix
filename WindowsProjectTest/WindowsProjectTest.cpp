// WindowsProjectTest.cpp : Defines the entry point for the application.
//

#include "resource.h"
#include "framework.h"
#include "WindowsProjectTest.h"
#include "Winuser.h"
#include "Windows.h"
#include "NEWLDRSCRATCH.h"
#include "NewLdrAppend.h"
#include "NEWHIHAppend.h"
#include "NEWHIHScratch.h"
#include "NEWBLDScratch.h"
#include "NEWBLDAppend.h"
#include "OpenAllDirectoryFiles.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <Commdlg.h>
#include "atlstr.h"


#include <fstream>
#include <iostream>
#include <string>
#include <vector>
//#include <stream>
using namespace std;

#define CreateWindowA(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)\
CreateWindowExA(0L, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)

#define CreateWindowW(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)\
CreateWindowExW(0L, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)

#ifdef UNICODE
#define CreateWindow  CreateWindowW
#else
#define CreateWindow  CreateWindowA
#endif

#define in_test 1
#define OPEN_FILE_BUTTON 2
#define ldr 3
#define hih 4
#define bld 5
#define NEW_ldr 6
#define NEW_hih 7
#define NEW_bld 8
#define append 9
#define build_full 10
#define CHOOSE_ARCHIVE 11
#define Build_Header 12
#define helpstep1 13
#define helpstep2 14
#define helpstep3 15
#define helpstep4 16
#define getArchive 17
#define setArchive 18



#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
//HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void buttonTest(HWND);
HMENU hMenu;

HWND hEdit, hIn, hOut;

//HWND helpstep1, helpstep2, helpstep3, helpstep4;

bool ldr_bool, hih_bool, bld_bool, scratch_bool, NEW_ldr_bool, NEW_hih_bool, NEW_bld_bool = false;
bool append_bool = true;
bool findSpace(wchar_t a);
bool findNull(wchar_t a);
bool findSlash(wchar_t a);
void mockAppend(WCHAR fPath[], bool final);

//========================================Convert wide to normal string=============================

#define BUFFER_SIZE 100


//Returns false if the charcater plugged in is a period
bool findPeriod(wchar_t a) {
    if (int(a) == 46) {
        return false;
    }
    else {
        return true;
    }
}

struct LPWSTR_and_int {
    LPWSTR rootPath_pop;
    int pathIndex_pop;
};

vector <int> create_FileNameindexes(LPWSTR fullPath, int pathLength) {
    vector <int> fileNameIndexes;
    for (int i = 0; i < pathLength; i++) {
        //if (!findSpace(fullPath[i])) {
        if (!findNull(fullPath[i])) {
            fileNameIndexes.push_back(i);
            //MessageBox(NULL, L"=========================================================================================================================================================================================", L"", MB_OK);
        }
    }
    return fileNameIndexes;
}

bool check_number_of_files(int a) {
    if (a == 0) {
        MessageBox(NULL, L"Exception for no selected file 2", L"", MB_OK);
        return false;
    }
    else if (a == 1) {
        MessageBox(NULL, L"Exception for single selected file", L"", MB_OK);
        return false;
    }
    else {
        return true;
    }
}

LPWSTR_and_int create_rootPath(LPWSTR fullPath, int pathLength) {
    int pathIndex = 0;
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(fullPath[i])) {
            pathIndex = i;
        }
    }
    if (pathIndex != 0) {
        pathIndex = pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid file path", L"", MB_OK);
        LPWSTR_and_int john;
        john.pathIndex_pop = -5;
        return john;
    }

    wstring fix;
    fix = fullPath;
    fix.erase(pathIndex, pathLength);
    LPWSTR rootPath = &fix[0];
    LPWSTR_and_int rootPath_temp;
    rootPath_temp.pathIndex_pop = pathIndex;
    rootPath_temp.rootPath_pop = rootPath;
    return rootPath_temp;
}

WCHAR *build_file_path(LPWSTR rootPath, LPWSTR frick,vector <int> fileNameIndexes, int pathLength, int pathIndex, int i ) {
    std::wstring temp = frick;

    if (i == fileNameIndexes.size() - 1) {
        temp.erase(0, fileNameIndexes.at(i));
    }
    else {
        temp.erase(fileNameIndexes.at(i + 1), pathLength);
        temp.erase(0, fileNameIndexes.at(i));
    }

    WCHAR fLoc[1024];

    for (int f = 0; f < pathIndex; f++) {
        fLoc[f] = rootPath[f];
    }
    for (int f = pathIndex; f < temp.length() + pathIndex; f++) {
        fLoc[f] = temp[f - pathIndex + 1];
    }
    for (int f = temp.length() + pathIndex; f < 1024; f++) {
        fLoc[f] = NULL;
    }
    return fLoc;
}





//==================================== "Global functions for file selection navigation =====================
int howManySelectedFiles(LPWSTR fPath) {
    int pathLength = wcslen(fPath);

    //find location of "space" delimeted filenames
    vector <int> fileNameIndexes;
    for (int i = 0; i < pathLength; i++) {
        if (!findSpace(fPath[i])) {
            fileNameIndexes.push_back(i);
            //MessageBox(NULL, L"=========================================================================================================================================================================================", L"", MB_OK);
        }
        else {

        }
    }
    return fileNameIndexes.size();
}


char* convertToString(const wchar_t* pWCBuffer)
{
    size_t i;
    char* pMBBuffer = (char*)malloc(BUFFER_SIZE);
    //const wchar_t* pWCBuffer = L"Hello, world.";

    //printf("Convert wide-character string:\n");
    MessageBox(NULL, L"In Function", L"", MB_OK);


    // Conversion
    wcstombs_s(&i, pMBBuffer, (size_t)BUFFER_SIZE,
        pWCBuffer, (size_t)BUFFER_SIZE - 1); // -1 so the appended NULL doesn't fall outside the allocated buffer
    MessageBoxA(NULL, pMBBuffer, "", MB_OK);

    // Output
        //printf("   Characters converted: %u\n", i);
        //printf("    Multibyte character: %s\n\n", pMBBuffer);

        // Free multibyte character buffer
    if (pMBBuffer)
    {
        free(pMBBuffer);
    }

    return pMBBuffer;
}


//=============================================LDR APPEND==========================================
struct header
{
    string in;
    int skip;
    int wordCount;
};

struct orgDate {
    string line;
    string date;
};
void buildOut_ldr(header* temp, string data, bool* flightTrap, string bString, string buildString[], int* numElements) {

    /*
    to allign the rows, make each string set number of characters long
    move entire string one over, place space in first then space after so it has set length
    have it alligned with whatever comes up

    Default initial variables to empty strings of correct length and read in through if statement
    so that spacing and iteration synchonicity stays

    For RS model turn: Vaisala RS80 into: Vaisala_RS80

     If ozone is not in the thing then close and reopen file to start form top


    */

    if (data == "Flight") {
        temp->in = data;
        temp->skip = -3;
        temp->wordCount = 0;
    }
    else if (data == "Launch") {

        if (*flightTrap) {
            bool letTrap = false;
            if (bString[10] == 'a' || bString[10] == 'b') letTrap = true;

            if (letTrap) {
                string newString = "MM/DD/YYYY";
                int s = 0;
                newString[0] = bString[s + 6];
                newString[1] = bString[s + 7];
                newString[2] = '/';
                newString[3] = bString[s + 8];
                newString[4] = bString[s + 9];
                newString[5] = '/';
                newString[6] = bString[s + 2];
                newString[7] = bString[s + 3];
                newString[8] = bString[s + 4];
                newString[9] = bString[s + 5];
                *(buildString + *numElements) = newString;
                *numElements = *numElements + 1;
                temp->in = "NIF";
                *flightTrap = false;
            }
            else {
                string newString = "MM/DD/YYYY";
                int s = 1;
                newString[0] = bString[s + 6];
                newString[1] = bString[s + 7];
                newString[2] = '/';
                newString[3] = bString[s + 8];
                newString[4] = bString[s + 9];
                newString[5] = '/';
                newString[6] = bString[s + 2];
                newString[7] = bString[s + 3];
                newString[8] = bString[s + 4];
                newString[9] = bString[s + 5];
                *(buildString + *numElements) = newString;
                *numElements = *numElements + 1;
                temp->in = "NIF";
                *flightTrap = false;
            }
        }
        else {
            temp->in = data;
            temp->skip = -3;
            temp->wordCount = 0;
        }



    }
    else if (data == "Radiosonde") {
        temp->in = data;
        temp->skip = -2;
        temp->wordCount = 0;
    }
    else if (data == "ID") {
        temp->in = data;
        temp->skip = -2;
        temp->wordCount = 0;
    }
    else {
        data = "NIF";
        temp->in = data;
        temp->skip = 12;
        temp->wordCount = 0;
    }
}

bool file_ldr(WCHAR fName[], int* numOfElements, string outStr[], int pathLength, bool issaFlight) {
    ifstream fin;
    fin.open(fName);
    int f = 0;
    int j = 0;
    bool trap = true;
    float redV;
    float buildArr[300];
    string buildStr[400];
    int totElements = 1;
    int* ptotElements = &totElements;
    string line;
    bool flightTrap = true;
    bool* pflightTrap = &flightTrap;
    string sspver = "";
    if (fin.is_open()) {
        header temp[1];
        temp->in = "NIF";
        string bString = "laplaya :) ";
        //char bString[10];
        int m = 0; 

        if (issaFlight) {
            for (int i = 16;i > 5; i--) { //Build laYYYYMMDD for within file from filename

                bString[m] = *(fName + pathLength - i);//fName[pathLength - i];
                m = m + 1;
            }
        }
        else {
            for (int i = 17;i > 6; i--) { //Build laYYYYMMDD for within file from filename

                bString[m] = *(fName + pathLength - i);//fName[pathLength - i];
                m = m + 1;
            }
        }
        if (bString[0] == '\\') {
            //bString = bString.erase(0,1);
            bString[0] = ' ';
            //MessageBox(NULL, L"trip the \\ detector", L"", MB_OK);
        }

        buildStr[0] = bString;
        totElements = 1;
        string outString = "";
        for (int j = 0; j < 250;j++) {
            fin >> line;
            if (line == "Version") {
                fin >> line;
                sspver = line;
            }
            if (temp->in == "NIF") {
                buildOut_ldr(temp, line, pflightTrap, bString, buildStr, ptotElements);
            }
            outString = "";
            if (temp->skip >= 0) {
                if (temp->skip == 0) {
                    if (temp->in == "Radiosonde") {
                        outString = line;
                        if (outString == ":") {
                            fin >> line;
                            outString = line;
                        }
                        else if (outString == "InterMet" || outString == "InterMet-H" || outString == "InterMet-E") {
                            //?please
                        }
                        else {
                            fin >> line;
                            outString = outString + "_" + line;
                        }
                    }
                    else {
                        outString = outString + line;
                    }

                    if (temp->wordCount == 0) {
                        if (outString == "Level") {
                            buildStr[totElements] = "       ";
                            buildStr[totElements + 1] = "       ";
                            totElements = totElements + 2;
                        }
                        else {
                            buildStr[totElements] = outString;
                            totElements = totElements + 1;
                            temp->in = "NIF";
                            outString = "";
                        }
                    }
                    temp->wordCount = temp->wordCount - 1;
                }
            }
            else {
                temp->skip = temp->skip + 1;
            }

        }
        fin.close();
    }
    else {
        return false;
    }
    buildStr[totElements] = sspver;
    totElements = totElements + 2;
    for (int i = 0; i < totElements; i++) {
        *(outStr + i) = buildStr[i];
    }
    *numOfElements = totElements;

    return true;
}

void buildVertical_ldr(WCHAR fName[], int* numOfElements, string outStr[], string dataType, int pathLength) {
    ifstream fin;
    fin.open(fName);
    string check19 = "nope";
    //char check19[4];
    string line = "";
    bool dataSection = false;
    int m = 0; 
    for (int i = 13; i > 9; i--) {
        check19[m] = fName[pathLength - i];
        m = m + 1;
    }
    //MessageBox(NULL, fName, L"", MB_OK);

    int column = 0;
    //Choose column based on which data has been selected for reading

    if (dataType == "water") {
        column = 14;
    }
    else if (dataType == "press") {
        column = 1;
    }
    else if (dataType == "pottp") {
        column = 3;
    }
    else if (dataType == "Temp") {
        column = 4;
    }
    else if (dataType == "ftempv") {
        column = 5;
    }
    else if (dataType == "hum") {
        column = 6;
    }
    else if (dataType == "ozone_mPa") {
        column = 7;
    }
    else if (dataType == "o3_mr") {
        column = 8;
    }
    else if (dataType == "ozone_atmcm") {
        column = 9;
    }
    else if (dataType == "ptemp") {
        column = 10;
    }
    else if (dataType == "O3DN") {
        column = 11;
    }
    else if (dataType == "O3RES") {
        column = 12;
    }
    else if (dataType == "ftemp") {
        column = 13;
    }
    else {
        cout << "Incorrect keyword " << dataType << endl;
    }
    bool smum = false;
    for (int i = 0; i < pathLength; i++) {
        if (fName[i] == L'1') {
            if (fName[i + 1] == L'9' && fName[i + 2] == L'9') {
                smum = true;
            }
        }
    }
    if (check19[0] == '1' || fName[pathLength-13] == L'1' || int(fName[pathLength - 13]) == 31 || int(fName[pathLength - 13]) == 49 || fName[30] == L'1' || smum) {
        if (dataType == "water") {
            column = 8;
        }
        else if (dataType == "press") {
            column = 1;
        }
        else if (dataType == "pottp") {
            column = 3;
        }
        else if (dataType == "Temp") {
            column = 4;
        }
        else if (dataType == "ftempv") {
            column = 5;
        }
        else if (dataType == "hum") {
            column = 6;
        }
        else if (dataType == "ozone_mPa") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "o3_mr") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "ozone_atmcm") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "ptemp") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "O3DN") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "O3RES") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "ftemp") {
            column = 7;
        }
        else {
            cout << "Invalid keyword used" << endl;
        }
        while (fin >> line) {
            //Cycle through header until Level is found
            //Once its found use switch with keyword to find
            //which column data will be read from
            //Then iterate through 14 or 15 dummy
            //then read in that column's data

            if (line == "Level") {
                dataSection = true;
                for (int i = 0; i < 17 + column;i++) {//cycle to first case of data 
                    fin >> line;
                }
            }
            if (dataSection) {
                fin >> line;
                outStr[*numOfElements] = "  " + line;
                *numOfElements = *numOfElements + 1;
                for (int i = 0;i < 7;i++) {//cycle one row
                    if (fin >> line) {
                    }
                    else {
                        fin.close();
                        return;
                    }
                }
            }
        }
    }
    else {
        while (fin >> line) {
            //Cycle through header until Level is found
            //Once its found use switch with keyword to find
            //which column data will be read from
            //Then iterate through 14 or 15 dummy
            //then read in that column's data

            if (line == "Level") {
                dataSection = true;
                for (int i = 0; i < 32 + column;i++) {//cycle to first case of data
                    fin >> line;
                }
            }
            if (dataSection) {
                fin >> line;
                outStr[*numOfElements] = "  " + line;
                *numOfElements = *numOfElements + 1;
                for (int i = 0;i < 13;i++) {//cycle to next row
                    if (fin >> line) {
                    }
                    else {
                        fin.close();
                        return;
                    }
                }
            }
        }
    }
    fin.close();
}


void normalizeNan_ldr(string dataType, string outstr[], int size) {
    ifstream fin;
    string fstring;

    string inS;
    string replace;
    //decided what overflow value to place
    if (dataType == "water") {
        replace = "9.99E+09";
    }
    else if (dataType == "press") {
        replace = "9999.9";
    }
    else if (dataType == "pottp") {
        replace = "9999.9";
    }
    else if (dataType == "Temp") {
        replace = "999.9";
    }
    else if (dataType == "ftempv") {
        replace = "999.9";
    }
    else if (dataType == "hum") {
        replace = "999";
    }
    else if (dataType == "ozone_mPa") {
        replace = "99.90";
    }
    else if (dataType == "o3_mr") {
        replace = "99.999";
    }
    else if (dataType == "ozone_atmcm") {
        replace = "99.9990";
    }
    else if (dataType == "ptemp") {
        replace = "999.9";
    }
    else if (dataType == "O3DN") {
        replace = "999.999";
    }
    else if (dataType == "O3RES") {
        replace = "9999";
    }
    else if (dataType == "ftemp") {
        replace = "999.99";
    }
    for (int i = 0; i < 160; i++) {
        if (outstr[i] == "      nan" || outstr[i] == "") {
            outstr[i] = replace;
        }
    }
}

bool buildF_ldr(string outStr[], int* size, LPWSTR outputF_date, LPWSTR archive_rootPath, LPWSTR archive_date, wstring L_keywords, bool first) {
    ofstream f;
    wstring fstring;
    //fstring = "tz" + loc + "_MATLEV_" + keyword + "_" + date + ".csv";
    //fstring = L"LDR_MATLEV_" + L_keywords + L"_" + outputF_date + L".csv";

    //=============================Change the archvie location when updatign
    //=============================Newer versions of file thing
     wstring archiveLocation = archive_rootPath;
     archiveLocation = archiveLocation + L"LDR_MATLEV_" + L_keywords + L"_" + archive_date + L".csv";

    //string archiveLocation = "C:\\Users\\Kadlecek\\Desktop\\LongNotableName.txt";
    if(first){
        //write header

       
        ofstream fArchive;
        fArchive.open(archiveLocation, std::ios_base::app);
        if (fArchive.is_open()) {
            fArchive << "Filename,  FltNum,    FltDate,  FltTime,      RSModel,   RSNum,       O3ID,   HygroID,  SSPver,    X370m,   X500m,   X750m,   X1000m,   X1250m,   X1500m,   X1750m,   X2000m,   X2250m,   X2500m,   X2750m,   X3000m,   X3250m,   X3500m,   X3750m,   X4000m,   X4250m,   X4500m,   X4750m,   X5000m,   X5250m,   X5500m,   X5750m,   X6000m,   X6250m,   X6500m,   X6750m,   X7000m,   X7250m,   X7500m,   X7750m,   X8000m,   X8250m,   X8500m,   X8750m,   X9000m,   X9250m,   X9500m,   X9750m,   X10000m,   X10250m,   X10500m,   X10750m,   X11000m,   X11250m,   X11500m,   X11750m,   X12000m,   X12250m,   X12500m,   X12750m,   X13000m,   X13250m,   X13500m,   X13750m,   X14000m,   X14250m,   X14500m,   X14750m,   X15000m,   X15250m,   X15500m,   X15750m,   X16000m,   X16250m,   X16500m,   X16750m,   X17000m,   X17250m,   X17500m,   X17750m,   X18000m,   X18250m,   X18500m,   X18750m,   X19000m,   X19250m,   X19500m,   X19750m,   X20000m,   X20250m,   X20500m,   X20750m,   X21000m,   X21250m,   X21500m,   X21750m,   X22000m,   X22250m,   X22500m,   X22750m,   X23000m,   X23250m,   X23500m,   X23750m,   X24000m,   X24250m,   X24500m,   X24750m,   X25000m,   X25250m,   X25500m,   X25750m,   X26000m,   X26250m,   X26500m,   X26750m,   X27000m,   X27250m,   X27500m,   X27750m,   X28000m,   X28250m,   X28500m,   X28750m,   X29000m,   X29250m,   X29500m,   X29750m,   X30000m,   X30250m,   X30500m,   X30750m,   X31000m,   X31250m,   X31500m,   X31750m,   X32000m,   X32250m,   X32500m,   X32750m,   X33000m,   X33250m,   X33500m,   X33750m,   X34000m,   X34250m,   X34500m,   X34750m,   X35000m,   X35250m,   X35500m,   X35750m,   X36000m,   X36250m,   X36500m,   X36750m,   X37000m,   X37250m,   X37500m,   X37750m" << "\n";
        }
        else {
            MessageBox(NULL, L"Archive File did not open, please check the naming convention", L"", MB_OK);
            LPWSTR stupd = &archiveLocation[0];
            MessageBox(NULL, stupd, L"", MB_OK);
            return false;
        }
        fArchive.close();
        /*
        f.open(fstring, std::ios_base::app);
        if (fArchive.is_open()) {
            string line = "";
            while (!fArchive.eof()) {
                getline(fArchive, line);
                if (line != "") {
                    f << line + "\n";
                }
            }
            fArchive.close();
        }
        else {
            MessageBox(NULL, L"Archive file did not open, please check the naming convention", L"", MB_OK);
            return false;
        }
        f.close();
        */
    }
    f.open(archiveLocation, std::ios_base::app);
    //f << "Filename,  FltNum,    FltDate,  FltTime,      RSModel,   RSNum, HygroID,      O3ID,  SSPver,    X370m,";
    for (int i = 0; i < 160;i++) {
        string write;
        if (i != 159) {
            write = outStr[i] + ", ";
            f << write;
        }
        else {
            write = outStr[i];
            f << write;
        }
    }
    f << endl;
    f.close();
    return true;
}

void clearOutputCreate_ldr(wstring L_keywords, wstring outputF_date) {
    ifstream f;
    wstring fstring;
    fstring = L"LDR_MATLEV_" + L_keywords + L"_" + outputF_date + L".csv";
    f.open(fstring);
    if (f.is_open()) {
        f.close();
        f.open(fstring, ios::out | ios::trunc);
        f.close();
    }
    else {
        cout << "No file was found" << endl;
    }
}



void main_ldr(LPWSTR fullPath) {
    MessageBox(NULL, L"Be prepared for at least 10 minutes of waiting while the input files are gathered from this directory X:\\balloon\\water\\new_zealand\\LEV\\ ", L"Ldr LEV main selected", MB_OK);

    vector<string> filePaths = createLDR_LEV_file_archive();
    //convert string to LPWSTR:
    vector<wstring> WfilePaths;
    vector<LPWSTR> LfilePaths;
    for (int i = 0; i < filePaths.size(); i++) {
        wstring temp(filePaths.at(i).begin(),filePaths.at(i).end());
        LfilePaths.push_back(&temp[0]);
    }
    //-----------------------Enter Output File Information here---------------------------------
    //command
    //Get current date:
    string currentDateI = getTodaysDate();
    wstring WcurrentDate(currentDateI.begin(),currentDateI.end());
    LPWSTR lookAtDate = &WcurrentDate[0];
    MessageBox(NULL, lookAtDate, L"This is the output date", MB_OK);
    string site = "LDR";
    //WCHAR outputF_date[64];

    LPWSTR outputF_date = lookAtDate;


    /*
    GetWindowTextW(hIn, outputF_date, 64);
 
    for (int i = 0; i < 8; i++) {
        if (outputF_date[i] != NULL) {
        }
        else {
            MessageBox(NULL, L"======================WARNING=====================\n\n\nThe output file will be successfully made, but the length of this date does not conform to the append programs naming convention and will not be able to be appended in the future\n\n======================WARNING=====================", L"Caution!!!", MB_OK);
            break;
        }
    }
    //MessageBox(NULL, outputF_date, L"", MB_OK);

    //-----------------------Information for naming output files ---------------------  
            //Floc is the Location of folder containing files
            //FLoc path should end in a "/"" (i.e. .../downloads/Boulder_LEV2/)

    //Counting how many files were selected


    int pathLength = wcslen(fullPath);
    //MessageBox(NULL, fullPath, L"", MB_OK);
    //find location of "space" delimeted filenames
    vector <int> fileNameIndexes;
    for (int i = 0; i < pathLength; i++) {
        //if (!findSpace(fullPath[i])) {
        if (!findNull(fullPath[i])) {
            fileNameIndexes.push_back(i);
            //MessageBox(NULL, L"=========================================================================================================================================================================================", L"", MB_OK);
        }
        else {

        }
    }


    //Find file suffix
    int lastPeriod = 0;
    for (int i = 0; i < pathLength; i++) {
        if (!findPeriod(fullPath[i])) {
            lastPeriod = i;
        }
    }
    wstring suffer;
    suffer = fullPath;
    suffer.erase(0,lastPeriod);
    LPWSTR suffish = &suffer[0];


    //MessageBox(NULL, suffish, L"", MB_OK);

    //Find out if there are 1, zero or files of incorrect extension
    const LPWSTR fuck = fullPath;
    ifstream f;
    f.open(fullPath);
    bool issa_flight = false;
    if (f.is_open()) {
        MessageBox(NULL, L"Exception for single selected file ", L"", MB_OK);
        issa_flight = true;
    }
    if (!f.is_open() && fileNameIndexes.size() == 0) {
        MessageBox(NULL, L"Exception for no selected file ", L"", MB_OK);
        return;
    }


    f.close();
    //Find directory path to folder containing multiple files

    int pathIndex = 0; //rootPath Index rather
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(fullPath[i])) {
            pathIndex = i;
        }
    }
    if (pathIndex != 0) {
        pathIndex = pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid file path", L"", MB_OK);
        return;
    }

    wstring fix;
    fix = fullPath;
    fix.erase(pathIndex, pathLength);
    LPWSTR rootPath = &fix[0];

    //MessageBox(NULL, L"root path", L"", MB_OK);
    //MessageBox(NULL, rootPath, L"", MB_OK);

    //LPWSTR hurt;


    //Arhive stuff========================================
    WCHAR Archive_Selection_FilePath[1024];
    GetWindowTextW(hOut, Archive_Selection_FilePath, 1024);

    ifstream testingThing;
    testingThing.open(Archive_Selection_FilePath);
    if (testingThing.is_open()) {
        testingThing.close();
    }
    else {
        MessageBox(NULL, L"did not open the archive file so the file could not be appended", L"", MB_OK);
        MessageBox(NULL, Archive_Selection_FilePath, L"", MB_OK);
        return;
    }
    //Finding the directory to the folder of the archive file chosen
    int archive_pathIndex = 0;
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(Archive_Selection_FilePath[i])) {
            archive_pathIndex = i;
        }
    }
    if (archive_pathIndex != 0) {
        archive_pathIndex = archive_pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid archive file path", L"", MB_OK);
        return;
    }


    wstring archive_fix;
    archive_fix = Archive_Selection_FilePath;
    int archive_pathLength = wcslen(Archive_Selection_FilePath);
    archive_fix.erase(archive_pathIndex, archive_pathLength);
    LPWSTR archive_rootPath = &archive_fix[0];

    wstring archive_date_fix;
    archive_date_fix = Archive_Selection_FilePath;
    archive_date_fix.erase(0,archive_pathLength-12);
    archive_date_fix.erase(8, 12);
    LPWSTR Archive_date = &archive_date_fix[0];

    //MessageBox(NULL, archive_rootPath, Archive_date, MB_OK);


    */
    //string fLoc = "/Users/collinkadlecek/Downloads/LDR_LEV_thru20220406/la20220406.lev2";
    string outStrArr[450];//Adjust size 
    //Note this code can take up to 140 seconds to run


//-------------------------Column Head data types---------------------------------
    string keywords[13] = { "O3DN","O3RES","ftemp","ftempv","hum","ozone_atmcm","ozone_mPa","o3_mr","pottp","press","Temp","water","ptemp" };
    wstring L_keywords[13] = { L"O3DN",L"O3RES",L"ftemp",L"ftempv",L"hum",L"ozone_atmcm",L"ozone_mPa",L"o3_mr",L"pottp",L"press",L"Temp",L"water",L"ptemp" };


    //--------------------------File prefixes-----------------------------------------

    for (int i = 0; i < 13; i++) {
        clearOutputCreate_ldr(L_keywords[i], outputF_date);
    }
    int itemCount = 0;
    int maxAlt = 0;
    int allCount = 0;
    int* numOfElements = &allCount;


    
    for (int i = 0; i < filePaths.size(); i++) {
        
        LPWSTR fLoc = LfilePaths.at(i);
        int pathLength = filePaths.at(i).length();
        
        bool first = false;
        if (i == 0) {
            first = true;
        }
        else {
            first = false;
        }
        /*
        std::wstring temp = fuck;
        if (i == fileNameIndexes.size() - 1) {
            temp.erase(0, fileNameIndexes.at(i));
        }
        else {
            temp.erase(fileNameIndexes.at(i + 1), pathLength);
            temp.erase(0, fileNameIndexes.at(i));
        }

        //WCHAR fLoc[1024];

        for (int f = 0; f < pathIndex; f++) {
            fLoc[f] = rootPath[f];
        }
        for (int f = pathIndex; f < temp.length() + pathIndex; f++) {
            fLoc[f] = temp[f - pathIndex + 1];
        }
        for (int f = temp.length() + pathIndex; f < 1024; f++) {
            fLoc[f] = NULL;
        }
        //MessageBox(NULL, fLoc, L"", MB_OK);
        //mockAppend(fLoc, false);
        int pathLength = temp.length() + pathIndex;
        */
        for (int d = 0; d < 13; d++) {
            for (int f = 0; f < 160; f++) {
                outStrArr[f] = "";
            }
            if (file_ldr(fLoc, numOfElements, outStrArr, pathLength,false)) {
                //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
                itemCount = itemCount + 1;
                *numOfElements = 9;
                //BuildVertical() takes data of keywords[d] type from data section

                buildVertical_ldr(fLoc, numOfElements, outStrArr, keywords[d], pathLength);
                //fill array which currently contains: 9 heading components and numOfElements pieces of data
                //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                //200 refers to the possible 
                normalizeNan_ldr(keywords[d], outStrArr, 249);
                //Build temporary files containing rows in order which the files were read in
                wstring silly = L"X:\\balloon\\water\\new_zealand\\LEV_Matrix\\PlaceHolder.txt";
                LPWSTR archiveRootpath = &silly[0];
                if (!buildF_ldr(outStrArr, numOfElements, outputF_date, archiveRootpath, outputF_date, L_keywords[d],first)) {
                    return;
                }
                //Find the maximum altitue reached by any balloon
                if (*numOfElements - 10 > maxAlt) {
                    maxAlt = *numOfElements - 10;
                }
            }
            else {
                MessageBox(NULL, fLoc, L" was not found", MB_OK);
            }
        }
    }
    
    /*
    else {
        MessageBox(NULL, L"single Flight buildpath", L"", MB_OK);
        for (int d = 0; d < 13; d++) {
            for (int f = 0; f < 160; f++) {
                outStrArr[f] = "";
            }
            if (file_ldr(fullPath, numOfElements, outStrArr, pathLength, issa_flight)) {
                //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
                itemCount = itemCount + 1;
                //BuildVertical() takes data of keywords[d] type from data section
                *numOfElements = 9;
                buildVertical_ldr(fullPath, numOfElements, outStrArr, keywords[d], pathLength);
                //fill array which currently contains: 9 heading components and numOfElements pieces of data
                //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                //200 refers to the possible 
                normalizeNan_ldr(keywords[d], outStrArr, 249);
                //Build temporary files containing rows in order which the files were read in
                if (!buildF_ldr(outStrArr, numOfElements, outputF_date, archive_rootPath, Archive_date, L_keywords[d],true)) {
                    return;
                }
                //Find the maximum altitue reached by any balloon
                if (*numOfElements - 10 > maxAlt) {
                    maxAlt = *numOfElements - 10;
                }
            }
            else {
                MessageBox(NULL, fullPath, L" wasNot found", MB_OK);
            }
        }
    }
    */
}

void clearOutput_ldr_scratch(string keyword, string date) {
    ifstream f;
    string fstring;
    fstring = "LDR_MATLEV_" + keyword + "_" + date + ".csv";
    f.open(fstring);
    if (f.is_open()) {
        f.close();
        f.open(fstring, ios::out | ios::trunc);
        f.close();
    }
}

void buildHeader_ldr_scratch(wstring keyword, wstring date) {
    ofstream f;
    wstring fstring;
    fstring = L"LDR_MATLEV_" + keyword + L"_" + date + L".csv";
    f.open(fstring, std::ios_base::app);
    f << "Filename,  FltNum,    FltDate,  FltTime,      RSModel,   RSNum,       O3ID,   HygroID,  SSPver,    X370m,";
    for (int i = 1; i < 151;i++) {

        int temp = 250;
        temp = temp + 250 * i;
        string write;
        if (i == 150) {
            write = "   X" + to_string(temp) + "m";
        }
        else {
            write = "   X" + to_string(temp) + "m,";
        }
        f << write;
    }
    f << "\n";
    f.close();
}

void buildF_ldr_scratch(string outStr[], int* size, string keyword, string date) {
    ofstream f;
    string fstring;
    fstring = "LDR_MATLEV_" + keyword + "_" + date + ".csv";
    f.open(fstring, std::ios_base::app);
    //f << "Filename,  FltNum,    FltDate,  FltTime,      RSModel,   RSNum, HygroID,      O3ID,  SSPver,    X370m,";
    for (int i = 0; i < 160;i++) {
        string write;
        if (i != 159) {
            write = outStr[i] + ", ";
            f << write;
        }
        else {
            write = outStr[i];
            f << write;
        }
    }
    f << endl;
    f.close();
}

void main_ldr_scratch(LPWSTR fullPath) {
    MessageBox(NULL, L"Ldr Scratch", L"", MB_OK);
    //-----------------------Enter Output File Information here---------------------------------
    //command
    string site = "LDR";
    string date = "20220510";
    //-----------------------Information for naming output files ---------------------  
            //Floc is the Location of folder containing files
            //FLoc path should end in a "/"" (i.e. .../downloads/Boulder_LEV2/)

    //Counting how many files were selected


    int pathLength = wcslen(fullPath);
    MessageBox(NULL, fullPath, L"", MB_OK);
    //find location of "space" delimeted filenames
    vector <int> fileNameIndexes;
    for (int i = 0; i < pathLength; i++) {
        //if (!findSpace(fullPath[i])) {
        if (!findNull(fullPath[i])) {
            fileNameIndexes.push_back(i);
            //MessageBox(NULL, L"=========================================================================================================================================================================================", L"", MB_OK);
        }
        else {

        }
    }

    const LPWSTR fuck = fullPath;

    if (fileNameIndexes.size() == 0) {
        MessageBox(NULL, L"Exception for no selected file ", L"", MB_OK);
        return;
    }
    else if (fileNameIndexes.size() == 1) {
        MessageBox(NULL, L"Exception for single selected file", L"", MB_OK);
        return;
    }

    //Find directory path to folder containing multiple files

    int pathIndex = 0;
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(fullPath[i])) {
            pathIndex = i;
        }
    }
    if (pathIndex != 0) {
        pathIndex = pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid file path", L"", MB_OK);
        return;
    }

    wstring fix;
    fix = fullPath;
    fix.erase(pathIndex, pathLength);
    LPWSTR rootPath = &fix[0];

    //MessageBox(NULL, L"root path", L"", MB_OK);
    //MessageBox(NULL, rootPath, L"", MB_OK);

    //LPWSTR hurt;
    WCHAR hurt[1024];
    GetWindowTextW(hOut, hurt, 1024);

    ifstream testingThing;
    testingThing.open(hurt);
    if (testingThing.is_open()) {
        MessageBox(NULL, L"archive Opened!!!!!!!!!!!", hurt, MB_OK);
        MessageBox(NULL, hurt, L"", MB_OK);
        testingThing.close();
    }
    else {
        MessageBox(NULL, L"did not open the archive file", L"", MB_OK);
        MessageBox(NULL, hurt, L"", MB_OK);

    }





    //string fLoc = "/Users/collinkadlecek/Downloads/LDR_LEV_thru20220406/la20220406.lev2";
    string outStrArr[450];//Adjust size 
    //Note this code can take up to 140 seconds to run


//-------------------------Column Head data types---------------------------------
    string keywords[13] = { "O3DN","O3RES","ftemp","ftempv","hum","ozone_atmcm","ozone_mPa","o3_mr","pottp","press","Temp","water","ptemp" };
    string keyword = "";
    int l = 0;
    for (int d = 0; d < 13; d++) {
        keyword = keywords[d];
        clearOutput_ldr_scratch(keyword, date);
        int allCount = 0;
        int* numOfElements = &allCount;


        //buildHeader_ldr_scratch(keyword, date);
        string fLoc;
        fLoc = "/Users/collinkadlecek/Downloads/LDR_LEV_thru20220406/la";
        string archive = fLoc;
        string outStrArr[450];
        for (int i = 0; i < fileNameIndexes.size(); i++) {
            std::wstring temp = fuck;

            if (i == fileNameIndexes.size() - 1) {
                temp.erase(0, fileNameIndexes.at(i));
            }
            else {
                temp.erase(fileNameIndexes.at(i + 1), pathLength);
                temp.erase(0, fileNameIndexes.at(i));
            }

            WCHAR fLoc[1024];

            for (int f = 0; f < pathIndex; f++) {
                fLoc[f] = rootPath[f];
            }
            for (int f = pathIndex; f < temp.length() + pathIndex; f++) {
                fLoc[f] = temp[f - pathIndex + 1];
            }
            for (int f = temp.length() + pathIndex; f < 1024; f++) {
                fLoc[f] = NULL;
            }
            //MessageBox(NULL, fLoc, L"", MB_OK);
            //mockAppend(fLoc, false);
            int pathLength = temp.length() + pathIndex;
            for (int j = 0; j < 250; j++) {
                outStrArr[j] = "      nan";
            }
            // cout << buildDate(i) << endl;
            if (file_ldr(fLoc, numOfElements, outStrArr,pathLength,false)) {
                buildVertical_ldr(fLoc, numOfElements, outStrArr, keyword, pathLength);
                normalizeNan_ldr(keyword, outStrArr, 249);
                buildF_ldr_scratch(outStrArr, numOfElements, keyword, date);
            }
            //File folder location followed by /la
        }
    }
}


//=======================================HIH APPEND================================================================

void buildOut_hih(header* temp, string data, bool* flightTrap, string bString, string buildString[], int* numElements) {


    if (data == "Flight") {
        temp->in = data;
        temp->skip = -3;
        temp->wordCount = 0;
    }
    else if (data == "Launch") {
        if (*flightTrap) {
            bool letTrap = false;
            if (bString[10] == 'a' || bString[10] == 'b') letTrap = true;

            if (letTrap) {
                string newString = "MM/DD/YYYY";
                int s = 0;
                newString[0] = bString[s + 6];
                newString[1] = bString[s + 7];
                newString[2] = '/';
                newString[3] = bString[s + 8];
                newString[4] = bString[s + 9];
                newString[5] = '/';
                newString[6] = bString[s + 2];
                newString[7] = bString[s + 3];
                newString[8] = bString[s + 4];
                newString[9] = bString[s + 5];
                *(buildString + *numElements) = newString;
                *numElements = *numElements + 1;
                temp->in = "NIF";
                *flightTrap = false;
            }
            else {
                string newString = "MM/DD/YYYY";
                int s = 1;
                newString[0] = bString[s + 6];
                newString[1] = bString[s + 7];
                newString[2] = '/';
                newString[3] = bString[s + 8];
                newString[4] = bString[s + 9];
                newString[5] = '/';
                newString[6] = bString[s + 2];
                newString[7] = bString[s + 3];
                newString[8] = bString[s + 4];
                newString[9] = bString[s + 5];
                *(buildString + *numElements) = newString;
                *numElements = *numElements + 1;
                temp->in = "NIF";
                *flightTrap = false;
            }
        }
        else {
            temp->in = data;
            temp->skip = -3;
            temp->wordCount = 0;
        }



    }
    else if (data == "Radiosonde") {
        temp->in = data;
        temp->skip = -2;
        temp->wordCount = 0;
    }
    else if (data == "ID") {
        temp->in = data;
        temp->skip = -2;
        temp->wordCount = 0;
    }
    else {
        data = "NIF";
        temp->in = data;
        temp->skip = 12;
        temp->wordCount = 0;
    }
}

bool file_hih(WCHAR fName[], int* numOfElements, string outStr[], int pathLength, bool issa_Flight) {
    ifstream fin;
    fin.open(fName);
    int f = 0;
    int j = 0;
    bool trap = true;
    float redV;
    float buildArr[300];
    string buildStr[400];
    int totElements = 1;
    int* ptotElements = &totElements;
    string line;
    bool flightTrap = true;
    bool* pflightTrap = &flightTrap;
    string sspver = "";
    // cout << fName << endl;
    string bString = "Lickasnails";

    if (fin.is_open()) {
        // cout << "in file" << endl;
        header temp[1];
        temp->in = "NIF";
        int strLen = pathLength;
        int m = 0;

        if (issa_Flight) {
            for (int i = 16;i > 5; i--) { //Build laYYYYMMDD for within file from filename

                bString[m] = *(fName + pathLength - i);//fName[pathLength - i];
                m = m + 1;
            }
        }
        else {
            for (int i = 17;i > 6; i--) { //Build laYYYYMMDD for within file from filename

                bString[m] = *(fName + pathLength - i);//fName[pathLength - i];
                m = m + 1;
            }
        }
        if (bString[0] == '\\') {
            //bString = bString.erase(0,1);
            bString[0] = ' ';
            //MessageBox(NULL, L"trip the \\ detector", L"", MB_OK);
        }
        buildStr[0] = bString;
        outStr[0] = buildStr[0];
        totElements = 1;
        string outString = "";
        //Cycle through header
        for (int j = 0; j < 250;j++) {
            fin >> line;
            //Get SkySonde Processor Version
            if (line == "Version") {
                fin >> line;
                sspver = line;
            }
            if (temp->in == "NIF") {
                buildOut_hih(temp, line, pflightTrap, bString, buildStr, ptotElements);
            }
            outString = "";
            if (temp->skip >= 0) {
                if (temp->skip == 0) {
                    if (temp->in == "Radiosonde") {
                        outString = line;
                        if (outString == ":") {
                            fin >> line;
                            outString = line;
                        }
                        else if (outString == "InterMet" || outString == "InterMet-H" || outString == "InterMet-E") {
                        }//Ignroe if radiosonde type
                        else if (outString == "Radiosonde") {
                            outString = "   ";
                            buildStr[totElements] = "   ";
                            totElements = totElements + 1;
                        }
                        else {//Hyphonate radiosonde type
                            fin >> line;
                            outString = outString + "_" + line;
                        }
                    }
                    else {
                        outString = outString + line;
                    }

                    if (temp->wordCount == 0) {
                        if (outString == "Level") {
                            buildStr[totElements] = "       ";
                            buildStr[totElements + 1] = "       ";
                            totElements = totElements + 2;
                        }
                        else {
                            buildStr[totElements] = outString;
                            totElements = totElements + 1;
                            temp->in = "NIF";
                            outString = "";
                        }
                    }
                    temp->wordCount = temp->wordCount - 1;
                }
            }
            else {
                temp->skip = temp->skip + 1;
            }

        }
        fin.close();
    }
    else {
        cout << "target file did not open" << endl;
        return false;
    }
    buildStr[totElements] = sspver;
    totElements = totElements + 1;
    for (int i = 0; i < totElements; i++) {
        *(outStr + i) = buildStr[i];
    }
    *(outStr) = bString;
    *numOfElements = totElements;
    return true;
}

void buildVertical_hih(WCHAR fName[], int* numOfElements, string outStr[], string dataType, int pathLength) {
    ifstream fin;
    fin.open(fName);
    string check19 = "noll";
    string line = "";
    bool dataSection = false;
    int m = 0;
    for (int i = 13;i > 9; i--) { //Build laYYYYMMDD for within file from filename
        check19[m] = fName[pathLength - i];
        m = m + 1;
    }

    // cout << "lolol" << check19 << endl;
// for(int i = 53; i< 57; i++){
//     check19 = check19 + fName[i];
// }
    int column = 0;

    //Choose column based on which data has been selected for reading

    if (dataType == "water") {
        column = 14;
    }
    else if (dataType == "press") {
        column = 1;
    }
    else if (dataType == "pottp") {
        column = 3;
    }
    else if (dataType == "Temp") {
        column = 4;
    }
    else if (dataType == "ftempv") {
        column = 5;
    }
    else if (dataType == "hum") {
        column = 6;
    }
    else if (dataType == "ozone_mPa") {
        column = 7;
    }
    else if (dataType == "o3_mr") {
        column = 8;
    }
    else if (dataType == "ozone_atmcm") {
        column = 9;
    }
    else if (dataType == "ptemp") {
        column = 10;
    }
    else if (dataType == "O3DN") {
        column = 11;
    }
    else if (dataType == "O3RES") {
        column = 12;
    }
    else if (dataType == "ftemp") {
        column = 13;
    }
    else {
        cout << "Incorrect keyword " << dataType << endl;
    }
    bool smum = false;
    for (int i = 0; i < pathLength; i++) {
        if (fName[i] == L'1') {
            if (fName[i + 1] == L'9' && fName[i + 2] == L'9') {
                smum = true;
            }
        }
    }
    if (check19 == "1992" || check19 == "1991" || smum) {
        if (dataType == "water") {
            column = 8;
        }
        else if (dataType == "press") {
            column = 1;
        }
        else if (dataType == "pottp") {
            column = 3;
        }
        else if (dataType == "Temp") {
            column = 4;
        }
        else if (dataType == "ftempv") {
            column = 5;
        }
        else if (dataType == "hum") {
            column = 6;
        }
        else if (dataType == "ozone_mPa") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "o3_mr") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "ozone_atmcm") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "ptemp") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "O3DN") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "O3RES") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "ftemp") {
            column = 7;
        }
        else {
            cout << "Invalid keyword used" << endl;
        }
        while (fin >> line) {
            //Cycle through header until "Level" is found
            //Once its found use switch with keyword to find
            //which column data will be read from
            //Then iterate through 14 or 15 dummy
            //then read in that column's data

            if (line == "Level") {
                dataSection = true;
                for (int i = 0; i < 17 + column;i++) {//cycle to first case of data 
                    fin >> line;
                }
            }
            if (dataSection) {
                fin >> line;
                outStr[*numOfElements] = "  " + line;
                *numOfElements = *numOfElements + 1;
                for (int i = 0;i < 7;i++) {//cycle one row
                    if (fin >> line) {
                    }
                    else {
                        fin.close();
                        return;
                    }
                }
            }
        }
    }
    else {
        while (fin >> line) {
            //Cycle through header until "Level" is found
            //Once its found use switch with keyword to find
            //which column data will be read from
            //Then iterate through 14 or 15 dummy
            //then read in that column's data

            if (line == "Level") {
                dataSection = true;
                for (int i = 0; i < 32 + column;i++) {//cycle to first case of data
                    fin >> line;
                }
            }
            if (dataSection) {
                fin >> line;
                outStr[*numOfElements] = "  " + line;
                *numOfElements = *numOfElements + 1;
                for (int i = 0;i < 13;i++) {//cycle to next row
                    if (fin >> line) {
                    }
                    else {
                        fin.close();
                        return;
                    }
                }
            }
        }
    }
    fin.close();
}

bool buildF_hih(string outStr[], int* size, wstring L_keyword, LPWSTR outputF_date, LPWSTR archive_rootPath, LPWSTR archive_date, bool first) {
    ofstream f;
    wstring fstring;
    fstring = L"HIH_MATLEV_" + L_keyword + L"_" + outputF_date + L".csv";

    //=============================Change the archvie location when updatign
    //=============================Newer versions of file thing
    if (first) {
        wstring archiveLocation = archive_rootPath;
        archiveLocation = archiveLocation + L"HIH_MATLEV_" + L_keyword + L"_" + archive_date + L".csv";

        //string archiveLocation = "/Users/collinkadlecek/Downloads/HIH20220509/HIH_MATLEV_" + keyword + "_Archive.csv";
        ifstream fArchive;
        fArchive.open(archiveLocation);
        if (fArchive.is_open()) {
        }
        else {
            MessageBox(NULL, L"Archive File did not open, please check the naming convention", L"", MB_OK);
            LPWSTR stupd = &archiveLocation[0];
            MessageBox(NULL, stupd, L"", MB_OK);
            return false;
        }
        f.open(fstring, std::ios_base::app);
        if (fArchive.is_open()) {
            string line = "";
            while (!fArchive.eof()) {
                getline(fArchive, line);
                if (line != "") {
                    f << line + "\n";
                }
            }
            fArchive.close();
        }
        else {
            MessageBox(NULL, L"Archive file did not open, please check the naming convention", L"", MB_OK);
            return false;
        }
        f.close();
    }
    

    //f << "Filename,  FltNum,    FltDate,  FltTime,      RSModel,   RSNum, HygroID,      O3ID,  SSPver,    X370m,";
    f.open(fstring, std::ios_base::app);
    for (int i = 0; i < 156;i++) {
        string write;
        if (i != 155) {
            write = outStr[i] + ", ";
            f << write;
        }
        else {
            write = outStr[i];
            f << write;
        }
    }
    f << endl;
    f.close();
    return true;
}


void clearOutputCreate_hih(wstring L_keywords, wstring outputF_date) {
    ifstream f;
    wstring fstring;
    fstring = L"HIH_MATLEV_" + L_keywords + L"_" + outputF_date + L".csv";
    f.open(fstring);
    if (f.is_open()) {
        f.close();
        f.open(fstring, ios::out | ios::trunc);
        f.close();
    }
    else {
        cout << "No file was found" << endl;
    }
}



void normalizeNan_hih(string keyword, string outstr[], int size) {
    ifstream fin;
    string fstring;

    string inS;
    string replace;
    //decided what overflow value to place
    if (keyword == "water") {
        replace = "9.99E+09";
    }
    else if (keyword == "press") {
        replace = "9999.9";
    }
    else if (keyword == "pottp") {
        replace = "9999.9";
    }
    else if (keyword == "Temp") {
        replace = "999.9";
    }
    else if (keyword == "ftempv") {
        replace = "999.9";
    }
    else if (keyword == "hum") {
        replace = "999";
    }
    else if (keyword == "ozone_mPa") {
        replace = "99.90";
    }
    else if (keyword == "o3_mr") {
        replace = "99.999";
    }
    else if (keyword == "ozone_atmcm") {
        replace = "99.9990";
    }
    else if (keyword == "ptemp") {
        replace = "999.9";
    }
    else if (keyword == "O3DN") {
        replace = "999.999";
    }
    else if (keyword == "O3RES") {
        replace = "99999";
    }
    else if (keyword == "ftemp") {
        replace = "999.99";
    }
    for (int i = 0; i < 160; i++) {
        if (outstr[i] == "      nan" || outstr[i] == "") {
            outstr[i] = replace;
        }
    }
}

void main_hih(LPWSTR fullPath) {
    MessageBox(NULL, L"HIH append selected", L"", MB_OK);

    //-----------------------Information for naming output files ---------------------
    string site = "HIH";
    WCHAR outputF_date[64];
    GetWindowTextW(hIn, outputF_date, 64);
 
    for (int i = 0; i < 8; i++) {
        if (outputF_date[i] == NULL) {
        }
        else {
            MessageBox(NULL, L"======================WARNING=====================\n\n\nThe output file will be successfully made, but the length of this date does not conform to the append programs naming convention and will not be able to be appended in the future\n\n======================WARNING=====================", L"Caution!!!", MB_OK);
            break;
        }
    }
    //-------------------------Column Head data types---------------------------------



     //Counting how many files were selected


    int pathLength = wcslen(fullPath);
    //find location of "space" delimeted filenames
    vector <int> fileNameIndexes;
    for (int i = 0; i < pathLength; i++) {
        if (!findSpace(fullPath[i])) {
            fileNameIndexes.push_back(i);
            //MessageBox(NULL, L"=========================================================================================================================================================================================", L"", MB_OK);
        }
        else {

        }
    }

    const LPWSTR fuck = fullPath;
    ifstream f;
    f.open(fullPath);
    bool issa_flight = false;
    if (f.is_open()) {
        MessageBox(NULL, L"Exception for single selected file ", L"", MB_OK);
        issa_flight = true;
    }
    if (!f.is_open() && fileNameIndexes.size() == 0) {
        MessageBox(NULL, L"Exception for no selected file ", L"", MB_OK);
        return;
    }

    f.close();

    //Find directory path to folder containing multiple files

    int pathIndex = 0;
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(fullPath[i])) {
            pathIndex = i;
        }
    }
    if (pathIndex != 0) {
        pathIndex = pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid file path", L"", MB_OK);
        return;
    }

    wstring fix;
    fix = fullPath;
    fix.erase(pathIndex, pathLength);
    LPWSTR rootPath = &fix[0];

    //MessageBox(NULL, L"root path", L"", MB_OK);
    //MessageBox(NULL, rootPath, L"", MB_OK);

    //LPWSTR hurt;
    WCHAR hurt[1024];
    GetWindowTextW(hOut, hurt, 1024);

    //Arhive stuff========================================
    WCHAR Archive_Selection_FilePath[1024];
    GetWindowTextW(hOut, Archive_Selection_FilePath, 1024);

    ifstream testingThing;
    testingThing.open(Archive_Selection_FilePath);
    if (testingThing.is_open()) {
        testingThing.close();
    }
    else {
        MessageBox(NULL, L"did not open the archive file so the file could not be appended", L"", MB_OK);
        MessageBox(NULL, L"The input file path is the following", L"Input Archive file path", MB_OK);
        MessageBox(NULL, Archive_Selection_FilePath, L"Input Archive file path", MB_OK);
        return;
    }
    //Finding the directory to the folder of the archive file chosen
    int archive_pathIndex = 0;
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(Archive_Selection_FilePath[i])) {
            archive_pathIndex = i;
        }
    }
    if (archive_pathIndex != 0) {
        archive_pathIndex = archive_pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid archive file path", L"", MB_OK);
        return;
    }


    wstring archive_fix;
    archive_fix = Archive_Selection_FilePath;
    int archive_pathLength = wcslen(Archive_Selection_FilePath);
    archive_fix.erase(archive_pathIndex, archive_pathLength);
    LPWSTR archive_rootPath = &archive_fix[0];

    wstring archive_date_fix;
    archive_date_fix = Archive_Selection_FilePath;
    archive_date_fix.erase(0, archive_pathLength - 12);
    archive_date_fix.erase(8, 12);
    LPWSTR Archive_date = &archive_date_fix[0];







    string keywords[13] = { "O3DN","O3RES","ftemp","ftempv","hum","ozone_atmcm","ozone_mPa","o3_mr","pottp","press","Temp","water","ptemp" };
    wstring L_keywords[13] = { L"O3DN",L"O3RES",L"ftemp",L"ftempv",L"hum",L"ozone_atmcm",L"ozone_mPa",L"o3_mr",L"pottp",L"press",L"Temp",L"water",L"ptemp" };

    int itemCount = 0;
    int maxAlt = 0;
    int allCount = 0;
    int* numOfElements = &allCount;
    string outStrArr[450];
    for (int i = 0; i < 13; i++) {
        clearOutputCreate_hih(L_keywords[i], outputF_date);
    }

    if (!issa_flight) {
        for (int i = 0; i < fileNameIndexes.size(); i++) {
            bool first = false;
            if (i == 0) {
                first = true;
            }
            else {
                first = false;
            }
            std::wstring temp = fuck;

            if (i == fileNameIndexes.size() - 1) {
                temp.erase(0, fileNameIndexes.at(i));
            }
            else {
                temp.erase(fileNameIndexes.at(i + 1), pathLength);
                temp.erase(0, fileNameIndexes.at(i));
            }

            WCHAR fLoc[1024];

            for (int f = 0; f < pathIndex; f++) {
                fLoc[f] = rootPath[f];
            }
            for (int f = pathIndex; f < temp.length() + pathIndex; f++) {
                fLoc[f] = temp[f - pathIndex + 1];
            }
            for (int f = temp.length() + pathIndex; f < 1024; f++) {
                fLoc[f] = NULL;
            }
            //MessageBox(NULL, fLoc, L"", MB_OK);
            //mockAppend(fLoc, false);
            int pathLength = temp.length() + pathIndex;


            for (int d = 0; d < 13; d++) {
                for (int i = 0; i < 249; i++) {
                    outStrArr[i] = "";
                }
                if (file_hih(fLoc, numOfElements, outStrArr, pathLength, issa_flight)) {
                    //if (outStrArr[0][0] == 'h' || outStrArr[0][0] == 'H') {
                      //  MessageBox(NULL, L"1:)", L"", MB_OK);

                    //}
                    //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
                    itemCount = itemCount + 1;
                    //BuildVertical() takes data of keywords[d] type from data section
                    buildVertical_hih(fLoc, numOfElements, outStrArr, keywords[d], pathLength);
                    //if (outStrArr[0][0] == 'h' || outStrArr[0][0] == 'H') {
                        //MessageBox(NULL, L"2:)", L"", MB_OK);

                    //}
                    //fill array which currently contains: 9 heading components and numOfElements pieces of data
                    //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                    normalizeNan_hih(keywords[d], outStrArr, 249);
                    //if (outStrArr[0][0] == 'h' || outStrArr[0][0] == 'H') {
                        //MessageBox(NULL, L"3:)", L"", MB_OK);

                    //}
                    //Build temporary files containing rows in order which the files were read in
                    //buildF_hih(outStrArr, numOfElements, keywords[d], site, outputF_date)
                    if (!buildF_hih(outStrArr, numOfElements, L_keywords[d], outputF_date, archive_rootPath, Archive_date, first)) {
                        return;
                    }
                    //if (outStrArr[0][0] == 'h' || outStrArr[0][0] == 'H') {
                        //MessageBox(NULL, L"4:)", L"", MB_OK);

                    //}
                    //Find the maximum altitue reached by any balloon
                    if (*numOfElements - 10 > maxAlt) {
                        maxAlt = *numOfElements - 10;
                    }
                }
            }
        }
    }
    else {

        for (int d = 0; d < 13; d++) {
            for (int i = 0; i < 249; i++) {
                outStrArr[i] = "";
            }
            if (file_hih(fullPath, numOfElements, outStrArr, pathLength, issa_flight)) {
                //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
                itemCount = itemCount + 1;
                //BuildVertical() takes data of keywords[d] type from data section
                buildVertical_hih(fullPath, numOfElements, outStrArr, keywords[d], pathLength);
                //fill array which currently contains: 9 heading components and numOfElements pieces of data
                //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                normalizeNan_hih(keywords[d], outStrArr, 249);
                //Build temporary files containing rows in order which the files were read in
                if (!buildF_hih(outStrArr, numOfElements, L_keywords[d], outputF_date, archive_rootPath, Archive_date, true)) {
                    return;
                }                //Find the maximum altitue reached by any balloon
                if (*numOfElements - 10 > maxAlt) {
                    maxAlt = *numOfElements - 10;
                }
            }
        }
    }

}

void clearOutput_hih_scratch(string keyword, string loc, string date) {
    ifstream f;
    string fstring;
    fstring = loc + "_MATLEV_" + keyword + "_" + date + ".csv";
    f.open(fstring);
    if (f.is_open()) {
        f.close();
        f.open(fstring, ios::out | ios::trunc);
        f.close();
    }
    fstring = "temp_" + keyword + ".csv";
    f.open(fstring);
    if (f.is_open()) {
        f.close();
        f.open(fstring, ios::out | ios::trunc);
        f.close();
    }
}

void buildF_hih_scratch(string outStr[], int* size, string keyword) {
    ofstream f;
    string fstring;
    fstring = "temp_" + keyword + ".csv";
    f.open(fstring, std::ios_base::app);
    //f << "Filename,  FltNum,    FltDate,  FltTime,      RSModel,   RSNum, HygroID,      O3ID,  SSPver,    X370m,";
    for (int i = 0; i < 160;i++) {
        string write;
        if (i != 159) {
            write = outStr[i] + ", ";
            f << write;
        }
        else {
            write = outStr[i];
            f << write;
        }
    }
    f << endl;
    f.close();
}

void buildHeader_hih_scratch(wstring keyword, wstring date) {
    //ofstream f;
    //wstring fstring;
    ////fstring = L"HIH_MATLEV_" + keyword + L"_" + date + L".csv";
    //f.open(fstring, std::ios_base::app);
    //f << "Filename,  FltNum,    FltDate,  FltTime,      RSModel,   RSNum,       O3ID,   HygroID,  SSPver,    X10m,"; //Initial height
    //for (int i = 1; i <= 146;i++) {

    //    int temp = 0;
    //    temp = temp + 250 * i;
    //    string write;
    //    if (i == 146) {
    //        write = "   X" + to_string(temp) + "m";
    //    }
    //    else {
    //        write = "   X" + to_string(temp) + "m,";
    //    }
    //    f << write;
    //}
    //f << "\n";
    //f.close();
}

void organize_hih_scratch(string location, string date, int itemCount, string keyword) {

    string fName;
    ifstream fin;
    orgDate fileLines[250];
    int index = 0;
    fName = "Temp_" + keyword + ".csv";
    fin.open(fName.c_str());
    if (!fin.is_open()) {
        MessageBox(NULL, L"Data files were not written to temporary files", L"", MB_OK);
        return;
    }
    bool finished = false;
    string earliest = "1";
    while (fin.is_open() && !finished) {
        MessageBox(NULL, L"douby", L"", MB_OK);

        string line = "";
        getline(fin, line);
        if (line == "") {
            finished = true;
        }
        fileLines[index].line = line;
        string deconstruct = "";
        string date = "";
        deconstruct = line;

        for (int j = 2; j < 10; j++) {
            date = date + deconstruct[j];
        }
        fileLines[index].date = date;
        if (earliest < date) {
            earliest = date;
        }
        if (index == itemCount) {
            finished = true;
        }
        index = index + 1;

    }
    MessageBox(NULL, L"during6", L"", MB_OK);

    index = index - 1;
    finished = false;
    int rowCount = 0;
    string finalOutStr[250];
    bool found = false;
    while (!finished) {
        MessageBox(NULL, L"during 5", L"", MB_OK);

        int w = 0;
        found = false;
        while (!found)
        {
            if (earliest == "1") {
                MessageBox(NULL, L"asdfds", L"", MB_OK);

            }
            if (fileLines[w].date == earliest) {
                MessageBox(NULL, L"douby", L"", MB_OK);

                found = true;
                break;
            }
            w = w + 1;
        }

        finalOutStr[rowCount] = fileLines[w].line;

        if (rowCount == index) {
            finished = true;
        }
        MessageBox(NULL, L"during 7", L"", MB_OK);

        string champ = fileLines[index].date;
        MessageBox(NULL, L"during 8", L"", MB_OK);

        for (int j = 0; j < index; j++) {

            if (fileLines[j].date < earliest && fileLines[j].date > champ) {
                champ = fileLines[j].date;
            }
        }
        earliest = champ;
        rowCount = rowCount + 1;
    }

    //flip array
    string temp[250];
    for (int l = index - 1; l >= 0; l--) {
        temp[index - l] = finalOutStr[l];
    }
    ofstream f;
    string fstring;
    fstring = location + "_MATLEV_" + keyword + "_" + date + ".csv";
    f.open(fstring, std::ios_base::app);
    for (int i = 1; i <= index; i++) {
        f << temp[i] << endl;
    }
    f.close();
}

void clean_hih_scratch(string keyword) {
    string a = "temp_" + keyword + ".csv";
    char fileName[50];
    for (int i = 0; i < a.length(); i++) {
        fileName[i] = a[i];
    }
    int result = remove(fileName);

}

void main_hih_scratch(LPWSTR fullPath) {
    //-----------------------Information for naming output files ---------------------
    string site = "HIH";
    string date = "20220127";

    int pathLength = wcslen(fullPath);
    MessageBox(NULL, fullPath, L"", MB_OK);
    //find location of "space" delimeted filenames

    vector <int> fileNameIndexes = create_FileNameindexes(fullPath, pathLength);

    const LPWSTR fuck = fullPath;

    if (!check_number_of_files(fileNameIndexes.size())) {
        MessageBox(NULL, L"Check number of files??", L"", MB_OK);
    }

    //Find directory path to folder containing multiple files

    LPWSTR_and_int rootPath_temp = create_rootPath(fullPath, pathLength);
    MessageBox(NULL, rootPath_temp.rootPath_pop, L"", MB_OK);

    LPWSTR rootPath = rootPath_temp.rootPath_pop;
    int pathIndex = rootPath_temp.pathIndex_pop;
    if (pathIndex == -5) {
        MessageBox(NULL, L"Sooooooooooomething went wrong", L"", MB_OK);
        return;
    }
    //-------------------------Column Head data types---------------------------------
    string keywords[13] = { "O3DN","O3RES","ftemp","ftempv","hum","ozone_atmcm","ozone_mPa","o3_mr","pottp","press","Temp","water","ptemp" };
    int itemCount = 0;
    int maxAlt = 0;
    int allCount = 0;
    int* numOfElements = &allCount;
    //Do following for each data type

    for (int d = 0; d < 13; d++) {
        clearOutput_hih_scratch(keywords[d], site, date);
        //-----------------------Enter File location here---------------------------------
        
        string outStrArr[450];
        //--------------------------File prefixes-----------------------------------------
        string prefix[2] = { "hh","hi" };
        //Iterate over all prefixes
        for (int f = 0; f < 2; f++) {
            //Iterate for 17000 days starting in 01/01/1991
            MessageBox(NULL, rootPath, L"", MB_OK);

            for (int i = 0; i < fileNameIndexes.size(); i++) {
                std::wstring temp = fuck;

                if (i == fileNameIndexes.size() - 1) {
                    temp.erase(0, fileNameIndexes.at(i));
                }
                else {
                    temp.erase(fileNameIndexes.at(i + 1), pathLength);
                    temp.erase(0, fileNameIndexes.at(i));
                }

                WCHAR fLoc[1024];

                for (int f = 0; f < pathIndex; f++) {
                    fLoc[f] = rootPath[f];
                }
                for (int f = pathIndex; f < temp.length() + pathIndex; f++) {
                    fLoc[f] = temp[f - pathIndex + 1];
                }
                for (int f = temp.length() + pathIndex; f < 1024; f++) {
                    fLoc[f] = NULL;
                }
                //MessageBox(NULL, fLoc, L"", MB_OK);
                //mockAppend(fLoc, false);
                int pathLength = temp.length() + pathIndex;

                for (int j = 0; j < 250; j++) {
                    outStrArr[j] = "      nan";
                }
                //Build file name at a date
                //If file name built is an existing file

                if (file_hih(fLoc, numOfElements, outStrArr, pathLength, false)) {
                    MessageBox(NULL, fLoc, L"", MB_OK);

                    //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
                    itemCount = itemCount + 1;
                    //BuildVertical() takes data of keywords[d] type from data section
                    buildVertical_hih(fLoc, numOfElements, outStrArr, keywords[d],pathLength);
                    //fill array which currently contains: 9 heading components and numOfElements pieces of data
                    //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                    normalizeNan_hih(keywords[d], outStrArr, 249);
                    //Build temporary files containing rows in order which the files were read in
                    buildF_hih_scratch(outStrArr, numOfElements, keywords[d]);
                    //Find the maximum altitue reached by any balloon
                    if (*numOfElements - 10 > maxAlt) {
                        maxAlt = *numOfElements - 10;
                    }
                }
                //prepare file location for next iteration by stripping file specific information
            }
        }
    }

    //For each datatype organize the file by date and output file with naming following conventions input above
    for (int e = 0; e < 13; e++) {
        MessageBox(NULL, L"during 1", L"", MB_OK);

        //buildHeader_hih_scratch(keywords[e], site, date, maxAlt);
        MessageBox(NULL, L"during 2", L"", MB_OK);

        organize_hih_scratch(site, date, itemCount, keywords[e]);
        MessageBox(NULL, L"during 3", L"", MB_OK);

        //Delete unorganized files
        clean_hih_scratch(keywords[e]);
        MessageBox(NULL, L"during 4", L"", MB_OK);

    }

}

//======================================BLD APPEND================================================================
void buildOut_bld(header* temp, string data, bool* flightTrap, string bString, string buildString[], int* numElements, bool* pKTOE, bool* pRTrap) {

    //Function which provides a unique word which once found provides instructions to the file() function 
    // to read in the data for the keyword
    if (data == "Flight") {
        temp->in = data;
        temp->skip = -3;
        temp->wordCount = 0;
    }
    else if (data == "Launch") {
        if (*flightTrap) {
            bool letTrap = false;
            if (bString[10] == 'a' || bString[10] == 'b') letTrap = true;
            if (letTrap) {
                string newString = "MM/DD/YYYY";
                int s = 0;
                newString[0] = bString[s + 6];
                newString[1] = bString[s + 7];
                newString[2] = '/';
                newString[3] = bString[s + 8];
                newString[4] = bString[s + 9];
                newString[5] = '/';
                newString[6] = bString[s + 2];
                newString[7] = bString[s + 3];
                newString[8] = bString[s + 4];
                newString[9] = bString[s + 5];
                *(buildString + *numElements) = newString;
                *numElements = *numElements + 1;
                temp->in = "NIF";
                *flightTrap = false;
                *(pKTOE + 1) = true;
            }
            else {
                string newString = "MM/DD/YYYY";
                int s = 1;
                newString[0] = bString[s + 6];
                newString[1] = bString[s + 7];
                newString[2] = '/';
                newString[3] = bString[s + 8];
                newString[4] = bString[s + 9];
                newString[5] = '/';
                newString[6] = bString[s + 2];
                newString[7] = bString[s + 3];
                newString[8] = bString[s + 4];
                newString[9] = bString[s + 5];
                *(buildString + *numElements) = newString;
                *numElements = *numElements + 1;
                temp->in = "NIF";
                *flightTrap = false;
                *(pKTOE + 1) = true;
            }

        }
        else {
            temp->in = data;
            temp->skip = -3;
            temp->wordCount = 0;
            *(pKTOE + 2) = true;
        }
    }
    else if (data == "Radiosonde") {
        temp->in = data;
        temp->skip = -2;
        temp->wordCount = 0;
        if (*pRTrap) {
            *(pKTOE + 3) = true;
        }
    }
    else if (data == "ID") {
        temp->in = data;
        temp->skip = -2;
        temp->wordCount = 0;
        if (*pRTrap) {
            *pRTrap = false;
        }
        else {
            *pRTrap = true;
        }
    }
    else {
        data = "NIF";
        temp->in = data;
        temp->skip = 12;
        temp->wordCount = 0;
    }
}

bool file_bld(WCHAR fName[], int* numOfElements, string outStr[], int pathLength, bool issa_flight) {
    ifstream fin;
    fin.open(fName);
    int f = 0;
    int j = 0;
    bool trap = true;
    //Array to keep track of which header elements have been added to the array in order
    //Filename, FltNum, FltDate, FltTime, RSModel, RSNum, O3ID, HygroID, SSPver
    bool keepTrackOfElements[10] = { false,false,false,false,false,false,false,false,false,false };
    bool* pKTOE = &keepTrackOfElements[0];
    bool radiosondeTrap = true;
    bool* pRTrap = &radiosondeTrap;
    float redV;
    float buildArr[300];
    string buildStr[400];
    int totElements = 1;
    int* ptotElements = &totElements;
    string line;
    bool flightTrap = true;
    bool* pflightTrap = &flightTrap;
    string sspver = "";
    // //cout << fName << endl;

    if (fin.is_open()) {
        // //cout << "in file" << endl;
        header temp[1];
        temp->in = "NIF";
        int strLen = pathLength;
        int m = 0; 
        string bString = "Lickasnails";

        if (issa_flight) {
            for (int i = 16;i > 5; i--) { //Build laYYYYMMDD for within file from filename

                bString[m] = *(fName + pathLength - i);//fName[pathLength - i];
                m = m + 1;
            }
        }
        else {
            for (int i = 17;i > 6; i--) { //Build laYYYYMMDD for within file from filename
                bString[m] = *(fName + pathLength - i);
                m = m + 1;
                keepTrackOfElements[0] = true;
            }
        }
        if (bString[0] == '\\') {
            //bString = bString.erase(0,1);
            bString[0] = ' ';
            //MessageBox(NULL, L"trip the \\ detector", L"", MB_OK);
        }
        buildStr[0] = bString;
        outStr[0] = buildStr[0];
        totElements = 1;
        string outString = "";
        //Cycle through header
        for (int j = 0; j < 250;j++) {
            fin >> line;
            //Get SkySonde Processor Version
            if (line == "Version") {
                fin >> line;
                sspver = line;
            }
            if (temp->in == "NIF") {
                buildOut_bld(temp, line, pflightTrap, bString, buildStr, ptotElements, pKTOE, pRTrap);
            }
            outString = "";
            if (temp->skip >= 0) {
                if (temp->skip == 0) {
                    if (temp->in == "Radiosonde") {
                        outString = line;
                        if (outString == ":") {
                            fin >> line;
                            outString = line;
                        }
                        else if (outString == "InterMet" || outString == "InterMet-H" || outString == "InterMet-E") {
                        }//Ignroe if radiosonde type
                        else if (outString == "Radiosonde") {
                            outString = "         ";
                            buildStr[totElements] = "         ";
                            totElements = totElements + 1;
                        }
                        else {//Hyphonate radiosonde type
                            fin >> line;
                            outString = outString + "_" + line;
                        }
                    }
                    else {
                        outString = outString + line;
                    }

                    if (temp->wordCount == 0) {
                        if (outString == "Level") {//If Hygrometer ID is missing

                            buildStr[totElements] = "       ";
                            // buildStr[totElements+1] = "    /   ";
                            totElements = totElements + 1;
                        }
                        else {
                            buildStr[totElements] = outString;
                            totElements = totElements + 1;
                            temp->in = "NIF";
                            outString = "";
                        }
                    }
                    temp->wordCount = temp->wordCount - 1;
                }
            }
            else {
                temp->skip = temp->skip + 1;
            }

        }
        fin.close();
    }
    else {
        return false;
    }
    //If only one radiosonde value was read in
    if (!*pRTrap) {
        //cout << buildStr[totElements-1] << "Hi " << endl;
        buildStr[totElements] = buildStr[totElements - 1];
        buildStr[totElements - 1] = "      ";
        totElements = totElements + 1;
    }
    //If neither radiosonde value was entered
    if (!*(pKTOE + 3)) {
        buildStr[totElements] = "        ";
        buildStr[totElements + 1] = "        ";
        totElements = totElements + 2;
    }
    buildStr[totElements] = sspver;
    totElements = totElements + 1;
    for (int i = 0; i < totElements; i++) {
        *(outStr + i) = buildStr[i];
    }
    *numOfElements = totElements;
    return true;
}

bool ozoneCheck(string fstring) {
    ifstream fin;
    fin.open(fstring);
    string line;
    fin >> line;
    while (line != "Level" || fin.eof()) {
        fin >> line;
    }
    int count = 0;
    while (line != "Num" || fin.eof()) {
        fin >> line;
        count = count + 1;
    }
    if (count == 9) {//return false if ozone data is missing
        return false;
    }
    else if (count == 18) { //return true if the number of columns indicate ozone data is present
        return true;
    }
    else {
        cout << "Unknown file structure, treated as if ozone data is missing " << count << endl;
        return false;
    }
}

void buildVertical_bld(WCHAR fName[], int* numOfElements, string outStr[], string dataType, int pathLength) {
    ifstream fin;
    fin.open(fName);
    string line = "";
    bool dataSection = false;
    // for(int i = 13;i > 10; i--){ //Build laYYYYMMDD for within file from filename
    //     check19 = check19 + fName[fName.length() - i];
    // }
    
    
    
    string check2015 = "eightyes";
    int l = 0;
    for (int i = 13; i > 5; i--) {
        check2015[l ] = fName[pathLength - i];
    }
   
   
    // if(check2015[0] == '2' && check2015[2] == '1' && check2015[3] == '5'){
    //     cout << check2015 << endl;
    // }
    int column = 0;

    //Choose column based on which data has been selected for reading

    if (dataType == "water") {
        column = 14;
    }
    else if (dataType == "press") {
        column = 1;
    }
    else if (dataType == "pottp") {
        column = 3;
    }
    else if (dataType == "Temp") {
        column = 4;
    }
    else if (dataType == "ftempv") {
        column = 5;
    }
    else if (dataType == "hum") {
        column = 6;
    }
    else if (dataType == "ozone_mPa") {
        column = 7;
    }
    else if (dataType == "o3_mr") {
        column = 8;
    }
    else if (dataType == "ozone_atmcm") {
        column = 9;
    }
    else if (dataType == "ptemp") {
        column = 10;
    }
    else if (dataType == "O3DN") {
        column = 11;
    }
    else if (dataType == "O3RES") {
        column = 12;
    }
    else if (dataType == "ftemp") {
        column = 13;
    }
    else {
        //cout << "Incorrect keyword " << dataType << endl;
    }
    bool smum = false;
    for (int i = 0; i < pathLength; i++) {
        if (fName[i] == L'1') {
            if (fName[i + 1] == L'9' && fName[i + 2] == L'9') {
                smum = true;
            }
        }
    }
    if (smum) {
        if (dataType == "water") {
            column = 8;
        }
        else if (dataType == "press") {
            column = 1;
        }
        else if (dataType == "pottp") {
            column = 3;
        }
        else if (dataType == "Temp") {
            column = 4;
        }
        else if (dataType == "ftempv") {
            column = 5;
        }
        else if (dataType == "hum") {
            column = 6;
        }
        else if (dataType == "ozone_mPa") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "o3_mr") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "ozone_atmcm") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "ptemp") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "O3DN") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "O3RES") {
            *numOfElements = *numOfElements + 144;
            return;
        }
        else if (dataType == "ftemp") {
            column = 7;
        }
        else {
            //cout << "Invalid keyword used" << endl;
        }
        while (fin >> line) {
            //Cycle through header until "Level" is found
            //Once its found use switch with keyword to find
            //which column data will be read from
            //Then iterate through 14 or 15 dummy
            //then read in that column's data

            if (line == "Level") {
                dataSection = true;
                for (int i = 0; i < 17 + column;i++) {//cycle to first case of data 
                    fin >> line;
                }
                if (check2015 == "20151219") {
                    for (int i = 0; i < 9; i++) {
                        fin >> line;
                    }
                }
            }
            if (dataSection) {
                fin >> line;
                outStr[*numOfElements] = "  " + line;
                *numOfElements = *numOfElements + 1;
                for (int i = 0;i < 7;i++) {//cycle one row
                    if (fin >> line) {
                    }
                    else {
                        fin.close();
                        return;
                    }
                }
            }
        }
    }
    else {
        while (fin >> line) {
            //Cycle through header until "Level" is found
            //Once its found use switch with keyword to find
            //which column data will be read from
            //Then iterate through 14 or 15 dummy
            //then read in that column's data

            if (line == "Level") {
                dataSection = true;
                for (int i = 0; i < 32 + column;i++) {//cycle to first case of data
                    fin >> line;
                }
                // if(check2015 == "bu20151219"){
                //     for(int i =0; i< 15; i++){
                //         fin>>line;
                //     }
                // } 
            }
            if (dataSection) {
                fin >> line;
                outStr[*numOfElements] = "  " + line;
                *numOfElements = *numOfElements + 1;
                for (int i = 0;i < 13;i++) {//cycle to next row
                    if (fin >> line) {
                    }
                    else {
                        fin.close();
                        return;
                    }
                }
            }
        }
    }
    fin.close();
}

void normalizeNan_bld(string keyword, string outstr[], int size) {
    string replace;
    //table of overflow values based on keywords
    if (keyword == "water") {
        replace = "9.99E+09";
    }
    else if (keyword == "press") {
        replace = "9999.9";
    }
    else if (keyword == "pottp") {
        replace = "9999.9";
    }
    else if (keyword == "Temp") {
        replace = "999.9";
    }
    else if (keyword == "ftempv") {
        replace = "999.9";
    }
    else if (keyword == "hum") {
        replace = "999";
    }
    else if (keyword == "ozone_mPa") {
        replace = "99.90";
    }
    else if (keyword == "o3_mr") {
        replace = "99.999";
    }
    else if (keyword == "ozone_atmcm") {
        replace = "99.9990";
    }
    else if (keyword == "ptemp") {
        replace = "999.9";
    }
    else if (keyword == "O3DN") {
        replace = "999.999";
    }
    else if (keyword == "O3RES") {
        replace = "99999";
    }
    else if (keyword == "ftemp") {
        replace = "999.99";
    }
    cout << keyword << endl;
    cout << replace << endl;
    for (int i = 0; i < 160; i++) {
        cout << outstr[i] << ",";
    }
    cout << endl << endl;
    for (int i = 0; i < 160; i++) {
        if (outstr[i] == "      nan" || outstr[i] == "") {
            outstr[i] = replace;
        }
    }
}


bool buildF_bld_append(string outStr[], int* size, LPWSTR outputF_date, LPWSTR archive_rootPath, LPWSTR archive_date, wstring L_keywords, bool first) {
    ofstream f;
    wstring fstring;
    fstring = L"BLD_MATLEV_" + L_keywords + L"_" + outputF_date + L".csv";
    //=============================Change the archvie location when updatign
    //=============================Newer versions of file thing

    if (first) {
        wstring archiveLocation = archive_rootPath;
        archiveLocation = archiveLocation + L"BLD_MATLEV_" + L_keywords + L"_" + archive_date + L".csv";

        //string archiveLocation = "/Users/collinkadlecek/Downloads/HIH20220509/HIH_MATLEV_" + keyword + "_Archive.csv";
        ifstream fArchive;
        fArchive.open(archiveLocation);
        if (fArchive.is_open()) {
        }
        else {
            MessageBox(NULL, L"Archive File did not open, please check the naming convention", L"", MB_OK);
            LPWSTR stupd = &archiveLocation[0];
            MessageBox(NULL, stupd, L"", MB_OK);
            return false;
        }
        f.open(fstring, std::ios_base::app);
        if (fArchive.is_open()) {
            string line = "";
            while (!fArchive.eof()) {
                getline(fArchive, line);
                if (line != "") {
                    f << line + "\n";
                }
            }
            fArchive.close();
        }
        else {
            MessageBox(NULL, L"Archive file did not open, please check the naming convention", L"", MB_OK);
            return false;
        }
        f.close();
    }


    //f << "Filename,  FltNum,    FltDate,  FltTime,      RSModel,   RSNum, HygroID,      O3ID,  SSPver,    X370m,";
    f.open(fstring, std::ios_base::app);
    for (int i = 0; i < 147;i++) {
        string write;
        if (i != 146) {
            write = outStr[i] + ", ";
            f << write;
        }
        else {
            write = outStr[i];
            f << write;
        }
    }
    f << endl;
    f.close();
    return true;
}

void clearOutputCreate_bld(wstring L_keywords, string loc, wstring outputF_date) {
    ifstream f;
    wstring fstring;
    fstring = L"BLD_MATLEV_" + L_keywords + L"_" + outputF_date + L".csv";
    f.open(fstring);
    if (f.is_open()) {
        f.close();
        f.open(fstring, ios::out | ios::trunc);
        f.close();
    }
    else {
        cout << "No file was found" << endl;
    }
}


void main_bld(LPWSTR fullPath) {
    MessageBox(NULL, L"BLD append selected", L"", MB_OK);

    //-----------------------Enter Output File Information here---------------------------------
    //command
    string site = "BLD";
    WCHAR outputF_date[64];
    GetWindowTextW(hIn, outputF_date, 64);
 
    for (int i = 0; i < 8; i++) {
        if (outputF_date[i] == NULL) {
        }
        else {
            MessageBox(NULL, L"======================WARNING=====================\n\n\nThe output file will be successfully made, but the length of this date does not conform to the append programs naming convention and will not be able to be appended in the future\n\n======================WARNING=====================", L"Caution!!!", MB_OK);
            break;
        }
    }    //-----------------------Information for naming output files ---------------------  
            //Floc is the Location of folder containing files
            //FLoc path should end in a "/"" (i.e. .../downloads/Boulder_LEV2/)
    //string fLoc = "/Users/collinkadlecek/Downloads/zzBLD_LEV_20220510/bu20220414.lev2";        string archive = fLoc;



    int pathLength = wcslen(fullPath);
    //MessageBox(NULL, fullPath, L"", MB_OK);
    //find location of "space" delimeted filenames
    vector <int> fileNameIndexes;
    for (int i = 0; i < pathLength; i++) {
        if (!findSpace(fullPath[i])) {
            fileNameIndexes.push_back(i);
            //MessageBox(NULL, L"=========================================================================================================================================================================================", L"", MB_OK);
        }
        else {

        }
    }

    const LPWSTR fuck = fullPath;
    ifstream f;
    f.open(fullPath);
    bool issa_flight = false;
    if (f.is_open()) {
        MessageBox(NULL, L"Exception for single selected file ", L"", MB_OK);
        issa_flight = true;
    }
    if (!f.is_open() && fileNameIndexes.size() == 0) {
        MessageBox(NULL, L"Exception for no selected file ", L"", MB_OK);
        return;
    }
    f.close();
    //Find directory path to folder containing multiple files

    int pathIndex = 0;
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(fullPath[i])) {
            pathIndex = i;
        }
    }
    if (pathIndex != 0) {
        pathIndex = pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid file path", L"", MB_OK);
        return;
    }

    wstring fix;
    fix = fullPath;
    fix.erase(pathIndex, pathLength);
    LPWSTR rootPath = &fix[0];

    //MessageBox(NULL, L"root path", L"", MB_OK);
    //MessageBox(NULL, rootPath, L"", MB_OK);

//Arhive stuff========================================
    WCHAR Archive_Selection_FilePath[1024];
    GetWindowTextW(hOut, Archive_Selection_FilePath, 1024);

    ifstream testingThing;
    testingThing.open(Archive_Selection_FilePath);
    if (testingThing.is_open()) {
        testingThing.close();
    }
    else {
        MessageBox(NULL, L"did not open the archive file so the file could not be appended", L"", MB_OK);
        MessageBox(NULL, Archive_Selection_FilePath, L"", MB_OK);
        return;
    }
    //Finding the directory to the folder of the archive file chosen
    int archive_pathIndex = 0;
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(Archive_Selection_FilePath[i])) {
            archive_pathIndex = i;
        }
    }
    if (archive_pathIndex != 0) {
        archive_pathIndex = archive_pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid archive file path", L"", MB_OK);
        return;
    }


    wstring archive_fix;
    archive_fix = Archive_Selection_FilePath;
    int archive_pathLength = wcslen(Archive_Selection_FilePath);
    archive_fix.erase(archive_pathIndex, archive_pathLength);
    LPWSTR archive_rootPath = &archive_fix[0];

    wstring archive_date_fix;
    archive_date_fix = Archive_Selection_FilePath;
    archive_date_fix.erase(0, archive_pathLength - 12);
    archive_date_fix.erase(8, 12);
    LPWSTR Archive_date = &archive_date_fix[0];







    string outStrArr[450];//Adjust size 
    //Note this code can take up to 140 seconds to run


//-------------------------Column Head data types---------------------------------
    string keywords[13] = { "O3DN","O3RES","ftemp","ftempv","hum","ozone_atmcm","ozone_mPa","o3_mr","pottp","press","Temp","water","ptemp" };
    wstring L_keywords[13] = { L"O3DN",L"O3RES",L"ftemp",L"ftempv",L"hum",L"ozone_atmcm",L"ozone_mPa",L"o3_mr",L"pottp",L"press",L"Temp",L"water",L"ptemp" };

    //--------------------------File prefixes-----------------------------------------

    for (int i = 0; i < 13; i++) {
        clearOutputCreate_bld(L_keywords[i], site, outputF_date);
    }
    int itemCount = 0;
    int maxAlt = 0;
    int allCount = 0;
    int* numOfElements = &allCount;

    if (!issa_flight) {
        for (int i = 0; i < fileNameIndexes.size(); i++) {
            std::wstring temp = fuck;

            bool first = false;
            if (i == 0) {
                first = true;
            }
            else {
                first = false;
            }

            if (i == fileNameIndexes.size() - 1) {
                temp.erase(0, fileNameIndexes.at(i));
            }
            else {
                temp.erase(fileNameIndexes.at(i + 1), pathLength);
                temp.erase(0, fileNameIndexes.at(i));
            }

            WCHAR fLoc[1024];

            for (int f = 0; f < pathIndex; f++) {
                fLoc[f] = rootPath[f];
            }
            for (int f = pathIndex; f < temp.length() + pathIndex; f++) {
                fLoc[f] = temp[f - pathIndex + 1];
            }
            for (int f = temp.length() + pathIndex; f < 1024; f++) {
                fLoc[f] = NULL;
            }
            //MessageBox(NULL, fLoc, L"", MB_OK);
            //mockAppend(fLoc, false);
            int pathLength = temp.length() + pathIndex;


            for (int d = 0; d < 13; d++) {
                for (int i = 0; i < 160; i++) {
                    outStrArr[i] = "";
                }
                if (file_bld(fLoc, numOfElements, outStrArr, pathLength, issa_flight)) {
                    //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
                    itemCount = itemCount + 1;

                    //BuildVertical() takes data of keywords[d] type from data section
                    buildVertical_bld(fLoc, numOfElements, outStrArr, keywords[d], pathLength);
                    //fill array which currently contains: 9 heading components and numOfElements pieces of data
                    //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                    //200 refers to the possible 

 
                    normalizeNan_bld(keywords[d], outStrArr, 249);

                    //Build temporary files containing rows in order which the files were read in

                    if (!buildF_bld_append(outStrArr, numOfElements, outputF_date, archive_rootPath, Archive_date, L_keywords[d], first)) {
                        // buildF_bld_append(outStrArr, numOfElements, keywords[d], "BLD", date)){

                        MessageBox(NULL, L"returned", &L_keywords[d][0], MB_OK);

                        return;
                    }


                    //Find the maximum altitue reached by any balloon
                    if (*numOfElements - 10 > maxAlt) {
                        maxAlt = *numOfElements - 10;
                    }
                }
                else {
                    cout << "File apth was incorrect" << endl;
                }
            }
        }
    }
    else {
        MessageBox(NULL, L"single Flight buildpath", L"", MB_OK);
        for (int d = 0; d < 13; d++) {
            for (int i = 0; i < 160; i++) {
                outStrArr[i] = "";
            }
            if (file_bld(fullPath, numOfElements, outStrArr, pathLength, issa_flight)) {
                //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
                itemCount = itemCount + 1;
                //BuildVertical() takes data of keywords[d] type from data section
                buildVertical_bld(fullPath, numOfElements, outStrArr, keywords[d], pathLength);
                //fill array which currently contains: 9 heading components and numOfElements pieces of data
                //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                //200 refers to the possible 
                normalizeNan_bld(keywords[d], outStrArr, 249);
                //Build temporary files containing rows in order which the files were read in


                if (!buildF_bld_append(outStrArr, numOfElements, outputF_date, archive_rootPath, Archive_date, L_keywords[d], true)) {
                    return;
                }

                //Find the maximum altitue reached by any balloon
                if (*numOfElements - 10 > maxAlt) {
                    maxAlt = *numOfElements - 10;
                }
            }
            else {
                cout << "File apth was incorrect" << endl;
            }
        }
    }
}

void buildHeader_bld(string keyword, string location, string date, int Alt) {
    ofstream f;
    string fstring;
    fstring = location + "_MATLEV_" + keyword + "_" + date + ".csv";
    f.open(fstring, std::ios_base::app);
    f << "Filename,  FltNum,    FltDate,  FltTime,      RSModel,   RSNum,       O3ID,   HygroID,  SSPver,    X1743m,"; //Initial height
    for (int i = 7; i <= 143;i++) {

        int temp = 0;
        temp = temp + 250 * i;
        string write;
        if (i == 143) {
            write = "   X" + to_string(temp) + "m";
        }
        else {
            write = "   X" + to_string(temp) + "m,";
        }
        f << write;
    }
    f << "\n";
    f.close();
}

void clean_bld(string keyword) {
    //Delete temporary files used when reading data in directly from folder
    string a = "temp_" + keyword + ".csv";
    char fileName[64];
    for (int i = 0; i < a.length(); i++) {
        fileName[i] = a[i];
    }
    int result = remove(fileName);
    // cout << result << "  " << keyword << endl;

}

void clearOutput_bld(string keyword, string loc, string date) {
    ifstream f;
    string fstring;
    fstring = loc + "_MATLEV_" + keyword + "_" + date + ".csv";
    f.open(fstring);
    if (f.is_open()) {
        f.close();
        f.open(fstring, ios::out | ios::trunc);
        f.close();
    }
    fstring = "temp_" + keyword + ".csv";
    f.open(fstring);
    if (f.is_open()) {
        f.close();
        f.open(fstring, ios::out | ios::trunc);
        f.close();
    }
}

void buildF_bld_scratch(string outStr[], int* size, string keyword) {
    ofstream f;
    string fstring;
    fstring = "temp_" + keyword + ".csv";
    f.open(fstring, std::ios_base::app);
    //f << "Filename,  FltNum,    FltDate,  FltTime,      RSModel,   RSNum, HygroID,      O3ID,  SSPver,    X370m,";
    for (int i = 0; i < 160;i++) {
        string write;
        if (i != 159) {
            write = outStr[i] + ", ";
            f << write;
        }
        else {
            write = outStr[i];
            f << write;
        }
    }
    f << endl;
    f.close();
}


void main_bld_scratch(LPWSTR fullPath) {


    //-----------------------Enter Output File Information here---------------------------------
        //command
    MessageBox(NULL, L"bld", L"", MB_OK);

    //-----------------------Enter Output File Information here---------------------------------
    //command
    string site = "BLD";
    string date = "20220510";
    //-----------------------Information for naming output files ---------------------  
            //Floc is the Location of folder containing files
            //FLoc path should end in a "/"" (i.e. .../downloads/Boulder_LEV2/)
    //string fLoc = "/Users/collinkadlecek/Downloads/zzBLD_LEV_20220510/bu20220414.lev2";        string archive = fLoc;



    int pathLength = wcslen(fullPath);
    //MessageBox(NULL, fullPath, L"", MB_OK);
    //find location of "space" delimeted filenames
    vector <int> fileNameIndexes;
    for (int i = 0; i < pathLength; i++) {
        if (!findSpace(fullPath[i])) {
            fileNameIndexes.push_back(i);
            //MessageBox(NULL, L"=========================================================================================================================================================================================", L"", MB_OK);
        }
        else {

        }
    }

    const LPWSTR fuck = fullPath;

    if (fileNameIndexes.size() == 0) {
        MessageBox(NULL, L"Exception for no selected file ", L"", MB_OK);
        return;
    }
    else if (fileNameIndexes.size() == 1) {
        MessageBox(NULL, L"Exception for single selected file", L"", MB_OK);
        return;
    }

    //Find directory path to folder containing multiple files

    int pathIndex = 0;
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(fullPath[i])) {
            pathIndex = i;
        }
    }
    if (pathIndex != 0) {
        pathIndex = pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid file path", L"", MB_OK);
        return;
    }

    wstring fix;
    fix = fullPath;
    fix.erase(pathIndex, pathLength);
    LPWSTR rootPath = &fix[0];

    //MessageBox(NULL, L"root path", L"", MB_OK);
    //MessageBox(NULL, rootPath, L"", MB_OK);

    //LPWSTR hurt;
    WCHAR hurt[1024];
    GetWindowTextW(hOut, hurt, 1024);

    ifstream testingThing;
    testingThing.open(hurt);
    if (testingThing.is_open()) {
        MessageBox(NULL, L"archive Opened!!!!!!!!!!!", hurt, MB_OK);
        MessageBox(NULL, hurt, L"", MB_OK);
        testingThing.close();
    }
    else {
        MessageBox(NULL, L"did not open the archive file", L"", MB_OK);
        MessageBox(NULL, hurt, L"", MB_OK);

    }









    string outStrArr[450];//Adjust size 
    //Note this code can take up to 140 seconds to run


//-------------------------Column Head data types---------------------------------
    string keywords[13] = { "O3DN","O3RES","ftemp","ftempv","hum","ozone_atmcm","ozone_mPa","o3_mr","pottp","press","Temp","water","ptemp" };

    //--------------------------File prefixes-----------------------------------------
    string prefix[5] = { "ba","bl","bu","bw","bz" };


    int itemCount = 0;
    int maxAlt = 0;
    int allCount = 0;
    int* numOfElements = &allCount;
    for (int i = 0; i < 13; i++) {
        clearOutput_bld(keywords[i], site, date);
    }
    for (int d = 0; d < 13; d++) {
        int numPrefixes = 5;
        //Iterate over all prefixes
        for (int f = 0; f < numPrefixes; f++) {
            //Iterate for 17000 days starting in 01/01/1991
            for (int i = 0; i < fileNameIndexes.size(); i++) {
                std::wstring temp = fuck;

                if (i == fileNameIndexes.size() - 1) {
                    temp.erase(0, fileNameIndexes.at(i));
                }
                else {
                    temp.erase(fileNameIndexes.at(i + 1), pathLength);
                    temp.erase(0, fileNameIndexes.at(i));
                }

                WCHAR fLoc[1024];

                for (int f = 0; f < pathIndex; f++) {
                    fLoc[f] = rootPath[f];
                }
                for (int f = pathIndex; f < temp.length() + pathIndex; f++) {
                    fLoc[f] = temp[f - pathIndex + 1];
                }
                for (int f = temp.length() + pathIndex; f < 1024; f++) {
                    fLoc[f] = NULL;
                }
                //MessageBox(NULL, fLoc, L"", MB_OK);
                //mockAppend(fLoc, false);
                int pathLength = temp.length() + pathIndex;

                for (int j = 0; j < 400; j++) {
                    outStrArr[j] = "      nan";
                }
                //Build file name at a date
                // //cout << buildDate(i) << endl;
                //If file name built is an existing file
                if (file_bld(fLoc, numOfElements, outStrArr,pathLength,false)) {
                    //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
                    itemCount = itemCount + 1;
                    //BuildVertical() takes data of keywords[d] type from data section
                    buildVertical_bld(fLoc, numOfElements, outStrArr, keywords[d],pathLength);
                    //fill array which currently contains: 9 heading components and numOfElements pieces of data
                    //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                    //200 refers to the possible 
                    normalizeNan_bld(keywords[d], outStrArr, 249);
                    //Build temporary files containing rows in order which the files were read in
                    buildF_bld_scratch(outStrArr, numOfElements, keywords[d]);
                    //Find the maximum altitue reached by any balloon
                    if (*numOfElements - 10 > maxAlt) {
                        maxAlt = *numOfElements - 10;
                    }
                }
                //prepare file location for next iteration by stripping file specific information
            }
        }

    }




    //For each datatype organize the file by date and output file with naming following conventions input above
    for (int e = 0; e < 13; e++) {
        // cout << e << " <- e" << endl;
        buildHeader_bld(keywords[e], site, date, maxAlt);
        //organize(site, date, itemCount, keywords[e]);
        //Delete unorganized files
        // cout << "ljdsflkdj" << endl;
        clean_bld(keywords[e]);
        // cout << "ldsjf" << endl;

    }
}

//======================NEW MAIN FUNCTIONS=================
void main_NEW_LDR_Scratch(LPWSTR fullPath) {
    //Command F jump
    //-----------------------Information for naming output files ---------------------
    string site = "LDR_MATNEW_";
    string date = "20220414_archive";
    //-----------------------Enter File Information here---------------------------------
        //Location of folder containing files
        //This file path should 
        //Make sure the location ends with a '/'
    string fLoc = "/Users/collinkadlecek/Downloads/LDR_NEW_20220214/aaLDR_NEW_Test";
    fLoc = fLoc + '/';
    string archive = fLoc;
    string outStrArr[450];//Adjust size 
    string prefix[1] = { "LDR_H2O_" }; //All characters preceding date portion of filename
    string suffix = ".txt";//file type, ex. .txt, .lev, .lev2... '.' is neccessary in suffix string
//-------------------------Column Head data types---------------------------------
    string keywords[13] = { "Press","Temp","Theta","RH_FPH","RH_RS","H2Omr","H2Osd","H2Omr_orig","TFP_FPH","TFP_RS","O3mr" };
    int itemCount = 0;
    int maxAlt = 0;
    int allCount = 0;
    int* numOfElements = &allCount;
    //Do following for each data type


    for (int i = 0; i < 13; i++) {
        clearOutput_NEW_LDR_Scratch(keywords[i], site, date);
    }

    //--------------------------File prefixes-----------------------------------------

    int numPrefixes = 1;
    // bool test;
    // fLoc = "/Volumes/MacKiller/NOAA/UpsandDowns/LDR_NEW/LDR_H2O_20O3O312.txt";
    // test = file(fLoc,numOfElements,outStrArr);


//Iterate over all data types
    for (int d = 0; d < 12; d++) {
        buildHeader_NEW_LDR_Scratch(keywords[d], site, date, maxAlt);
        //Iterate over all prefixes
        for (int f = 0; f < numPrefixes; f++) {

            itemCount = 0;
            //Iterate for 12000 days starting in 01/01/1991
            for (int i = 0; i < 17000; i++) {

                for (int j = 0; j < 449; j++) {
                    outStrArr[j] = "      nan";
                }
                //Build file name at a date
                fLoc = fLoc + prefix[f] + buildDate_NEW_LDR_Scratch(i) + suffix;
                //If file name built is an existing file

                if (file_NEW_LDR_Scratch(fLoc, numOfElements, outStrArr)) {
                    //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
                    itemCount = itemCount + 1;
                    //BuildVertical() takes data of keywords[d] type from data section
                    buildVertical_NEW_LDR_Scratch(fLoc, numOfElements, outStrArr, keywords[d]);
                    // //fill array which currently contains: 9 heading components and numOfElements pieces of data
                    // //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                    // //200 refers to the possible 
                    normalizeNan_NEW_LDR_Scratch(keywords[d], outStrArr, 249);

                    // //Build temporary files containing rows in order which the files were read in
                    buildF_NEW_LDR_Scratch(outStrArr, numOfElements, keywords[d]);
                    //Find the maximum altitue reached by any balloon
                    if (*numOfElements > maxAlt) {
                        maxAlt = *numOfElements;
                    }
                }

                *numOfElements = 8;
                //         //prepare file location for next iteration by stripping file specific information
                fLoc = archive;
            }
            if (itemCount == 0) {
                cout << "Check data folder location\n type \"C0MMAND\" into C0MMAND f search to find input file folder  " << endl;
            }

        }
    }
    createTemplate_NEW_LDR_Scratch();
    for (int i = 0; i < 11; i++) {
        string fileName = "temp_" + keywords[i] + ".csv";
        ////////This fixes nans which is in the sorted file
       // organize(site, date, itemCount, keywords[i]);
        useTemplate_NEW_LDR_Scratch(fileName, keywords[i], date, itemCount);
        clean_NEW_LDR_Scratch(keywords[i]);
    }
    // for(int i =0; i < 11; i++){
    //     organize(site, date, itemCount, keywords[i]);
    // }
    // useTemplate("temp_.csv","Alt", date);
    remove("temp_.csv");
    //For each datatype organize the file by date and output file with naming following conventions input above

}


void old_main_NEW_LDR_Append(LPWSTR fullPath) {
//    //Command F jump
//    //-----------------------Information for naming output files ---------------------
//    string site = "LDR_MATNEW_";
//    string date = "20220414";
//    //-----------------------Enter File Information here---------------------------------
//        //Location of folder containing files
//        //This file path should 
//        //Make sure the location ends with a '/'
//    string fLoc = "/Users/collinkadlecek/Downloads/LDR_NEW_20220214/aaLDR_NEW_Test/LDR_H2O_20220124.txt";
//    string archive = fLoc;
//    string outStrArr[512];//Adjust size 
//    string prefix[1] = { "LDR_H2O_" }; //All characters preceding date portion of filename
//    string suffix = ".txt";//file type, ex. .txt, .lev, .lev2... '.' is neccessary in suffix string
////-------------------------Column Head data types---------------------------------
//    string keywords[13] = { "Press","Temp","Theta","RH_FPH","RH_RS","H2Omr","H2Osd","H2Omr_orig","TFP_FPH","TFP_RS","O3mr" };
//    int itemCount = 0;
//    int maxAlt = 0;
//    int allCount = 0;
//    int* numOfElements = &allCount;
//    //Do following for each data type
//
//
//    // for(int i = 0; i < 13; i++){
//    //     clearOutput(keywords[i],site, date);
//    // }
//
////--------------------------File prefixes-----------------------------------------
//
//    int numPrefixes = 1;
//    // bool test;
//    // fLoc = "/Volumes/MacKiller/NOAA/UpsandDowns/LDR_NEW/LDR_H2O_20O3O312.txt";
//    // test = file(fLoc,numOfElements,outStrArr);
//
//
////Iterate over all data types
//    for (int d = 0; d < 13; d++) {
//        //Iterate over all prefixes
//        itemCount = 0;
//
//        //Iterate for 12000 days starting in 01/01/1991
//        for (int j = 0; j < 512; j++) {
//            outStrArr[j] = "nan";
//        }
//        //Build file name at a date
//            //If file name built is an existing file
//        if (file_NEW_LDR_Append(fLoc, numOfElements, outStrArr)) {
//            //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
//            itemCount = itemCount + 1;
//            //BuildVertical() takes data of keywords[d] type from data section
//            buildVertical_NEW_LDR_Append(fLoc, numOfElements, outStrArr, keywords[d]);
//            // //fill array which currently contains: 9 heading components and numOfElements pieces of data
//            // //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
//            // //200 refers to the possible 
//            normalizeNan_NEW_LDR_Append(keywords[d], outStrArr, 500);
//
//            // //Build temporary files containing rows in order which the files were read in
//            buildF_NEW_LDR_Append(outStrArr, numOfElements, keywords[d]);
//
//            //Find the maximum altitue reached by any balloon
//            if (*numOfElements > maxAlt) {
//                maxAlt = *numOfElements;
//            }
//        }
//        else {
//            cout << fLoc << "\n was not found " << endl;
//        }
//        *numOfElements = 8;
//
//        //prepare file location for next iteration by stripping file specific information
//
//    }
//    if (itemCount == 0) {
//        cout << "check data folder location" << endl << "You can type \"C0MMAND\" into the C0MMAND f bar and it will send you to the appropriate location \n" << endl << endl;
//    }
//
//    createTemplate_NEW_LDR_Append(itemCount);
//    for (int i = 0; i < 11; i++) {
//        string fileName = "temp_" + keywords[i] + ".csv";
//        ////////This fixes nans which is in the sorted file
//       // organize(site, date, itemCount, keywords[i], fileName);
//        useTemplate_NEW_LDR_Append(fileName, keywords[i], date, itemCount);
//
//    }
//    for (int i = 0; i < 11; i++) {
//        string fileName = "temp_" + keywords[i] + ".csv";
//        clean_NEW_LDR_Append(keywords[i]);
//        // organize(site, date, itemCount, keywords[i], fileName);
//    }
//
//    // useTemplate("temp_.csv","Alt", date);
//    remove("temp_.csv");
//    remove("temp_alt.csv");
//    remove("temp_H2Omr_orig.csv");
//    remove("temp_TFP_FPH.csv");
//
}
void main_NEW_LDR_Append(LPWSTR fullPath) {
    //Command F jump
    //-----------------------Information for naming output files ---------------------
    MessageBox(NULL, L"NEW LDR Append selected", L"", MB_OK);

    //Command F jump
    //-----------------------Information for naming output files ---------------------
    string site = "LDR_MATNEW_";
    WCHAR outputF_date[64];
    GetWindowTextW(hIn, outputF_date, 64);
 
    for (int i = 0; i < 8; i++) {
        if (outputF_date[i] == NULL) {
        }
        else {
            MessageBox(NULL, L"======================WARNING=====================\n\n\nThe output file will be successfully made, but the length of this date does not conform to the append programs naming convention and will not be able to be appended in the future\n\n======================WARNING=====================", L"Caution!!!", MB_OK);
            break;
        }
    }

    int pathLength = wcslen(fullPath);

    //find location of "space" delimeted filenames
    vector <int> fileNameIndexes;
    for (int i = 0; i < pathLength; i++) {
        //if (!findSpace(fullPath[i])) {
        if (!findNull(fullPath[i])) {
            fileNameIndexes.push_back(i);
            //MessageBox(NULL, L"=========================================================================================================================================================================================", L"", MB_OK);
        }
        else {

        }
    }


    const LPWSTR fuck = fullPath;
    ifstream f;
    f.open(fullPath);
    bool issa_flight = false;
    if (f.is_open()) {
        MessageBox(NULL, L"Exception for single selected file ", L"", MB_OK);
        issa_flight = true;
    }
    if (!f.is_open() && fileNameIndexes.size() == 0) {
        MessageBox(NULL, L"Exception for no selected file ", L"", MB_OK);
        return;
    }


    f.close();


    int pathIndex = 0; //rootPath Index rather
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(fullPath[i])) {
            pathIndex = i;
        }
    }
    if (pathIndex != 0) {
        pathIndex = pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid file path", L"", MB_OK);
        return;
    }

    wstring fix;
    fix = fullPath;
    fix.erase(pathIndex, pathLength);
    LPWSTR rootPath = &fix[0];


    //Arhive stuff========================================
    WCHAR Archive_Selection_FilePath[1024];
    GetWindowTextW(hOut, Archive_Selection_FilePath, 1024);


    ifstream testingThing;
    testingThing.open(Archive_Selection_FilePath);
    if (testingThing.is_open()) {
        testingThing.close();
    }
    else {
        MessageBox(NULL, L"did not open the archive file so the file could not be appended", L"", MB_OK);
        MessageBox(NULL, Archive_Selection_FilePath, L"", MB_OK);
        return;
    }
    //Finding the directory to the folder of the archive file chosen
    int archive_pathIndex = 0;
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(Archive_Selection_FilePath[i])) {
            archive_pathIndex = i;
        }
    }
    if (archive_pathIndex != 0) {
         archive_pathIndex = archive_pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid archive file path", L"", MB_OK);
        return;
    }


    wstring archive_fix;
    archive_fix = Archive_Selection_FilePath;

    int archive_pathLength = wcslen(Archive_Selection_FilePath);
    archive_fix.erase(archive_pathIndex, archive_pathLength);
    LPWSTR archive_rootPath = &archive_fix[0];



    wstring archive_date_fix;
    archive_date_fix = Archive_Selection_FilePath;
    archive_date_fix.erase(0, archive_pathLength - 12);
    archive_date_fix.erase(8, 12);
    LPWSTR Archive_date = &archive_date_fix[0];


    //-----------------------Enter File Information here---------------------------------
        //Location of folder containing files
        //This file path should 
        //Make sure the location ends with a '/'
    string outStrArr[320];//Adjust size 
    string prefix[1] = { "LDR_H2O_" }; //All characters preceding date portion of filename
    string suffix = ".txt";//file type, ex. .txt, .lev, .lev2... '.' is neccessary in suffix string
//-------------------------Column Head data types---------------------------------
    string keywords[13] = { "Press","Temp","Theta","RH_FPH","RH_RS","H2Omr","H2Osd","H2Omr_orig","TFP_FPH","TFP_RS","O3mr" };
    wstring L_keywords[13] = { L"Press",L"Temp",L"Theta",L"RH_FPH",L"RH_RS",L"H2Omr",L"H2Osd",L"H2Omr_orig",L"TFP_FPH",L"TFP_RS",L"O3mr" };

    int itemCount = 0;
    int maxAlt = 0;
    int allCount = 0;
    int* numOfElements = &allCount;
    //Do following for each data type


    // for(int i = 0; i < 13; i++){
    //     clearOutput(keywords[i],site, date);
    // }

//--------------------------File prefixes-----------------------------------------

    int numPrefixes = 1;
    // bool test;
    // fLoc = "/Volumes/MacKiller/NOAA/UpsandDowns/LDR_NEW/LDR_H2O_20O3O312.txt";
    // test = file(fLoc,numOfElements,outStrArr);
    for (int i = 0; i < 13; i++) {
        clearOutput_NEW_LDR_Append(keywords[i], L_keywords[i], outputF_date);
    }

    if (!issa_flight) {
        for (int i = 0; i < fileNameIndexes.size(); i++) {
            bool first = false;
            if (i == 0) {
                first = true;
            }
            else {
                first = false;
            }
            std::wstring temp = fuck;
            if (i == fileNameIndexes.size() - 1) {
                temp.erase(0, fileNameIndexes.at(i));
            }
            else {
                temp.erase(fileNameIndexes.at(i + 1), pathLength);
                temp.erase(0, fileNameIndexes.at(i));
            }

            WCHAR fLoc[1024];

            for (int f = 0; f < pathIndex; f++) {
                fLoc[f] = rootPath[f];
            }
            for (int f = pathIndex; f < temp.length() + pathIndex; f++) {
                fLoc[f] = temp[f - pathIndex + 1];
            }
            for (int f = temp.length() + pathIndex; f < 1024; f++) {
                fLoc[f] = NULL;
            }
            //MessageBox(NULL, fLoc, L"", MB_OK);
            //mockAppend(fLoc, false);
            int pathLength = temp.length() + pathIndex;
            //Iterate over all data types
            for (int d = 0; d < 13; d++) {
                //Iterate over all prefixes
                itemCount = 0;

                //Iterate for 12000 days starting in 01/01/1991
                for (int j = 0; j < 320; j++) {
                    outStrArr[j] = "nan";
                }
                //Build file name at a date
                    //If file name built is an existing file
                if (file_NEW_LDR_Append(fLoc, numOfElements, outStrArr, pathLength, issa_flight,first)) {

                    //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
                    itemCount = itemCount + 1;
                    //BuildVertical() takes data of keywords[d] type from data section
                    if (first) {
                        *numOfElements = 8;
                    }
                    buildVertical_NEW_LDR_Append(fLoc, numOfElements, outStrArr, keywords[d], pathLength);

                    // //fill array which currently contains: 9 heading components and numOfElements pieces of data
                    // //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                    // //200 refers to the possible 
                    normalizeNan_NEW_LDR_Append(keywords[d], outStrArr, 500);

                    // //Build temporary files containing rows in order which the files were read in
                    buildF_NEW_LDR_Append(outStrArr, numOfElements, keywords[d], first);

                    //Find the maximum altitue reached by any balloon
                    if (*numOfElements > maxAlt) {
                        maxAlt = *numOfElements;
                    }
                }
                else {
                    cout << fLoc << "\n was not found " << endl;
                }
                *numOfElements = 8;

                //prepare file location for next iteration by stripping file specific information

            }
        }
    }
    else {
        for (int d = 0; d < 13; d++) {
            if (file_NEW_LDR_Append(fullPath, numOfElements, outStrArr, pathLength, true,true)) {
                //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
                itemCount = itemCount + 1;
                //BuildVertical() takes data of keywords[d] type from data section
                *numOfElements = 8;

                buildVertical_NEW_LDR_Append(fullPath, numOfElements, outStrArr, keywords[d], pathLength);
                // //fill array which currently contains: 9 heading components and numOfElements pieces of data
                // //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                // //200 refers to the possible 
                normalizeNan_NEW_LDR_Append(keywords[d], outStrArr, 500);

                // //Build temporary files containing rows in order which the files were read in
                buildF_NEW_LDR_Append(outStrArr, numOfElements, keywords[d],true);

                //Find the maximum altitue reached by any balloon
                if (*numOfElements > maxAlt) {
                    maxAlt = *numOfElements;
                }
            }
            else {
                MessageBox(NULL, fullPath, L"This file was not found", MB_OK);
            }
        }
    }



    if (itemCount == 0) {
        cout << "check data folder location" << endl << "You can type \"C0MMAND\" into the C0MMAND f bar and it will send you to the appropriate location \n" << endl << endl;
    }

    int fixes = fileNameIndexes.size();
    if (issa_flight) {
        fixes = 1;
    }
    
    int numFiles = createTemplate_NEW_LDR_Append(fixes);

    for (int i = 0; i < 11; i++) {
        string fileName = "temp_" + keywords[i] + ".csv";
        ////////This fixes nans which is in the sorted file
       // organize(site, date, itemCount, keywords[i], fileName);
        if (!useTemplate_NEW_LDR_Append(fileName, keywords[i], outputF_date, archive_rootPath, Archive_date, L_keywords[i], fixes)) {
            return;
        }

    }
    for (int i = 0; i < 11; i++) {
        string fileName = "temp_" + keywords[i] + ".csv";
        clean_NEW_LDR_Append(keywords[i]);
        // organize(site, date, itemCount, keywords[i], fileName);
    }

    // useTemplate("temp_.csv","Alt", date);
    /*remove("temp_.csv");
    remove("temp_alt.csv");
    remove("temp_H2Omr_orig.csv");
    remove("temp_TFP_FPH.csv");*/
    //remove("Append_template_NEW_LDR.txt");

}

void main_NEW_HIH_Append(LPWSTR fullPath) {
    MessageBox(NULL, L"NEW HIH Append selected", L"", MB_OK);

    //Command F jump
    //-----------------------Information for naming output files ---------------------
    string site = "HIH_MATNEW_";

    WCHAR outputF_date[64];
    GetWindowTextW(hIn, outputF_date, 64);
 
    for (int i = 0; i < 8; i++) {
        if (outputF_date[i] == NULL) {
        }
        else {
            MessageBox(NULL, L"======================WARNING=====================\n\n\nThe output file will be successfully made, but the length of this date does not conform to the append programs naming convention and will not be able to be appended in the future\n\n======================WARNING=====================", L"Caution!!!", MB_OK);
            break;
        }
    }
  
    
    int pathLength = wcslen(fullPath);

    //find location of "space" delimeted filenames
    vector <int> fileNameIndexes;
    for (int i = 0; i < pathLength; i++) {
        //if (!findSpace(fullPath[i])) {
        if (!findNull(fullPath[i])) {
            fileNameIndexes.push_back(i);
            //MessageBox(NULL, L"=========================================================================================================================================================================================", L"", MB_OK);
        }
        else {

        }
    }
    
    const LPWSTR fuck = fullPath;
    ifstream f;
    f.open(fullPath);
    bool issa_flight = false;
    if (f.is_open()) {
        MessageBox(NULL, L"Exception for single selected file ", L"", MB_OK);
        issa_flight = true;
    }
    if (!f.is_open() && fileNameIndexes.size() == 0) {
        MessageBox(NULL, L"Exception for no selected file ", L"", MB_OK);
        return;
    }


    f.close();

    int pathIndex = 0; //rootPath Index rather
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(fullPath[i])) {
            pathIndex = i;
        }
    }
    if (pathIndex != 0) {
        pathIndex = pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid file path", L"", MB_OK);
        return;
    }

    wstring fix;
    fix = fullPath;
    fix.erase(pathIndex, pathLength);
    LPWSTR rootPath = &fix[0];


    //Arhive stuff========================================
    WCHAR Archive_Selection_FilePath[1024];
    GetWindowTextW(hOut, Archive_Selection_FilePath, 1024);


    ifstream testingThing;
    testingThing.open(Archive_Selection_FilePath);
    if (testingThing.is_open()) {
        testingThing.close();
    }
    else {
        MessageBox(NULL, L"did not open the archive file so the file could not be appended", L"", MB_OK);
        MessageBox(NULL, Archive_Selection_FilePath, L"", MB_OK);
        return;
    }
    //Finding the directory to the folder of the archive file chosen
    int archive_pathIndex = 0;
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(Archive_Selection_FilePath[i])) {
            archive_pathIndex = i;
        }
    }
    if (archive_pathIndex != 0) {
        archive_pathIndex = archive_pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid archive file path", L"", MB_OK);
        return;
    }


    wstring archive_fix;
    archive_fix = Archive_Selection_FilePath;

    int archive_pathLength = wcslen(Archive_Selection_FilePath);
    archive_fix.erase(archive_pathIndex, archive_pathLength);
    LPWSTR archive_rootPath = &archive_fix[0];



    wstring archive_date_fix;
    archive_date_fix = Archive_Selection_FilePath;
    archive_date_fix.erase(0, archive_pathLength - 12);
    archive_date_fix.erase(8, 12);
    LPWSTR Archive_date = &archive_date_fix[0];


    
    //-----------------------Enter File Information here---------------------------------
    //Location of folder containing files
    //Make sure the location ends with a '/'
    string outStrArr[512];//Adjust size 
    string prefix[1] = { "HIH_H2O_" }; //All characters preceding date portion of filename
    string suffix = ".txt";//file type, ex. .txt, .lev, .lev2... '.' is neccessary in suffix string
//-------------------------Column Head data types---------------------------------
    string keywords[13] = { "Press","Temp","Theta","RH_FPH","RH_RS","H2Omr","H2Osd","H2Omr_orig","TFP_FPH","TFP_RS","O3mr","alt" };
    wstring L_keywords[13] = { L"Press",L"Temp",L"Theta",L"RH_FPH",L"RH_RS",L"H2Omr",L"H2Osd",L"H2Omr_orig",L"TFP_FPH",L"TFP_RS",L"O3mr" };

    // string keywords[13] = {"alt"};
    int itemCount = 0;
    int maxAlt = 0;
    int allCount = 0;
    int* numOfElements = &allCount;
    //Do following for each data type


    for (int i = 0; i < 13; i++) {
        clearOutput_NEW_HIH_Append(keywords[i], L_keywords[i], outputF_date);
    }



    //--------------------------File prefixes-----------------------------------------

    int numPrefixes = 1;
    int startDatePrefix[1] = { 0 };
    int endDatePrefix[1] = { 25000 };

    if (!issa_flight) {
        for (int i = 0; i < fileNameIndexes.size(); i++) {
            bool first = false;
            if (i == 0) {
                first = true;
            }
            else {
                first = false;
            }
            std::wstring temp = fuck;
            if (i == fileNameIndexes.size() - 1) {
                temp.erase(0, fileNameIndexes.at(i));
            }
            else {
                temp.erase(fileNameIndexes.at(i + 1), pathLength);
                temp.erase(0, fileNameIndexes.at(i));
            }

            WCHAR fLoc[1024];

            for (int f = 0; f < pathIndex; f++) {
                fLoc[f] = rootPath[f];
            }
            for (int f = pathIndex; f < temp.length() + pathIndex; f++) {
                fLoc[f] = temp[f - pathIndex + 1];
            }
            for (int f = temp.length() + pathIndex; f < 1024; f++) {
                fLoc[f] = NULL;
            }
            int pathLength = temp.length() + pathIndex;

            //Iterate over all data types
            for (int d = 0; d < 13; d++) {
                //Iterate over all prefixes
                itemCount = 0;

                //Iterate for 12000 days starting in 01/01/1991
                for (int j = 0; j < 512; j++) {
                    outStrArr[j] = "nan";
                }
                //Build file name at a date

                    //If file name built is an existing file
                if (file_NEW_HIH_Append(fLoc, numOfElements, outStrArr, pathLength, issa_flight)) {
                    //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
                    itemCount = itemCount + 1;
                    if (first) {
                        *numOfElements = 8;
                    }
                    //BuildVertical() takes data of keywords[d] type from data section
                    buildVertical_NEW_HIH_Append(fLoc, numOfElements, outStrArr, keywords[d]);

                    // //fill array which currently contains: 9 heading components and numOfElements pieces of data
                    // //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                    // //200 refers to the possible 
                    normalizeNan_NEW_HIH_Append(keywords[d], outStrArr, 500);

                    // //Build temporary files containing rows in order which the files were read in
                    buildF_NEW_HIH_Append(outStrArr, numOfElements, keywords[d],first);

                    //Find the maximum altitue reached by any balloon
                    if (*numOfElements > maxAlt) {
                        maxAlt = *numOfElements;
                    }
                }
                else {
                    cout << fLoc << "\n was not found " << endl;
                }
                *numOfElements = 8;

                //prepare file location for next iteration by stripping file specific information

            }

        }
    }
    else {
        for (int d = 0; d < 13; d++) {
            //Iterate over all prefixes
            itemCount = 0;

            //Iterate for 12000 days starting in 01/01/1991
            for (int j = 0; j < 512; j++) {
                outStrArr[j] = "nan";
            }
            //Build file name at a date

                //If file name built is an existing file
            if (file_NEW_HIH_Append(fullPath, numOfElements, outStrArr, pathLength, issa_flight)) {
                //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
                itemCount = itemCount + 1;
                *numOfElements = 8;

                //BuildVertical() takes data of keywords[d] type from data section
                buildVertical_NEW_HIH_Append(fullPath, numOfElements, outStrArr, keywords[d]);

                // //fill array which currently contains: 9 heading components and numOfElements pieces of data
                // //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                // //200 refers to the possible 
                normalizeNan_NEW_HIH_Append(keywords[d], outStrArr, 500);

                // //Build temporary files containing rows in order which the files were read in
                buildF_NEW_HIH_Append(outStrArr, numOfElements, keywords[d], true);

                //Find the maximum altitue reached by any balloon
                if (*numOfElements > maxAlt) {
                    maxAlt = *numOfElements;
                }
            }

            *numOfElements = 8;

            //prepare file location for next iteration by stripping file specific information

        }
    }

    if (itemCount == 0) {
        cout << "check data folder location" << endl << "You can type \"C0MMAND\" into the C0MMAND f bar and it will send you to the appropriate location \n" << endl << endl;
    }
    int fixes = fileNameIndexes.size();
    if (issa_flight) {
        fixes = 1;
    }

    createTemplate_NEW_HIH_Append(fixes);

    for (int i = 0; i < 11; i++) {
        string fileName = "temp_" + keywords[i] + ".csv";
        ////////This fixes nans which is in the sorted file
       // organize(site, date, itemCount, keywords[i], fileName);
        if (!useTemplate_NEW_HIH_Append(fileName, keywords[i], outputF_date, archive_rootPath, Archive_date, L_keywords[i], fixes)) {
            return;
        }
    }

    for (int i = 0; i < 11; i++) {
        string fileName = "temp_" + keywords[i] + ".csv";
        clean_NEW_HIH_Append(keywords[i]);
        // organize(site, date, itemCount, keywords[i], fileName);
    }

    // useTemplate("temp_.csv","Alt", date);
    remove("temp_.csv");
    remove("temp_alt.csv");
    remove("temp_H2Omr_orig.csv");
    remove("temp_TFP_FPH.csv");

    //For each datatype organize the file by date and output file with naming following conventions input above

}

void main_NEW_HIH_Scratch(LPWSTR fullPath) {
    //Command F jump
    //-----------------------Information for naming output files ---------------------
    string site = "HIH_";
    string date = "20220725_archive";
    //-----------------------Enter File Information here---------------------------------
    string fLoc; //Location of folder containing files
    //Make sure the location ends with a '/'
    fLoc = "/Users/collinkadlecek/Downloads/HIH_NEW_20220216/HIH_MATNEW_20220218/aaHilo_NEW";
    string archive = fLoc + "/";
    string outStrArr[450];//Adjust size 
    string prefix[1] = { "HIH_H2O_" }; //All characters preceding date portion of filename
    string suffix = ".txt";//file type, ex. .txt, .lev, .lev2... '.' is neccessary in suffix string
//-------------------------Column Head data types---------------------------------
    string keywords[13] = { "Press","Temp","Theta","RH_FPH","RH_RS","H2Omr","H2Osd","H2Omr_orig","TFP_FPH","TFP_RS","O3mr","alt" };
    // string keywords[13] = {"alt"};
    int itemCount = 0;
    int maxAlt = 0;
    int allCount = 0;
    int* numOfElements = &allCount;
    //Do following for each data type


    for (int i = 0; i < 13; i++) {
        clearOutput_NEW_HIH_Scratch(keywords[i], site, date);
    }



    //--------------------------File prefixes-----------------------------------------

    int numPrefixes = 1;
    int startDatePrefix[1] = { 0 };
    int endDatePrefix[1] = { 25000 };


    //Iterate over all data types
    for (int d = 0; d < 13; d++) {
        //buildHeader_NEW_HIH_Scratch(keywords[d], site, date, maxAlt);
        //Iterate over all prefixes
        for (int f = 0; f < numPrefixes; f++) {

            itemCount = 0;
            //Iterate for 12000 days starting in 01/01/1991
            for (int i = startDatePrefix[f]; i < endDatePrefix[f]; i++) {

                for (int j = 0; j < 449; j++) {
                    outStrArr[j] = "nan";
                }
                //Build file name at a date
                fLoc = fLoc + prefix[f] + buildDate_NEW_HIH_Scratch(i) + suffix;
                //If file name built is an existing file

                if (file_NEW_HIH_Scratch(fLoc, numOfElements, outStrArr)) {
                    //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)

                    itemCount = itemCount + 1;
                    //BuildVertical() takes data of keywords[d] type from data section
                    buildVertical_NEW_HIH_Scratch(fLoc, numOfElements, outStrArr, keywords[d]);
                    // //fill array which currently contains: 9 heading components and numOfElements pieces of data
                    // //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                    // //200 refers to the possible 
                    normalizeNan_NEW_HIH_Scratch(keywords[d], outStrArr, 292, date);

                    // //Build temporary files containing rows in order which the files were read in
                    buildF_NEW_HIH_Scratch(outStrArr, numOfElements, keywords[d]);
                    //Find the maximum altitue reached by any balloon
                    if (*numOfElements > maxAlt) {
                        maxAlt = *numOfElements;
                    }
                }
                *numOfElements = 8;
                //prepare file location for next iteration by stripping file specific information
                fLoc = archive;
            }

        }
    }
    if (itemCount == 0) {
        cout << "check data folder location" << endl << "You can type \"C0MMAND\" into the C0MMAND f bar and it will send you to the appropriate location \n" << endl << endl;
    }
    createTemplate_NEW_HIH_Scratch();
    for (int i = 0; i < 11; i++) {
        string fileName = "temp_" + keywords[i] + ".csv";
        ////////This fixes nans which is in the sorted file
       // organize(site, date, itemCount, keywords[i], fileName);
        useTemplate_NEW_HIH_Scratch(fileName, keywords[i], date, itemCount);

    }
    for (int i = 0; i < 11; i++) {
        string fileName = "temp_" + keywords[i] + ".csv";
        clean_NEW_HIH_Scratch(keywords[i]);
        // organize(site, date, itemCount, keywords[i], fileName);
    }

    // useTemplate("temp_.csv","Alt", date);
    remove("temp_.csv");
    remove("temp_alt.csv");
    remove("temp_H2Omr_orig.csv");
    remove("temp_TFP_FPH.csv");

    //For each datatype organize the file by date and output file with naming following conventions input above

}

void main_NEW_BLD_Scratch(LPWSTR fullPath) {
    //Command F jump
    //-----------------------Information for naming output files ---------------------
    string site = "BLD";
    string date = "202204O3";
    //-----------------------Enter File Information here---------------------------------
    string fLoc; //Location of folder containing files
    fLoc = "/Users/collinkadlecek/Downloads/NEW_BLD2022O307/aaBLD_H2O_2022O304";
    string archive = fLoc + "/";
    string outStrArr[512];//Adjust size 
    string prefix[1] = { "BLD_H2O_" }; //All characters preceding date portion of filename
    string suffix = ".txt";//file type, ex. .txt, .lev, .lev2... '.' is neccessary in suffix string
//-------------------------Column Head data types---------------------------------
    string keywords[13] = { "Press","Temp","Theta","RH_FPH","RH_RS","H2Omr","H2Osd","H2Omr_orig","TFP_FPH","TFP_RS","O3mr","alt" };
    // string keywords[13] = {"alt"};
    int itemCount = 0;
    int maxAlt = 0;
    int allCount = 0;
    int* numOfElements = &allCount;
    //Do following for each data type


    for (int i = 0; i < 13; i++) {
        clearOutput_NEW_BLD_Scratch(keywords[i], site, date);
    }



    //--------------------------File prefixes-----------------------------------------

    int numPrefixes = 1;
    int startDatePrefix[1] = { 0 };
    int endDatePrefix[1] = { 25000 };


    //Iterate over all data types
    for (int d = 0; d < 13; d++) {
        buildHeader_NEW_BLD_Scratch(keywords[d], site, date, maxAlt);
        //Iterate over all prefixes
        for (int f = 0; f < numPrefixes; f++) {
            itemCount = 0;
            //Iterate for 12000 days starting in 01/01/1991
            for (int i = startDatePrefix[f]; i < endDatePrefix[f]; i++) {

                for (int j = 0; j < 512; j++) {
                    outStrArr[j] = "nan";
                }
                //Build file name at a date
                for (int q = 0; q < 3; q++) {
                    if (q == 0) {
                        fLoc = fLoc + prefix[f] + buildDate_NEW_BLD_Scratch(i) + suffix;
                    }
                    else if (q == 1) {
                        fLoc = fLoc + prefix[f] + buildDate_NEW_BLD_Scratch(i) + "a" + suffix;
                    }
                    else {
                        fLoc = fLoc + prefix[f] + buildDate_NEW_BLD_Scratch(i) + "b" + suffix;
                    }
                    //If file name built is an existing file
                    if (file_NEW_BLD_Scratch(fLoc, numOfElements, outStrArr)) {
                        //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
                        itemCount = itemCount + 1;
                        //BuildVertical() takes data of keywords[d] type from data section
                        buildVertical_NEW_BLD_Scratch(fLoc, numOfElements, outStrArr, keywords[d]);
                        // //fill array which currently contains: 9 heading components and numOfElements pieces of data
                        // //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                        // //200 refers to the possible 
                        normalizeNan_NEW_BLD_Scratch(keywords[d], outStrArr, 500, date);

                        // //Build temporary files containing rows in order which the files were read in
                        buildF_NEW_BLD_Scratch(outStrArr, numOfElements, keywords[d]);
                        //Find the maximum altitue reached by any balloon
                        if (*numOfElements > maxAlt) {
                            maxAlt = *numOfElements;
                        }
                    }
                    fLoc = archive;
                    *numOfElements = 8;
                }

                //prepare file location for next iteration by stripping file specific information
                fLoc = archive;
            }
            //Secion of code used to deal with leap years

            // for(int r = 1980; r<2021; r++){
            //     for(int j = 0; j < 250; j++){
            //         outStrArr[j] = "      nan";
            //     }
            //     //Build file name at a date
            //     string leapYear = to_string(r);
            //     leapYear = testLeapYears(leapYear);
            //     fLoc = fLoc + prefix[f] + leapYear + suffix;
            //     //If file name built is an existing file
            //     if(file(fLoc,numOfElements,outStrArr)){
            //         //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
            //         itemCount = itemCount + 1;
            //         // BuildVertical() takes data of keywords[d] type from data section
            //         buildVertical(fLoc,numOfElements,outStrArr,keywords[d]);
            //         // //fill array which currently contains: 9 heading components and numOfElements pieces of data
            //         // //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
            //         normalizeNan(keywords[d], outStrArr, 249);
            //         // //Build temporary files containing rows in order which the files were read in
            //         // buildF(outStrArr,numOfElements,keywords[d]);
            //         // //Find the maximum altitue reached by any balloon
            //         // if(*numOfElements > maxAlt){
            //         //     maxAlt = *numOfElements;
            //         // }
            //     }
            //     //prepare file location for next iteration by stripping file specific information
            //     fLoc = archive;

            // }

        }
    }
    if (itemCount == 0) {
        cout << "No files were located, check data folder location" << endl << "You can type \"C0MMAND\" into the C0MMAND f bar and it will send you to the appropriate location \n" << endl << endl;
    }
    createTemplate_NEW_BLD_Scratch(itemCount);
    for (int i = 0; i < 11; i++) {
        string fileName = "temp_" + keywords[i] + ".csv";
        ////////This fixes nans which is in the sorted file
       // organize(site, date, itemCount, keywords[i], fileName);
        useTemplate_NEW_BLD_Scratch(fileName, keywords[i], date, itemCount);

    }


    for (int i = 0; i < 11; i++) {
        string fileName = "temp_" + keywords[i] + ".csv";
        clean_NEW_BLD_Scratch(keywords[i]);
        // organize(site, date, itemCount, keywords[i], fileName);
    }

    remove("temp_.csv");
    remove("temp_alt.csv");
    remove("temp_H2Omr_orig.csv");
    remove("temp_TFP_FPH.csv");

    //For each datatype organize the file by date and output file with naming following conventions input above

}

void main_NEW_BLD_Append(LPWSTR fullPath) {
    MessageBox(NULL, L"NEW BLD Append selected", L"", MB_OK);

    //Command F jump
    //-----------------------Information for naming output files ---------------------
    string site = "BLD_MATNEW_";
    WCHAR outputF_date[64];
    GetWindowTextW(hIn, outputF_date, 64);
 
    for (int i = 0; i < 8; i++) {
        if (outputF_date[i] == NULL) {
        }
        else {
            MessageBox(NULL, L"======================WARNING=====================\n\n\nThe output file will be successfully made, but the length of this date does not conform to the append programs naming convention and will not be able to be appended in the future\n\n======================WARNING=====================", L"Caution!!!", MB_OK);
            break;
        }
    }    //-----------------------Enter File Information here---------------------------------

    int pathLength = wcslen(fullPath);

    //find location of "space" delimeted filenames
    vector <int> fileNameIndexes;
    for (int i = 0; i < pathLength; i++) {
        //if (!findSpace(fullPath[i])) {
        if (!findNull(fullPath[i])) {
            fileNameIndexes.push_back(i);
            //MessageBox(NULL, L"=========================================================================================================================================================================================", L"", MB_OK);
        }
        else {

        }
    }

    const LPWSTR fuck = fullPath;
    ifstream f;
    f.open(fullPath);
    bool issa_flight = false;
    if (f.is_open()) {
        MessageBox(NULL, L"Exception for single selected file ", L"", MB_OK);
        issa_flight = true;
    }
    if (!f.is_open() && fileNameIndexes.size() == 0) {
        MessageBox(NULL, L"Exception for no selected file main NEW BLD", L"", MB_OK);
        return;
    }


    f.close();


    int pathIndex = 0; //rootPath Index rather
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(fullPath[i])) {
            pathIndex = i;
        }
    }
    if (pathIndex != 0) {
        pathIndex = pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid file path", L"", MB_OK);
        return;
    }

    wstring fix;
    fix = fullPath;
    fix.erase(pathIndex, pathLength);
    LPWSTR rootPath = &fix[0];


    //Arhive stuff========================================
    WCHAR Archive_Selection_FilePath[1024];
    GetWindowTextW(hOut, Archive_Selection_FilePath, 1024);


    ifstream testingThing;
    testingThing.open(Archive_Selection_FilePath);
    if (testingThing.is_open()) {
        testingThing.close();
    }
    else {
        MessageBox(NULL, L"did not open the archive file so the file could not be appended", L"", MB_OK);
        MessageBox(NULL, Archive_Selection_FilePath, L"", MB_OK);
        return;
    }
    //Finding the directory to the folder of the archive file chosen
    int archive_pathIndex = 0;
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(Archive_Selection_FilePath[i])) {
            archive_pathIndex = i;
        }
    }
    if (archive_pathIndex != 0) {
        archive_pathIndex = archive_pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid archive file path", L"", MB_OK);
        return;
    }


    wstring archive_fix;
    archive_fix = Archive_Selection_FilePath;

    int archive_pathLength = wcslen(Archive_Selection_FilePath);
    archive_fix.erase(archive_pathIndex, archive_pathLength);
    LPWSTR archive_rootPath = &archive_fix[0];



    wstring archive_date_fix;
    archive_date_fix = Archive_Selection_FilePath;
    archive_date_fix.erase(0, archive_pathLength - 12);
    archive_date_fix.erase(8, 12);
    LPWSTR Archive_date = &archive_date_fix[0];


    string outStrArr[512];//Adjust size 
    string prefix[1] = { "BLD_H2O_" }; //All characters preceding date portion of filename
    string suffix = ".txt";//file type, ex. .txt, .lev, .lev2... '.' is neccessary in suffix string
//-------------------------Column Head data types---------------------------------
    string keywords[13] = { "Press","Temp","Theta","RH_FPH","RH_RS","H2Omr","H2Osd","H2Omr_orig","TFP_FPH","TFP_RS","O3mr" };
    wstring L_keywords[13] = { L"Press",L"Temp",L"Theta",L"RH_FPH",L"RH_RS",L"H2Omr",L"H2Osd",L"H2Omr_orig",L"TFP_FPH",L"TFP_RS",L"O3mr" };

    // string keywords[13] = {"alt"};
    int itemCount = 0;
    int maxAlt = 0;
    int allCount = 0;
    int* numOfElements = &allCount;
    //Do following for each data type


    for (int i = 0; i < 13; i++) {
        clearOutput_NEW_BLD_Append(keywords[i], L_keywords[i], outputF_date);
    }


    //--------------------------File prefixes-----------------------------------------
    if (!issa_flight) {
        for (int i = 0; i < fileNameIndexes.size(); i++) {
            bool first = false;
            if (i == 0) {
                first = true;
            }
            else {
                first = false;
            }
            std::wstring temp = fuck;
            if (i == fileNameIndexes.size() - 1) {
                temp.erase(0, fileNameIndexes.at(i));
            }
            else {
                temp.erase(fileNameIndexes.at(i + 1), pathLength);
                temp.erase(0, fileNameIndexes.at(i));
            }

            WCHAR fLoc[1024];

            for (int f = 0; f < pathIndex; f++) {
                fLoc[f] = rootPath[f];
            }
            for (int f = pathIndex; f < temp.length() + pathIndex; f++) {
                fLoc[f] = temp[f - pathIndex + 1];
            }
            for (int f = temp.length() + pathIndex; f < 1024; f++) {
                fLoc[f] = NULL;
            }

            //mockAppend(fLoc, false);
            int pathLength = temp.length() + pathIndex;
            //Iterate over all data types   

            for (int d = 0; d < 12; d++) {

                if (file_NEW_BLD_Append(fLoc, numOfElements, outStrArr, pathLength, issa_flight, first)) {
                    if (first) {
                        *numOfElements = 8;
                    }
                    //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
                    itemCount = itemCount + 1;
                    //BuildVertical() takes data of keywords[d] type from data section
                    buildVertical_NEW_BLD_Append(fLoc, numOfElements, outStrArr, keywords[d], pathLength);

                    // //fill array which currently contains: 9 heading components and numOfElements pieces of data
                    // //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                    // //200 refers to the possible 
                    normalizeNan_NEW_BLD_Append(keywords[d], outStrArr, 500);

                    // //Build temporary files containing rows in order which the files were read in

                    buildF_NEW_BLD_Append(outStrArr, numOfElements, keywords[d]);

                    //Find the maximum altitue reached by any balloon
                    if (*numOfElements > maxAlt) {
                        maxAlt = *numOfElements;
                    }
                }
                *numOfElements = 8;
            }
        }
    }
    else {
        for (int d = 0; d < 12; d++) {
            //Iterate over all prefixes
            itemCount = 0;
            //Iterate for 12000 days starting in 01/01/1991

            for (int j = 0; j < 512; j++) {
                outStrArr[j] = "nan";
            }
            //Build file name at a date

                //If file name built is an existing file
            if (file_NEW_BLD_Append(fullPath, numOfElements, outStrArr, pathLength, issa_flight, true)) {
                //file() function takes data from human readable and section containing flight specifics (e.g. radiosonde type, O3 sonde ID)
                itemCount = itemCount + 1;
                *numOfElements = 8;

                //BuildVertical() takes data of keywords[d] type from data section
                buildVertical_NEW_BLD_Append(fullPath, numOfElements, outStrArr, keywords[d], pathLength);

                // //fill array which currently contains: 9 heading components and numOfElements pieces of data
                // //remaining index contain "nan" and are replaced with the overflow value for the relevant data type
                // //200 refers to the possible 
                normalizeNan_NEW_BLD_Append(keywords[d], outStrArr, 500);

                // //Build temporary files containing rows in order which the files were read in
                buildF_NEW_BLD_Append(outStrArr, numOfElements, keywords[d]);

                //Find the maximum altitue reached by any balloon
                if (*numOfElements > maxAlt) {
                    maxAlt = *numOfElements;
                }
            }
            *numOfElements = 8;

            //prepare file location for next iteration by stripping file specific information

        }
    }
    if (itemCount == 0) {
        cout << "No files were located, check data folder location" << endl << "You can type \"C0MMAND\" into the C0MMAND f bar and it will send you to the appropriate location \n" << endl << endl;
    }
    int fixes = fileNameIndexes.size();
    if (issa_flight) {
        fixes = 1;
    }

    createTemplate_NEW_BLD_Append(fixes);
    for (int i = 0; i < 11; i++) {
        string fileName = "temp_" + keywords[i] + ".csv";
        ////////This fixes nans which is in the sorted file
       // organize(site, date, itemCount, keywords[i], fileName);
        if (!useTemplate_NEW_BLD_Append(fileName, keywords[i], outputF_date, archive_rootPath, Archive_date, L_keywords[i], fixes)) {
            return;
        }
    }


    for (int i = 0; i < 11; i++) {
        string fileName = "temp_" + keywords[i] + ".csv";
        // organize(site, date, itemCount, keywords[i], fileName);
    }
    //For each datatype organize the file by date and output file with naming following conventions input above
    return;
}





//===============Nonsense===============


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.\
    

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECTTEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECTTEST));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECTTEST));
    //wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOICO));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECTTEST);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

/*
void display_file(char* path){
    FILE* file;
    file = fopen(path, "rb");
}
*/

//======================================================================================================================
//=====================================Open file functions==============================================================
//=====================================================================================================================

template <size_t size>
errno_t wcstombs_s(
    size_t* pReturnValue,
    char(&mbstr)[size],
    const wchar_t* wcstr,
    size_t count
); // C++ only

template <size_t size>
errno_t _wcstombs_s_l(
    size_t* pReturnValue,
    char(&mbstr)[size],
    const wchar_t* wcstr,
    size_t count,
    _locale_t locale
); // C++ only

char o_file(LPWSTR fName) {
    wchar_t filePath[128];
    int l = 0;
    bool trapTrip = false;
    string Testing;
    Testing = convertToString(fName);

    //SetWindowTextA(hOut, Testing);


    char really[8192];
    for (int i = 0; i < 12; i++) {
        really[i] = Testing[i];
    }
    MessageBoxA(NULL, really, "", MB_OK);
    ifstream fing;
    fing.open(Testing.c_str());
    if (fing.is_open()) {
        MessageBox(NULL, L"I'm quirte sure it worked!", L"", MB_OK);
        fing.close();
    }
    else {
        MessageBox(NULL, L"I'm vaguely sure it did nbot worked!", L"", MB_OK);

    }


    while (fName[l] != '.') {
        filePath[l] = fName[l];
        l = l + 1;
        if (l > 120) {
            MessageBox(NULL, L"Yikes! no period found", L"", MB_OK);
            trapTrip = true;
            break;
        }
    }
    if (trapTrip) {
        return 'e';
    }
    else {

    }

    ifstream fin;
    fin.open(fName);
    MessageBox(NULL, fName, L"", MB_OK);
    if (fin.is_open()) {
        MessageBox(NULL, L"fin open", L"", MB_OK);
        MessageBox(NULL, fName, L"", MB_OK);
    }
    else {
        MessageBox(NULL, L"fin not open", L"", MB_OK);

    }
    string line;
    string ret;
    bool it = true;
    int cnt = 0;
    fin >> line;

    while (!fin.eof() && it) {
        cnt = cnt + 1;
        if (cnt > 100) {
            it = false;
        }
        ret = ret + line;
        fin >> line;
    }
    fin.close();
    ofstream f;
    f.open(fName, std::ios_base::app);
    if (f.is_open()) {
        MessageBox(NULL, L"f open", L"", MB_OK);
        MessageBox(NULL, fName, L"", MB_OK);

    }
    else {
        MessageBox(NULL, L"f not open", L"", MB_OK);
    }
    f << ret + "\n";
    f << ret + "\n";
    f << fName;
    f << "\nldsjf\n";

    f.close();
    return 'i';
}
//Returns true if the character plugged in is a character which is common to be typed
bool asciiMask(wchar_t a) {
    if (int(a) > 31 && int(a) < 127) {
        return true;
    }
    else
    {
        return false;
    }
}

//Returns false if the charcater plugged in is a period
bool findNull(wchar_t a) {
        return findSpace(a);
}


//Returns false if question mark slash is found
bool findSlash(wchar_t a) {
    if (int(a) == 92) {
        return false;
    }
    else {
        return true;
    }
}

bool findSpace(wchar_t a) {
    if (int(a) == 32) {
        return false;
    }
    else {
        return true;
    }
}

void mockAppend(WCHAR fPath[], bool final) {
    ifstream fout;

    fout.open(fPath);
    if (fout.is_open()) {
        MessageBox(NULL, L"opened", L"", MB_OK);
        MessageBox(NULL, fPath, L"", MB_OK);
        LPWSTR com;
        string calm;
        getline(fout,calm);
        fout.close();
    }
    else {
        MessageBox(NULL, L"did not open", L"", MB_OK);
    }

}


void testFunc(LPWSTR fPath) {
    MessageBox(NULL, L"testFucntion", L"", MB_OK);
    WCHAR test;
    const LPWSTR fuck = fPath;
    int pathLength = wcslen(fPath);

    //find location of "space" delimeted filenames
    vector <int> fileNameIndexes;
    for (int i = 0; i < pathLength; i++) {
        if (!findSpace(fPath[i])) {
            fileNameIndexes.push_back(i);
            //MessageBox(NULL, L"=========================================================================================================================================================================================", L"", MB_OK);
        }
        else {
        }
    }


    if (fileNameIndexes.size() == 0) {
        MessageBox(NULL, L"Exception for no selected file in directory ", L"", MB_OK);
        return;
    }


    //Find directory path to folder containing multiple files

    int pathIndex = 0;
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(fPath[i])) {
            pathIndex = i;
        }
    }
    if (pathIndex != 0) {
        pathIndex = pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid file path", L"", MB_OK);
        return;
    }

    wstring fix;
    fix = fPath;
    fix.erase(pathIndex, pathLength);
    LPWSTR rootPath = &fix[0];

    MessageBox(NULL, L"root path", L"", MB_OK);
    MessageBox(NULL, rootPath, L"", MB_OK);

    int endCondition;
    vector <LPWSTR> fileNames;

    for (int i = 0; i < fileNameIndexes.size(); i++) {
        //fileNames.push_back(fuck);
    }

    for (int i = 0; i < fileNameIndexes.size(); i++) {

        std::wstring temp = fuck;

        if (i == fileNameIndexes.size() - 1) {
            temp.erase(0, fileNameIndexes.at(i));
        }
        else {
            temp.erase(fileNameIndexes.at(i + 1), pathLength);
            temp.erase(0, fileNameIndexes.at(i));
        }

        fileNames.push_back(&temp[0]);

        MessageBox(NULL, L"preceed", L"", MB_OK);

        WCHAR fName[1024];
        
        for (int f = 0; f < pathIndex; f++) {
            fName[f] = rootPath[f];
        }
        for (int f = pathIndex; f < temp.length() + pathIndex; f++) {
            fName[f] = temp[f - pathIndex+1];
        }
        for (int f = temp.length() + pathIndex; f < 1024; f++) {
            fName[f] = NULL;
        }
        MessageBox(NULL, fName, L"", MB_OK);
        mockAppend(fName, false);

    }

    MessageBox(NULL, L"You better watch out and I hope you don't cry", L"", MB_OK);
    for (int i = 0; i < fileNameIndexes.size(); i++) {
        //MessageBox(NULL, fileNames.at(i), L"", MB_OK);
    }




    vector <std::wstring> build;
    std::wstring translate;
    for (int i = 0; i < fileNames.size(); i++) {
        build.push_back(rootPath);
    }
    for (int i = 0; i < fileNames.size(); i++) {
        translate = fileNames.at(i);
        build.at(i).insert(build.at(i).length(), translate);
    }


    vector <LPWSTR> allFilePaths;
    for (int i = 0; i < fileNames.size(); i++) {
        allFilePaths.push_back(&build.at(i)[0]);

        MessageBox(NULL, allFilePaths.at(i), L"", MB_OK);
    }
}

LPWSTR open_file(HWND hWnd) {
    OPENFILENAME ofn;



    wchar_t file_name[10000];

    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = file_name;

    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 10000;
    wchar_t cast[107] = L"All files\0*.*\0Source Files\0*.CPP*\0Text Files\0*.TXT*\0LEV\0*.lev*\0LEV2\0*.lev2*\0Comma Seperated Values\0*.csv*\0";
    ofn.lpstrFilter = cast;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_ALLOWMULTISELECT| OFN_EXPLORER;
    //OPENFILENAME(ofn);

    //FileDialog ofn(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ALLOWMULTISELECT, NULL,NULL);
    //&ofn == OFN_ALLOWMULTISELECT;
    //&ofn = OFN_ALLOWMULTISELECT;


    GetOpenFileName(&ofn);


    //LPWSTR temp = GetOpenFileName(&ofn);

    /*
    vector<wchar_t> temp;
    temp.push_back(L'L');
    for (int i = 0; i < L; i++) {
        temp.push_back(ofn.lpstrFile[i]);
    }
    LPWSTR  imagining_auto_typing = const_cast<LPTSTR>(TEXT("&Sample Text"));
    for (int i = 0; i < L; i++) {
        imagining_auto_typing = imagining_auto_typing + ofn.lpstrFile[i];
    }

    MessageBox(NULL, imagining_auto_typing, L"", MB_OK);
    */

    //MessageBox(NULL,ofn.lpstrFile, L"", MB_OK);



    //o_file(ofn.lpstrFile);
    //testFunc(ofn.lpstrFile);
    if (ldr_bool && append_bool) {
        main_ldr(ofn.lpstrFile);
        MessageBox(NULL, L"Finished main_ldr append", L"", MB_OK);

    }
    else if (hih_bool && append_bool) {
        main_hih(ofn.lpstrFile);
        MessageBox(NULL, L"Finished main_hih append", L"", MB_OK);

    }
    else if (bld_bool && append_bool) {
        main_bld(ofn.lpstrFile);
        MessageBox(NULL, L"Finished main_bld append", L"", MB_OK);

    }
    else if (ldr_bool && scratch_bool) {
        main_ldr_scratch(ofn.lpstrFile);
        MessageBox(NULL, L"Finished main_ldr scratch", L"", MB_OK);

    }
    else if (hih_bool && scratch_bool) {
        MessageBox(NULL, L"Before main hih scratch", L"", MB_OK);
        main_hih_scratch(ofn.lpstrFile);
        MessageBox(NULL, L"Finished main_hih scratch", L"", MB_OK);
    }
    else if (bld_bool && scratch_bool) {
        main_bld_scratch(ofn.lpstrFile);
        MessageBox(NULL, L"Finished main_bld scratch", L"", MB_OK);

    }
    else if(NEW_ldr_bool && scratch_bool) {
        //main_NEW_LDR_Scratch(ofn.lpstrFile);
        MessageBox(NULL, L"Finished main_NEW_ldr scratch", L"", MB_OK);

    }
    else if (NEW_bld_bool && scratch_bool) {
        //main_NEW_BLD_Scratch(ofn.lpstrFile);
        MessageBox(NULL, L"Finished main_NEW_b scratch", L"", MB_OK);

    }
    else if (NEW_hih_bool && scratch_bool) {
        //main_NEW_HIH_Scratch(ofn.lpstrFile);
        MessageBox(NULL, L"Finished main_NEW_hih scratch", L"", MB_OK);

    }
    else if (NEW_ldr_bool && append_bool) {
        main_NEW_LDR_Append(ofn.lpstrFile);
        MessageBox(NULL, L"Finished main_NEW_ldr append", L"", MB_OK);

    }
    else if (NEW_bld_bool && append_bool) {
        main_NEW_BLD_Append(ofn.lpstrFile);

        MessageBox(NULL, L"Finished main_NEW_bld append", L"", MB_OK);
    }
    else if (NEW_hih_bool && append_bool) {

        main_NEW_HIH_Append(ofn.lpstrFile);
        MessageBox(NULL, L"Finished main_NEW_hih append", L"", MB_OK);
    }
    else if (ldr_bool || hih_bool || bld_bool || NEW_bld_bool || NEW_hih_bool || NEW_ldr_bool && !append_bool && !scratch_bool) {
        MessageBox(NULL, L"Please select an option to either append an existing file \n\n                                                   or\n\nto build a file from scratch", L"", MB_OK);
    }
    else if (append_bool || scratch_bool) {
        MessageBox(NULL, L"Please select a site\n\n i.e. ldr, hih, bld, NEW_LDR etc. ", L"", MB_OK);
    }
    else {
        MessageBox(NULL, L"Please select a site and method for creating the file ", L"", MB_OK);
    }
    return ofn.lpstrFile;
}

LPWSTR choose_archive_file(HWND hWnd) {
    OPENFILENAME ofn;



    wchar_t file_name[256];

    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = file_name;

    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 256;
    wchar_t cast[107] = L"All files\0*.*\0Source Files\0*.CPP*\0Text Files\0*.TXT*\0LEV\0*.lev*\0LEV2\0*.lev2*\0Comma Seperated Values\0*.csv*\0";
    ofn.lpstrFilter = cast;
    ofn.nFilterIndex = 1;
    //ofn.Flags = OFN_ALLOWMULTISELECT;
    //OPENFILENAME(ofn);

    //FileDialog ofn(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ALLOWMULTISELECT, NULL,NULL);
    //&ofn == OFN_ALLOWMULTISELECT;
    //&ofn = OFN_ALLOWMULTISELECT;


    GetOpenFileName(&ofn);


    //LPWSTR temp = GetOpenFileName(&ofn);

    /*
    vector<wchar_t> temp;
    temp.push_back(L'L');
    for (int i = 0; i < L; i++) {
        temp.push_back(ofn.lpstrFile[i]);
    }
    LPWSTR  imagining_auto_typing = const_cast<LPTSTR>(TEXT("&Sample Text"));
    for (int i = 0; i < L; i++) {
        imagining_auto_typing = imagining_auto_typing + ofn.lpstrFile[i];
    }

    MessageBox(NULL, imagining_auto_typing, L"", MB_OK);
    */



    //o_file(ofn.lpstrFile);
    //testFunc(ofn.lpstrFile);
    SetWindowText(hOut, ofn.lpstrFile);
    return ofn.lpstrFile;
}


void getarchivefile(HWND hWnd) {

    string lev_keywords[13] = { "O3DN","O3RES","ftemp","ftempv","hum","ozone_atmcm","ozone_mPa","o3_mr","pottp","press","Temp","water","ptemp" };
    wstring lev_L_keywords[13] = { L"O3DN",L"O3RES",L"ftemp",L"ftempv",L"hum",L"ozone_atmcm",L"ozone_mPa",L"o3_mr",L"pottp",L"press",L"Temp",L"water",L"ptemp" };
    
    string new_keywords[11] = { "Press","Temp","Theta","RH_FPH","RH_RS","H2Omr","H2Osd","H2Omr_orig","TFP_FPH","TFP_RS","O3mr" };
    wstring new_L_keywords[11] = { L"Press",L"Temp",L"Theta",L"RH_FPH",L"RH_RS",L"H2Omr",L"H2Osd",L"H2Omr_orig",L"TFP_FPH",L"TFP_RS",L"O3mr" };

    WCHAR Archive_Selection_FilePath[1024];
    GetWindowTextW(hOut, Archive_Selection_FilePath, 1024);

    ifstream testingThing;
    testingThing.open(Archive_Selection_FilePath);
    testingThing.close();
    //Finding the directory to the folder of the archive file chosen
    int archive_pathIndex = 0;
    int pathLength = wcslen(Archive_Selection_FilePath);
    for (int i = 0; i < pathLength; i++) {
        if (!findSlash(Archive_Selection_FilePath[i])) {
            archive_pathIndex = i;
        }
    }
    if (archive_pathIndex != 0) {
        archive_pathIndex = archive_pathIndex + 1;
    }
    else {
        MessageBox(NULL, L"Invalid archive file path", L"", MB_OK);
        //return;
    }
/*
    std::ifstream fin;
    fin.open("C:\\Users\\kadlecek\\Desktop\\Archive_Files_All_Locations\\LDR_Archive\\LDR_MATLEV_O3RES_20220812.csv");

    std::ofstream fout;
    fout.open("Text.txt");
    if (fout.is_open());
    MessageBox(NULL, L"Everyone heard you,", L"", MB_OK);
    if (fout.is_open()) {
        MessageBox(NULL, L"That's pretty cool", L"", MB_OK);

    }

    string lying = "lion";

    getline(fin, lying);
    if (lying != "lion") {
        MessageBox(NULL, L"I will remain calm", L"", MB_OK);

    }
    MessageBox(NULL, L"no one thought it was funny", L"", MB_OK);


    return;

*/

    wstring fix;
    fix = Archive_Selection_FilePath;
    fix.erase(archive_pathIndex, pathLength);
    LPWSTR rootPath = &fix[0];

    if (ldr_bool && append_bool) {
        string temp = "LDR_MATLEV_";
        string late = "_20220812.csv";

        wstring wtemp = L"LDR_MATLEV_";
        wstring wlate = L"_20220812.csv";

        for (int i = 0; i < 13; i++) {
            ifstream fin;
            ofstream fout;
            fin.open(temp + lev_keywords[i] + late);
            fout.open(rootPath + wtemp + lev_L_keywords[i] + wlate);
            string line = "";
            /*
            WCHAR line[255];
            int j = 0;

            while (!fin.eof() && j < 220) {
                //getline(fin, line);
                fin >> line;
                if (j > 210) {
                    MessageBox(NULL, L"EOF failed", L"", MB_OK);
                    break;
                }
                j = j + 1;
            }*/
            wstring conc = wtemp + lev_L_keywords[i] + wlate;
            LPWSTR weird = &conc[0];

            int j = 0;
            while (!fin.eof()) {
                getline(fin, line);
                //fin >> line;
                fout << line;
                fout << "\n";
                if (j > 210) {
                    MessageBox(NULL, L"EOF failed", L"", MB_OK);
                    break;
                }
                j = j + 1;
            }
            
            fin.close();
            fout.close();

        }

    }
    else if (hih_bool && append_bool) {
        string temp = "LDR_MATLEV_";
        string late = "_20220812.csv";

        wstring wtemp = L"LDR_MATLEV_";
        wstring wlate = L"_20220812.csv";
        for (int i = 0; i < 13; i++) {

        }
    }
    else if (bld_bool && append_bool) {
        string temp = "BLD_MATLEV_";
        string late = "_20220812.csv";

        wstring wtemp = L"BLD_MATLEV_";
        wstring wlate = L"_20220812.csv";
        MessageBox(NULL, L"smile and wave", L"", MB_OK);

        ofstream test;
        //wstring what = L"JohnWayne";
        //test.open(rootPath + what);


        for (int i = 0; i < 13; i++) {
            ifstream fin;
            ofstream fout;
            fin.open(temp + lev_keywords[i] + late, ios::in);
            fout.open(rootPath + wtemp + lev_L_keywords[i] + wlate, ios::out);
            string line = "";

            if (fin.is_open()) {
                MessageBox(NULL, L"read file is open", L"", MB_OK);
            }
            if (fout.is_open()) {
                MessageBox(NULL, L"fout is open", L"", MB_OK);

            }
            /*
            WCHAR line[255];
            int j = 0;

            while (!fin.eof() && j < 220) {
                //getline(fin, line);
                fin >> line;
                if (j > 210) {
                    MessageBox(NULL, L"EOF failed", L"", MB_OK);
                    break;
                }
                j = j + 1;
            }*/
            wstring conc = rootPath + wtemp + lev_L_keywords[i] + wlate;
            LPWSTR weird = &conc[0];
            
            int j = 0;
            while (!fin.eof()) {
                line = "asdf";
                getline(fin, line);
                fin >> line;
                //fin >> line;
                fout << line;
                fout << "\n";
                if (j > 2000) {
                    MessageBox(NULL, L"EOF failed", L"", MB_OK);
                    break;
                }
                j = j + 1;
            }

            fin.close();
            fout.close();
            //MessageBox(NULL,weird, L"", MB_OK);

        
        }
        MessageBox(NULL, L"Archived files have been uploaded", L"", MB_OK);

    }
    else if (NEW_ldr_bool && append_bool) {
        string temp = "LDR_MATNEW_";
        string late = "_20220812.csv";

        wstring wtemp = L"LDR_MATNEW_";
        wstring wlate = L"_20220812.csv";
        for (int i = 0; i < 11; i++) {

        }
    }
    else if (NEW_bld_bool && append_bool) {

        for (int i = 0; i < 11; i++) {

        }
    }
    else if (NEW_hih_bool && append_bool) {
        for (int i = 0; i < 11; i++) {

        }
    }
    else if (ldr_bool || hih_bool || bld_bool || NEW_bld_bool || NEW_hih_bool || NEW_ldr_bool && !append_bool && !scratch_bool) {
        MessageBox(NULL, L"Please select an option to either append an existing file \n\n                                                   or\n\nto build a file from scratch", L"", MB_OK);
    }
    else if (append_bool || scratch_bool) {
        MessageBox(NULL, L"Please select a site\n\n i.e. ldr, hih, bld, NEW_LDR etc. ", L"", MB_OK);
    }
    else {
        MessageBox(NULL, L"Please select a site and method for creating the file ", L"", MB_OK);
    }

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LPWSTR wa;
    TCHAR greeting[] = _T("For first time users of the software, please click the ? buttons to learn more about each step");
    //Hello, Windows desktop!\n\n\n\nStep 2: Choose a file to append\nNOTE:
    //place all files to be appended in the same folder. All these files need 
    //to fit the naming convention: site_MATLEV/MATNEW_datatype_eight charcter
    //date.csv\nex. LDR_MATLEV_water_20220510.csv LDR_MATNEW_20220812.csv \n
    //\n\n\n Finally navigate to the lev or lev2 files you would like to appe
    //nd to the archive file selected"
    wstring L_keywords[13] = { L"O3DN",L"O3RES",L"ftemp",L"ftempv",L"hum",L"ozone_atmcm",L"ozone_mPa",L"o3_mr",L"pottp",L"press",L"Temp",L"water",L"ptemp" };

    switch (message)
    {

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        // Parse the menu selections:
        switch (wmId)
        {
        case OPEN_FILE_BUTTON:
        {
            LPWSTR ws = (open_file(hWnd));
            //MessageBox(NULL, ws, L"", MB_OK);
            //o_file(ws);
            //SetWindowText(hOut, ws);



            break;
        }
        case CHOOSE_ARCHIVE:
            wa = (choose_archive_file(hWnd));
            break;
        case Build_Header:
            //Get outputfile date from that one box
            WCHAR outputF_date[64];
            GetWindowTextW(hIn, outputF_date, 64);
            for (int i = 0; i < 8; i++) {
                if (outputF_date[i] == NULL) {
                }
                else {
                    MessageBox(NULL, L"======================WARNING=====================\n\n\nThe output file will be successfully made, but the length of this date does not conform to the append programs naming convention and will not be able to be appended in the future\n\n======================WARNING=====================", L"Caution!!!", MB_OK);
                    break;
                }
            }
            //for use later

            if (ldr_bool) {
                for (int i = 0; i < 13; i++) {
                    buildHeader_ldr_scratch(L_keywords[i], outputF_date);
                }
                MessageBox(NULL, L"Routine finished", L"Caution!!!", MB_OK);

            }
            else if (hih_bool) {
                for (int i = 0; i < 13; i++) {
                    //buildHeader_hih_scratch(L_keywords[i], outputF_date);
                }
            }
            else if (bld_bool) {
                MessageBox(NULL, L"bld_header has not been completed", L"Caution!!!", MB_OK);

                for (int i = 0; i < 13; i++) {
                    //buildHeader_bld_scratch(L_keywords[i], outputF_date);
                }
            }
            else if (NEW_ldr_bool) {
                MessageBox(NULL, L"NEW LDR header has not been completed", L"Caution!!!", MB_OK);

                for (int i = 0; i < 13; i++) {
                    //buildHeader_NEW_ldr_scratch(L_keywords[i], outputF_date);
                }
            }
            else if (NEW_hih_bool) {
                MessageBox(NULL, L"NEW HIH header has not been completed", L"Caution!!!", MB_OK);

                for (int i = 0; i < 13; i++) {
                    //buildHeader_ldr_scratch(L_keywords[i], outputF_date);
                }
            }
            else if (NEW_bld_bool) {
                MessageBox(NULL, L"NEW BLD header has not been completed", L"Caution!!!", MB_OK);

                for (int i = 0; i < 13; i++) {
                    //buildHead/*e*/r_ldr_scratch(L_keywords[i], outputF_date);
                }
            }
            else {
                MessageBox(NULL, L"A site must be selected to create a header file", L"", MB_OK);

            }
            break;
        case ldr:
            ldr_bool = true;
            hih_bool = false;
            bld_bool = false;
            NEW_ldr_bool = false;
            NEW_hih_bool = false;
            NEW_bld_bool = false;
            break;
        case hih:
            ldr_bool = false;
            hih_bool = true;
            bld_bool = false;
            NEW_ldr_bool = false;
            NEW_hih_bool = false;
            NEW_bld_bool = false;
            break;
        case bld:
            ldr_bool = false;
            hih_bool = false;
            bld_bool = true;
            NEW_ldr_bool = false;
            NEW_hih_bool = false;
            NEW_bld_bool = false;
            break;
        case NEW_ldr:
            ldr_bool = false;
            hih_bool = false;
            bld_bool = false;
            NEW_ldr_bool = true;
            NEW_hih_bool = false;
            NEW_bld_bool = false;
            break;
        case NEW_hih:
            ldr_bool = false;
            hih_bool = false;
            bld_bool = false;
            NEW_ldr_bool = false;
            NEW_hih_bool = true;
            NEW_bld_bool = false;
            break;
        case NEW_bld:
            ldr_bool = false;
            hih_bool = false;
            bld_bool = false;
            NEW_ldr_bool = false;
            NEW_hih_bool = false;
            NEW_bld_bool = true;
            break;
        case build_full:
            append_bool = false;
            scratch_bool = true;
            /*
            if (ldr_bool) {
                MessageBox(NULL, L"ITTTTTTTTTTT ldr worked", L"", MB_OK);
            }
            if (hih_bool) {
                MessageBox(NULL, L"ITTTTTTTTTTT hih worked", L"", MB_OK);
            }
            if (bld_bool) {
                MessageBox(NULL, L"ITTTTTTTTTTT bld worked", L"", MB_OK);
            }
            if (NEW_ldr_bool) {
                MessageBox(NULL, L"ITTTTTTTTTTT NEW_ldr worked", L"", MB_OK);
            }
            if (NEW_hih_bool) {
                MessageBox(NULL, L"ITTTTTTTTTTT NEW_hih worked", L"", MB_OK);
            }
            if (NEW_bld_bool) {
                MessageBox(NULL, L"ITTTTTTTTTTT NEW_bld worked", L"", MB_OK);
            }
            */

            break;
        case append:
            scratch_bool = false;
            append_bool = true;
            /*
            if (ldr_bool) {
                MessageBox(NULL, L"Ig ldr worked", L"", MB_OK);
            }
            if (hih_bool) {
                MessageBox(NULL, L"Ig hih worked", L"", MB_OK);
            }
            if (bld_bool) {
                MessageBox(NULL, L"Ig bld worked", L"", MB_OK);
            }
            if (NEW_ldr_bool) {
                MessageBox(NULL, L"Ig NEW_ldr worked", L"", MB_OK);
            }
            if (NEW_hih_bool) {
                MessageBox(NULL, L"Ig NEW_hih worked", L"", MB_OK);
            }
            if (NEW_bld_bool) {
                MessageBox(NULL, L"Ig NEW_bld worked", L"", MB_OK);
            }
            */
            break;
        case in_test:
        {
            char hurt[10];
            //LPWSTR sure = const_cast<LPTSTR>(TEXT("&Sample Text"));
            GetWindowTextA(hIn, hurt, 10);
            SetWindowTextA(hOut, hurt);
            /*
            wchar_t testgin[10] = L"nice";

            SetWindowTextW(hIn, testgin);
            */
            break;
        }
        case helpstep1:
        {
            //MessageBox(NULL, L"Data from different sites has slightly different formatting, so select the site from which the data is from to allow the program to correctly process the data", L"", MB_OK);
            MessageBox(NULL, L"Trying to run main_ldr", L"", MB_OK);
            wstring notCash = L"ahahah";
            LPWSTR sike = &notCash[0];

            main_ldr(sike);
            break;
        }        
        case helpstep2:
        {
            MessageBox(NULL, L"This code is made to append new data to an existing file. This step tells the code where these existing (to-be-appended) files are. In order for the code to run properly, the files to be appended need to follow the strict naming convention which is described in the textbox Labeled as \"step2\" which is located next to the question mark you just clicked. Select any file (which follows the appropriate naming convention) within the folder which contains all the files which will hold the data for each datatype. This tells the code where the to-be-appended files are, and will automatically append each datatype-file, even though only one datatype-file was selected\n=================================================================\nNOTE: If an error appears describing an incorrect archive path which seems correct, the path may have exceeded the character limit. Place the archive file in the desktop to lessen the number of characters in the archive filepath", L"", MB_OK);

            break;
        }
        case helpstep3:
        {
            MessageBox(NULL, L"When the program is run, a new file will be created. This new file will contain all the information which was held in the files selected in step 2 as to be appended. When this new file is created, it is created with the same naming convention, but a new date. In the textbox below, you can select what new date should be attached to the new files created.", L"", MB_OK);

            break;
        }
        case helpstep4:
        {
            MessageBox(NULL, L"Once files are selected the code will run. The files selected should be of the appropriate format as selected in step 1\n\n=============WARNING=============\nDue to the limitations of the language of this program was written in, only 120 files can be selected to be appended during one run of the program. \n\n Finally, the created files will be located in the folder where the selected lev files are located.", L"", MB_OK);

            break;
        }
        case getArchive:
        {
            
            getarchivefile(hWnd);
            break;

        }

        case IDM_ABOUT:
        {
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        }
        case IDM_EXIT:
        {
            DestroyWindow(hWnd);
            break;
        }
        default:
        {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        }
    }
    break;
    case WM_PAINT:
    {
        /*
            HWND hwndButton = CreateWindowExW(
                0L,
                L"BUTTON",  // Predefined class; Unicode assumed
                L"OK",      // Button text
                WS_TABSTOP,  // Styles
                100,         // x position
                100,         // y position
                100,        // Button width
                100,        // Button height
                hWnd,     // Parent window
                NULL,       // No menu.
                (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                NULL);      // Pointer not needed.
                */


        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc,
            5, 5,
            greeting, _tcslen(greeting));
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_CREATE:
        buttonTest(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


void buttonTest(HWND hWnd) {
    //CreateWindowW(
    //    L"BUTTON",  // Predefined class; Unicode assumed 
    //    L"Click",      // Button text 
    //    WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_AUTOCHECKBOX,  // Styles 
    //    100,         // x position 
    //    100,         // y position 
    //    38,        // Button width
    //    30,        // Button height
    //    hWnd,     // Parent window
    //    (HMENU)in_test,       // No menu.
    //    NULL,
    //    NULL);      // Pointer not needed.

    CreateWindowW(L"Static", L"Step 1: Choose the input data's format", WS_VISIBLE | WS_CHILD, 
        50, 30, 400, 30, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"Button", L"?", WS_VISIBLE | WS_CHILD, 
        475, 30, 30, 30, hWnd, (HMENU)helpstep1, NULL, NULL);

    ldr_bool = CreateWindowW(L"Button", L"ldr", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_AUTORADIOBUTTON, 50, 75, 100, 30, hWnd, (HMENU)ldr, NULL, NULL);
    hih_bool = CreateWindowW(L"Button", L"hih", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_AUTORADIOBUTTON, 150, 75, 90, 30, hWnd, (HMENU)hih, NULL, NULL);
    bld_bool = CreateWindowW(L"Button", L"bld", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_AUTORADIOBUTTON, 250, 75, 90, 30, hWnd, (HMENU)bld, NULL, NULL);
    
    NEW_ldr_bool = CreateWindowW(L"Button", L"NEW ldr", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_AUTORADIOBUTTON, 350, 75, 90, 30, hWnd, (HMENU)NEW_ldr, NULL, NULL);
    NEW_hih_bool = CreateWindowW(L"Button", L"NEW hih", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_AUTORADIOBUTTON, 450, 75, 90, 30, hWnd, (HMENU)NEW_hih, NULL, NULL);
    NEW_bld_bool = CreateWindowW(L"Button", L"NEW bld", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_AUTORADIOBUTTON, 550, 75, 90, 30, hWnd, (HMENU)NEW_bld, NULL, NULL);

    //o
    CreateWindowW(L"Static", L"Step 2: Choose a file to append\nNOTE: place all files to be appended in the same folder. All these files need to fit the naming convention: site_MATLEV/MATNEW_datatype_eight charcter date.csv\nex. LDR_MATLEV_water_20220510.csv LDR_MATNEW_20220812.csv", WS_VISIBLE | WS_CHILD
        , 50, 125, 400, 100, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"Button", L"?", WS_VISIBLE | WS_CHILD,
         475, 125, 30, 30, hWnd, (HMENU)helpstep2, NULL, NULL);

    hOut = CreateWindowW(L"EDIT", L"Full path to file which will be appended", WS_VISIBLE | WS_CHILD | WS_BORDER, 225, 250, 900, 30, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"Button", L"Choose archive path", WS_VISIBLE | WS_CHILD, 50, 250, 150, 30, hWnd, (HMENU)CHOOSE_ARCHIVE, NULL, NULL);


    CreateWindowW(L"Static", L"Step 3: Enter a date which will be the date assigned to the appended files", WS_VISIBLE | WS_CHILD
        , 50, 300, 400, 30, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"Button", L"?", WS_VISIBLE | WS_CHILD,
        475, 300, 30, 30, hWnd, (HMENU)helpstep3, NULL, NULL);

    hIn = CreateWindowW(L"EDIT", L"Type date here", WS_VISIBLE | WS_CHILD | WS_BORDER, 50, 350, 200, 30, hWnd, NULL, NULL, NULL);


    //CreateWindowW(L"Button", L"append", WS_VISIBLE | WS_CHILD | BS_3STATE, 400, 75, 90, 30, hWnd, (HMENU)append, NULL, NULL);
    //CreateWindowW(L"Button", L"From Scratch", WS_VISIBLE | WS_CHILD | BS_3STATE, 400, 125, 90, 30, hWnd, (HMENU)build_full, NULL, NULL);

    //CreateWindowW(L"Button", L"Create Header file", WS_VISIBLE | WS_CHILD, 500, 100, 90, 30, hWnd, (HMENU)Build_Header, NULL, NULL);


    //WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_AUTOCHECKBOX
    CreateWindowW(L"Static", L"Final Step: Select the file(s) which will be appended to the selected archive file"
        , WS_VISIBLE | WS_CHILD, 50, 400, 400, 30, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"Button", L"?", WS_VISIBLE | WS_CHILD,
        475, 400, 30, 30, hWnd, (HMENU)helpstep4, NULL, NULL);

    CreateWindowW(L"Button", L"Open File", WS_VISIBLE | WS_CHILD
        , 50, 450, 90, 30, hWnd, (HMENU)OPEN_FILE_BUTTON, NULL, NULL);
    //CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER | ES_AUTOVSCROLL
      //  , 150, 450, 90, 30, hWnd, NULL, NULL, NULL);
    //CreateWindowW(L"Button", L"God I'm bad", WS_VISIBLE | WS_CHILD,
        //600, 15, 100, 30, hWnd, (HMENU)getArchive, NULL, NULL);



}