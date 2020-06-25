#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "symbol_table.h"
#include "Syntax_analysis.h"
#include "Translate_Mips.h"
#include "New_Quadruple.h"
#include <iostream>
#include <string.h>
using namespace std;

int look_Temp_Register(int loction_num, int global_num)
{
	for (int i = 0; i < 10; i++)
	{
		if (Temp_Register[i].status == 1 && Temp_Register[i].number_in == loction_num && Temp_Register[i].global_in == global_num)
		{
			return i;
		}
	}
	return -1;
}

int look_Global_Register(int number_in, int global_in)
{
	for (int i = 0; i < 8; ++i)
	{
		if (Global_Register[i].status == 1 && Global_Register[i].number_in == number_in && Global_Register[i].global_in == global_in)
		{
			return i;
		}
	}
	return -1;
}

int enter_global_register(int loc_num, int glo_num, int if_new)
{
	int empty_num = 0;
	while (empty_num < 8 && Global_Register[empty_num].status == 1)
	{
		empty_num += 1;
	}
	if (empty_num != 8)
	{
		Global_Register[empty_num].number_in = loc_num;
		Global_Register[empty_num].global_in = glo_num;
		Global_Register[empty_num].status = 1;
		// 登录寄存器的时候要 li ，表示登录寄存器
		if (glo_num == 0 && if_new == 1)         //这是局部变量，,并且是从内存取数据到寄存器
		{
			MIPS_code << "lw $s" << empty_num << ", " << -Partial_symbol_table[loc_num]->address - 4 << "($fp)" << endl;
			Partial_symbol_table[loc_num]->Where = 2;		//表示存入寄存器中
		}
		else if (glo_num == 1 && if_new == 1)          //如果是全局变量,并且是从内存去数据到寄存器
		{
			MIPS_code << "lw $s" << empty_num << ", " << Global_symbol_table[loc_num]->address << endl;
			Global_symbol_table[loc_num]->Where = 2;
		}
		else if (glo_num == 1)
		{
		    // 如果是全局变量就要从内存中取出来
            MIPS_code << "lw $s" << empty_num << ", " << Global_symbol_table[loc_num]->address << endl;
            Global_symbol_table[loc_num]->Where = 2;		//表示存入寄存器中
		}
		else if (glo_num == 0)
		{
			if (Partial_symbol_table[loc_num]->type_information == parameter_kind)
			{
				MIPS_code << "lw $s" << empty_num << "," << -Partial_symbol_table[loc_num]->address - 4 << "($fp)" << endl;
				Partial_symbol_table[loc_num]->Where = 2;		//表示存在寄存器中
			}
			else if (Partial_symbol_table[loc_num]->Where == 1)
			{
				MIPS_code << "lw $s" << empty_num << "," << -Partial_symbol_table[loc_num]->address - 4 << "($fp)" << endl;
				Partial_symbol_table[loc_num]->Where = 2;
			}
		}

		return empty_num;
	}
	return empty_num;
}

int deal_Global_register(int num_location, int num_global)
{
	int Reg_num = 0, enter_num = 0;
	if (look_Global_Register(num_location, num_global) == -1)
	{
		enter_num = enter_global_register(num_location, num_global, 0);
		if (enter_num == 8)
		{
			int unlucky_reg = 0;
			unlucky_reg = (Last_unlucky_Global + 1) % 8;
			Last_unlucky_Global = unlucky_reg;

			// 连续两次换到的寄存器不能一样

			//寄存器满了，要将全局寄存器的一个数据存储到对应的内存中,随机选一个寄存器，存到内存中去把
			if (Global_Register[unlucky_reg].global_in == 1)        //如果这个寄存器存的是全局变量
			{
				MIPS_code << "sw $s" << unlucky_reg << ", " << Global_symbol_table[Global_Register[unlucky_reg].number_in]->address << endl;
				Global_symbol_table[Global_Register[unlucky_reg].number_in]->Where = 1;
			}
			else        //如果这个寄存器存的是局部变量
			{
				MIPS_code << "sw $s" << unlucky_reg << ", " << -Partial_symbol_table[Global_Register[unlucky_reg].number_in]->address - 4 << "($fp)" << endl;
				Partial_symbol_table[Global_Register[unlucky_reg].number_in]->Where = 1;
			}
			Global_Register[unlucky_reg].status = 0;
			enter_global_register(num_location, num_global, 1);
			//登录寄存器，并且是从内存取数据
			return unlucky_reg;
		}
		else
		{
			Reg_num = enter_num;
			return Reg_num;
		}
	}
	else
	{
		Reg_num = look_Global_Register(num_location, num_global);
		return Reg_num;
	}
}

int enter_temp_register(int loc_num, int glo_num)
{
	int empty_num = 0;
	while (empty_num < 10 && Temp_Register[empty_num].status == 1)
	{
		empty_num += 1;
	}
	if (empty_num != 10)
	{
		Temp_Register[empty_num].status = 1;
		Temp_Register[empty_num].number_in = loc_num;
		Temp_Register[empty_num].global_in = glo_num;
		Temp_Register[empty_num].Parent_num = MIPS_funct_addr;
		if (Temp_Register[empty_num].global_in == 3)
		{
			// 如果是常数直接 li
			MIPS_code << "li $t" << empty_num << ", " << loc_num << endl;
		}
		else if (Temp_Register[empty_num].global_in == 2)
		{
			// 如果是临时变量，则
			if (loc_num > 0 && Temp_symbol_table[loc_num]->Where == 1)
			{
				MIPS_code << "lw $t" << empty_num << "," << -Temp_symbol_table[Temp_Register[empty_num].number_in]->address << "($fp)" << endl;
				Temp_symbol_table[Temp_Register[empty_num].number_in]->Where = 2;
				// 表示存储到寄存器中了，从内存中取出
			}
		}
	}
	return empty_num;
}

