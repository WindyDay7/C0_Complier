#pragma  once
#include <stdio.h>
#include "Lexical_func.h"
#define line_longest  200
#include <fstream>
using namespace std;


extern enumType symbol;

//Reserved word declaration
extern char key_word[13][10];
extern enumType ksy_word[13];
extern char special_char[10];
extern enumType special_char_num[10];

extern char line[200];
extern int Current_line_length; //
extern int Current_location;   //
extern int Count_line;

extern char temp_symbol;			//
extern char temp_ident[200];			//当前读到的标识符的name
extern int number;					//
extern char real_temp_char;

extern enumType ALL_symbol[5000];		//所有的标识符存起来
extern int symbol_count;
extern String_code* sentence[200];
extern int sentence_count;
ofstream SaveFile("result.txt");   //
