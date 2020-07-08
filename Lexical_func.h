#pragma once
#include <fstream>
using namespace std;

//Type Declarations, use enumeration type for all kinds of
enum enumType {
	intcon, charcon, stringcon, un_intcon, plus_sign, minus_sign, multiply, wrong_sy, endow, \
	division, beq, bne, bge, bgt, ble, blt, l_brace, r_brace, l_bracket, r_bracket, \
	l_parentheses, r_parentheses, semicolon, comma, sqm, dqm, ident, int_sy, char_sy, \
	main_sy, void_sy, do_sy, while_sy, for_sy, if_sy, else_sy, printf_sy, scanf_sy, const_sy, return_sy
};

enum Symbol_Table_Type {valued_function_kind, invalued_function_kind, variable_kind, constant_kind, \
                        array_kind, parameter_kind
                       };

enum Quaternary_kind { funct_declare, funct_call, funct_return, expression, endow_assign ,Con_judge, GOTO, BNZ, BZ, With_Label, \
					   array_number, number_array, Negate, push, funct_result, para_declare, read_code, write_code
};

struct Expression_result
{
	int result_data_kind;		//数据类型，用于数据类型是否匹配
	// int un_know;
	int un_partial;		//非局部的，如果是局部变量,或者是临时变量的标号
	int temp_local;		//符号表中的位置
};

struct Intermediate_Code
{
	Quaternary_kind Code_type;		//中间代码的类型，例入跳转或者计算赋值
	// int compare_cal;		//关系运算
	// int temp_count;			//临时变量的序号
	int number1_lc, number2_lc, number3_lc;		//三个操作数在符号表的位置,或者是临时变量的序号
	int global_1, global_2, global_3;		//记录这个三个数在是否在全局符号表，是为1，否为0, 2表示是临时变量, 3表示就是数字，4表示是字符串
	int operate_cal;		//记录计算用的操作符1表示'+', 2表示'-', 3表示'*' ,4表示'/', 5表示取反，6表示直接赋值, 7表示相等
	// 8表示不相等， 9表示 bge， 10表示bgt， 11表示ble， 12表示blt,  13表示是不等于0跳转, 14表示字符串输出， 15表示整数输出， 16表示字符输出
	char Label[20];		//存储label
};

struct String_code
{
    char string_name[200];
    char string_sce[200];
};

struct Register
{
    int status;
    int number_in;
    int global_in;          //Parent_num 如果是临时寄存器，表示如果使用过为0，没用过为1，
    int Parent_num;         //表示这个寄存器在哪个函数使用，临时寄存器的parent表示是否存入了内存
};

struct Funct_para_List
{

};

extern enumType symbol;

//function declaration
char next_char(FILE *read_in);
enumType next_symbol(FILE *read_in);
void initialize();
void Read_until(FILE *read_in);
