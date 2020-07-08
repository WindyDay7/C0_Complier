#ifndef SYMBOL_TABLE_H_INCLUDED
#define SYMBOL_TABLE_H_INCLUDED
#include "Lexical_func.h"
#include <string>
struct Symbol_Table
{
	char name[50];
	Symbol_Table_Type type_information;
	int data_type;          //数据类型，1表示int， 0表示char
	int length;
	char parent_function[50];
	int address;			//对于函数，这个存储了函数第一个参数在符号表的位置
	//对于参数，这个值存储了参数相对于函数首地址的地址，那么对于函数，是否需要存储函数的首地址，答案是需要的，但是存在哪是个问题，
	// 不需要，在函数调用完之后，把用到的栈减去就好了
	int temp_address;           //表示函数对应的临时变量表开头的地址
	int const_int;
	char const_char;
	int Where;  //1表示在内存中， 2表示在寄存器中
};

extern Symbol_Table* Global_symbol_table[100];
extern Symbol_Table* Partial_symbol_table[100];
extern Symbol_Table* Temp_symbol_table[2000];
extern int Pstn;
extern int Gstn;
extern int ident_data_kind;

void enter_Partial_symbol_table(char name_in[], Symbol_Table_Type type_information, int data_type, int length, \
                                 char parent_function[], int address, int const_int, char const_char);

void enter_Global_symbol_table(char name_in[], Symbol_Table_Type type_information, int data_type, int length, \
                                  int address, int const_int, char const_char);

void enter_Temp_symbol_table(int count_num, int address, int parent_addr);

int look_Global(char look_name[]);
int look_Partial(char look_name[], int function_satrt);
int look_Temp(int count_num);

#endif