int deal_Temp_register(int num_location, int num_global)
{
	int Reg_num = 0, enter_num = 0;
	if (look_Temp_Register(num_location, num_global) == -1)
	{
		enter_num = enter_temp_register(num_location, num_global);
		if (enter_num == 10)
		{
			int unlucky_reg = 0;
			unlucky_reg = (Last_unlucky_Temp + 1) % 10;
			Last_unlucky_Temp = unlucky_reg;
			// 随机选一个寄存器
			//寄存器满了，要将全局寄存器的一个数据存储到对应的内存中,随机选一个寄存器，存到内存中去把
			while (Temp_Register[unlucky_reg].number_in < 0)
			{
				unlucky_reg = (Last_unlucky_Temp + 1) % 10;
				Last_unlucky_Temp = unlucky_reg;
			}

			MIPS_code << "sw $t" << unlucky_reg << ", " << -Temp_symbol_table[Temp_Register[unlucky_reg].number_in]->address << "($fp)" << endl;
			Temp_symbol_table[Temp_Register[unlucky_reg].number_in]->Where = 1;     //1表示在寄存器中
			Temp_Register[unlucky_reg].status = 0;
			enter_num = enter_temp_register(num_location, num_global);
			//登录寄存器，并且是从内存取数据
			return enter_num;
		}
		else
		{
			return enter_num;
		}
	}
	else
	{
		Reg_num = look_Temp_Register(num_location, num_global);
		return Reg_num;
	}

}

void Save_Reg(int IF_BACK)
{
	int i = 0, k = 0, save_count = 0;
	for (i = 0; i < 8; i++)
	{
		if (i < 8 && Global_Register[i].status == 1)
		{
			if (Global_Register[i].global_in == 0)
			{
				//如果是局部变量的话，就放回内存去
				MIPS_code << "sw $s" << i << ", " << -Partial_symbol_table[Global_Register[i].number_in]->address - 4 << "($fp)" << endl;
				//这个寄存器的值保存在内存中了，就不管他事了
				Partial_symbol_table[Global_Register[i].number_in]->Where = 1;
			}
			else        //如果是全局变量，那么我要将寄存器存到全局地址
			{
				MIPS_code << "sw $s" << i << ", " << Global_symbol_table[Global_Register[i].number_in]->address << endl;
				Global_symbol_table[Global_Register[i].number_in]->Where = 1;	//将变量存到内存中去
			}

			if (!IF_BACK)
			{
				Global_Register[i].status = 0;
			}
		}
	}
	for (i = 0; i < 10; i++)
	{
		if (Temp_Register[i].global_in == 2 && Temp_Register[i].number_in > 0 && i < 10 && Temp_Register[i].status == 1 && Temp_Register[i].Parent_num == MIPS_funct_addr)
		{
			MIPS_code << "sw $t" << i << ", " <<  -Temp_symbol_table[Temp_Register[i].number_in]->address << "($fp)" << endl;
			Temp_symbol_table[Temp_Register[i].number_in]->Where = 1;   		//将变量存到内存中去
			if (!IF_BACK)
			{
				Temp_Register[i].status = 0;
			}
		}
	}
	return ;
}

void Fetch_Reg()
{
	int i = 0, j = 0;
	for (i = 0; i < 8; i++)
	{
		if (i < 8 && Global_Register[i].status == 1)
		{
			if (Global_Register[i].global_in == 0)
			{
				MIPS_code << "lw $s" << i << ", " << -Partial_symbol_table[Global_Register[i].number_in]->address - 4 << "($fp)" << endl;
				// 取出内存中的变量到寄存器
				Partial_symbol_table[Global_Register[i].number_in]->Where = 1;
			}
			else        //如果是全局变量，那么我要将寄存器存到全局地址
			{
				MIPS_code << "lw $s" << i << ", " << Global_symbol_table[Global_Register[i].number_in]->address << endl;
				Global_symbol_table[Global_Register[i].number_in]->Where = 1;
			}
		}
	}
	for (i = 0; i < 10; i++)
	{
		if (Temp_Register[i].global_in == 2 && Temp_Register[i].number_in > 0 && Temp_Register[i].status == 1 && Temp_Register[i].Parent_num == MIPS_funct_addr)
		{
			MIPS_code << "lw $t" << i << ", " << -Temp_symbol_table[Temp_Register[i].number_in]->address << "($fp)" << endl;
			j++;
			Temp_symbol_table[Temp_Register[i].number_in]->Where = 1;
			// 表示存储到内存中去
		}
	}
}

void Initial_Reg()
{
	for (int i = 0; i < 8; i++)
	{
		Global_Register[i].status = 0;
	}
	for (int i = 0; i < 10; i++)
	{
		Temp_Register[i].status = 0;
	}
}

