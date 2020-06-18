#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "symbol_table.h"
#include "Syntax_analysis.h"
#include "Translate_Mips.h"
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

int enter_global_register(int loc_num, int glo_num, int func_addr, int if_new)
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
        Global_Register[empty_num].Parent_num = func_addr;
        Global_Register[empty_num].status = 1;
        // 登录寄存器的时候要 li ，表示登录寄存器
        if (glo_num == 0 && if_new==1)           //这是局部变量，,并且是从内存去数据到寄存器
        {
            if (Partial_symbol_table[loc_num]->type_information == constant_kind)
            {
                if (Partial_symbol_table[loc_num]->data_type == 1)
                {
                    MIPS_code << "li $s" << empty_num << ", " << Partial_symbol_table[loc_num]->const_int << endl;
                }
                else
                {
                    MIPS_code << "li $s" << empty_num << ", " << (int)Partial_symbol_table[loc_num]->const_char << endl;
                }
            }
            else
            {
                MIPS_code << "lw $s" << empty_num << ", " << -Partial_symbol_table[loc_num]->address - 4 << "($fp)" << endl;
            }
        }
        else if (glo_num == 1 && if_new == 1)          //如果是全局变量,并且是从内存去数据到寄存器
        {
            if (Global_symbol_table[loc_num]->type_information == constant_kind)
            {
                if (Global_symbol_table[loc_num]->data_type == 1)
                {
                    MIPS_code << "li $s" << empty_num << ", " << Global_symbol_table[loc_num]->const_int << endl;
                }
                else
                {
                    MIPS_code << "li $s" << empty_num << ", " << (int)Global_symbol_table[loc_num]->const_char << endl;
                }
            }
            else
            {
                MIPS_code << "lw $s" << empty_num << ", " << Global_symbol_table[loc_num]->address << endl;
            }
        }
        else if(glo_num == 1)
        {
            if (Global_symbol_table[loc_num]->type_information == constant_kind)
            {
                if (Global_symbol_table[loc_num]->data_type == 1)
                {
                    MIPS_code << "li $s" << empty_num << ", " << Global_symbol_table[loc_num]->const_int << endl;
                }
                else
                {
                    MIPS_code << "li $s" << empty_num << ", " << (int)Global_symbol_table[loc_num]->const_char << endl;
                }
            }

        }
        else if(glo_num == 0)
        {
            if(Partial_symbol_table[loc_num]->type_information == constant_kind)
            {
                if (Partial_symbol_table[loc_num]->data_type == 1)
                {
                    MIPS_code << "li $s" << empty_num << ", " << Partial_symbol_table[loc_num]->const_int << endl;
                }
                else
                {
                    MIPS_code << "li $s" << empty_num << ", " << (int)Partial_symbol_table[loc_num]->const_char << endl;
                }
            }
        }

        return empty_num;
    }
    return empty_num;
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
        if (Temp_Register[empty_num].global_in == 3)
        {
            MIPS_code << "li $t" << empty_num << ", " << loc_num << endl;
        }
    }
    return empty_num;
}

