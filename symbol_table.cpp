#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "Syntax_analysis.h"
using namespace std;
void enter_Partial_symbol_table(char name_in[], Symbol_Table_Type type_information, int data_type, int length, \
                                char parent_function[], int address, int const_int, char const_char)
{
	// Partial_symbol_table_num 计数为Pstn
	Partial_symbol_table[Pstn] = (Symbol_Table*)malloc(sizeof(Symbol_Table));
	strcpy(Partial_symbol_table[Pstn]->name, name_in);
	Partial_symbol_table[Pstn]->type_information = type_information;
	Partial_symbol_table[Pstn]->data_type = data_type;
	Partial_symbol_table[Pstn]->length = length;
	strcpy(Partial_symbol_table[Pstn]->parent_function, parent_function);
	Partial_symbol_table[Pstn]->address = address;
	Partial_symbol_table[Pstn]->const_int = const_int;
	Partial_symbol_table[Pstn]->const_char = const_char;
	Pstn += 1;
}

void enter_Global_symbol_table(char name_in[], Symbol_Table_Type type_information, int data_type, int length, \
                                int address, int const_int, char const_char)
{

	// Global_symbol_table_num 标记为，计数为Gstn

	Global_symbol_table[Gstn] = (Symbol_Table*)malloc(sizeof(Symbol_Table));
	strcpy(Global_symbol_table[Gstn]->name, name_in);
	Global_symbol_table[Gstn]->type_information = type_information;
	Global_symbol_table[Gstn]->data_type = data_type;
	Global_symbol_table[Gstn]->length = length;
	Global_symbol_table[Gstn]->address = address;
	Global_symbol_table[Gstn]->const_int = const_int;
	Global_symbol_table[Gstn]->const_char = const_char;
	Gstn += 1;
	return ;
}


int look_Global(char Look_name[])		//符号表需要返回标识符在符号表的位置
{
	for (int i = 1; i < Gstn; ++i)
	{
		if (strcmp(Global_symbol_table[i]->name, Look_name) == 0)
		{
			ident_data_kind = Global_symbol_table[i]->data_type;
			return i;
		}
	}
	return 0;
}

int look_Partial(char Look_name[], int function_start)			//符号表需要返回标识符在符号表的位置
{
	for (int i = function_start; i < Pstn; ++i)
	{
		if (strcmp(Partial_symbol_table[i]->name, Look_name) == 0)
		{
			ident_data_kind = Partial_symbol_table[i]->data_type;
			return i;
		}
	}
	return 0;
}