void mips_funct_declare()
{
	MIPS_funct_addr = New_Code[MIPS_count_line]->number1_lc;
	MIPS_code << endl << New_Code[MIPS_count_line]->Label << ":" << endl;
	// 输出函数的标签
	int para_sapce = 0, variable_space = 0, const_addr = 0, const_space = 0, last_space = 0;
	int const_Reg = 0, out_number = 0;         //存储这个常量的寄存器
	int next_funct_addr = 0, Clean_funt_Reg = 4;        //记录下一个函数的变量开始的地址，不能超过了啊，
	char cur_func[20] = {0}, nume_str[3] = {0};            //表示当前的函数名字
	int SUM_space = 0, temp_space = 0, temp_start = 0, temp_start_lc = 0;
	strcpy(cur_func, Global_symbol_table[New_Code[MIPS_count_line]->number1_lc]->name);      //call 调用的函数名
	para_sapce = Global_symbol_table[MIPS_funct_addr]->length * 4;
	MIPS_code << "addu $fp, $sp, " << para_sapce << endl;        //将$fp加到栈顶
	Initial_Reg();
	const_addr = Global_symbol_table[MIPS_funct_addr]->address +  Global_symbol_table[MIPS_funct_addr]->length;   //第一个常量在局部符号表的位置

	while (const_addr < Pstn && strcmp(cur_func, Partial_symbol_table[const_addr]->parent_function) == 0)
	{
		const_addr ++;
	}
	const_addr > 1 ? variable_space = Partial_symbol_table[const_addr - 1]->address - para_sapce : 0;
	// 这里的address存储的是首地址，所以后面还要加最后一个变量的大小
	if (const_addr > 1)
	{
		if (Partial_symbol_table[const_addr - 1]->type_information == array_kind)
		{
			if (Partial_symbol_table[const_addr - 1]->data_type == 1)
			{
				variable_space = variable_space + 4 * Partial_symbol_table[const_addr - 1]->length;
			}
			else
			{
				variable_space = variable_space + Partial_symbol_table[const_addr - 1]->length;
			}
		}
		else
		{
			variable_space = variable_space + 4;
		}
	}

	temp_start_lc = variable_space + para_sapce;
	temp_start = Global_symbol_table[MIPS_funct_addr]->temp_address;
	while (Temp_symbol_table[temp_start] != NULL && Temp_symbol_table[temp_start]->temp_address == MIPS_funct_addr)
	{
		variable_space += 4;        //表示用来存临时变量的
		Temp_symbol_table[temp_start]->address = temp_start_lc + Temp_symbol_table[temp_start]->address;
		temp_start ++;
	}

	SUM_space = variable_space + para_sapce + 4;
	Global_symbol_table[MIPS_funct_addr]->const_int = SUM_space;
	MIPS_code << "addi $sp, $sp, " << -variable_space << endl;      //在栈中开一个空间，用来存储变量

	MIPS_code << "addi $sp, $sp, -4" << endl;
	MIPS_code << "sw $ra, 0($sp)" << endl;
}

void mips_push()
{
	int Reg_para = 0;

	MIPS_code << "addi $sp, $sp, -4" << endl;

	if (New_Code[MIPS_count_line]->global_1 == 2 || New_Code[MIPS_count_line]->global_1 == 3)
	{
		Reg_para = deal_Temp_register(New_Code[MIPS_count_line]->number1_lc, New_Code[MIPS_count_line]->global_1);
		MIPS_code << "sw " << "$t" << Reg_para << ", 0($sp)" << endl;
	}
	else
	{
		Reg_para = deal_Global_register(New_Code[MIPS_count_line]->number1_lc, New_Code[MIPS_count_line]->global_1);
		MIPS_code << "sw "  << "$s" << Reg_para << ", 0($sp)" << endl;
	}
	if (New_Code[MIPS_count_line]->global_1 == 2 || New_Code[MIPS_count_line]->global_1 == 3)
	{
		Temp_Register[Reg_para].status = 0;
	}
}

void mips_funct_call()
{

	MIPS_code << endl;
	//我们首先要清空$s 寄存器，，然后我们将$a 寄存器的值，赋给空的 $s寄存器
	// 此时全局变量中data_type存储的是临时寄存器占的空间
	Save_Reg(1);
	MIPS_code << "jal " << New_Code[MIPS_count_line]->Label << endl ;
	MIPS_code << endl;
	MIPS_code << "addu $fp, $fp, " << Global_symbol_table[MIPS_funct_addr]->const_int << endl;
	Fetch_Reg();
}

void mips_funct_result()
{
	int Reg_return = 0, i = 0, j = 0;

	if (New_Code[MIPS_count_line]->global_1 == 2 || New_Code[MIPS_count_line]->global_1 == 3)
	{
		Reg_return = deal_Temp_register(New_Code[MIPS_count_line]->number1_lc, New_Code[MIPS_count_line]->global_1);
		MIPS_code << "move $t" << Reg_return << ", $v1" << endl;
	}
	else
	{
		Reg_return = deal_Global_register(New_Code[MIPS_count_line]->number1_lc, New_Code[MIPS_count_line]->global_1);
		MIPS_code << "move $s" << Reg_return << ", $v1" << endl;
	}
}

void mips_funct_return()
{
	int Reg_return = 0, i = 0;
	if (strcmp(Global_symbol_table[MIPS_funct_addr]->name, "main") == 0)
	{
		MIPS_code << "li $v0, 10" << endl;
		MIPS_code << "syscall" << endl;
		return ;
	}
	if (New_Code[MIPS_count_line]->number1_lc != -1)
	{

		if (New_Code[MIPS_count_line]->global_1 == 2 || New_Code[MIPS_count_line]->global_1 == 3)
		{
			Reg_return = deal_Temp_register(New_Code[MIPS_count_line]->number1_lc, New_Code[MIPS_count_line]->global_1);
			MIPS_code << "move $v1" << ", $t" << Reg_return << endl;
			Temp_Register[Reg_return].status = 0;
		}
		else
		{
			Reg_return = deal_Global_register(New_Code[MIPS_count_line]->number1_lc, New_Code[MIPS_count_line]->global_1);
			MIPS_code << "move $v1" << ", $s" << Reg_return << endl;
		}
	}
	Save_Reg(0);
	MIPS_code << "lw $ra, " << 0 << "($sp)" << endl;
	MIPS_code << "move $sp, $fp" << endl;
	MIPS_code << "jr $ra" << endl;
}