int deal_Global_register(int num_location, int num_global, int func_addr)
{
    int Reg_num = 0, enter_num = 0;
    if (look_Global_Register(num_location, num_global) == -1)
    {
        enter_num = enter_global_register(num_location, num_global, func_addr, 0);
        if (enter_num == 8)
        {
            int unlucky_reg = 0;
            srand((unsigned)time(NULL));
            unlucky_reg = (rand() % (8)) + 0;

            //寄存器满了，要将全局寄存器的一个数据存储到对应的内存中,随机选一个寄存器，存到内存中去把
            if (Global_Register[unlucky_reg].global_in == 1)        //如果这个寄存器存的是全局变量
            {
                MIPS_code << "sw $s" << unlucky_reg << ", " << Global_symbol_table[Global_Register[unlucky_reg].number_in]->address << endl;
                //MIPS_code << "lw $s" << unlucky_reg << ", " << -Global_symbol_table[num_location]->address << endl;
            }
            else        //如果这个寄存器存的是局部变量
            {
                MIPS_code << "sw $s" << unlucky_reg << ", " << -Partial_symbol_table[Global_Register[unlucky_reg].number_in]->address - 4 << "($fp)" << endl;
                //MIPS_code << "lw $s" << unlucky_reg << ", " << -Partial_symbol_table[num_location]->address - 4 << "($fp)" << endl;
            }
            Global_Register[unlucky_reg].status = 0;
            enter_global_register(num_location, num_global, func_addr, 1);
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

int deal_Temp_register(int num_location, int num_global)
{
    int Reg_num = 0, enter_num = 0;
    if (look_Temp_Register(num_location, num_global) == -1)
    {
        enter_num = enter_temp_register(num_location, num_global);
        if (enter_num == 10)
        {
            //几乎不可能
            MIPS_code << "The probability is too small, it is considered not to happen" << endl;
        }
    }
    else
    {
        Reg_num = look_Temp_Register(num_location, num_global);
        return Reg_num;
    }
    return enter_num;
}

void Pass_On()
{
    int i = 0;
    for (i = 0; i < 4 && i < Global_symbol_table[MIPS_funct_addr]->length; i++)
    {
        Global_Register[i].status = 1;
        Global_Register[i].global_in = 0;
        Global_Register[i].number_in = Global_symbol_table[MIPS_funct_addr]->address + i;
        Global_Register[i].Parent_num = MIPS_funct_addr;
        MIPS_code << "move $s" << i << ", " << "$a" << i << endl;
        Function_Register[i].status = 0;
    }
}

void Save_Reg()
{
    int i = 0, k = 0, save_count = 0;
    for (i = 0; i < 8; i++)
    {
        if (i < 8 && Global_Register[i].status == 1 && Global_Register[i].Parent_num == MIPS_funct_addr)
        {
            if (Global_Register[i].global_in == 0)
            {
                //如果是局部变量的话，就放回内存去
                MIPS_code << "sw $s" << i << ", " << -Partial_symbol_table[Global_Register[i].number_in]->address - 4 << "($fp)" << endl;
                //这个寄存器的值保存在内存中了，就不管他事了
            }
            else        //如果是全局变量，那么我要将寄存器存到全局地址
            {
                MIPS_code << "sw $s" << i << ", " << Global_symbol_table[Global_Register[i].number_in]->address << endl;
            }
            save_count += 1;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        Function_Register[i].status = 0;
    }
    return ;
}

void Initial_Reg()
{
    for (int i = 0; i < 8; i++)
    {
        Global_Register[i].status = 0;
    }
    for (int i = 0; i < 4; i++)
    {
        Function_Register[i].status = 0;
    }
}

void mips_funct_declare()
{
    MIPS_funct_addr = Code_txt[MIPS_count_line]->number1_lc;
    MIPS_code << endl << Code_txt[MIPS_count_line]->Label << ":" << endl;
    int para_sapce = 0, variable_space = 0, const_addr = 0, const_space = 0, last_space = 0;
    int const_Reg = 0, out_number = 0;         //存储这个常量的寄存器
    int next_funct_addr = 0, Clean_funt_Reg = 4;        //记录下一个函数的变量开始的地址，不能超过了啊，
    char cur_func[20] = {0}, nume_str[3] = {0};            //表示当前的函数名字
    int SUM_space = 0;
    strcpy(cur_func, Global_symbol_table[Code_txt[MIPS_count_line]->number1_lc]->name);      //call 调用的函数名

    para_sapce = Global_symbol_table[MIPS_funct_addr]->length * 4;
    MIPS_code << "add $fp, $sp, " << para_sapce << endl;
    Initial_Reg();
    Pass_On();
    const_addr = Global_symbol_table[Code_txt[MIPS_count_line]->number1_lc]->address +  Global_symbol_table[MIPS_funct_addr]->length;   //第一个常量在局部符号表的位置
    while (Partial_symbol_table[const_addr]->type_information == constant_kind && strcmp(cur_func, Partial_symbol_table[const_addr]->parent_function) == 0)
    {
        const_space += 4;
        const_Reg = deal_Temp_register(-1, 2);          //将常量存到内存中
        if (Partial_symbol_table[const_addr]->data_type == 1)
        {
            out_number = Partial_symbol_table[const_addr]->const_int;
        }
        else
        {
            out_number = (int)Partial_symbol_table[const_addr]->const_char;
        }
        MIPS_code << "li $t" << const_Reg << ", " << out_number << endl;
        MIPS_code << "sw $t" << const_Reg << ", -" << const_space << "($sp)" << endl;
        const_addr += 1;        //下一个局部常量
    }
    //MIPS_code << "sub $sp, $sp, " << const_space << endl;
    Temp_Register[const_Reg].status = 0;
    while (const_addr < Pstn && strcmp(cur_func, Partial_symbol_table[const_addr]->parent_function) == 0)
    {
        const_addr ++;
    }
    variable_space = Partial_symbol_table[const_addr - 1]->address - para_sapce;
    if (Partial_symbol_table[const_addr - 1]->type_information == array_kind)
    {
        if (Partial_symbol_table[const_addr - 1]->data_type == 1)
        {
            variable_space = variable_space + 4 * Partial_symbol_table[const_addr - 1]->length;
        }
        else
        {
            variable_space = variable_space + Partial_symbol_table[const_addr = 1]->length;
        }
    }
    else
    {
        variable_space = variable_space + 4;
    }
    SUM_space = variable_space + para_sapce + 4;
    Global_symbol_table[MIPS_funct_addr]->const_int = SUM_space;
    MIPS_code << "addi $sp, $sp, " << -variable_space << endl;      //在栈中开一个空间，用来存储变量
    //MIPS_code << para_sapce << endl;
    MIPS_code << "addi $sp, $sp, -4" << endl;
    MIPS_code << "sw $ra, 0($sp)" << endl;
}

void mips_push()
{
    int i = 0, Reg_number[10] = {0}, save_num = 0;
    int Reg_para = 0, temp_line = 0;

    temp_line = MIPS_count_line;
    if (Code_txt[temp_line - 1]->Code_type != push)
    {
        while (Code_txt[temp_line]->Code_type == push)
        {
            if (Code_txt[temp_line]->global_1 == 2)       //如果是临时变量的话，
            {
                int j = 0;
                for (j = 0; j < 10; j++)
                {
                    if (Temp_Register[j].number_in == Code_txt[temp_line]->number1_lc)
                        break;
                }
                Reg_number[j] = 1;
            }
            temp_line += 1;
        }
        for (i = 0; i < 10; i++)
        {
            if (Temp_Register[i].status == 1 && Reg_number[i] == 0)     //如果临时寄存器没有被使用过，并且也不会在push阶段被使用，那么就要存起来
            {
                MIPS_code << "addi $sp, $sp, -4" << endl;
                MIPS_code << "sw $t" << i << ", " << "0($sp)" << endl;
                Global_symbol_table[MIPS_funct_addr]->const_int = Global_symbol_table[MIPS_funct_addr]->const_int + 4;
                save_num += 1;
            }
        }
        Global_symbol_table[MIPS_funct_addr]->data_type = 4 * save_num;     //MIPS_funct_addr存储当前编译函数在全局符号表的位置
    }
    i = 0;
    while (i < 4  && Function_Register[i].status == 1)
    {
        i++;
    }
    if (i == 4)     //等于4表示4个函数寄存器都用上了，这时函数的参数要使用内存
    {
        MIPS_code << "addi $sp, $sp, -4" << endl;
        MIPS_code << "sw ";
        if (Code_txt[MIPS_count_line]->global_1 == 2 || Code_txt[MIPS_count_line]->global_1 == 3)
        {
            Reg_para = deal_Temp_register(Code_txt[MIPS_count_line]->number1_lc, Code_txt[MIPS_count_line]->global_1);
            MIPS_code << "$t" << Reg_para << ", 0($sp)" << endl;
        }
        else
        {
            Reg_para = deal_Global_register(Code_txt[MIPS_count_line]->number1_lc, Code_txt[MIPS_count_line]->global_1, MIPS_funct_addr);
            MIPS_code << "$s" << Reg_para << ", 0($sp)" << endl;
        }
    }
    else        //直接使用
    {
        MIPS_code << "addi $sp, $sp, -4" << endl;

        if (Code_txt[MIPS_count_line]->global_1 == 2 || Code_txt[MIPS_count_line]->global_1 == 3)
        {
            Reg_para = deal_Temp_register(Code_txt[MIPS_count_line]->number1_lc, Code_txt[MIPS_count_line]->global_1);
            MIPS_code << "move $a" << i << ", $t" << Reg_para << endl;
        }
        else
        {
            Reg_para = deal_Global_register(Code_txt[MIPS_count_line]->number1_lc, Code_txt[MIPS_count_line]->global_1, MIPS_funct_addr);
            MIPS_code << "move $a" << i << ", $s" << Reg_para << endl;
        }
        Function_Register[i].status = 1;
    }
    if (Code_txt[MIPS_count_line]->global_1 == 2 || Code_txt[MIPS_count_line]->global_1 == 3)
    {
        Temp_Register[Reg_para].status = 0;
    }
}

void mips_funct_call()
{
    int save_num = 0, i = 0, Reg_number[10] = {0}, j=0;
    MIPS_code << endl;
    int Clean_funt_Reg = 0, para_sapce = 0, temp_line =0;
    //我们首先要清空$s 寄存器，，然后我们将$a 寄存器的值，赋给空的 $s寄存器
    if(Global_symbol_table[Code_txt[MIPS_count_line]->number1_lc]->length == 0)
    //如果是无参数的函数调用，那么没有push，也就没有存储$t寄存器
    {
        for (i = 0; i < 10; i++)
        {
            if (Temp_Register[i].status == 1)     //如果$t寄存器还未被使用过，要存起来
            {
                MIPS_code << "addi $sp, $sp, -4" << endl;
                MIPS_code << "sw $t" << i << ", " << "0($sp)" << endl;
                Global_symbol_table[MIPS_funct_addr]->const_int = Global_symbol_table[MIPS_funct_addr]->const_int + 4;
                save_num += 1;
            }
        }
        Global_symbol_table[MIPS_funct_addr]->data_type = 4 * save_num;     //MIPS_funct_addr存储当前编译函数在全局符号表的位置
    }
    // 此时全局变量中data_type存储的是临时寄存器占的空间
    Save_Reg();
    MIPS_code << "jal " << Code_txt[MIPS_count_line]->Label << endl ;
    MIPS_code << endl;
    MIPS_code << "add $fp, $fp, " << Global_symbol_table[MIPS_funct_addr]->const_int << endl;
    for (i = 0; i < 8; i++)
    {
        if (i < 8 && Global_Register[i].status == 1 && Global_Register[i].Parent_num == MIPS_funct_addr)
        {
            if (Global_Register[i].global_in == 0)
            {
                //如果是局部变量的话，就放回内存去
                MIPS_code << "lw $s" << i << ", " << -Partial_symbol_table[Global_Register[i].number_in]->address - 4 << "($fp)" << endl;
                //这个寄存器的值保存在内存中了，就不管他事了
            }
            else        //如果是全局变量，那么我要将寄存器存到全局地址
            {
                MIPS_code << "lw $s" << i << ", " << Global_symbol_table[Global_Register[i].number_in]->address << endl;
            }
        }
    }
    for (i = 0; i < 10; i++)
    {
        if (Temp_Register[i].status == 1)
        {
            MIPS_code << "lw $t" << i << ", " << 4 * j << "($sp)" << endl;
            j++;
            MIPS_code << "addi $sp, $sp, 4" << endl;
            Global_symbol_table[MIPS_funct_addr]->const_int = Global_symbol_table[MIPS_funct_addr]->const_int - 4;
        }
    }
}

void mips_funct_result()
{
    int Reg_return = 0, i = 0, j = 0;

    if (Code_txt[MIPS_count_line]->global_1 == 2 || Code_txt[MIPS_count_line]->global_1 == 3)
    {
        Reg_return = deal_Temp_register(Code_txt[MIPS_count_line]->number1_lc, Code_txt[MIPS_count_line]->global_1);
        MIPS_code << "move $t" << Reg_return << ", $v1" << endl;
    }
    else
    {
        Reg_return = deal_Global_register(Code_txt[MIPS_count_line]->number1_lc, Code_txt[MIPS_count_line]->global_1, MIPS_funct_addr);
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
    if (Code_txt[MIPS_count_line]->number1_lc != -1)
    {

        if (Code_txt[MIPS_count_line]->global_1 == 2 || Code_txt[MIPS_count_line]->global_1 == 3)
        {
            Reg_return = deal_Temp_register(Code_txt[MIPS_count_line]->number1_lc, Code_txt[MIPS_count_line]->global_1);
            MIPS_code << "move $v1" << ", $t" << Reg_return << endl;
            Temp_Register[Reg_return].status = 0;
        }
        else
        {
            Reg_return = deal_Global_register(Code_txt[MIPS_count_line]->number1_lc, Code_txt[MIPS_count_line]->global_1, MIPS_funct_addr);
            MIPS_code << "move $v1" << ", $s" << Reg_return << endl;
        }
    }
    MIPS_code << "lw $ra, " << 0 << "($sp)" << endl;

    MIPS_code << "move $sp, $fp" << endl;
    //MIPS_code << "add $fp, $fp, " << Global_symbol_table[MIPS_funct_addr]->const_int << endl;
    MIPS_code << "jr $ra" << endl;
}

void mips_cal_expression()
{
    int Reg_data1 = 0, Reg_data2 = 0, Reg_result = 0;
    char Temp_MIPS_Code [30] = {0}, Temp_Reg_num[3] = {0};

    switch (Code_txt[MIPS_count_line]->operate_cal)
    {
    case 1: strcpy(Temp_MIPS_Code, "add "); break;
    case 2: strcpy(Temp_MIPS_Code, "sub "); break;
    case 3: strcpy(Temp_MIPS_Code, "mul "); break;
    case 4: strcpy(Temp_MIPS_Code, "div "); break;
    }
    if (Code_txt[MIPS_count_line]->global_1 == 0 || Code_txt[MIPS_count_line]->global_1 == 1)
    {
        Reg_result = deal_Global_register(Code_txt[MIPS_count_line]->number1_lc, Code_txt[MIPS_count_line]->global_1, MIPS_funct_addr);
        //MIPS_code << "$s" << Reg_result ;
        strcat(Temp_MIPS_Code, "$s");
        sprintf(Temp_Reg_num, "%d", Reg_result);
        strcat(Temp_MIPS_Code, Temp_Reg_num);
    }
    else
    {
        Reg_result = deal_Temp_register(Code_txt[MIPS_count_line]->number1_lc, 2);
        //MIPS_code << "$t" << Reg_result ;           //将计算结果设为寄存器
        strcat(Temp_MIPS_Code, "$t");
        sprintf(Temp_Reg_num, "%d", Reg_result);
        strcat(Temp_MIPS_Code, Temp_Reg_num);
    }
    /*######################################################################################################*/
    /*######################################################################################################*/
    //如果第一个计算数是临时变量或者是立即数，那么用临时寄存器存储
    if (Code_txt[MIPS_count_line]->global_2 == 2 || Code_txt[MIPS_count_line]->global_2 == 3)
    {
        Reg_data1 = deal_Temp_register(Code_txt[MIPS_count_line]->number2_lc, Code_txt[MIPS_count_line]->global_2);
        //MIPS_code << ", $t" << Reg_data1 << ", ";
        strcat(Temp_MIPS_Code, ", $t");
        sprintf(Temp_Reg_num, "%d", Reg_data1);
        strcat(Temp_MIPS_Code, Temp_Reg_num);
    }
    else        //否则用全局寄存器存储
    {
        Reg_data1 = deal_Global_register(Code_txt[MIPS_count_line]->number2_lc, Code_txt[MIPS_count_line]->global_2, MIPS_funct_addr);
        //MIPS_code << ", $s" << Reg_data1 << ", ";
        strcat(Temp_MIPS_Code, ", $s");
        sprintf(Temp_Reg_num, "%d", Reg_data1);
        strcat(Temp_MIPS_Code, Temp_Reg_num);
    }

    //如果第三个变量是临时变量或者是立即数，那么用临时寄存器存储
    if (Code_txt[MIPS_count_line]->global_3 == 2 || Code_txt[MIPS_count_line]->global_3 == 3)
    {
        Reg_data2 = deal_Temp_register(Code_txt[MIPS_count_line]->number3_lc, Code_txt[MIPS_count_line]->global_3);
        //MIPS_code << "$t" << Reg_data2 << endl;
        strcat(Temp_MIPS_Code, ", $t");
        sprintf(Temp_Reg_num, "%d", Reg_data2);
        strcat(Temp_MIPS_Code, Temp_Reg_num);
    }
    else
    {
        Reg_data2 = deal_Global_register(Code_txt[MIPS_count_line]->number3_lc, Code_txt[MIPS_count_line]->global_3, MIPS_funct_addr);
        //MIPS_code << "$s" << Reg_data2 << endl;
        strcat(Temp_MIPS_Code, ", $s");
        sprintf(Temp_Reg_num, "%d", Reg_data2);
        strcat(Temp_MIPS_Code, Temp_Reg_num);
    }

    //为三个操作数分配寄存器
    if (Code_txt[MIPS_count_line]->global_2 == 2 || Code_txt[MIPS_count_line]->global_2 == 3)
    {
        Temp_Register[Reg_data1].status = 0;
    }
    if (Code_txt[MIPS_count_line]->global_3 == 2 || Code_txt[MIPS_count_line]->global_3 == 3)
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
    if (Code_txt[MIPS_count_line]->global_1 == 0 || Code_txt[MIPS_count_line]->global_1 == 1)
    {
        Reg_result = deal_Global_register(Code_txt[MIPS_count_line]->number1_lc, Code_txt[MIPS_count_line]->global_1, MIPS_funct_addr);
        strcat(Temp_MIPS_Code, "$s");
        sprintf(Temp_Reg_num, "%d", Reg_result);
        strcat(Temp_MIPS_Code, Temp_Reg_num);
    }
    else
    {
        Reg_result = deal_Temp_register(Code_txt[MIPS_count_line]->number1_lc, 2);
        //将计算结果设为寄存器
        strcat(Temp_MIPS_Code, "$t");
        sprintf(Temp_Reg_num, "%d", Reg_result);
        strcat(Temp_MIPS_Code, Temp_Reg_num);
    }
    /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
    if (Code_txt[MIPS_count_line]->global_2 == 2 || Code_txt[MIPS_count_line]->global_2 == 3)
    {
        Reg_data2 = deal_Temp_register(Code_txt[MIPS_count_line]->number2_lc, Code_txt[MIPS_count_line]->global_2);
        //MIPS_code << ", $t" << Reg_data2 << endl;
        strcat(Temp_MIPS_Code, ", $t");
        sprintf(Temp_Reg_num, "%d", Reg_data2);
        strcat(Temp_MIPS_Code, Temp_Reg_num);
    }
    else
    {
        Reg_data2 = deal_Global_register(Code_txt[MIPS_count_line]->number2_lc, Code_txt[MIPS_count_line]->global_2, MIPS_funct_addr);
        //MIPS_code << ", $s" << Reg_data2 << endl;
        strcat(Temp_MIPS_Code, ", $s");
        sprintf(Temp_Reg_num, "%d", Reg_data2);
        strcat(Temp_MIPS_Code, Temp_Reg_num);
    }
    if (Code_txt[MIPS_count_line]->global_2 == 2 || Code_txt[MIPS_count_line]->global_2 == 3)
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
    switch (Code_txt[MIPS_count_line]->operate_cal)
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
    if (Code_txt[MIPS_count_line]->global_2 == 2 || Code_txt[MIPS_count_line]->global_2 == 3)
    {
        Reg_data2 = deal_Temp_register(Code_txt[MIPS_count_line]->number2_lc, Code_txt[MIPS_count_line]->global_2);
        strcat(Temp_MIPS_Code, ", $t");
        sprintf(Temp_Reg_num, "%d", Reg_data2);
        strcat(Temp_MIPS_Code, Temp_Reg_num);
    }
    else
    {
        Reg_data2 = deal_Global_register(Code_txt[MIPS_count_line]->number2_lc, Code_txt[MIPS_count_line]->global_2, MIPS_funct_addr);
        strcat(Temp_MIPS_Code, ", $s");
        sprintf(Temp_Reg_num, "%d", Reg_data2);
        strcat(Temp_MIPS_Code, Temp_Reg_num);
    }

    //第一个比较数字处理完，要看是不是只有一个表达式的条件
    if (Code_txt[MIPS_count_line]->operate_cal == 13)
    {
        MIPS_code << Temp_MIPS_Code << ", $0" << endl;
        return ;
    }
    /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
    if (Code_txt[MIPS_count_line]->global_3 == 2 || Code_txt[MIPS_count_line]->global_3 == 3)
    {
        Reg_data3 = deal_Temp_register(Code_txt[MIPS_count_line]->number3_lc, Code_txt[MIPS_count_line]->global_3);
        strcat(Temp_MIPS_Code, ", $t");
        sprintf(Temp_Reg_num, "%d", Reg_data3);
        strcat(Temp_MIPS_Code, Temp_Reg_num);
    }
    else
    {
        Reg_data3 = deal_Global_register(Code_txt[MIPS_count_line]->number3_lc, Code_txt[MIPS_count_line]->global_3, MIPS_funct_addr);
        strcat(Temp_MIPS_Code, ", $s");
        sprintf(Temp_Reg_num, "%d", Reg_data3);
        strcat(Temp_MIPS_Code, Temp_Reg_num);
    }
    MIPS_code << Temp_MIPS_Code << endl;

    if (Code_txt[MIPS_count_line]->global_2 == 2 || Code_txt[MIPS_count_line]->global_2 == 3)
    {
        Temp_Register[Reg_data2].status = 0;
    }
    if (Code_txt[MIPS_count_line]->global_3 == 2 || Code_txt[MIPS_count_line]->global_3 == 3)
    {
        Temp_Register[Reg_data3].status = 0;
    }
}

void mips_array_number()
{
    int Reg_Array = 0, Reg_data2 = 0, Reg_data3 = 0;
    char Reg_2[3] = {0}, Reg_3[3] = {0};
    //不管怎么样，数组中与结果的表达式都是要算的，所以先算好把
    if (Code_txt[MIPS_count_line]->global_2 == 2 || Code_txt[MIPS_count_line]->global_2 == 3)
    {
        Reg_data2 = deal_Temp_register(Code_txt[MIPS_count_line]->number2_lc, Code_txt[MIPS_count_line]->global_2);
        strcpy(Reg_2, "$t");
    }
    else
    {
        Reg_data2 = deal_Global_register(Code_txt[MIPS_count_line]->number2_lc, Code_txt[MIPS_count_line]->global_2, MIPS_funct_addr);
        strcpy(Reg_2, "$s");
    }
    //计算第二个操作数的寄存器
    if (Code_txt[MIPS_count_line]->global_3 == 2 || Code_txt[MIPS_count_line]->global_3 == 3)
    {
        Reg_data3 = deal_Temp_register(Code_txt[MIPS_count_line]->number3_lc, Code_txt[MIPS_count_line]->global_3);
        strcpy(Reg_3, "$t");
    }
    else
    {
        Reg_data3 = deal_Global_register(Code_txt[MIPS_count_line]->number3_lc, Code_txt[MIPS_count_line]->global_3, MIPS_funct_addr);
        strcpy(Reg_3, "$s");
    }
    //计算第三个操作数的寄存器

    if (Code_txt[MIPS_count_line]->global_1 == 1)    //如果是全局数组的话，全局变量
    {
        Reg_Array = deal_Temp_register(-1, 2);
        MIPS_code << "li $t" << Reg_Array << ", " << Global_symbol_table[Code_txt[MIPS_count_line]->number1_lc]->address << endl;
        //将组数的下表地址存到临时寄存器中
        if (Global_symbol_table[Code_txt[MIPS_count_line]->number1_lc]->data_type == 1)      //如果是全局整型数组的话
        {
            MIPS_code << "sll " << Reg_2 << Reg_data2 << ", " << Reg_2 << Reg_data2 << ", 2" << endl;
            MIPS_code << "add $t" << Reg_Array << ", $t" << Reg_Array << ", " << Reg_2 << Reg_data2 << endl;
            MIPS_code << "sw " << Reg_3 << Reg_data3 << ", 0($t" <<  Reg_Array << ")" << endl;
        }
        else        //如果是全局变量，并且是字符型数组的话
        {
            MIPS_code << "add $t" << Reg_Array << ", $t" << Reg_Array << ", " << Reg_2 << Reg_data2 << endl;
            MIPS_code << "sb " << Reg_3 << Reg_data3 << ", 0($t" <<  Reg_Array << ")" << endl;
        }
        Temp_Register[Reg_Array].status = 0;
    }
    else        //如果是局部数组的话，局部变量
    {
        Reg_Array = deal_Temp_register(-1, 2);          //找一个一定是临时的寄存器
        MIPS_code << "add $t" << Reg_Array << ", $fp, " << Partial_symbol_table[Code_txt[MIPS_count_line]->number1_lc]->address << endl;
        if (Partial_symbol_table[Code_txt[MIPS_count_line]->number1_lc]->data_type == 1)      //如果是整型数组的话，下标要乘以4
        {
            MIPS_code << "sll " << Reg_2 << Reg_data2 << ", " << Reg_2 << Reg_data2 << ", 2" << endl;
            MIPS_code << "add $t" << Reg_Array << ", $t" << Reg_Array << ", " << Reg_2 << Reg_data2 << endl;
            MIPS_code << "sw " << Reg_3 << Reg_data3 << ", 0($t" <<  Reg_Array << ")" << endl;
        }
        else        //如果是局部变量，并且是字符型数组的话
        {
            MIPS_code << "add $t" << Reg_Array << ", $t" << Reg_Array << ", " << Reg_2 << Reg_data2 << endl;
            MIPS_code << "sb " << Reg_3 << Reg_data3 << ", 0($t" <<  Reg_Array << ")" << endl;
        }
        Temp_Register[Reg_Array].status = 0;
    }
    if (Code_txt[MIPS_count_line]->global_2 == 2 || Code_txt[MIPS_count_line]->global_2 == 3)
    {
        Temp_Register[Reg_data2].status = 0;
    }
    if (Code_txt[MIPS_count_line]->global_3 == 2 || Code_txt[MIPS_count_line]->global_3 == 3)
    {
        Temp_Register[Reg_data3].status = 0;
    }
}

void mips_number_array()
{
    int Reg_Array = 0, Reg_result = 0, Reg_data = 0;
    char Reg_r[3] = {0}, Reg_d[3] = {0};
    //先算结果类型，就是四元式中的number1
    if (Code_txt[MIPS_count_line]->global_1 == 0 || Code_txt[MIPS_count_line]->global_1 == 1)
    {
        Reg_result = deal_Global_register(Code_txt[MIPS_count_line]->number1_lc, Code_txt[MIPS_count_line]->global_1, MIPS_funct_addr);
        strcpy(Reg_r, "$s");
    }
    else
    {
        Reg_result = deal_Temp_register(Code_txt[MIPS_count_line]->number1_lc, 2);
        //将计算结果设为寄存器
        strcpy(Reg_r, "$t");
    }
    //计算数组中括号中的表达式类型，用寄存器保存起来
    if (Code_txt[MIPS_count_line]->global_3 == 2 || Code_txt[MIPS_count_line]->global_3 == 3)
    {
        Reg_data = deal_Temp_register(Code_txt[MIPS_count_line]->number3_lc, Code_txt[MIPS_count_line]->global_3);
        strcpy(Reg_d, "$t");
    }
    else
    {
        Reg_data = deal_Global_register(Code_txt[MIPS_count_line]->number3_lc, Code_txt[MIPS_count_line]->global_3, MIPS_funct_addr);
        strcpy(Reg_d, "$s");
    }
    //然后开始计算数组的下标，取出下标的值， lw或者lb
    if (Code_txt[MIPS_count_line]->global_2 == 1)        //如果是全局数组，表示是全局数组
    {
        Reg_Array = deal_Temp_register(-1, 2);
        MIPS_code << "li $t" << Reg_Array << ", " << Global_symbol_table[Code_txt[MIPS_count_line]->number2_lc]->address << endl;
        //将组数的下表地址存到临时寄存器中
        if (Global_symbol_table[Code_txt[MIPS_count_line]->number2_lc]->data_type == 1)      //如果是全局整型数组的话
        {
            MIPS_code << "sll " << Reg_d << Reg_data << ", " << Reg_d << Reg_data << ", 2" << endl;
            MIPS_code << "add $t" << Reg_Array << ", $t" << Reg_Array << ", " << Reg_d << Reg_data << endl;
            MIPS_code << "lw " << Reg_r << Reg_result << ", 0($t" <<  Reg_Array << ")" << endl;
        }
        else        //如果是全局变量，并且是字符型数组的话
        {
            MIPS_code << "add $t" << Reg_Array << ", $t" << Reg_Array << ", " << Reg_d << Reg_data << endl;
            MIPS_code << "lb " << Reg_r << Reg_result << ", 0($t" <<  Reg_Array << ")" << endl;
        }
        Temp_Register[Reg_Array].status = 0;
    }
    else            //如果是局部数组变量的话
    {
        Reg_Array = deal_Temp_register(-1, 2);          //找一个一定是临时的寄存器
        MIPS_code << "add $t" << Reg_Array << ", $fp, " << Partial_symbol_table[Code_txt[MIPS_count_line]->number2_lc]->address << endl;
        if (Partial_symbol_table[Code_txt[MIPS_count_line]->number2_lc]->data_type == 1)      //如果是整型数组的话，下标要乘以4
        {
            MIPS_code << "sll " << Reg_d << Reg_data << ", " << Reg_d << Reg_data << ", 2" << endl;
            MIPS_code << "add $t" << Reg_Array << ", $t" << Reg_Array << ", " << Reg_d << Reg_data << endl;
            MIPS_code << "lw " << Reg_r << Reg_result << ", 0($t" <<  Reg_Array << ")" << endl;
        }
        else        //如果是局部变量，并且是字符型数组的话
        {
            MIPS_code << "add $t" << Reg_Array << ", $t" << Reg_Array << ", " << Reg_d << Reg_data << endl;
            MIPS_code << "lb " << Reg_r << Reg_result << ", 0($t" <<  Reg_Array << ")" << endl;
        }
        Temp_Register[Reg_Array].status = 0;
    }
    if (Code_txt[MIPS_count_line]->global_3 == 2 || Code_txt[MIPS_count_line]->global_3 == 3)
    {
        Temp_Register[Reg_data].status = 0;
    }
}

void mips_Negate()
{
    int Reg_Result = 0, Reg_data = 0;
    Reg_Result = deal_Temp_register(Code_txt[MIPS_count_line]->number1_lc, Code_txt[MIPS_count_line]->global_1);
    MIPS_code << "neg $t" << Reg_Result ;

    if (Code_txt[MIPS_count_line]->global_2 == 2 || Code_txt[MIPS_count_line]->global_2 == 3)
    {
        Reg_data = deal_Temp_register(Code_txt[MIPS_count_line]->number2_lc, Code_txt[MIPS_count_line]->global_2);
        MIPS_code << ", $t" << Reg_data << endl;
        Temp_Register[Reg_data].status = 0;
    }
    else
    {
        Reg_data = deal_Global_register(Code_txt[MIPS_count_line]->number2_lc, Code_txt[MIPS_count_line]->global_2, MIPS_funct_addr);
        MIPS_code << ",$s" << Reg_data << endl;
    }

}


void mips_read_code()
{
    int Reg_data = 0;
    if (Code_txt[MIPS_count_line]->operate_cal == 1)       //表示是int类型，
    {
        MIPS_code << "li $v0, 5" << endl << "syscall" << endl;
    }
    else        //表示读入的标识符是char类型
    {
        MIPS_code << "li $v0, 12" << endl << "syscall" << endl;
    }
    //然后判断是全局变量，还是局部变量，
    Reg_data = deal_Global_register(Code_txt[MIPS_count_line]->number1_lc, Code_txt[MIPS_count_line]->global_1, MIPS_funct_addr);
    MIPS_code << "move $s" << Reg_data << ", $v0" << endl;
}

void mips_write_code()
{
    int Reg_data = 0;
    if (Code_txt[MIPS_count_line]->operate_cal == 14)
    {
        MIPS_code << "la $a0, " << Code_txt[MIPS_count_line]->Label << endl;
        MIPS_code << "li $v0, 4" << endl;
        MIPS_code << "syscall" << endl;
    }
    else
    {
        if (Code_txt[MIPS_count_line]->global_1 == 2 || Code_txt[MIPS_count_line]->global_1 == 3)
        {
            Reg_data = deal_Temp_register(Code_txt[MIPS_count_line]->number1_lc, Code_txt[MIPS_count_line]->global_1);
            MIPS_code << "move $a0, $t" << Reg_data << endl;
        }
        else
        {
            Reg_data = deal_Global_register(Code_txt[MIPS_count_line]->number1_lc, Code_txt[MIPS_count_line]->global_1, MIPS_funct_addr);
            MIPS_code << "move $a0, $s" << Reg_data << endl;
        }
        if (Code_txt[MIPS_count_line]->operate_cal == 1)        //如果是整型数字输出的话
        {
            MIPS_code << "li $v0, 1" << endl << "syscall" << endl;
        }
        else        //如果是字符型数据输出的话
        {
            MIPS_code << "li $v0, 11" << endl << "syscall" << endl;
        }
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

    while (Global_symbol_table[temp_number]->type_information == constant_kind)
    {

        int out_number = 0;
        Temp_Register[temp_register_number].status = 1;
        //MIPS_code << Global_symbol_table[temp_number]->name << "  " << Global_symbol_table[temp_number]->const_int << endl;
        if (Global_symbol_table[temp_number]->data_type == 1)
        {
            out_number = Global_symbol_table[temp_number]->const_int;
        }
        else
        {
            out_number = (int)Global_symbol_table[temp_number]->const_char;
        }
        MIPS_code << "li " << "$t" << temp_register_number << " " << out_number << endl;
        MIPS_code << "sw $t" << temp_register_number << ", " << Global_symbol_table[temp_number]->address << endl;
        Temp_Register[temp_register_number].status = 0;
        temp_number ++;
    }

    MIPS_code << "j main" << endl << endl;
    for (MIPS_count_line = 0; MIPS_count_line < Inter_Code_Line; MIPS_count_line++)
    {
        if (Code_txt[MIPS_count_line]->Code_type == expression && Code_txt[MIPS_count_line]->operate_cal != 6)
        {
            mips_cal_expression();
        }
        else if (Code_txt[MIPS_count_line]->Code_type == expression && Code_txt[MIPS_count_line]->operate_cal == 6)
        {
            mips_endow_expression();

        }
        else if (Code_txt[MIPS_count_line]->Code_type == Con_judge)
        {
            mips_Con_judge();
        }
        //如果是条件满足就跳转，那么满足条件就跳转
        else if (Code_txt[MIPS_count_line]->Code_type == BZ)
        {
            int Reg_jump = 0;
            Reg_jump = look_Temp_Register(-1, 2);
            MIPS_code << "beq $t" << Reg_jump << ", " << "$0 " << Code_txt[MIPS_count_line]->Label << endl;
            Temp_Register[Reg_jump].status = 0;
        }
        else if (Code_txt[MIPS_count_line]->Code_type == BNZ)
        {
            int Reg_jump = 0;
            Reg_jump = deal_Temp_register(-1, 2);
            MIPS_code << "bne $t" << Reg_jump << ", " << "$0 " << Code_txt[MIPS_count_line]->Label << endl;
            Temp_Register[Reg_jump].status = 0;
        }
        else if (Code_txt[MIPS_count_line]->Code_type == GOTO)
        {
            MIPS_code << "j " << Code_txt[MIPS_count_line]->Label << endl;
        }
        else if (Code_txt[MIPS_count_line]->Code_type == With_Label)
        {
            MIPS_code << endl << Code_txt[MIPS_count_line]->Label << endl;
        }
        else if (Code_txt[MIPS_count_line]->Code_type == array_number)
        {
            mips_array_number();
        }
        //如果是取数组的值的话，需要先算地址，然后lw
        else if (Code_txt[MIPS_count_line]->Code_type == number_array)
        {
            mips_number_array();
        }
        else if (Code_txt[MIPS_count_line]->Code_type == funct_declare)
        {
            mips_funct_declare();
        }
        else if (Code_txt[MIPS_count_line]->Code_type == push)
        {
            mips_push();
        }
        else if (Code_txt[MIPS_count_line]->Code_type == funct_call)          //如果是函数的调用，那么就需要新开运行栈
        {
            mips_funct_call();
        }
        else if (Code_txt[MIPS_count_line]->Code_type == funct_return)
        {
            mips_funct_return();
        }
        else if (Code_txt[MIPS_count_line]->Code_type == funct_result)
        {
            mips_funct_result();
        }
        else if (Code_txt[MIPS_count_line]->Code_type == Negate)
        {
            mips_Negate();
        }
        else if (Code_txt[MIPS_count_line]->Code_type == read_code)
        {
            mips_read_code();
        }
        else if (Code_txt[MIPS_count_line]->Code_type == write_code)
        {
            mips_write_code();
        }
    }
}
