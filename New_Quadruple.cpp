#include <stdlib.h>
#include <stdio.h>
#include "symbol_table.h"
#include "Syntax_analysis.h"
#include "Translate_Mips.h"
#include "New_Quadruple.h"
#include <iostream>
using namespace std;




void Change_Quadruple()
{
    int change_line = 0 ;
    int Temp_counter = 0;
    if(Inter_Code_Line > 0 && Code_txt[Inter_Code_Line-1]->Code_type != funct_return)
    {
        Intermediate_Code temp_Code = {};
        temp_Code.Code_type = funct_return;
        temp_Code.number1_lc = -1;
        temp_Code.global_1 = 2;
        new_MidLine(temp_Code);
    }
    for (change_line = 0; change_line < Inter_Code_Line; change_line++)
    {
        if(Code_txt[change_line]->Code_type == funct_declare)
        {
            MIPS_funct_addr = Code_txt[change_line]->number1_lc;
            Global_symbol_table[MIPS_funct_addr]->temp_address = Temp_counter+1;
        }
        else if(Code_txt[change_line]->Code_type == funct_return)
        {
            int j = 0;
            j = Global_symbol_table[MIPS_funct_addr]->temp_address;
            for(int i = j; i <= Temp_counter; i++)
            {
                enter_Temp_symbol_table(i, (i-j+1) * 4 , MIPS_funct_addr);
            }
        }
        else if(Code_txt[change_line]->Code_type == expression)
        {
            int temp1 = 0, temp2 = 0, temp3 = 0;
            if(Code_txt[change_line]->global_1 == 2)
            {
                Temp_counter = Code_txt[change_line]->number1_lc;
                if(Code_txt[change_line]->global_2 == 2 && Code_txt[change_line]->number2_lc > Temp_counter)
                {
                    Temp_counter = Code_txt[change_line]->number2_lc;
                }
                if(Code_txt[change_line]->global_3 == 2 && Code_txt[change_line]->number3_lc > Temp_counter)
                {
                    Temp_counter = Code_txt[change_line]->number3_lc;
                }
            }
            else
            {
                if(Code_txt[change_line]->global_2 == 2)
                {
                    Temp_counter = Code_txt[change_line]->number2_lc;
                    if(Code_txt[change_line]->global_3 == 2 && Code_txt[change_line]->number3_lc > Temp_counter)
                    {
                        Temp_counter = Code_txt[change_line]->number3_lc;
                    }
                }
                else
                {
                    if(Code_txt[change_line]->global_3 == 2)
                    {
                        Temp_counter = Code_txt[change_line]->number3_lc;
                    }
                }
            }
        }
        else if(Code_txt[change_line]->Code_type == endow_assign)
        {
            if(Code_txt[change_line]->global_1 == 2)
            {
                Temp_counter = Code_txt[change_line]->number1_lc;
                if(Code_txt[change_line]->global_2 == 2 && Code_txt[change_line]->number2_lc > Temp_counter)
                {
                    Temp_counter = Code_txt[change_line]->number2_lc;
                }
            }
            else
            {
                if(Code_txt[change_line]->global_2 == 2)
                {
                    Temp_counter = Code_txt[change_line]->number2_lc;
                }
            }
        }
        else if(Code_txt[change_line]->Code_type == funct_result)
        {
            Temp_counter = Code_txt[change_line]->number1_lc;
        }
        else if(Code_txt[change_line]->Code_type == Con_judge)
        {
            if(Code_txt[change_line]->global_2 == 2)
            {
                Temp_counter = Code_txt[change_line]->number2_lc;
                if(Code_txt[change_line]->global_3 == 2 && Code_txt[change_line]->number3_lc > Temp_counter)
                {
                    Temp_counter = Code_txt[change_line]->number3_lc;
                }
            }
            else
            {
                if(Code_txt[change_line]->global_3 == 2)
                {
                    Temp_counter = Code_txt[change_line]->number3_lc;
                }
            }

        }
        else if(Code_txt[change_line]->Code_type == array_number)
        {
            if(Code_txt[change_line]->global_2 == 2)
            {
                Temp_counter = Code_txt[change_line]->number2_lc;
                if(Code_txt[change_line]->global_3 == 2 && Code_txt[change_line]->number3_lc > Temp_counter)
                {
                    Temp_counter = Code_txt[change_line]->number3_lc;
                }
            }
            else
            {
                if(Code_txt[change_line]->global_3 == 2)
                {
                    Temp_counter = Code_txt[change_line]->number3_lc;
                }
            }
        }
        else if(Code_txt[change_line]->Code_type == number_array)
        {
            Temp_counter = Code_txt[change_line]->number1_lc;
            if(Code_txt[change_line]->global_3 == 2 && Code_txt[change_line]->number3_lc > Temp_counter)
            {
                Temp_counter = Code_txt[change_line]->number3_lc;
            }
        }

    }
}