void mips_cal_expression()
{
	int Reg_data1 = 0, Reg_data2 = 0, Reg_result = 0;
	char Temp_MIPS_Code [30] = {0}, Temp_Reg_num[20] = {0};

	switch (New_Code[MIPS_count_line]->operate_cal)
	{
	case 1: strcpy(Temp_MIPS_Code, "addu "); break;
	case 2: strcpy(Temp_MIPS_Code, "subu "); break;
	case 3: strcpy(Temp_MIPS_Code, "mul "); break;
	case 4: strcpy(Temp_MIPS_Code, "div "); break;
	}
	if (New_Code[MIPS_count_line]->global_1 == 0 || New_Code[MIPS_count_line]->global_1 == 1)
	{
		Reg_result = deal_Global_register(New_Code[MIPS_count_line]->number1_lc, New_Code[MIPS_count_line]->global_1);
		//MIPS_code << "$s" << Reg_result ;
		strcat(Temp_MIPS_Code, "$s");
		sprintf(Temp_Reg_num, "%d", Reg_result);
		strcat(Temp_MIPS_Code, Temp_Reg_num);
	}
	else
	{
		Reg_result = deal_Temp_register(New_Code[MIPS_count_line]->number1_lc, 2);
		// 保存计算结果的寄存器
		strcat(Temp_MIPS_Code, "$t");
		sprintf(Temp_Reg_num, "%d", Reg_result);
		strcat(Temp_MIPS_Code, Temp_Reg_num);
	}
	/*######################################################################################################*/
	/*######################################################################################################*/
	//如果第一个计算数是临时变量或者是立即数，那么用临时寄存器存储
	if (New_Code[MIPS_count_line]->global_2 == 2 || New_Code[MIPS_count_line]->global_2 == 3)
	{
		Reg_data1 = deal_Temp_register(New_Code[MIPS_count_line]->number2_lc, New_Code[MIPS_count_line]->global_2);
		strcat(Temp_MIPS_Code, ", $t");
		sprintf(Temp_Reg_num, "%d", Reg_data1);
		strcat(Temp_MIPS_Code, Temp_Reg_num);
	}
	else        //否则用全局寄存器存储
	{
		Reg_data1 = deal_Global_register(New_Code[MIPS_count_line]->number2_lc, New_Code[MIPS_count_line]->global_2);
		//MIPS_code << ", $s" << Reg_data1 << ", ";
		strcat(Temp_MIPS_Code, ", $s");
		sprintf(Temp_Reg_num, "%d", Reg_data1);
		strcat(Temp_MIPS_Code, Temp_Reg_num);
	}

	//如果第三个变量是临时变量或者是立即数，那么直接拿来计算
	if(New_Code[MIPS_count_line]->global_3 == 3)
    {
        strcat(Temp_MIPS_Code, ", ");
        sprintf(Temp_Reg_num, "%d", New_Code[MIPS_count_line]->number3_lc);
		strcat(Temp_MIPS_Code, Temp_Reg_num);
    }
	else if (New_Code[MIPS_count_line]->global_3 == 2)
	{
		Reg_data2 = deal_Temp_register(New_Code[MIPS_count_line]->number3_lc, New_Code[MIPS_count_line]->global_3);
		strcat(Temp_MIPS_Code, ", $t");
		sprintf(Temp_Reg_num, "%d", Reg_data2);
		strcat(Temp_MIPS_Code, Temp_Reg_num);
	}
	else
	{
		Reg_data2 = deal_Global_register(New_Code[MIPS_count_line]->number3_lc, New_Code[MIPS_count_line]->global_3);
		//MIPS_code << "$s" << Reg_data2 << endl;
		strcat(Temp_MIPS_Code, ", $s");
		sprintf(Temp_Reg_num, "%d", Reg_data2);
		strcat(Temp_MIPS_Code, Temp_Reg_num);
	}

	//为三个操作数分配寄存器
	if (New_Code[MIPS_count_line]->global_2 == 2 || New_Code[MIPS_count_line]->global_2 == 3)
	{
		Temp_Register[Reg_data1].status = 0;
	}
	if (New_Code[MIPS_count_line]->global_3 == 2)
	{
		Temp_Register[Reg_data2].status = 0;
	}
	// 如果是临时变量，那么用了一次之后就不可能再用了，也就是说只用一次
	MIPS_code << Temp_MIPS_Code << endl;

}

