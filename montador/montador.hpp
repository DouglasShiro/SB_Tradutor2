#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <locale>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

using namespace std;

int _erro = FALSE ;
int	_sectionText = -1, 	_sectionData = -1;
int _begin = FALSE, _end = FALSE;

int primeira_passagem(map<string,string>& opTable, map<string,int>& dirTable,
						vector<string>& token,	map<string,int>& simbTable,
						unordered_map<string,int>& defTable, unordered_multimap<string,int>& useTable);
 int segunda_passagem(map<string,string> &opTable, map<string,int> &dirTable,
						vector<string> &token,	map<string,int> &simbTable,
						unordered_map<string,int> &defTable, unordered_multimap<string,int> &useTable,
						vector<string> &code, vector<int>&relativo);
int token_valido(string token);
