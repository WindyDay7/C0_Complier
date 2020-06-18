#include <stdio.h>
#include "Lexical_func.h"
#include "Syntax_analysis.h"
#include "Global_variable.h"
#include "Translate_Mips.h"

int main()
{
	fp = fopen("test2.txt", "r");
	initialize();
	//next_symbol(fp);
	symbol = next_symbol(fp);
	symbol = next_symbol(fp);
	deal_constant(1);  		//处理全局的常量定义
	deal_variable(1);		//处理全局的变量定义,会往后读一个单词
	Reserved = data_address;
	// symbol = next_symbol(fp);
	deal_function();		//处理函数的声明
    output_Quaternary();
    Translate();
    fclose(fp);
}