void mips_endow_expression()
{
	int Reg_data1 = 0, Reg_data2 = 0, Reg_result = 0;
	char Temp_MIPS_Code [30] = {0}, Temp_Reg_num[3] = {0};
	strcpy(Temp_MIPS_Code, "move ");
	if (New_Code[MIPS_count_line]->global_1 == 0 || New_Code[MIPS_count_line]->global_1 == 1)
	{
		Reg_result = deal_Global_register(New_Code[MIPS_count_line]->number1_lc, New_Code[MIPS_count_line]->global_1);
		strcat(Temp_MIPS_Code, "$s");
		sprintf(Temp_Reg_num, "%d", Reg_result);
		strcat(Temp_MIPS_Code, Temp_Reg_num);
	}
	else
	{
		Reg_result = deal_Temp_register(New_Code[MIPS_count_line]->number1_lc, 2);
		//将计算结果设为寄存器
		strcat(Temp_MIPS_Code, "$t");
		sprintf(Temp_Reg_num, "%d", Reg_result);
		strcat(Temp_MIPS_Code, Temp_Reg_num);
	}
	/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
	if (New_Code[MIPS_count_line]->global_2 == 2 || New_Code[MIPS_count_line]->global_2 == 3)
	{
		Reg_data2 = deal_Temp_register(New_Code[MIPS_count_line]->number2_lc, New_Code[MIPS_count_line]->global_2);
        // 构建输出的MIPS代码
		strcat(Temp_MIPS_Code, ", $t");
		sprintf(Temp_Reg_num, "%d", Reg_data2);
		strcat(Temp_MIPS_Code, Temp_Reg_num);
	}
	else
	{
		Reg_data2 = deal_Global_register(New_Code[MIPS_count_line]->number2_lc, New_Code[MIPS_count_line]->global_2);
        // 构建输出的MIPS代码
		strcat(Temp_MIPS_Code, ", $s");
		sprintf(Temp_Reg_num, "%d", Reg_data2);
		strcat(Temp_MIPS_Code, Temp_Reg_num);
	}
	if (New_Code[MIPS_count_line]->global_2 == 2 || New_Code[MIPS_count_line]->global_2 == 3)
	{
		Temp_Register[Reg_data2].status = 0;
	}
	MIPS_code << Temp_MIPS_Code << endl;

}

void mips_Con_judge()
{
	int Reg_result = 0, Reg_data2 = 0, Reg_data3 = 0;
	int enter_num = 0;
	char Temp_MIPS_Code [30] = {0}, Temp_Reg_num[3] = {0};  //存储临时的一行MIPS代码
	switch (New_Code[MIPS_count_line]->operate_cal)
	{
	case 7: strcpy(Temp_MIPS_Code, "seq "); break;
	case 8: strcpy(Temp_MIPS_Code, "sne "); break;
	case 9: strcpy(Temp_MIPS_Code, "sge "); break;
	case 10: strcpy(Temp_MIPS_Code, "sgt "); break;
	case 11: strcpy(Temp_MIPS_Code, "sle "); break;
	case 12: strcpy(Temp_MIPS_Code, "slt "); break;
	case 13: strcpy(Temp_MIPS_Code, "sne "); break;
	}
	Reg_result = deal_Temp_register(-1, 2);
	strcat(Temp_MIPS_Code, "$t");
	sprintf(Temp_Reg_num, "%d", Reg_result);
	strcat(Temp_MIPS_Code, Temp_Reg_num);
	///处理一个临时寄存器，假设临时寄存器为-1，
	if (New_Code[MIPS_count_line]->global_2 == 2 || New_Code[MIPS_count_line]->global_2 == 3)
	{
		Reg_data2 = deal_Temp_register(New_Code[MIPS_count_line]->number2_lc, New_Code[MIPS_count_line]->global_2);
		strcat(Temp_MIPS_Code, ", $t");
		sprintf(Temp_Reg_num, "%d", Reg_data2);
		strcat(Temp_MIPS_Code, Temp_Reg_num);
	}
	else
	{
		Reg_data2 = deal_Global_register(New_Code[MIPS_count_line]->number2_lc, New_Code[MIPS_count_line]->global_2);
		strcat(Temp_MIPS_Code, ", $s");
		sprintf(Temp_Reg_num, "%d", Reg_data2);
		strcat(Temp_MIPS_Code, Temp_Reg_num);
	}

	//第一个比较数字处理完，要看是不是只有一个表达式的条件
	if (New_Code[MIPS_count_line]->operate_cal == 13)
	{
		MIPS_code << Temp_MIPS_Code << ", $0" << endl;
		if (New_Code[MIPS_count_line]->global_2 == 2 || New_Code[MIPS_count_line]->global_2 == 3)
        {
            Temp_Register[Reg_data2].status = 0;
        }
        return ;
	}
	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
	if (New_Code[MIPS_count_line]->global_3 == 2 || New_Code[MIPS_count_line]->global_3 == 3)
	{
		Reg_data3 = deal_Temp_register(New_Code[MIPS_count_line]->number3_lc, New_Code[MIPS_count_line]->global_3);
		strcat(Temp_MIPS_Code, ", $t");
		sprintf(Temp_Reg_num, "%d", Reg_data3);
		strcat(Temp_MIPS_Code, Temp_Reg_num);
	}
	else
	{
		Reg_data3 = deal_Global_register(New_Code[MIPS_count_line]->number3_lc, New_Code[MIPS_count_line]->global_3);
		strcat(Temp_MIPS_Code, ", $s");
		sprintf(Temp_Reg_num, "%d", Reg_data3);
		strcat(Temp_MIPS_Code, Temp_Reg_num);
	}
	MIPS_code << Temp_MIPS_Code << endl;

	if (New_Code[MIPS_count_line]->global_2 == 2 || New_Code[MIPS_count_line]->global_2 == 3)
	{
		Temp_Register[Reg_data2].status = 0;
	}
	if (New_Code[MIPS_count_line]->global_3 == 2 || New_Code[MIPS_count_line]->global_3 == 3)
	{
		Temp_Register[Reg_data3].status = 0;
	}
}

