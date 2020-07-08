#include "Syntax_analysis.h"
#include "symbol_table.h"
#include <iostream>
#include <string.h>
#include "New_Quadruple.h"
using namespace std;

void output_Quaternary()
{
    int count_line = 0;
    ofstream Quaternary("Quaternary.txt");
    if(New_Code_Line > 0 && New_Code[New_Code_Line-1]->Code_type != funct_return)
    {
        Intermediate_Code temp_Code = {};
        temp_Code.Code_type = funct_return;
        temp_Code.number1_lc = -1;
        temp_Code.global_1 = 2;
        New_QuardLine(temp_Code);
    }

    for (count_line = 0; count_line < New_Code_Line; count_line++)
    {
        if (New_Code[count_line]->Code_type == funct_declare)
        {
            Quaternary << New_Code[count_line]->Label << ":" << endl;
        }
        else if (New_Code[count_line]->Code_type == para_declare)
        {
            if (Partial_symbol_table[New_Code[count_line]->number1_lc]->data_type == 1)
            {
                Quaternary << "para " << "int " << Partial_symbol_table[New_Code[count_line]->number1_lc]->name << endl;
            }
            else
            {
                Quaternary << "para " << "char " << Partial_symbol_table[New_Code[count_line]->number1_lc]->name << endl;
            }

        }
        else if (New_Code[count_line]->Code_type == push)
        {
            if (New_Code[count_line]->global_1 == 0)    //局部变量
            {
                Quaternary << "push " << Partial_symbol_table[New_Code[count_line]->number1_lc]->name << endl;
            }
            else if (New_Code[count_line]->global_1 == 1)       //全局变量
            {
                Quaternary << "push " << Global_symbol_table[New_Code[count_line]->number1_lc]->name << endl;
            }
            else if (New_Code[count_line]->global_1 == 2)          //临时变量
            {
                Quaternary << "push t" << New_Code[count_line]->number1_lc << endl;
            }
            else if (New_Code[count_line]->global_1 == 3)          //临时变量
            {
                Quaternary << "push " << New_Code[count_line]->number1_lc << endl;
            }
        }
        else if (New_Code[count_line]->Code_type == funct_call)
        {
            Quaternary << "call  " << New_Code[count_line]->Label << ";" << endl;
        }
        else if (New_Code[count_line]->Code_type == funct_result)
        {
            Quaternary << "t" << New_Code[count_line]->number1_lc << " = " << "RET" << endl;
        }
        else if (New_Code[count_line]->Code_type == funct_return)
        {
            Quaternary << "return " ;
            if (New_Code[count_line]->global_1 == 0)
            {
                Quaternary << Partial_symbol_table[New_Code[count_line]->number1_lc]->name << endl;;
            }
            else if (New_Code[count_line]->global_1 == 1)           //1表示是全局变量
            {
                Quaternary << Global_symbol_table[New_Code[count_line]->number1_lc]->name << endl;;
            }
            else if (New_Code[count_line]->global_1 == 2)
            {
                Quaternary << "t" << New_Code[count_line]->number1_lc << endl;;
            }
            else
            {
                Quaternary << New_Code[count_line]->number1_lc << endl;;
            }
        }
        else if(New_Code[count_line]->Code_type == endow_assign)
        {
            if (New_Code[count_line]->global_1 == 0)
            {
                Quaternary << Partial_symbol_table[New_Code[count_line]->number1_lc]->name << " = ";
            }
            else if (New_Code[count_line]->global_1 == 1)
            {
                Quaternary << Global_symbol_table[New_Code[count_line]->number1_lc]->name << " = ";
            }
            else
            {
                Quaternary << "t" << New_Code[count_line]->number1_lc << " = " ;
            }
            /**************************************************************************************************/
            /**************************************************************************************************/
            if (New_Code[count_line]->global_2 == 0)
            {
                Quaternary << Partial_symbol_table[New_Code[count_line]->number2_lc]->name;
            }
            else if (New_Code[count_line]->global_2 == 1)           //1表示是全局变量
            {
                Quaternary << Global_symbol_table[New_Code[count_line]->number2_lc]->name;
            }
            else if (New_Code[count_line]->global_2 == 2)
            {
                Quaternary << "t" << New_Code[count_line]->number2_lc;
            }
            else
            {
                Quaternary << New_Code[count_line]->number2_lc;
            }
            Quaternary << endl;
        }
        else if (New_Code[count_line]->Code_type == expression)
        {
            if (New_Code[count_line]->global_1 == 0)
            {
                Quaternary << Partial_symbol_table[New_Code[count_line]->number1_lc]->name << " = ";
            }
            else if (New_Code[count_line]->global_1 == 1)
            {
                Quaternary << Global_symbol_table[New_Code[count_line]->number1_lc]->name << " = ";
            }
            else
            {
                Quaternary << "t" << New_Code[count_line]->number1_lc << " = " ;
            }
            /**************************************************************************************************/
            /**************************************************************************************************/
            if (New_Code[count_line]->global_2 == 0)
            {
                Quaternary << Partial_symbol_table[New_Code[count_line]->number2_lc]->name;
            }
            else if (New_Code[count_line]->global_2 == 1)           //1表示是全局变量
            {
                Quaternary << Global_symbol_table[New_Code[count_line]->number2_lc]->name;
            }
            else if (New_Code[count_line]->global_2 == 2)
            {
                Quaternary << "t" << New_Code[count_line]->number2_lc;
            }
            else
            {
                Quaternary << New_Code[count_line]->number2_lc;
            }
            /**************************************************************************************************/
            /**************************************************************************************************/
            if (New_Code[count_line]->operate_cal == 1)
            {
                Quaternary << " + ";
            }
            else if (New_Code[count_line]->operate_cal == 2)
            {
                Quaternary << " - ";
            }
            else if (New_Code[count_line]->operate_cal == 3)
            {
                Quaternary << " * ";
            }
            else if (New_Code[count_line]->operate_cal == 4)
            {
                Quaternary << " / ";
            }
            else if(New_Code[count_line]->operate_cal == 6)
            {
                Quaternary << endl;
                continue;
            }
            /**************************************************************************************************/
            /**************************************************************************************************/
            if (New_Code[count_line]->global_3 == 0)
            {
                Quaternary << Partial_symbol_table[New_Code[count_line]->number3_lc]->name << endl;;
            }
            else if (New_Code[count_line]->global_3 == 1)           //1表示是全局变量
            {
                Quaternary << Global_symbol_table[New_Code[count_line]->number3_lc]->name << endl;;
            }
            else if (New_Code[count_line]->global_3 == 2)
            {
                Quaternary << "t" << New_Code[count_line]->number3_lc << endl;;
            }
            else
            {
                Quaternary << New_Code[count_line]->number3_lc << endl;;
            }
        }
        else if (New_Code[count_line]->Code_type == Con_judge)
        {
            if (New_Code[count_line]->global_2 == 0)
            {
                Quaternary << Partial_symbol_table[New_Code[count_line]->number2_lc]->name;
            }
            else if (New_Code[count_line]->global_2 == 1)           //1表示是全局变量
            {
                Quaternary << Global_symbol_table[New_Code[count_line]->number2_lc]->name;
            }
            else if (New_Code[count_line]->global_2 == 2)
            {
                Quaternary << "t" << New_Code[count_line]->number2_lc;
            }
            else
            {
                Quaternary << New_Code[count_line]->number2_lc;
            }
            /**************************************************************************************************/
            /**************************************************************************************************/
            switch (New_Code[count_line]->operate_cal)
            {
            case 7: Quaternary << " == "; break;
            case 8: Quaternary << " != "; break;
            case 9: Quaternary << " >= "; break;
            case 10: Quaternary << " > "; break;
            case 11: Quaternary << " <= "; break;
            case 12: Quaternary << " < "; break;
            case 13: Quaternary << " != 0" << endl; break;
            }
            /**************************************************************************************************/
            /**************************************************************************************************/
            if (New_Code[count_line]->operate_cal != 13)
            {
                if (New_Code[count_line]->global_3 == 0)
                {
                    Quaternary << Partial_symbol_table[New_Code[count_line]->number3_lc]->name << endl;
                }
                else if (New_Code[count_line]->global_3 == 1)           //1表示是全局变量
                {
                    Quaternary << Global_symbol_table[New_Code[count_line]->number3_lc]->name << endl;
                }
                else if (New_Code[count_line]->global_3 == 2)
                {
                    Quaternary << "t" << New_Code[count_line]->number3_lc << endl;;
                }
                else
                {
                    Quaternary << New_Code[count_line]->number3_lc << endl;
                }

            }
        }
        else if (New_Code[count_line]->Code_type == GOTO)
        {
            Quaternary << "GOTO " << New_Code[count_line]->Label << endl;
        }
        else if (New_Code[count_line]->Code_type == BNZ)
        {
            Quaternary << "BNZ " << New_Code[count_line]->Label << endl;
        }
        else if (New_Code[count_line]->Code_type == BZ)
        {
            Quaternary << "BZ " << New_Code[count_line]->Label << endl;
        }
        else if (New_Code[count_line]->Code_type == With_Label)
        {
            Quaternary << New_Code[count_line]->Label << endl;
        }
        else if (New_Code[count_line]->Code_type == array_number)           //数组的小标只能是临时变量或者全局变量
        {
            if (New_Code[count_line]->global_1 == 0)
            {
                Quaternary << Partial_symbol_table[New_Code[count_line]->number1_lc]->name;
            }
            else if (New_Code[count_line]->global_1 == 1)           //1表示是全局变量
            {
                Quaternary << Global_symbol_table[New_Code[count_line]->number1_lc]->name;
            }
            Quaternary << "[";
            /**************************************************************************************************/
            /**************************************************************************************************/
            if (New_Code[count_line]->global_2 == 0)
            {
                Quaternary << Partial_symbol_table[New_Code[count_line]->number2_lc]->name;
            }
            else if (New_Code[count_line]->global_2 == 1)           //1表示是全局变量
            {
                Quaternary << Global_symbol_table[New_Code[count_line]->number2_lc]->name;
            }
            else if (New_Code[count_line]->global_2 == 2)
            {
                Quaternary << "t" << New_Code[count_line]->number2_lc;
            }
            else
            {
                Quaternary << New_Code[count_line]->number2_lc;
            }
            Quaternary << "] = ";
            /**************************************************************************************************/
            /**************************************************************************************************/
            if (New_Code[count_line]->global_3 == 0)
            {
                Quaternary << Partial_symbol_table[New_Code[count_line]->number3_lc]->name << endl;
            }
            else if (New_Code[count_line]->global_3 == 1)           //1表示是全局变量
            {
                Quaternary << Global_symbol_table[New_Code[count_line]->number3_lc]->name << endl;
            }
            else if (New_Code[count_line]->global_3 == 2)
            {
                Quaternary << "t" << New_Code[count_line]->number3_lc << endl;
            }
            else
            {
                Quaternary << New_Code[count_line]->number3_lc << endl;
            }

        }
        /*################################################################################################################*/
        else if (New_Code[count_line]->Code_type == number_array)
        {
            Quaternary << "t" << New_Code[count_line]->number1_lc << " = ";
            if (New_Code[count_line]->global_2 == 0)
            {
                Quaternary << Partial_symbol_table[New_Code[count_line]->number2_lc]->name;
            }
            else if (New_Code[count_line]->global_2 == 1)           //1表示是全局变量
            {
                Quaternary << Global_symbol_table[New_Code[count_line]->number2_lc]->name;
            }
            else if (New_Code[count_line]->global_2 == 2)
            {
                Quaternary << "t" << New_Code[count_line]->number2_lc;;
            }
            else
            {
                Quaternary << New_Code[count_line]->number2_lc;
            }
            Quaternary << "[" ;
            /**************************************************************************************************/
            /**************************************************************************************************/
            if (New_Code[count_line]->global_3 == 0)
            {
                Quaternary << Partial_symbol_table[New_Code[count_line]->number3_lc]->name ;
            }
            else if (New_Code[count_line]->global_3 == 1)           //1表示是全局变量
            {
                Quaternary << Global_symbol_table[New_Code[count_line]->number3_lc]->name ;
            }
            else if (New_Code[count_line]->global_3 == 2)
            {
                Quaternary << "t" << New_Code[count_line]->number3_lc ;
            }
            else
            {
                Quaternary << New_Code[count_line]->number3_lc ;
            }
            Quaternary << "]" << endl;
        }

        else if(New_Code[count_line]->Code_type == Negate)
        {
            Quaternary << "t" << New_Code[count_line]->number1_lc << " = " << "-";
            if (New_Code[count_line]->global_2 == 0)
            {
                Quaternary << Partial_symbol_table[New_Code[count_line]->number2_lc]->name << endl;
            }
            else if (New_Code[count_line]->global_2 == 1)           //1表示是全局变量
            {
                Quaternary << Global_symbol_table[New_Code[count_line]->number2_lc]->name << endl;
            }
            else if (New_Code[count_line]->global_2 == 2)
            {
                Quaternary << "t" << New_Code[count_line]->number2_lc << endl;;
            }
            else
            {
                Quaternary << New_Code[count_line]->number2_lc << endl;
            }
        }
        else if (New_Code[count_line]->Code_type == read_code)
        {
            Quaternary << "scanf " ;
            if (New_Code[count_line]->global_1 == 0)
            {
                Quaternary << Partial_symbol_table[New_Code[count_line]->number1_lc]->name << endl;
            }
            else if (New_Code[count_line]->global_1 == 1)           //1表示是全局变量
            {
                Quaternary << Global_symbol_table[New_Code[count_line]->number1_lc]->name << endl;
            }
        }
        else if (New_Code[count_line]->Code_type == write_code)
        {
            if (New_Code[count_line]->operate_cal == 14)        //输出字符串
            {
                Quaternary << "printf " << New_Code[count_line]->Label << endl;
            }
            if (New_Code[count_line]->operate_cal == 15)
            {
                Quaternary << "printf ";
                if (New_Code[count_line]->global_1 == 0)
                {
                    Quaternary << Partial_symbol_table[New_Code[count_line]->number1_lc]->name << endl;
                }
                else if (New_Code[count_line]->global_1 == 1)           //1表示是全局变量
                {
                    Quaternary << Global_symbol_table[New_Code[count_line]->number1_lc]->name << endl;
                }
                else if (New_Code[count_line]->global_1 == 2)
                {
                    Quaternary << "t" << New_Code[count_line]->number1_lc << endl;;
                }
                else
                {
                    Quaternary << New_Code[count_line]->number1_lc << endl;
                }
            }
        }

    }
}