void New_QuardLine(Intermediate_Code temp_line)
{
	New_Code[New_Code_Line] = (Intermediate_Code*)malloc(sizeof(Intermediate_Code));
	(*New_Code[New_Code_Line]) = temp_line;
	New_Code_Line += 1;		//这一行确定了
}


void Drop_Quadruple()
{
    int change_line = 0, New_line_num = 0;
    int Temp_counter = 0;
    for(change_line = 0; change_line < Inter_Code_Line; change_line++)
    {
        if(Code_txt[change_line]->Code_type == expression)
        {
            int Flag = 0;
            Intermediate_Code temp_Code1 = {}, temp_Code2 = {};
            if(Code_txt[change_line]->global_2 == 3 && Code_txt[change_line]->global_3 == 3 && Code_txt[change_line]->number3_lc != 0)
            {
                Code_txt[change_line]->global_2 = 3;
                switch (Code_txt[change_line]->operate_cal)
                {
                case 1: Code_txt[change_line]->number2_lc = Code_txt[change_line]->number2_lc + Code_txt[change_line]->number3_lc; break;
                case 2: Code_txt[change_line]->number2_lc = Code_txt[change_line]->number2_lc - Code_txt[change_line]->number3_lc; break;
                case 3: Code_txt[change_line]->number2_lc = Code_txt[change_line]->number2_lc * Code_txt[change_line]->number3_lc; break;
                case 4: Code_txt[change_line]->number2_lc = Code_txt[change_line]->number2_lc / Code_txt[change_line]->number3_lc; break;
                }

                Code_txt[change_line]->Code_type = endow_assign;
                temp_Code1.Code_type = endow_assign;
                temp_Code1.global_1 = Code_txt[change_line]->global_1;
                temp_Code1.number1_lc = Code_txt[change_line]->number1_lc;
                temp_Code1.global_2 = Code_txt[change_line]->global_2;
                temp_Code1.number2_lc = Code_txt[change_line]->number2_lc;
                Flag = 1;
            }
            if(change_line < Inter_Code_Line - 1 && Code_txt[change_line + 1]->Code_type == endow_assign)
            {
                if(Code_txt[change_line]->number1_lc == Code_txt[change_line+1]->number2_lc && Code_txt[change_line]->global_1 == Code_txt[change_line+1]->global_2)
                {
                    if(Code_txt[change_line]->Code_type == endow_assign)
                    {
                        temp_Code2.Code_type = endow_assign;
                        temp_Code2.global_1 = Code_txt[change_line + 1]->global_1;
                        temp_Code2.number1_lc = Code_txt[change_line + 1]->number1_lc;
                        temp_Code2.global_2 = Code_txt[change_line]->global_2;
                        temp_Code2.number2_lc = Code_txt[change_line]->number2_lc;
                    }
                    else
                    {
                        temp_Code2.Code_type = expression;
                        temp_Code2.global_1 = Code_txt[change_line + 1]->global_1;
                        temp_Code2.number1_lc = Code_txt[change_line + 1]->number1_lc;
                        temp_Code2.global_2 = Code_txt[change_line]->global_2;
                        temp_Code2.number2_lc = Code_txt[change_line]->number2_lc;
                        temp_Code2.global_3 = Code_txt[change_line]->global_3;
                        temp_Code2.number3_lc = Code_txt[change_line]->number3_lc;
                        temp_Code2.operate_cal = Code_txt[change_line]->operate_cal;

                    }
                    New_QuardLine(temp_Code2);
                    change_line += 1;
                }
                else
                {
                    if(Flag == 1)
                    {
                        New_QuardLine(temp_Code1);
                    }
                    else
                    {
                        New_QuardLine(*Code_txt[change_line]);
                    }

                }
            }
            else
            {
                New_QuardLine(*Code_txt[change_line]);
            }
        }
        else if(Code_txt[change_line]->Code_type == endow_assign)
        {
            Intermediate_Code temp_Code2 = {};
            if(change_line < Inter_Code_Line - 1 && Code_txt[change_line + 1]->Code_type == endow_assign && Code_txt[change_line]->global_1 == 2)
            {
                if(Code_txt[change_line]->number1_lc == Code_txt[change_line+1]->number2_lc && Code_txt[change_line]->global_1 == Code_txt[change_line+1]->global_2)
                {
                    temp_Code2.Code_type = endow_assign;
                    temp_Code2.global_1 = Code_txt[change_line + 1]->global_1;
                    temp_Code2.number1_lc = Code_txt[change_line + 1]->number1_lc;
                    temp_Code2.global_2 = Code_txt[change_line]->global_2;
                    temp_Code2.number2_lc = Code_txt[change_line]->number2_lc;
                    New_QuardLine(temp_Code2);
                    change_line += 1;
                }
                else
                {
                    New_QuardLine(*Code_txt[change_line]);
                }
            }
            else
            {
                New_QuardLine(*Code_txt[change_line]);
            }
        }
        else
        {
            New_QuardLine(*Code_txt[change_line]);
        }
    }
}