void mips_array_number()
{
	int Reg_Array = 0, Reg_data2 = 0, Reg_data3 = 0, Reg_Array2;
	char Reg_2[3] = {0}, Reg_3[3] = {0};
	//不管怎么样，数组中与结果的表达式都是要算的，所以先算好把
	if (New_Code[MIPS_count_line]->global_2 == 2 || New_Code[MIPS_count_line]->global_2 == 3)
	{
		Reg_data2 = deal_Temp_register(New_Code[MIPS_count_line]->number2_lc, New_Code[MIPS_count_line]->global_2);
		strcpy(Reg_2, "$t");
	}
	else
	{
		Reg_data2 = deal_Global_register(New_Code[MIPS_count_line]->number2_lc, New_Code[MIPS_count_line]->global_2);
		strcpy(Reg_2, "$s");
	}
	//计算第二个操作数的寄存器
	if (New_Code[MIPS_count_line]->global_3 == 2 || New_Code[MIPS_count_line]->global_3 == 3)
	{
		Reg_data3 = deal_Temp_register(New_Code[MIPS_count_line]->number3_lc, New_Code[MIPS_count_line]->global_3);
		strcpy(Reg_3, "$t");
	}
	else
	{
		Reg_data3 = deal_Global_register(New_Code[MIPS_count_line]->number3_lc, New_Code[MIPS_count_line]->global_3);
		strcpy(Reg_3, "$s");
	}
	//计算第三个操作数的寄存器

	if (New_Code[MIPS_count_line]->global_1 == 1)    //如果是全局数组的话，全局变量
	{
		Reg_Array = deal_Temp_register(-3, 2);
		MIPS_code << "li $t" << Reg_Array << ", " << Global_symbol_table[New_Code[MIPS_count_line]->number1_lc]->address << endl;
		//将组数的下表地址存到临时寄存器中
		if (Global_symbol_table[New_Code[MIPS_count_line]->number1_lc]->data_type == 1)      //如果是全局整型数组的话
		{
			Reg_Array2 = deal_Temp_register(-2, 2);
			MIPS_code << "sll " << "$t" << Reg_Array2 << ", " << Reg_2 << Reg_data2 << ", 2" << endl;
			MIPS_code << "addu $t" << Reg_Array << ", $t" << Reg_Array << ", " << "$t" << Reg_Array2 << endl;
			MIPS_code << "sw " << Reg_3 << Reg_data3 << ", 0($t" <<  Reg_Array << ")" << endl;
			Temp_Register[Reg_Array2].status = 0;
		}
		else        //如果是全局变量，并且是字符型数组的话
		{
			MIPS_code << "addu $t" << Reg_Array << ", $t" << Reg_Array << ", " << Reg_2 << Reg_data2 << endl;
			MIPS_code << "sb " << Reg_3 << Reg_data3 << ", 0($t" <<  Reg_Array << ")" << endl;
		}
		Temp_Register[Reg_Array].status = 0;
	}
	else        //如果是局部数组的话，局部变量
	{
		Reg_Array = deal_Temp_register(-3, 2);          //找一个一定是临时的寄存器
		MIPS_code << "subu $t" << Reg_Array << ", $fp, " << Partial_symbol_table[New_Code[MIPS_count_line]->number1_lc]->address << endl;
		if (Partial_symbol_table[New_Code[MIPS_count_line]->number1_lc]->data_type == 1)      //如果是整型数组的话，下标要乘以4
		{
			Reg_Array2 = deal_Temp_register(-2, 2);
			MIPS_code << "sll " << "$t" << Reg_Array2 << ", " << Reg_2 << Reg_data2 << ", 2" << endl;
			MIPS_code << "subu $t" << Reg_Array << ", $t" << Reg_Array << ", " << "$t" << Reg_Array2 << endl;
			MIPS_code << "sw " << Reg_3 << Reg_data3 << ", -4($t" <<  Reg_Array << ")" << endl;
			/*MIPS_code << "addu $t" << Reg_Array << ", $t" << Reg_Array << ", " << "$t" << Reg_Array2 << endl;
			MIPS_code << "sw " << Reg_3 << Reg_data3 << ", 0($t" <<  Reg_Array << ")" << endl;*/
			Temp_Register[Reg_Array2].status = 0;
		}
		else        //如果是局部变量，并且是字符型数组的话
		{
			MIPS_code << "subu $t" << Reg_Array << ", $t" << Reg_Array << ", " << Reg_2 << Reg_data2 << endl;
			MIPS_code << "sb " << Reg_3 << Reg_data3 << ", -1($t" <<  Reg_Array << ")" << endl;
			/*MIPS_code << "addu $t" << Reg_Array << ", $t" << Reg_Array << ", " << Reg_2 << Reg_data2 << endl;
			MIPS_code << "sb " << Reg_3 << Reg_data3 << ", 0($t" <<  Reg_Array << ")" << endl;*/
		}
		Temp_Register[Reg_Array].status = 0;
	}
	if (New_Code[MIPS_count_line]->global_2 == 2 || New_Code[MIPS_count_line]->global_2 == 3)
	{
		Temp_Register[Reg_data2].status = 0;
	}
	if (New_Code[MIPS_count_line]->global_3 == 2 || New_Code[MIPS_count_line]->global_3 == 3)
	{
		Temp_Register[Reg_data3].status = 0;
	}
}

