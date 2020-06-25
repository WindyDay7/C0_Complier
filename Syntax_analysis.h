#ifndef GRAMMER_ANALYSIS_H_INCLUDED
#define GRAMMER_ANALYSIS_H_INCLUDED

#include "Lexical_func.h"
#include <stdio.h>
#include <fstream>
using namespace std;

extern FILE *fp;

extern int Current_location;
extern int Count_line;
// extern string ident_name;		//全局变量，记录读到的标识符和字符串
extern char temp_ident[200];			//当前标识符的name
extern int number;					//表示读到的数字大小
extern char real_temp_char;
extern int data_address;
extern int part_start;
extern char current_function[20];
// extern string Empty_name;
extern int Gstn;
extern int Pstn;
extern char temp_symbol;
extern char line[200];

extern enumType ALL_symbol[5000];		//所有的标识符存起来
extern int symbol_count;

extern int Inter_Code_Line;
extern int temp_count;
extern int Label_count;

extern ofstream Syntax_Result;
extern ofstream Wrong;

void enter_constant(int int_or_char, int wh_global);
void enter_variable(int int_or_char, int wh_global);
void enter_function(int func_kind);
int enter_parameter(int int_or_char);			//登陆参数到局部变量信息表，只要是参数就是局部变量，
void deal_constant(int wh_global);		//处理常量定义，判断是全局的还是局部的
void deal_variable(int wh_global);			//处理变量说明,判断是全局的还是局部的
void deal_parameter();		//处理参数，形式参数
void deal_arguments();			//处理有返回值的函数调用，无返回值的函数调用语句一样
Expression_result deal_factor();			//处理因子
Expression_result deal_item();		//处理项
Expression_result deal_expression();		//处理表达式
void deal_condition();		//处理条件
void deal_statement_if();
void deal_statement_while();			//处理do_while语句
Expression_result deal_for_ident();
void deal_statement_for();
void deal_assignment();		//
void deal_statement_read();
void deal_statement_write();
void deal_statement_return();
void deal_statement();		//处理语句
void deal_statement_column();
void deal_function();
void deal_error(int error_num);

void new_MidLine(Intermediate_Code temp_line);
void output_Quaternary();


extern Intermediate_Code* Code_txt[4000];
extern String_code* sentence[200];
extern int sentence_count;
extern int Reserved;
extern int WITHER_WRONG;
extern int Current_line_length;

#endif // GRAMMER_ANALYSIS_H_INCLUDED
