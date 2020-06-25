#ifndef TRANSLATE_MIPS_H_INCLUDED
#define TRANSLATE_MIPS_H_INCLUDED
#include "Lexical_func.h"
#include <fstream>
using namespace std;


void Translate();

int look_Temp_Register(int loction_num, int global_num);
int enter_temp_register(int loc_num, int glo_num);
int deal_Temp_register(int num_location, int num_global);
int look_Global_Register(int number_in, int global_in);
int enter_global_register(int loc_num, int glo_num, int if_new);
int deal_Global_register(int num_location, int num_global);


void Save_Reg(int IF_BACK);
void Fetch_Reg();
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
extern Register Funct_Para_List[500][30];
extern Register Function_Register[4];
extern ofstream MIPS_code;
extern int MIPS_funct_addr;
extern int MIPS_count_line;
extern int Last_unlucky_Temp;
extern int Last_unlucky_Global ;
extern int WITHER_WRONG;
#endif // TRANSLATE_MIPS_H_INCLUDED
