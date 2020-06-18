#include "Syntax_analysis.h"
#include "symbol_table.h"
#include <stdlib.h>
#include <iostream>
//#include <cstring.h>
#include <string.h>
using namespace std;
void enter_constant(int int_or_char, int wh_global)
{
	while (symbol != semicolon)
	{
		symbol = next_symbol(fp);		//读取标识符
		if (symbol != ident)
		{
			deal_error(1);
			return ;		//不是标识符的话，问题很大啊
		}
		else
		{
			symbol = next_symbol(fp);		//读取等号
			if (symbol != endow)
			{
				deal_error(2);
				return ;
			}
			else
			{
				symbol = next_symbol(fp);	//读取数字或者字母，
				if (int_or_char)		//如果是整数型常量
				{
					if (symbol != intcon && symbol != un_intcon)
					{
						deal_error(3);
						return ;
					}
					// 先读取一个标识符，然后读取一个等号，然后是数字，在读取等号与数字的时候均不改变temp_ident
					// 读取完数字之后number中存取数字的大小
					else
					{
						if (wh_global)		//如果是全局常量定义的话
						{
							if (look_Global(temp_ident))
							{
								deal_error(4);
								return ;			//全局常量重名
							}
							else
							{
								enter_Global_symbol_table(temp_ident, constant_kind, 1, 0, data_address, number, 0);
								data_address += 4;
							}
						}
						else 		//如果是局部常量定义的话
						{
							if (look_Partial(temp_ident, part_start))
							{
								deal_error(5);
								return ;   		//局部常量重名
							}
							else
							{
								enter_Partial_symbol_table(temp_ident, constant_kind, 1, 0, current_function, data_address, number, 0);
								data_address += 4;
							}
						}
					}
				}
				else  		//如果是字符型常量
				{
					if (symbol != charcon)
					{
						deal_error(6);
						return ;
					}
					// 先读取一个标识符，然后读取一个等号，然后是字符，在读取等号与数字的时候均不改变temp_ident
					// 读取完数字之后number中存取数字的大小
					else
					{
						if (wh_global)		//如果是全局常量定义的话
						{
							if (look_Global(temp_ident))
							{
								deal_error(4);
								return ;			//全局常量重名
							}
							else
							{
								enter_Global_symbol_table(temp_ident, constant_kind, 0, 0 , data_address, 0, real_temp_char);
								data_address += 4;
							}
						}
						else 		//如果是局部常量定义的话
						{
							if (look_Partial(temp_ident, part_start))
							{
								deal_error(5);
								return ;   		//局部常量重名
							}
							else
							{
								enter_Partial_symbol_table(temp_ident, constant_kind, 0, 0, current_function, data_address, 0, real_temp_char);
								data_address += 4;
							}
						}
					}
				}
			}
		}
		symbol = next_symbol(fp);	//读取逗号或者是分号。
		if (symbol != comma && symbol != semicolon)
		{
			deal_error(9);
			return ;		//常量声明的时候，末尾的分号不正确，或者中间的逗号不正确
		}
	}
}

void enter_variable(int int_or_char, int wh_global)
{
	while (symbol != semicolon)
	{
		symbol = next_symbol(fp);		//先读标识符
		if (symbol != ident)
		{
			deal_error(1);
			return ;	//不是标识符的话，问题很大啊，直接退出吧
		}
		symbol = next_symbol(fp);		//再读逗号，或者是'['，或者是'(',或者是分号
		if (symbol == l_parentheses && wh_global == 1)		//如果是左小括号，说明不是变量定义，而是函数声明,但这仅在全局变量声明的时候起作用
		{
			Current_location = 1;		//下一次读的时候从这一行的开头开始读
			temp_symbol = line[0];		//词法分析时候预读的一个
			break;				//这里是正常结束，不是错误结束
		}
		else if (symbol == comma || symbol == semicolon)
		{
			if (int_or_char)			//如果是整型变量
			{
				if (wh_global) 			//如果是全局变量
				{
					if (look_Global(temp_ident))
					{
						deal_error(4);
						return ;		//全局变量的名冲突
					}
					else
					{
						enter_Global_symbol_table(temp_ident, variable_kind, 1, 0, data_address, 0, 0);
						data_address += 4;		//整数型变量,登录全局变量符号表
					}
				}
				else 		//如果是局部变量
				{
					if (look_Partial(temp_ident, part_start))
					{
						deal_error(5);
						return ;   	//局部变量名冲突，
					}
					else
					{
						enter_Partial_symbol_table(temp_ident, variable_kind, 1, 0, current_function, data_address, 0, 0);
						data_address += 4;		//整型变量登录局部变量符号表
					}
				}
			}
			else    			//如果是字符型变量
			{
				if (wh_global)		//如果是全局变量
				{
					if (look_Global(temp_ident))
					{
						deal_error(4);
						return ;		//全局变量的名冲突
					}
					else
					{
						enter_Global_symbol_table(temp_ident, variable_kind, 0, 0, data_address, 0, 0);
						data_address += 4;		//字符型变量,登录全局变量符号表
					}
				}
				else  		//如果是局部变量
				{
					if (look_Partial(temp_ident, part_start))
					{
						deal_error(5);
						return ;   	//局部变量名冲突，
					}
					else
					{
						enter_Partial_symbol_table(temp_ident, variable_kind, 0, 0, current_function, data_address, 0, 0);
						data_address += 4;		//字符型变量登录局部变量符号表
					}
				}

			}
			continue;
		}
		else if (symbol == l_bracket)		//读到左中括号说明是数组
		{
			symbol = next_symbol(fp);		//读取数组的大小，也就是数字
			if (symbol != intcon  && symbol != un_intcon)
			{
				deal_error(7);
				return ;		//数组定义阶段，只能是用数字表示大小
			}
			symbol = next_symbol(fp);		//接下来读到右括号
			if (symbol != r_bracket)
			{
				deal_error(8);
				return ;			//数组的右括号不匹配
			}
			else 						//数组正确
			{
				if (int_or_char)		//如果是整数型数组
				{
					if (wh_global) 			//如果是全局数组变量
					{
						if (look_Global(temp_ident))
						{
							deal_error(4);
							return ;		//全局数组变量的名冲突
						}
						else
						{
							enter_Global_symbol_table(temp_ident, array_kind, 1, number, data_address, 0, 0);
							data_address += (4 * number);		//整数型数组变量,登录全局变量符号表
						}
					}
					else 		//如果是局部数组变量
					{
						if (look_Partial(temp_ident, part_start))
						{
							deal_error(5);
							return ;   	//局部变量名冲突，
						}
						else
						{
							enter_Partial_symbol_table(temp_ident, array_kind, 1, number, current_function, data_address, 0, 0);
							data_address += (4 * number);		//整型变量登录局部变量符号表
						}
					}
				}

				else  				//如果是字符型数组变量
				{
					if (wh_global) 			//如果是全局数组变量
					{
						if (look_Global(temp_ident))
						{
							deal_error(4);
							return ;		//全局数组变量的名冲突
						}
						else
						{
							if (number % 4 != 0)
							{
								number = number + 4 - number % 4;
							}
							enter_Global_symbol_table(temp_ident, array_kind, 0, number, data_address, 0, 0);
							data_address += number;		//字符型型数组变量,登录全局变量符号表
						}
					}
					else 		//如果是局部数组变量
					{
						if (look_Partial(temp_ident, part_start))
						{
							deal_error(5);
							return ;   	//局部变量名冲突，
						}
						else
						{
							if (number % 4 != 0)
							{
								number = number + 4 - number % 4;
							}
							enter_Partial_symbol_table(temp_ident, array_kind, 0, number, current_function, data_address, 0, 0);
							data_address += number;		//字符型变量登录局部变量符号表
						}
					}

				}
				symbol = next_symbol(fp);		//数组正确，读取下一个逗号，或者分号
			}
		}
		else if (symbol == semicolon)			//读到一行末尾是分号
		{
			break;
		}
		else
		{
			deal_error(9);
			return ;	//变量声明中出现未知符号，既不是',' 也不是 ';'
		}

	}
}

