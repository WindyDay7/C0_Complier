#ifndef NEW_QUADRUPLE_H_INCLUDED
#define NEW_QUADRUPLE_H_INCLUDED

void Change_Quadruple();
void Drop_Quadruple();
void New_QuardLine(Intermediate_Code temp_line);

extern Intermediate_Code* New_Code[4000];
extern Symbol_Table* Temp_symbol_table[2000];
extern int New_Code_Line;
extern int WITHER_WRONG;
#endif // NEW_QUADRUPLE_H_INCLUDED
