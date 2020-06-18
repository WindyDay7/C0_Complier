#ifndef SYMBOL_TABLE_H_INCLUDED
#define SYMBOL_TABLE_H_INCLUDED
#include "Lexical_func.h"
#include <string>
struct Symbol_Table
{
	char name[20];
	Symbol_Table_Type type_information;
	int data_type;          //数据类型，1表示int， 0表示char
	int length;
	char parent_function[20];
	int address;			//对于函数，这个存储了函数第一个参数在符号表的位置
	//对于参数，这个值存储了参数相对于函数首地址的地址，那么对于函数，是否需要存储函数的首地址，答案是需要的，但是存在哪是个问题，
	// 不需要，在函数调用完之后，把用到的栈减去就好了
	int const_int;
	char const_char;
};

extern Symbol_Table* Global_symbol_table[100];
extern Symbol_Table* Partial_symbol_table[100];
extern int Pstn;
extern int Gstn;
extern int ident_data_kind;

void enter_Partial_symbol_table(char name_in[], Symbol_Table_Type type_information, int data_type, int length, \
                                 char parent_function[], int address, int const_int, char const_char);

void enter_Global_symbol_table(char name_in[], Symbol_Table_Type type_information, int data_type, int length, \
                                  int address, int const_int, char const_char);

int look_Global(char look_name[]);
int look_Partial(char look_name[], int function_satrt);

#endif