void enter_function(int func_kind)
{
	symbol = next_symbol(fp);		//读取函数名，登录符号表，
	if (symbol != ident && (symbol != main_sy || func_kind != 0))
	{
		deal_error(10);
		return ;		//函数名不是标识符，错误
	}
	else
	{
		if (look_Global(temp_ident))			//如果有函数名冲突，全局变量冲突
		{
			deal_error(4);
			return ;
		}
		else
		{
			strcpy(current_function, temp_ident);
			if (func_kind == 0)			//如果是void型函数
			{
				enter_Global_symbol_table(temp_ident, invalued_function_kind, 0, 0, Pstn, 0, 0);
				// 函数的address是他的第一个参数在局部符号表的位置
				// 只有调用函数的时候才会在运行栈为他开一片数据区，也就是遇到参数的话就会将其登陆到
			}
			else if (func_kind == 1)		//如果返回值是int类型
			{
				enter_Global_symbol_table(temp_ident, valued_function_kind, 1, 0, Pstn, 0, 0);
			}
			else if (func_kind == 2)
			{
				enter_Global_symbol_table(temp_ident, valued_function_kind, 0, 0, Pstn, 0, 0);
			}
		}
	}
}

int enter_parameter(int int_or_char)			//登陆参数到局部变量信息表，只要是参数就是局部变量，
{
	if (symbol != ident)
	{
		deal_error(1);		//参数不是标识符
		return 0;
	}
	if (look_Partial(temp_ident, part_start))
	{
		deal_error(5);
		return 0;		//局部变量信息表中，参数名冲突
	}
	else 		//局部变量信息表，参数不冲突
	{
		if (int_or_char)
		{
			enter_Partial_symbol_table(temp_ident, parameter_kind, 1, 0, current_function, data_address, 0, 0);
			data_address += 4;
			return Pstn - 1;
		}
		else 		//如果是字符类型
		{
			enter_Partial_symbol_table(temp_ident, parameter_kind, 0, 0, current_function, data_address, 0, 0);
			data_address += 4;
			return Pstn - 1;
		}
	}

}


void deal_constant(int wh_global)		//处理常量定义，判断是全局的还是局部的
{
	if (symbol == const_sy)			//如果读到的是const，表示常量定义，
	{
		while (symbol == const_sy)			//可以有很多个常量说明
		{
			symbol = next_symbol(fp);
			if (symbol == int_sy)
			{
				Syntax_Result << "This is Integer constant definition in line  " << Count_line << endl;
				enter_constant(1, wh_global);
			}
			else if (symbol == char_sy)
			{
				Syntax_Result << "This is Character type constant definition in line  " << Count_line << endl;
				enter_constant(0, wh_global);
			}
			else
			{
				while (symbol != semicolon)
				{
					symbol = next_symbol(fp);
				}
				deal_error(0);
				return ;	//如果const后面不是int型也不是char型，那么说明这一行有错误，忽视这一行
			}
			symbol = next_symbol(fp);		//继续向下读，可能是const，也可能不是，不是表示常量定义完了,常量定义预读了一个
		}
	}
}


void deal_variable(int wh_global)			//处理变量说明,判断是全局的还是局部的
{
	while (symbol == int_sy || symbol == char_sy)		//变量的不断声明
	{
		if (symbol == int_sy)
		{
			enter_variable(1, wh_global);			//这里1表示的是整型数据，0表示字符型数据
			if (symbol == l_parentheses)
			{
				symbol = next_symbol(fp);		//表示遇到的是函数声明而不是变量声明，读函数开头的部分
				break;
			}
			Syntax_Result << "This is an integer variable definition in line  " << Count_line << endl;
		}
		else if (symbol == char_sy)			//字符型变量
		{
			enter_variable(0, wh_global);
			if (symbol == l_parentheses)
			{
				symbol = next_symbol(fp);		//表示遇到的是函数声明而不是变量声明，读函数开头的部分
				break;
			}
			Syntax_Result << "This is an Character type variable definition in line  " << Count_line << endl;
		}
		symbol = next_symbol(fp);			//登录一行的时候，一直读到分号为止，所以再往后读一个
	}
}


void deal_parameter()		//处理参数，形式参数
{
	int parameter_count = 0, para_location = 0;
	Intermediate_Code temp_Code;		//临时行，参数声明
	Syntax_Result << "Processing function parameters in line  " << Count_line << endl;
	while (symbol != r_parentheses)		//参数表，，右括号表示结束
	{
		symbol = next_symbol(fp);	//先读一个类型标识符，int或者char
		if (symbol == int_sy)
		{
			symbol = next_symbol(fp);
			para_location = enter_parameter(1);			//登陆参数符号表
			parameter_count += 1;       //参数的个数加一个
			temp_Code.Code_type = para_declare;
			temp_Code.number1_lc = para_location;
			temp_Code.global_1 = 0;
			strcpy(temp_Code.Label, "para");
			new_MidLine(temp_Code);
			//新开一行，存储这个变量声明
			symbol = next_symbol(fp);
			if (symbol != comma && symbol != r_parentheses)
			{
				deal_error(15);
				return ;		//标识符之间不是用逗号分割的,且不是右括号
			}
		}
		else if (symbol == char_sy)
		{
			symbol = next_symbol(fp);
			para_location = enter_parameter(0);
			parameter_count += 1;       //参数的个数加一个
			temp_Code.Code_type = para_declare;
			temp_Code.number1_lc = para_location;
			temp_Code.global_1 = 0;		//参数都是局部变量
			strcpy(temp_Code.Label, "para");
			new_MidLine(temp_Code);
			//新开一行，存储这个变量声明
			symbol = next_symbol(fp);
			if (symbol != comma && symbol != r_parentheses)
			{
				deal_error(15);
				return ;		//标识符之间的逗号错误。
			}
		}
		else if (symbol == r_parentheses)		//如果既不是int也不是char，而是空
		{
			break;
		}
		else 		//表示参数列表错误
		{
			deal_error(0);
			return ;		//标识符的类型错误，不是int也不是char
		}
	}
	//处理形式参数结束的时候，需要将参数的个数存储到该函数的符号表中
	Global_symbol_table[look_Global(current_function)]->length = parameter_count;

}

