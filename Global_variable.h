#ifndef GLOBAL_VARIABLE_H_INCLUDED
#define GLOBAL_VARIABLE_H_INCLUDED
#include "symbol_table.h"
#include <fstream>
using namespace std;
FILE *fp;

enumType symbol;

char key_word[13][10];
enumType ksy_word[13];
char special_char[10];
enumType special_char_num[10];
enumType ALL_symbol[5000];
int symbol_count = 0;
char line[200] = { 0 };		//当前的行
int Current_line_length = 0; //当前行最长长度
char temp_symbol = 0;			//当前的字符
char temp_ident[200] = { 0 };	//标识符最长为20
int Current_location = 0;
int Count_line = 0;			//词法分析
String_code* sentence[200];
int sentence_count = 0;

int number = 0;					//表示读到的数字大小
char real_temp_char;
int data_address = 0;			//变量在数据区的地址
int part_start = 0;			//搜索局部符号表开始的位置
char current_function[20];	//当前处理的函数名
// string Empty_name = "ALL";
int Reserved = 0;           //表示全局变量预留空间的大小

int Gstn = 1;
int Pstn = 1;
int ident_data_kind = 0;		//符号表里面获取当前标识符的数据类型
int Inter_Code_Line = 0;        //中间代码的行数的
Intermediate_Code* Code_txt[4000];       //语义分析，中间代码
int temp_count = 0;     //表示临时变量的标号
int Label_count = 0;		//表示用于作为Label的个数，，

ofstream Syntax_Result("syntax_result.txt");   //输出流，输出到文件

Symbol_Table* Global_symbol_table[100];
Symbol_Table* Partial_symbol_table[100];


Register Temp_Register[10] = {0};
Register Global_Register[8] = {0};
Register Function_Register[4] = {0};
Register try1[100][2] ={0};
ofstream MIPS_code("MIPS_code.txt");
int MIPS_funct_addr = 0;
int MIPS_count_line = 0;

#endif // GLOBAL_VARIABLE_H_INCLUDED
