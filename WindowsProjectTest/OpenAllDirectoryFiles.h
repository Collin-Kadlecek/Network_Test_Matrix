#ifndef OPENALLDIRECTORYFILES_H  // To make sure you don't declare the function more than once by including the header multiple times.
#define	OPENALLDIRECTORYFILES_H
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

int getDate();

string getTodaysDate();

vector<string> createBLD_LEV_file_archive( );

vector<string> createLDR_LEV_file_archive( );

vector<string> createHIH_LEV_file_archive( );

vector<string> createHIH_H2O_file_archive( );

vector<string> createLDR_H2O_file_archive( );

vector<string> createBLD_H2O_file_archive( );

void mainOpenAllDirectoryfiles();


#endif