void deal_arguments()			//处理有返回值的函数调用，无返回值的函数调用语句一样
{
	Intermediate_Code temp_Code;		//定义一个临时行
	int function_location = 0, argument_kind = 0, argument_count = 0;
	Expression_result temp_result;
	int parameter_location = 0;
	function_location = look_Global(temp_ident);		//这个返回值是函数在全局符号表的位置
	parameter_location = Global_symbol_table[function_location]->address;   //该函数的第一个参数在局部符号表的位置
	symbol = next_symbol(fp);	//对于有返回值的函数调用，需要看后面是不是')'
	while (symbol != r_parentheses)		//遇到右括号表示参数处理完了，如果一开始就读到右括号，表示没有实参
	{
		temp_result = deal_expression();			//这里已经预读了表达式里面的一个单词

		//递归处理表达式，返回表达式的结果          //处理完表达式要生成中间代码
		Code_txt[Inter_Code_Line] = (Intermediate_Code*)malloc(sizeof(Intermediate_Code));      //新开一行，存储四元式
		Code_txt[Inter_Code_Line]->number1_lc = temp_result.temp_local;
		Code_txt[Inter_Code_Line]->global_1 = temp_result.un_partial;
		Code_txt[Inter_Code_Line]->number2_lc = function_location;
		Code_txt[Inter_Code_Line]->global_2 = 1;
		Code_txt[Inter_Code_Line]->Code_type = push;
		Inter_Code_Line += 1;       //行数加1
		argument_count += 1;
		if (Partial_symbol_table[parameter_location]->data_type != temp_result.result_data_kind)	//在局部符号表中查找参数类型，
		{
			deal_error(27);
			return ;
		}
		// 表达式处理结束之后已经预读了下一个单词
		if (symbol != comma && symbol != r_parentheses)
		{
			deal_error(15);
			return ; 		//函数调用的时候，实参那里括号和逗号不对
		}
		else if (symbol == r_parentheses)
		{
			break;
		}
		parameter_location += 1;    //局部参数往后推一位
		symbol = next_symbol(fp);	//对于有返回值的函数调用，需要看后面是不是')'
	}
	temp_Code.Code_type = funct_call;
	strcpy(temp_Code.Label, Global_symbol_table[function_location]->name);
	temp_Code.number1_lc = function_location;
	new_MidLine(temp_Code);
	//call  function  函数调用四元式
	if (argument_count != Global_symbol_table[function_location]->length)
	{
		deal_error(30);
		return ;
	}
}




Expression_result deal_factor()
{
	// 因子已经预读了一个
	Expression_result result = {0};
	Intermediate_Code temp_Code = {};	//如果是数组或者时函数调用的话，要定义一个临时的Code,多一行代码
	if (symbol == l_parentheses)		//如果是左括号，表达式是因子
	{
		symbol = next_symbol(fp);		//读取表达式的开头
		result = deal_expression();				//表达式结束之后已经预读了下一个单词
		if (symbol != r_parentheses)
		{
			deal_error(16);
			return result;			//因子中的表达式处理不正确，缺失右小括号
		}
		symbol = next_symbol(fp);		//预读因子后面的单词
		// result->un_partial = 2;		//如果是表达式，那么第一个是临时变量
	}
	else if (symbol == ident)		//如果是标识符，那么就要去查符号表
	{
		if (!look_Partial(temp_ident, part_start))		//如果局部变量符号表未找到
		{
			if (!look_Global(temp_ident))			//如果全局符号表也没找到
			{
				deal_error(18);
				return result;		//未定义的标识符，找不到
			}
			result.temp_local = look_Global(temp_ident);
			result.un_partial = 1;		//表示全局变量
		}
		if (result.un_partial != 1)		//如果不是全局变量
		{
			result.temp_local = look_Partial(temp_ident, part_start);		//位置也知道了
			result.un_partial = 0;		//表示是局部变量

		}

		result.result_data_kind = ident_data_kind;
		symbol = next_symbol(fp);	//对于标识符，要向后读一个，看是不是数组
		if (symbol == l_bracket)	//标识符后面是左中括号，表示是数组
		{
			Expression_result temp_result;
			temp_Code.number2_lc = result.temp_local;		//第二个数字时数组标识符，即
			temp_Code.global_2 = result.un_partial;		//是否是全局变量

			symbol = next_symbol(fp);		//表达式开始之前也预读下一个单词
			temp_result = deal_expression();		//表达式结束之后已经预读了下一个单词

			temp_Code.number3_lc = temp_result.temp_local;
			temp_Code.global_3 = temp_result.un_partial;		//表达式的返回值作为第三个操作数
			temp_count += 1;            //不是处理完表达式，temp_count就加1，而是在使用temp_count之前加1
			temp_Code.number1_lc = temp_count;
			temp_Code.global_1 = 2;			//数组提取的第一个操作数，结果
			temp_Code.Code_type = number_array;		//获取数组的类型

			new_MidLine(temp_Code);	//新增一行，存到中间代码行中
			if (symbol != r_bracket)		//如果这个单词不是右中括号
			{
				deal_error(8);
				return result;			//数组在因子中的使用不正确
			}
			else 		//数组处理正确
			{
				symbol = next_symbol(fp);	//预读下一个字符
			}
			result.un_partial = 2;			//这时需要一个临时变量,表示取数组
			result.temp_local = temp_count;    //临时变量的标号
		}
		else if (symbol == l_parentheses)		//如果标识符后面是左小括号，表示有返回值的函数调用
		{
			if (!look_Partial(temp_ident, part_start) && look_Global(temp_ident) && Global_symbol_table[look_Global(temp_ident)]->type_information == valued_function_kind) //如果是左括号，并且是全局变量中的标识符，才是函数调用
			{
				deal_arguments();		//处理有返回值的函数调用的实参的格式
				symbol = next_symbol(fp);	//预读下一个字符
				temp_count += 1;
				temp_Code.number1_lc = temp_count;
				temp_Code.global_1 = 2;            //表示是临时变量，
				/*   i = RET  */
				temp_Code.Code_type = funct_result;
				new_MidLine(temp_Code);         //新的一行，
				result.temp_local = temp_count;
			}
			else
			{
				deal_error(29);
				return result;
			}
			result.un_partial = 2;		//表示函数调用，需要左括号，所以是临时变量
		}
	}
	else if (symbol == intcon || symbol == un_intcon)
	{
		result.result_data_kind = 1;
		// return true;  		//因子是一个int型整数
		result.un_partial = 3;		//表示就是数字，不是其他任何东西
		result.temp_local = number;
		symbol = next_symbol(fp);	//预读下一个字符
	}
	else if (symbol == charcon)
	{
		symbol = next_symbol(fp);	//预读下一个字符
		result.result_data_kind = 0;
		// return true;		//因子是一个字符
		result.un_partial = 3;
		result.temp_local = real_temp_char;		//强制类型转换，将字符转换为数字进行计算
	}
	else
	{
		symbol = next_symbol(fp);	//预读下一个字符
		deal_error(19);
		return result;		//因子中出现未识别的单词，错误
	}
	return result;
}


