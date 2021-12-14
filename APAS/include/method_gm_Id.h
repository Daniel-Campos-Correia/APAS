#ifndef METHOD_GM_ID_H
#define METHOD_GM_ID_H


#include <fstream>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <ctype.h>
#include <list>
#include <filesystem>
#include <dirent.h>
#include "globalVariable.h"

using namespace std;

void tableCreation(list<string> listLookUpDir, string path);
void testGainAndL(float ganho, float L_maximo, float L__minimo);
void testGainAndEfficiency(float ganho, float eficiencia_maxima, float eficiencia_minima);
vector<string> tokenize(string s, string del = " ");
string string_replace( string src, string const& target, string const& repl);
void FindReplace(string& line, string& oldString, string& newString);
list<string> listArchives(string path);



#endif // METHOD_GM_ID_H
