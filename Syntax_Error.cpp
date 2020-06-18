#include "Syntax_analysis.h"

void deal_error(int error_num)
{
	switch (error_num)
	{
	case 0:	Syntax_Result << "Identifier type, neither int nor char in Line " << Count_line << endl;
	case 1:	Syntax_Result << "Here should be the identifier in Line " << Count_line << endl;
	case 2:	Syntax_Result << "'=' error, here should be = in Line " << Count_line << endl;
	case 3:	Syntax_Result << "When an integer type is declared as a constant, it is not an integer in Line " << Count_line << endl;
	case 4:	Syntax_Result << "Global variable duplicate name in Line " << Count_line << endl;
	case 5:	Syntax_Result << "Local variable duplicate name in Line " << Count_line << endl;
	case 6:	Syntax_Result << "Character type constant definition is not a character type read " << Count_line << endl;
	case 7:	Syntax_Result << "The stage of the array definition can only be represented by numbers in Line " << Count_line << endl;
	case 8:	Syntax_Result << "The right bracket of the array does not match in Line " << Count_line << endl;
	case 9:	Syntax_Result << "Unknown symbol in constant declaration or variable declaration, neither ',' nor ';' in Line " << Count_line << endl;
	case 10: Syntax_Result << "The function name is not an identifier or an 'int main', 'char main' error in Line " << Count_line << endl;
	case 11: Syntax_Result << "Global variable function name conflict in Line " << Count_line << endl;
	case 12: Syntax_Result << "The type of function read is incorrect, neither void nor int, nor char in Line " << Count_line << endl;
	case 13: Syntax_Result << "Left parenthesis does not match in Line " << Count_line << endl;
	case 14: Syntax_Result << "Left brace error in Line " << Count_line << endl;
	case 15: Syntax_Result << "',' comma or right parenthesis ')' error in parameter list in Line " << Count_line << endl;
	case 16: Syntax_Result << "Right parenthesis error in Line " << Count_line << endl;
	case 17: Syntax_Result << "Error in the while keyword in Do__while in Line " << Count_line << endl;
	case 18: Syntax_Result << "Undefined identifier, first used in Line" << Count_line << endl;
	case 19: Syntax_Result << "Unrecognized characters appear in the factor, strange characters in Line " << Count_line << endl;
	case 20: Syntax_Result << "The '+' and '-' numbers in the for loop are incorrect in Line " << Count_line << endl;
	case 21: Syntax_Result << "The step size in the for loop is not an unsigned integer in Line " << Count_line << endl;
	case 22: Syntax_Result << "The semicolon after the statement is wrong in Line " << Count_line << endl;
	case 23: Syntax_Result << "The end of some statements is not a semicolon in Line" << Count_line << endl;
	case 24: Syntax_Result << "An unrecognized identifier appears at the beginning of the statement or in the statement in Line" << Count_line << endl;
	case 25: Syntax_Result << "Inconsistent identifier in the for statement in Line " <<Count_line << endl;
	case 26: Syntax_Result << "Assignment statement assigns an int data type to a char data type in Line" << Count_line<<endl;
	case 27: Syntax_Result << "When the function is called, the parameter data type does not match. in Line" << Count_line <<endl;
	case 28: Syntax_Result << "The loop identifier in the for statement can only be of type int in Line " << Count_line <<endl;
	case 29: Syntax_Result << "Illegal function call, in Line " << Count_line<<endl;
	case 30: Syntax_Result << "The number of parameters is too few in Line " << Count_line << endl;
	case 31: Syntax_Result << "There is no \"main\" function, or there is a function after the \"main\" function in Line " << Count_line <<endl;
	case 32: Syntax_Result << "The data type returned by the function is incorrect. in Line " << Count_line << endl;
	case 33: Syntax_Result << "Function with return value does not return data in Line " << Count_line << endl;
	case 34: Syntax_Result << "A function without a return value returns a value in Line " << Count_line << endl;
	}
}