Expression_result deal_item()		//处理项
{
	Expression_result result = {0};
	Intermediate_Code temp_Code = {};			//定义一个临时行
	int mult_flag = 0;
	result = deal_factor();  //返回已知变量，还是临时变量
	// symbol = next_symbol(fp);	//项结束的标志的是加法运算符，

	while (symbol == multiply || symbol == division)
	{
		if (symbol == multiply)
		{
			temp_Code.operate_cal = 3;
		}
		else
		{
			temp_Code.operate_cal = 4;
		}
		temp_Code.Code_type = expression;
		//先记录一些关于四元式的属性
		if (mult_flag == 0)
		{
			temp_Code.number2_lc = result.temp_local;
			temp_Code.global_2 = result.un_partial;
		}
		else
		{
			temp_Code.number2_lc = temp_count;
			temp_Code.global_2 = 2;
		}
		symbol = next_symbol(fp);	//读因子
		result = deal_factor();
		temp_Code.number3_lc = result.temp_local;
		temp_Code.global_3 = result.un_partial;
		// 处理第三个操作数
		temp_count += 1;
		temp_Code.number1_lc = temp_count;		//第一个操作数就必须是临时变量
		temp_Code.global_1 = 2;		//表示是临时变量
		new_MidLine(temp_Code);
		mult_flag = 1;
	}
	if (mult_flag == 1)
	{
		result.result_data_kind = 1;	//数据类型肯定是int型了
		result.un_partial = 2;		//表示临时变量
		result.temp_local = temp_count;	//临时变量的序号
	}
	return result;
}


void new_MidLine(Intermediate_Code temp_line)
{
	Code_txt[Inter_Code_Line] = (Intermediate_Code*)malloc(sizeof(Intermediate_Code));
	(*Code_txt[Inter_Code_Line]) = temp_line;
	Inter_Code_Line += 1;		//这一行确定了
}

Expression_result deal_expression()		//处理表达式
{
	Expression_result result = {0};
	Intermediate_Code temp_Code = {};		//定义一个临时行
	int plus_flag = 0, minus_flag = 0;
	int left_number = 0, right_number = 0, operate = 0;		//保存表达式
	if (symbol == plus_sign)
	{
		symbol = next_symbol(fp);	//读因子
		result.result_data_kind = 1;
	}
	else if (symbol == minus_sign)		//如果是减号，要生成一个四元式表示减号，取反的四元式
	{
		symbol = next_symbol(fp);	//读因子
		minus_flag = 1;		//表明减号，且减号只对第一项有效
	}
	/****************************************************************************************************************************/

	/****************************************************************************************************************************/
	result = deal_item();		//处理一个项
	if (minus_flag == 1)
	{
		Code_txt[Inter_Code_Line] = (Intermediate_Code*)malloc(sizeof(Intermediate_Code));
		Code_txt[Inter_Code_Line]->number2_lc = result.temp_local;
		Code_txt[Inter_Code_Line]->global_2 = result.un_partial;		//首先记录第二项
		Code_txt[Inter_Code_Line]->operate_cal = 5;			//5，表示取反
		temp_count += 1;
		Code_txt[Inter_Code_Line]->number1_lc = temp_count;
		Code_txt[Inter_Code_Line]->global_1 = 2;
		Code_txt[Inter_Code_Line]->Code_type = Negate;		//表示是取反类型
		Inter_Code_Line += 1;
		plus_flag = 1;
	}
	while (symbol == plus_sign || symbol == minus_sign)
	{
		if (symbol == plus_sign)
		{
			temp_Code.operate_cal = 1;
		}
		else
		{
			temp_Code.operate_cal = 2;		//表示算术类型，加法还是减法
		}
		temp_Code.Code_type = expression;
		// 先记录一些关于四元式的属性
		if (plus_flag == 0)
		{
			temp_Code.number2_lc = result.temp_local;
			temp_Code.global_2 = result.un_partial;
		}
		else
		{
			temp_Code.number2_lc = temp_count;
			temp_Code.global_2 = 2;
		}
		symbol = next_symbol(fp);
		result = deal_item();
		temp_Code.number3_lc = result.temp_local;
		temp_Code.global_3 = result.un_partial;
		//四元式的第三项是项返回的结果
		temp_count += 1;
		temp_Code.number1_lc = temp_count;		//第一个操作数就必须是临时变量
		temp_Code.global_1 = 2;		//表示是临时变量
		new_MidLine(temp_Code);
		plus_flag = 1;
	}
	if (plus_flag == 1)
	{
		result.result_data_kind = 1;	//数据类型肯定是int型了
		result.un_partial = 2;		//表示临时变量
		result.temp_local = temp_count;	//临时变量的序号
	}
	return result;
}


void deal_condition()			//处理条件, 处理条件后面一定是跳转，否则处理条件有什么意义
{
	Expression_result result_1 = {0}, result_2 = {0};
	Intermediate_Code temp_Code = {};		//一个条件可以作为一行
	enumType temp_symbol = wrong_sy;
	symbol = next_symbol(fp);		//表达式之前先读表达式中的一个字符
	result_1 = deal_expression();		//处理表达式,处理表达式后会预读下一个字符
	// beq, bne, bge, bgt, ble, blt,
	if (symbol == beq || symbol == bne || symbol == bge || symbol == bgt || symbol == ble || symbol == blt)
	{
		temp_symbol = symbol;
		symbol = next_symbol(fp);		//有关系运算符的话，继续处理表达式
		result_2 = deal_expression();		//处理表达式,处理表达式后会预读下一个字符
	}
	temp_Code.number2_lc = result_1.temp_local;
	temp_Code.global_2 = result_1.un_partial;
	temp_Code.number3_lc = result_2.temp_local;
	temp_Code.global_3 = result_2.un_partial;
	temp_Code.Code_type = Con_judge;			//表示是条件类型，比较类型，就是seq，相等则置于
	switch (temp_symbol)
	{
	case beq: temp_Code.operate_cal = 7; break;
	case bne: temp_Code.operate_cal = 8; break;
	case bge: temp_Code.operate_cal = 9; break;
	case bgt: temp_Code.operate_cal = 10; break;
	case ble: temp_Code.operate_cal = 11; break;
	case blt: temp_Code.operate_cal = 12; break;
	case wrong_sy: temp_Code.operate_cal = 13; break;		//表示不等于0跳转，表示条件之哟一个表达式
	}
	new_MidLine(temp_Code);
	// 不管这里有没有关系运算符，后面都会预读下一个字符
}

