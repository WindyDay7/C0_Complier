#ifndef TRANSLATE_MIPS_H_INCLUDED
#define TRANSLATE_MIPS_H_INCLUDED
#include "Lexical_func.h"
#include <fstream>
using namespace std;


void Translate();
int look_Temp_Register();
int look_Temp_Register(int loction_num, int global_num);
int enter_temp_register(int loc_num, int glo_num);
int deal_Temp_register(int num_location, int num_global);

void Save_Reg();
void Pass_On();
void Initial_Reg();


void mips_funct_declare();
void mips_push();
void mips_funct_call();
void mips_funct_result();
void mips_funct_return();
void mips_cal_expression();
void mips_endow_expression();
void mips_Con_judge();
void mips_array_number();
void mips_number_array();
void mips_Negate();
void mips_read_code();
void mips_write_code();


extern Register Temp_Register[10];
extern Register Global_Register[8];
extern Register Function_Register[4];
extern Register try1[100][2];
extern ofstream MIPS_code;
extern int MIPS_funct_addr;
extern int MIPS_count_line;
#endif // TRANSLATE_MIPS_H_INCLUDED