void mips_number_array()
{
	int Reg_Array = 0, Reg_result = 0, Reg_data = 0, Reg_Array2 = 0;
	char Reg_r[3] = {0}, Reg_d[3] = {0};
	//先算结果类型，就是四元式中的number1
	if (New_Code[MIPS_count_line]->global_1 == 0 || New_Code[MIPS_count_line]->global_1 == 1)
	{
		Reg_result = deal_Global_register(New_Code[MIPS_count_line]->number1_lc, New_Code[MIPS_count_line]->global_1);
		strcpy(Reg_r, "$s");
	}
	else
	{
		Reg_result = deal_Temp_register(New_Code[MIPS_count_line]->number1_lc, 2);
		//将计算结果设为寄存器
		strcpy(Reg_r, "$t");
	}
	//计算数组中括号中的表达式类型，用寄存器保存起来
	if (New_Code[MIPS_count_line]->global_3 == 2 || New_Code[MIPS_count_line]->global_3 == 3)
	{
		Reg_data = deal_Temp_register(New_Code[MIPS_count_line]->number3_lc, New_Code[MIPS_count_line]->global_3);
		strcpy(Reg_d, "$t");
	}
	else
	{
		Reg_data = deal_Global_register(New_Code[MIPS_count_line]->number3_lc, New_Code[MIPS_count_line]->global_3);
		strcpy(Reg_d, "$s");
	}
	//然后开始计算数组的下标，取出下标的值， lw或者lb
	if (New_Code[MIPS_count_line]->global_2 == 1)        //如果是全局数组，表示是全局数组
	{
		Reg_Array = deal_Temp_register(-3, 2);
		MIPS_code << "li $t" << Reg_Array << ", " << Global_symbol_table[New_Code[MIPS_count_line]->number2_lc]->address << endl;
		//将组数的下表地址存到临时寄存器中
		if (Global_symbol_table[New_Code[MIPS_count_line]->number2_lc]->data_type == 1)      //如果是全局整型数组的话
		{
			Reg_Array2 = deal_Temp_register(-2, 2);
			MIPS_code << "sll " << "$t" << Reg_Array2 << ", " << Reg_d << Reg_data << ", 2" << endl;
			MIPS_code << "addu $t" << Reg_Array << ", $t" << Reg_Array << ", " << "$t" << Reg_Array2 << endl;
			MIPS_code << "lw " << Reg_r << Reg_result << ", 0($t" <<  Reg_Array << ")" << endl;
			Temp_Register[Reg_Array2].status = 0;
		}
		else        //如果是全局变量，并且是字符型数组的话
		{
			MIPS_code << "addu $t" << Reg_Array << ", $t" << Reg_Array << ", " << Reg_d << Reg_data << endl;
			MIPS_code << "lb " << Reg_r << Reg_result << ", 0($t" <<  Reg_Array << ")" << endl;
		}
		Temp_Register[Reg_Array].status = 0;
	}
	else            //如果是局部数组变量的话
	{
		Reg_Array = deal_Temp_register(-3, 2);          //找一个一定是临时的寄存器
		MIPS_code << "subu $t" << Reg_Array << ", $fp, " << Partial_symbol_table[New_Code[MIPS_count_line]->number2_lc]->address << endl;
		if (Partial_symbol_table[New_Code[MIPS_count_line]->number2_lc]->data_type == 1)      //如果是整型数组的话，下标要乘以4
		{
			Reg_Array2 = deal_Temp_register(-2, 2);
			MIPS_code << "sll " << "$t" << Reg_Array2 << ", " << Reg_d << Reg_data << ", 2" << endl;
			MIPS_code << "subu $t" << Reg_Array << ", $t" << Reg_Array << ", " << "$t" << Reg_Array2 << endl;
			MIPS_code << "lw " << Reg_r << Reg_result << ", -4($t" <<  Reg_Array << ")" << endl;
			Temp_Register[Reg_Array2].status = 0;
		}
		else        //如果是局部变量，并且是字符型数组的话
		{
			MIPS_code << "subu $t" << Reg_Array << ", $t" << Reg_Array << ", " << Reg_d << Reg_data << endl;
			MIPS_code << "lb " << Reg_r << Reg_result << ", -1($t" <<  Reg_Array << ")" << endl;
		}
		Temp_Register[Reg_Array].status = 0;
	}
	if (New_Code[MIPS_count_line]->global_3 == 2 || New_Code[MIPS_count_line]->global_3 == 3)
	{
		Temp_Register[Reg_data].status = 0;
	}
}

void mips_Negate()
{
	int Reg_Result = 0, Reg_data = 0;
	Reg_Result = deal_Temp_register(New_Code[MIPS_count_line]->number1_lc, New_Code[MIPS_count_line]->global_1);
	// MIPS_code << "neg $t" << Reg_Result ;

	if (New_Code[MIPS_count_line]->global_2 == 2 || New_Code[MIPS_count_line]->global_2 == 3)
	{
		Reg_data = deal_Temp_register(New_Code[MIPS_count_line]->number2_lc, New_Code[MIPS_count_line]->global_2);
		MIPS_code << "neg $t" << Reg_Result << ", $t" << Reg_data << endl;
		Temp_Register[Reg_data].status = 0;
	}
	else
	{
		Reg_data = deal_Global_register(New_Code[MIPS_count_line]->number2_lc, New_Code[MIPS_count_line]->global_2);
		MIPS_code << "neg $t" << Reg_Result << ",$s" << Reg_data << endl;
	}
}

void mips_read_code()
{
	int Reg_data = 0;
	if (New_Code[MIPS_count_line]->operate_cal == 1)       //表示是int类型，
	{
		MIPS_code << "li $v0, 5" << endl << "syscall" << endl;
	}
	else        //表示读入的标识符是char类型
	{
		MIPS_code << "li $v0, 12" << endl << "syscall" << endl;
	}
	//然后判断是全局变量，还是局部变量，
	Reg_data = deal_Global_register(New_Code[MIPS_count_line]->number1_lc, New_Code[MIPS_count_line]->global_1);
	MIPS_code << "move $s" << Reg_data << ", $v0" << endl;
}