void deal_statement_if()
{
	Intermediate_Code temp_Code1 = {}, temp_Code2 = {}, temp_Code3 = {}, temp_Code4 = {};		//if中的跳转语句,可能有两个，
	char Label_num[10] = {0}, Label_num_2[10] = {0}, Label_num_3[10] = {0}, Label_num_4[10] = {0};
	Syntax_Result << "This is an if statement in line  " << Count_line << endl;
	symbol = next_symbol(fp);	//首先读到左小括号
	if (symbol != l_parentheses)
	{
		deal_error(13);
		return ;		//if语句中左边的小括号错误，
	}
	else 		//if语句中括号正确，开始处理条件
	{
		deal_condition();		//处理条件，会预读下一个字符, 条件之后一定处理跳转，这里是不满足跳转
		temp_Code1.Code_type = BZ;			//不满足条件跳转
		strcpy(temp_Code1.Label, "Label");
		sprintf(Label_num, "%d", Label_count);		//生成Label

		strcat(temp_Code1.Label, Label_num);
		strcpy(Label_num_2, temp_Code1.Label);
		strcat(Label_num_2, ":");
		Label_count += 1;
		new_MidLine(temp_Code1);		//跳转的那一行
		if (symbol != r_parentheses)
		{
			deal_error(16);
			return ;		//if语句中条件那里错误，')'错误，右小括号错误
		}
		else
		{
			symbol = next_symbol(fp);			//处理语句的时候要预读语句中的一个单词
			deal_statement();		//如果正确，条件格式正确，处理if之后的语句
			if(symbol == else_sy)   //如果有else语句，那么就要生成一条跳转到else语句末尾的指令
            {
                temp_Code3.Code_type = GOTO;
                strcpy(temp_Code3.Label, "Label");
                sprintf(Label_num_3, "%d", Label_count);
                strcat(temp_Code3.Label, Label_num_3);
                strcpy(Label_num_4, temp_Code3.Label);
                strcat(Label_num_4, ":");
                Label_count += 1;
                new_MidLine(temp_Code3);
            }
			strcpy(temp_Code2.Label, Label_num_2);	//第二个标签
			temp_Code2.Code_type = With_Label;		//表示是标签类型，就是一个标签
			new_MidLine(temp_Code2);
			if (symbol == else_sy)			//处理语句的时候，会预读下一条语句的开头
			{
				symbol = next_symbol(fp);			//处理语句的时候要预读语句中的一个单词
				deal_statement();		//如果是else，继续处理语句
				strcpy(temp_Code4.Label, Label_num_4);
				temp_Code4.Code_type = With_Label;
				new_MidLine(temp_Code4);
			}
		}
	}

}

void deal_statement_while()			//处理do_while语句
{

	Intermediate_Code temp_Code1 = {}, temp_Code2 = {};		//if中的跳转语句,可能有两个，
	char temp_label1[10] = {0}, temp_label2[10] = {0};

	strcpy(temp_Code1.Label, "Label");
	sprintf(temp_label1, "%d", Label_count);
	Label_count += 1;
	strcat(temp_Code1.Label, temp_label1);		//将
	strcpy(temp_label2, temp_Code1.Label);
	strcat(temp_Code1.Label, ":");    //	拼成一个标签四元式
	temp_Code1.Code_type = With_Label;
	new_MidLine(temp_Code1);

	Syntax_Result << "This is a do___while statement in line  " << Count_line << endl;
	symbol = next_symbol(fp);			//处理语句的时候要预读语句中的一个单词
	deal_statement();			//读到while语句的时候，开始处理do后面的语句
	//deal_statement会预读下一个单词
	if (symbol != while_sy)
	{
		deal_error(17);
		return ;		//do__while语句中，后面的那个while错误
	}
	symbol = next_symbol(fp);		//while之后继续读一个
	if (symbol != l_parentheses)
	{
		deal_error(13);
		return ;		//do__while语句中的条件那里的括号错误
	}
	else
	{
		deal_condition();		//处理条件，会预读下一个单词
		temp_Code2.Code_type = BNZ;		//满足条件跳转
		strcpy(temp_Code2.Label, temp_label2);
		new_MidLine(temp_Code2);

	}
	if (symbol != r_parentheses)
	{
		deal_error(16);
		return ;		//如果条件结束之后不是')',右小括号，那么说明错误
	}
}

Expression_result deal_for_ident()				//这里int返回的是for中记录变量在符号表的位置
{
	Expression_result temp_for_result = {0};
	int for_ident_number = 0;
	char for_ident_line[10][20] = {0};
	while (temp_symbol != '\n')
	{
		symbol = next_symbol(fp);
		if (symbol == ident)
		{
			strcpy(for_ident_line[for_ident_number], temp_ident);
			for_ident_number += 1;
		}
	}
	if (for_ident_number < 3)
	{
		deal_error(25);
		temp_for_result.temp_local = 0;
		return temp_for_result;
	}
	else
	{
		if (strcmp(for_ident_line[0], for_ident_line[for_ident_number - 1]) != 0)
		{
			deal_error(25);
			temp_for_result.temp_local = 0;
			return temp_for_result;
		}
		if (strcmp(for_ident_line[0], for_ident_line[for_ident_number - 2]) != 0)
		{
			deal_error(25);
			temp_for_result.temp_local = 0;
			return temp_for_result;
		}
	}
	if (!look_Partial(for_ident_line[0], part_start))		//如果局部变量符号表未找到
	{
		if (!look_Global(for_ident_line[0]))			//如果全局符号表也没找到
		{
			deal_error(18);
			return temp_for_result;		//未定义的标识符，找不到
		}
		temp_for_result.temp_local = look_Global(for_ident_line[0]);
		temp_for_result.un_partial = 1;
	}
	if (temp_for_result.un_partial == 0)
	{
		temp_for_result.un_partial = 0;
		temp_for_result.temp_local = look_Partial(for_ident_line[0], part_start);
	}
	if (ident_data_kind != 1)
	{
		deal_error(28);
		temp_for_result.temp_local = 0;
		return temp_for_result;
	}
	Current_location = 1;		//下一次读的时候从这一行的开头开始读
	temp_symbol = line[0];		//词法分析时候预读的一个
	symbol = next_symbol(fp);
	return temp_for_result;
}

void deal_statement_for()
{
	Expression_result for_result1 = {0}, for_result2 = {0}, for_result3 = {0};
	Intermediate_Code temp_Code1 = {}, temp_Code2 = {}, temp_Code3 = {}, temp_Code4 = {}, \
	                               temp_Code5 = {}, temp_Code6 = {};

	char for_label1[10] = {0}, for_label2[10] = {0}, for_label3 = {0}, for_label4 = {0};
	Syntax_Result << "This is a for statement in line  " << Count_line << endl;
	int plus_flag = 0, change_num = 0;
	for_result1 = deal_for_ident();
	if (for_result1.temp_local == 0)
	{
		return ;
	}
	symbol = next_symbol(fp);		//读取for语句的第一个左小括号'('
	if (symbol != l_parentheses)
	{
		deal_error(13);
		return ; 		//for后面的那个小括号不正确
	}
	else
	{
		symbol = next_symbol(fp);		//读标识符
		if (symbol != ident)
		{
			deal_error(1);
			return ;		//for语句中的那个标识符错误
		}
		else
		{
			symbol = next_symbol(fp);		//读等号
			if (symbol != endow)
			{
				deal_error(2);
				return ; 		//for语句中的那个等号错误
			}
			else
			{
				//for开始的一句，生成赋值语句，
				temp_Code1.number1_lc = for_result1.temp_local;
				temp_Code1.global_1 = for_result1.un_partial;
				symbol = next_symbol(fp);
				for_result2 = deal_expression();		//表达式处理会预读下一个单词
				temp_Code1.number2_lc = for_result2.temp_local;
				temp_Code1.global_2 = for_result2.un_partial;
				temp_Code1.operate_cal = 6;
				temp_Code1.Code_type = expression;
				new_MidLine(temp_Code1);		//for语句的第一句相当于一个赋值语句，
				if (symbol != semicolon)
				{
					deal_error(23);
					return ;		//for语句表达式后面不是分号错误';'
				}
				else
				{
					temp_Code2.Code_type = With_Label;
					strcpy(temp_Code2.Label, "Label");
					sprintf(for_label1, "%d", Label_count);
					strcat(temp_Code2.Label , for_label1);
					strcat(temp_Code2.Label, ":");
					Label_count += 1;
					new_MidLine(temp_Code2);
					// 条件判断之前，有一个标签，会跳到这里
					deal_condition();		//
					//判断语句后面会生成一个跳转，跳转到for结尾, 不满足条件跳转
					temp_Code3.Code_type = BZ;
					strcpy(temp_Code3.Label, "Label");
					sprintf(for_label2, "%d", Label_count);
					strcat(temp_Code3.Label, for_label2);
					new_MidLine(temp_Code3);
					Label_count += 1;


					if (symbol != semicolon)
					{
						deal_error(23);
						return ; 		//条件后面的分号不正确
					}
					else
					{

						symbol = next_symbol(fp);		//读分号后面的标识符
						if (symbol != ident)
						{
							deal_error(1);
							return ;
						}
						else
						{
							symbol = next_symbol(fp);		//读取等号，表示在for循环中的复制语句
							if (symbol != endow)			//等号错误，语句中的等号错误
							{
								deal_error(20);
								return ;
							}
							else
							{
								symbol = next_symbol(fp);		//读标识符，表示在for循环中的变量
								if (symbol != ident)
								{
									deal_error(1);
									return ;
								}
								else
								{
									symbol = next_symbol(fp);			//读加号或者减号，也就是改变的步长的，那个是加还是减
									if (symbol == plus_sign || symbol == minus_sign)
									{
										if (symbol == plus_sign)
										{
											plus_flag = 1;
										}
										symbol = next_symbol(fp);		//继续往下读
										if (symbol == un_intcon)
										{
											change_num = number;
											symbol = next_symbol(fp);		//读取步长后面的右小括号')'
											if (symbol != r_parentheses)
											{
												deal_error(16);
												return ;		//for后面的右小括号错误
											}
											else
											{
												symbol = next_symbol(fp);			//处理语句的时候要预读语句中的一个单词
												deal_statement();		//递归处理语句
												// for语句先处理循环体，再改变标识符
												temp_Code4.Code_type = expression;
												temp_Code4.number1_lc = for_result1.temp_local;
												temp_Code4.global_1 = for_result1.un_partial;
												temp_Code4.number2_lc = for_result1.temp_local;
												temp_Code4.global_2 = for_result1.un_partial;
												temp_Code4.operate_cal = (plus_flag == 1 ? 1 : 2);
												temp_Code4.number3_lc = change_num;
												temp_Code4.global_3 = 3;		//第三个数字就是数字
												new_MidLine(temp_Code4);
												//先i++，然后跳转到condition判断；
												temp_Code5.Code_type = GOTO;
												temp_Code2.Label[strlen(temp_Code2.Label) - 1] = 0;
												strcpy(temp_Code5.Label, temp_Code2.Label);
												new_MidLine(temp_Code5);

												// for循环结束，跳出循环的label
												temp_Code6.Code_type = With_Label;
												strcpy(temp_Code6.Label, temp_Code3.Label);
												strcat(temp_Code6.Label, ":");		//生成最后一个标签，
												new_MidLine(temp_Code6);
											}
										}
										else
										{
											deal_error(22);
											return ;		//步长不是无符号整数错误
										}
									}
									else
									{
										deal_error(21);
										return ;		//for循环中读到的'+' 与 '-'不正确
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void deal_assignment()		//
{
	Expression_result result = {0}, result_2 = {0};
	Intermediate_Code temp_Code = {};		//数组赋值的代码，a[expression] = expression
	int left_location = 0;
	int left_data_kind = 0, right_data_kind = 0;
	if (look_Partial(temp_ident, part_start))
	{
		temp_Code.number1_lc = look_Partial(temp_ident, part_start);	//找出被赋值的符号在符号表的位置
		temp_Code.global_1 = 0;
	}
	else
	{
		temp_Code.number1_lc = look_Global(temp_ident);
		temp_Code.global_1 = 1;
	}
	left_data_kind = ident_data_kind;		//找打的数据类型
	Syntax_Result << "This is an assignment statement in line  " << Count_line << endl;
	if (symbol == endow)		//如果标识符后面直接接等号，
	{
		symbol = next_symbol(fp);		//处理表达式
		result = deal_expression();		//获取右边表达式的数据类型
		if (left_data_kind == 0 && result.result_data_kind == 1)
		{
			deal_error(26);				//左边是字符型，表达式是整数型，错误
			return ;
		}
		if (symbol != semicolon)		//表达式处理完，预读了表达后面的分号
		{
			deal_error(23);
			return ;
		}
		temp_Code.number2_lc = result.temp_local;
		temp_Code.global_2 = result.un_partial;
		temp_Code.Code_type = expression;
	}
	else if (symbol == l_bracket)			//如果是数组赋值，
	{
		symbol = next_symbol(fp);	//左中括号，说明是数组，
		result = deal_expression();
		if (symbol != r_bracket)		//数组右中括号错误
		{
			deal_error(8);
			return ;
		}
		else
		{
			symbol = next_symbol(fp);		//读取中括号后面的等号
			if (symbol != endow)
			{
				deal_error(20);
				return ;
			}
			else 					//如果数组括号后面的等号正确
			{
				symbol = next_symbol(fp);	//处理表达式
				result_2 = deal_expression();
				if (left_data_kind == 0 && result_2.result_data_kind == 1)
				{
					deal_error(26);
					return ;
				}
				if (symbol != semicolon)
				{
					deal_error(23);
					return ;			//最后的分号不正确
				}
			}
		}
		temp_Code.number2_lc = result.temp_local;
		temp_Code.global_2 = result.un_partial;
		temp_Code.number3_lc = result_2.temp_local;
		temp_Code.global_3 = result_2.un_partial;
		temp_Code.Code_type = array_number;
	}

	temp_Code.operate_cal = 6;		//6表示直接赋值
	new_MidLine(temp_Code);
}

void deal_statement_read()
{
	Expression_result result = {0};
	Intermediate_Code temp_Code = {};		//数组赋值的代码，a[expression] = expression
	Syntax_Result << "This is a read statement in line  " << Count_line << endl;
	symbol = next_symbol(fp);
	if (symbol != l_parentheses)
	{
		deal_error(13);
		return ;		//scanf后面紧跟左小括号,没右小括号报错
	}
	else
	{
		while (symbol != r_parentheses)
		{
			symbol = next_symbol(fp);		//读一个标识符
			if (symbol != ident)			//标识符类型错误
			{
				deal_error(1);
				return ;
			}
			else 						//标识符类型正确
			{
				if (!look_Partial(temp_ident, part_start))		//如果局部变量符号表未找到
				{
					if (!look_Global(temp_ident))			//如果全局符号表也没找到
					{
						deal_error(18);
						return ;		//未定义的标识符，找不到
					}
					result.un_partial = 1;
					result.temp_local = look_Global(temp_ident);
					result.result_data_kind = ident_data_kind;
				}
				if (result.un_partial == 0)
				{
					result.un_partial = 0;
					result.temp_local = look_Partial(temp_ident, part_start);
					result.result_data_kind = ident_data_kind;
				}
				temp_Code.number1_lc = result.temp_local;
				temp_Code.global_1 = result.un_partial;
				temp_Code.operate_cal = result.result_data_kind;
				temp_Code.Code_type = read_code;
				new_MidLine(temp_Code);
				symbol = next_symbol(fp);		//读标识符后面的单词，是逗号，或者是右小括号
				if (symbol != comma && symbol != r_parentheses)
				{
					deal_error(15);
					return ;		//错误符号
				}
			}
		}
		symbol = next_symbol(fp);		//读到右小括号还要往后读，判断是否是分号
		if (symbol != semicolon)
		{
			deal_error(23);
			return ;
		}
	}
}

void deal_statement_write()
{
	Expression_result result = {0};
	Intermediate_Code temp_Code1 = {}, temp_Code2 = {};		//生产写语句，使用syscall
	Syntax_Result << "This is a write statement in line  " << Count_line << endl;
	symbol = next_symbol(fp);
	if (symbol != l_parentheses)
	{
		deal_error(13);
		return ;		//printf的第一个左小括号就不正确
	}
	else 			//printf括号正确
	{
		symbol = next_symbol(fp);
		if (symbol == stringcon)			//如果读到字符串
		{
			symbol = next_symbol(fp);		//继续向下读
			//读字符串，生成读字符串的代码
			//读到字符串代码后，需要获取字符串的首地址

			temp_Code1.Code_type = write_code;			//表示写字符串
			strcpy(temp_Code1.Label, sentence[sentence_count - 1]->string_name);			//字符串的首地址
			temp_Code1.operate_cal = 14;			//表示写字符串
			new_MidLine(temp_Code1);

			if (symbol == r_parentheses)		//如果字符串后面就直接读到右括号，输出
			{
				symbol = next_symbol(fp);		//读到右括号后面读分号
				if (symbol != semicolon)
				{
					deal_error(23);
					return ;			//分号错误
				}

			}
			else if (symbol == comma)		//字符串后面是逗号，处理表达式
			{
				symbol = next_symbol(fp);		//表达式预读表达式里面的一个单词
				result = deal_expression();			//读表达式，printf(表达式)
				temp_Code2.Code_type = write_code;		//写表达式语句
				temp_Code2.number1_lc = result.temp_local;
				temp_Code2.global_1 = result.un_partial;
				temp_Code2.operate_cal = result.result_data_kind;		//表示输出的是整数还是单个字符
				new_MidLine(temp_Code2);
				if (symbol != r_parentheses)
				{
					deal_error(16);
					return ;		//处理表达式会预读一个单词，不是右小括号报错
				}
				else
				{
					symbol = next_symbol(fp);		//右小括号后面还要读一个分号
					if (symbol != semicolon)
					{
						deal_error(23);
						return ;			//分号错误
					}

				}
			}
		}
		else
		{
			// symbol = next_symbol(fp);
			result = deal_expression();		//这个处理表达式之前不需要读一个symbol，因为这里存在预读
			temp_Code2.Code_type = write_code;		//写表达式语句
			temp_Code2.number1_lc = result.temp_local;
			temp_Code2.global_1 = result.un_partial;
			temp_Code2.operate_cal = result.result_data_kind;		//表示输出的是整数还是单个字符
			new_MidLine(temp_Code2);
			if (symbol != r_parentheses)
			{
				deal_error(16);
				return ;
			}
			else
			{
				symbol = next_symbol(fp);	//括号读完，读后面的分号
				if (symbol != semicolon)
				{
					deal_error(23);
					return ;		//分号错误
				}

			}
		}
	}
}

void deal_statement_return()
{
	int return_location = 0;
	Expression_result result = {0};		//return语句中表达式返回值
	Intermediate_Code temp_Code = {};		//存储四元式的临时行
	Syntax_Result << "This is a return statement in line  " << Count_line << endl;
	return_location = look_Global(current_function);
	symbol = next_symbol(fp);
	if (symbol == semicolon)
	{
		if (Global_symbol_table[return_location]->type_information == valued_function_kind)
		{
			deal_error(33);         //有返回值的函数，不能啥也不返回
			return ;
		}
		temp_Code.number1_lc = -1;
		temp_Code.global_1 = 2;
		temp_Code.number2_lc = return_location;
		temp_Code.global_2 = 1;
		temp_Code.Code_type = funct_return;
		new_MidLine(temp_Code);
		return ;		//什么都没有的返回语句
	}
	else if (symbol == l_parentheses)		//如果是左小括号，说明返回表达式
	{
		symbol = next_symbol(fp);		//表达式预读
		result = deal_expression();			//处理表达式，会预读后面一个
		if (symbol != r_parentheses)		//读表达式后面的括号
		{
			deal_error(16);
			return ;		//表达式后面不是右小括号，错误
		}
		else
		{
			if (Global_symbol_table[return_location]->data_type != result.result_data_kind)
			{
				deal_error(32);
				return ;
			}
			else
			{
				if (Global_symbol_table[return_location]->type_information == invalued_function_kind)
				{
					deal_error(34);         //无返回值函数返回数值了，错误
				}
				else
				{
					symbol = next_symbol(fp);		//读括号后面的分号
					if (symbol != semicolon)
					{
						deal_error(23);
						return ;
					}
				}
			}
		}
		temp_Code.number1_lc = result.temp_local;
		temp_Code.global_1 = result.un_partial;
		temp_Code.number2_lc = return_location;
		temp_Code.global_2 = 1;
		temp_Code.Code_type = funct_return;
		new_MidLine(temp_Code);
		//新的一行，存储函数返回值
	}
}

void deal_statement()		//处理语句
{

	if (symbol == l_brace)		//如果读到的第一个是一个大括号，递归处理语句列
	{
		symbol = next_symbol(fp);		//处理语句列的时候要预读语句中一个单词
		deal_statement_column();		//deal_statement_column结束是遇到有大括号，所以其实没有预读，所以还要预读一个
		symbol = next_symbol(fp);		//deal_statement需要预读一个单词
	}
	else if (symbol == if_sy)		//如果读到的是'if'，说明是条件语句
	{
		deal_statement_if();
	}
	else if (symbol == do_sy)		//如果读到的是'do'
	{
		deal_statement_while();
		symbol = next_symbol(fp);		//deal_statement需要预读一个单词

	}
	else if (symbol == for_sy)		//如果读到的是'for'
	{
		deal_statement_for();
	}
	else if (symbol == ident)		//如果读到的是标识符
	{
		if (!look_Partial(temp_ident, part_start))		//如果局部变量符号表未找到
		{
			if (!look_Global(temp_ident))			//如果全局符号表也没找到
			{
				deal_error(18);
				return ;		//未定义的标识符，找不到
			}
		}

		symbol = next_symbol(fp);			//由于读到标识符的话，不确定后面是什么
		if (symbol == endow || symbol == l_bracket)		//如果是等号或者是左中括号是赋值语句
		{
			deal_assignment();
		}
		else if (symbol == l_parentheses)		//如果是左小括号，那么是函数调用语句
		{
			if (!look_Partial(temp_ident, part_start) && look_Global(temp_ident) && (Global_symbol_table[look_Global(temp_ident)]->type_information == valued_function_kind || Global_symbol_table[look_Global(temp_ident)]->type_information == invalued_function_kind))   //如果是左括号，并且是全局变量中的标识符，才是函数调用
			{
				deal_arguments();		//处理有返回值的函数调用的实参的格式
				symbol = next_symbol(fp);	//预读下一个字符
			}
			else
			{
				deal_error(29);		//用的不是全局变量的函数调用，而是局部变量 + '('，或者全局变量 + '('
				return ;
			}
			if (symbol != semicolon)		//判断函数调用语句后面的分号
			{
				deal_error(23);
				return ;
			}
		}
		else
		{
			deal_error(24);
			return ;			//处理语句的时候，明明读到了标识符，后面又错了
		}
		symbol = next_symbol(fp);		//deal_statement需要预读一个单词

	}
	else if (symbol == scanf_sy)		//如果读到的是scanf
	{
		deal_statement_read();
		symbol = next_symbol(fp);		//deal_statement需要预读一个单词

	}
	else if (symbol == printf_sy)		//如果读到的是printf
	{
		deal_statement_write();
		symbol = next_symbol(fp);		//deal_statement需要预读一个单词

	}
	else if (symbol == return_sy)		//如果读到的是return
	{
		deal_statement_return();		//返回语句
		symbol = next_symbol(fp);		//deal_statement需要预读一个单词

	}
	else if (symbol == semicolon)
	{
		;				//空语句
		symbol = next_symbol(fp);		//deal_statement需要预读一个单词

	}
	else
	{
		deal_error(24);
		return ;		//不是语句中该出现的单词
	}
}

void deal_statement_column()
{
	Syntax_Result << "Processing statement column in line  " << Count_line << endl;
	while (symbol != r_brace)			//未读到右大括号，说明一直都在处理语句列
	{
		// 处理语句列的时候已经预读了一个单词，这里不用读了
		deal_statement();			//不是右大括号的时候循环处理语句，deal_statement会预读一个单词
		// symbol = next_symbol(fp);	//处理一个语句之后，读后面的一个，比如说分号之后的一个
	}
}

void deal_function()
{
	Intermediate_Code temp_Code = {}, temp_Code2 = {};		//定义一个临时行，表示函数声明
	while (symbol == int_sy || symbol == char_sy || symbol == void_sy)		//函数的声明，这里是函数的声明
	{
		if (symbol == void_sy)
		{
			Syntax_Result << "This is a no return value function definition in line   " << Count_line << endl;
			enter_function(0);		//登陆符号表，
		}
		else if (symbol == int_sy)
		{
			Syntax_Result << "This is an integer return value function definition in line   " << Count_line << endl;
			enter_function(1);
		}
		else if (symbol == char_sy)
		{
			Syntax_Result << "This is a character return value function definition in line   " << Count_line << endl;
			enter_function(2);
		}
		else
		{
			deal_error(12);
			return ;		//读到的函数类型不正确，既不是void，也不是int， 也不是char
		}
		if (Inter_Code_Line > 1 && Code_txt[Inter_Code_Line - 1]->Code_type != funct_return)
		{
			temp_Code2.Code_type = funct_return;
			temp_Code2.number1_lc = -1;
			temp_Code2.global_1 = 2;
			new_MidLine(temp_Code2);
		}
		temp_Code.Code_type = funct_declare;
		temp_Code.number1_lc = look_Global(current_function);
		temp_Code.global_1 = 4;			//表示是字符串
		strcpy(temp_Code.Label, current_function);		//当前函数
		new_MidLine(temp_Code);			//声明函数一行
		//如果这不是第一个函数，并且函数前不是return 返回语句，说明需要加一个返回



		data_address = 0;			//对于一个新的函数，参数的相对地址为0
		part_start = Pstn;		//这个函数的第一个参数在局部符号表的位置
		symbol = next_symbol(fp);	//再继续读一个，左小括号
		if (symbol != l_parentheses)
		{
			deal_error(13);
			return ;		//函数后面的括号不对
		}
		else 			//函数后面的括号匹配，开始处理形式参数
		{
			deal_parameter();
		}
		symbol = next_symbol(fp);	//再继续读一个，左大括号
		if (symbol != l_brace)
		{
			return ;		//参数声明结束，后面应该是大括号
		}
		else
		{
			symbol = next_symbol(fp);	//读取函数的开头的那个
			deal_constant(0);		//处理函数的局部常量说明
			deal_variable(0);		//处理函数的局部变量说明，处理局部变量的时候已经预读了一个
		}
		//接下来是处理语句列
		deal_statement_column();		//处理复合语句中的语句列
		if (strcmp(current_function, "main") == 0)
		{
			break;
		}
		symbol = next_symbol(fp);
	}
	if (strcmp(current_function, "main") != 0)
	{
		deal_error(31);
		return ;
	}
}