void mips_write_code()
{
	int Reg_data = 0;
	if (New_Code[MIPS_count_line]->operate_cal == 14)
	{
		MIPS_code << "la $a0, " << New_Code[MIPS_count_line]->Label << endl;
		MIPS_code << "li $v0, 4" << endl;
		MIPS_code << "syscall" << endl;
	}
	else
	{
		if (New_Code[MIPS_count_line]->global_1 == 2 || New_Code[MIPS_count_line]->global_1 == 3)
		{
			Reg_data = deal_Temp_register(New_Code[MIPS_count_line]->number1_lc, New_Code[MIPS_count_line]->global_1);
			MIPS_code << "move $a0, $t" << Reg_data << endl;
		}
		else
		{
			Reg_data = deal_Global_register(New_Code[MIPS_count_line]->number1_lc, New_Code[MIPS_count_line]->global_1);
			MIPS_code << "move $a0, $s" << Reg_data << endl;
		}
		if (New_Code[MIPS_count_line]->operate_cal == 1)        //如果是整型数字输出的话
		{
			MIPS_code << "li $v0, 1" << endl << "syscall" << endl;
		}
		else        //如果是字符型数据输出的话
		{
			MIPS_code << "li $v0, 11" << endl << "syscall" << endl;
		}
	}
	if (New_Code[MIPS_count_line]->global_1 == 2 || New_Code[MIPS_count_line]->global_1 == 3)
	{
		Temp_Register[Reg_data].status = 0;
	}
}

void Translate()
{

	int temp_register_number = 0;
	int temp_number = 1;             //记录翻译到的行数
	MIPS_code << ".data" << endl;

	MIPS_code << "Reserved: .space " << Reserved << endl;

	for (int i = 0; i < sentence_count; i++)
	{
		MIPS_code << sentence[i]->string_name << ": .asciiz " << "\"" << sentence[i]->string_sce << "\"" << endl;
	}
	while (Global_symbol_table[temp_number]->type_information != valued_function_kind && Global_symbol_table[temp_number]->type_information != invalued_function_kind)
	{
		Global_symbol_table[temp_number]->address += 268500992;
		temp_number += 1;
	}
	temp_number = 1;

	MIPS_code << endl << ".text" << endl;

	MIPS_code << "j main" << endl << endl;
	for (MIPS_count_line = 0; MIPS_count_line < New_Code_Line; MIPS_count_line++)
	{
	    cout << MIPS_count_line << endl;

		if (New_Code[MIPS_count_line]->Code_type == expression)
		{
			mips_cal_expression();
		}
		else if (New_Code[MIPS_count_line]->Code_type == endow_assign)
		{
			mips_endow_expression();
		}
		else if (New_Code[MIPS_count_line]->Code_type == Con_judge)
		{
		    mips_Con_judge();
		    if(MIPS_count_line < New_Code_Line-1 && (New_Code[MIPS_count_line+1]->Code_type == BNZ || New_Code[MIPS_count_line+1]->Code_type == BZ))
            {
                Save_Reg(0);
            }
		}
		//如果是条件满足就跳转，那么满足条件就跳转
		else if (New_Code[MIPS_count_line]->Code_type == BZ)
		{
			int Reg_jump = 0;
			Reg_jump = look_Temp_Register(-1, 2);
			MIPS_code << "beq $t" << Reg_jump << ", " << "$0 " << New_Code[MIPS_count_line]->Label << endl << endl;
			Temp_Register[Reg_jump].status = 0;
		}
		else if (New_Code[MIPS_count_line]->Code_type == BNZ)
		{
			int Reg_jump = 0;
			Reg_jump = deal_Temp_register(-1, 2);
			MIPS_code << "bne $t" << Reg_jump << ", " << "$0 " << New_Code[MIPS_count_line]->Label << endl << endl;
			Temp_Register[Reg_jump].status = 0;
		}
		else if (New_Code[MIPS_count_line]->Code_type == GOTO)
		{
            Save_Reg(0);         //如果是for循环的跳转，要向前跳转，所以需要存储寄存器
			MIPS_code << "j " << New_Code[MIPS_count_line]->Label << endl << endl;
		}
		else if (New_Code[MIPS_count_line]->Code_type == With_Label)
		{
			MIPS_code << endl << New_Code[MIPS_count_line]->Label << endl;
		}
		else if (New_Code[MIPS_count_line]->Code_type == array_number)
		{
			mips_array_number();
		}
		//如果是取数组的值的话，需要先算地址，然后lw
		else if (New_Code[MIPS_count_line]->Code_type == number_array)
		{
			mips_number_array();
		}
		else if (New_Code[MIPS_count_line]->Code_type == funct_declare)
		{
			mips_funct_declare();
		}
		else if (New_Code[MIPS_count_line]->Code_type == push)
		{
			mips_push();
		}
		else if (New_Code[MIPS_count_line]->Code_type == funct_call)          //如果是函数的调用，那么就需要新开运行栈
		{
			mips_funct_call();
		}
		else if (New_Code[MIPS_count_line]->Code_type == funct_return)
		{
			mips_funct_return();
		}
		else if (New_Code[MIPS_count_line]->Code_type == funct_result)
		{
			mips_funct_result();
		}
		else if (New_Code[MIPS_count_line]->Code_type == Negate)
		{
			mips_Negate();
		}
		else if (New_Code[MIPS_count_line]->Code_type == read_code)
		{
			mips_read_code();
		}
		else if (New_Code[MIPS_count_line]->Code_type == write_code)
		{
			mips_write_code();
		}
		if(MIPS_count_line < New_Code_Line-1 && New_Code[MIPS_count_line+1]->Code_type == With_Label)
		{
			Save_Reg(0);
		}
	}
}

