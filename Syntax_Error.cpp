#include "Syntax_analysis.h"
#include <iostream>
//#include <cstring.h>
#include <string.h>
using namespace std;


void deal_error(int error_num)
{
    WITHER_WRONG = 1;
    cout << line << endl;
	switch (error_num)
	{
	case 0:	Wrong << "Identifier type, neither int nor char in Line " << Count_line << endl;
	case 1:	Wrong << "Here should be the identifier in Line " << Count_line << endl;
	case 2:	Wrong << "'=' error, here should be = in Line " << Count_line << endl;
	case 3:	Wrong << "When an integer type is declared as a constant, it is not an integer in Line " << Count_line << endl;
	case 4:	Wrong << "Global variable duplicate name in Line " << Count_line << endl;
	case 5:	Wrong << "Local variable duplicate name in Line " << Count_line << endl;
	case 6:	Wrong << "Character type constant definition is not a character type read " << Count_line << endl;
	case 7:	Wrong << "The stage of the array definition can only be represented by numbers in Line " << Count_line << endl;
	case 8:	Wrong << "The right bracket of the array does not match in Line " << Count_line << endl;
	case 9:	Wrong << "Unknown symbol in constant declaration or variable declaration, neither ',' nor ';' in Line " << Count_line << endl;
	case 10: Wrong << "The function name is not an identifier or an 'int main', 'char main' error in Line " << Count_line << endl;
	case 11: Wrong << "Global variable function name conflict in Line " << Count_line << endl;
	case 12: Wrong << "The type of function read is incorrect, neither void nor int, nor char in Line " << Count_line << endl;
	case 13: Wrong << "Left parenthesis does not match in Line " << Count_line << endl;
	case 14: Wrong << "Left brace error in Line " << Count_line << endl;
	case 15: Wrong << "',' comma or right parenthesis ')' error in parameter list in Line " << Count_line << endl;
	case 16: Wrong << "Right parenthesis error in Line " << Count_line << endl;
	case 17: Wrong << "Error in the while keyword in Do__while in Line " << Count_line << endl;
	case 18: Wrong << "Undefined identifier, first used in Line " << Count_line << endl;
	case 19: Wrong << "Unrecognized characters appear in the factor, strange characters in Line " << Count_line << endl;
	case 20: Wrong << "The '+' and '-' numbers in the for loop are incorrect in Line " << Count_line << endl;
	case 21: Wrong << "The step size in the for loop is not an unsigned integer in Line " << Count_line << endl;
	case 22: Wrong << "The semicolon after the statement is wrong in Line " << Count_line << endl;
	case 23: Wrong << "The end of some statements is not a semicolon in Line" << Count_line << endl;
	case 24: Wrong << "An unrecognized identifier appears at the beginning of the statement or in the statement in Line" << Count_line << endl;
	case 25: Wrong << "Inconsistent identifier in the for statement in Line " <<Count_line << endl;
	case 26: Wrong << "Assignment statement assigns an int data type to a char data type in Line" << Count_line<<endl;
	case 27: Wrong << "When the function is called, the parameter data type does not match. in Line" << Count_line <<endl;
	case 28: Wrong << "The loop identifier in the for statement can only be of type int in Line " << Count_line <<endl;
	case 29: Wrong << "Illegal function call, in Line " << Count_line<<endl;
	case 30: Wrong << "The number of parameters is not right in Line " << Count_line << endl;
	case 31: Wrong << "There is no \"main\" function, or there is a function after the \"main\" function in Line " << Count_line <<endl;
	case 32: Wrong << "The data type returned by the function is incorrect. in Line " << Count_line << endl;
	case 33: Wrong << "Function with return value does not return data in Line " << Count_line << endl;
	case 34: Wrong << "A function without a return value returns a value in Line " << Count_line << endl;
	case 35: Wrong << "Constant is assigned in Line " << Count_line << endl;
	case 36: Wrong << " Judgment condition has characters in Line " << Count_line << endl;
	case 37: Wrong << "The array is out of bounds in Line" << Count_line << endl;
	case 38: Wrong << "Array subscript is not an integer in Line " << Count_line << endl;
	case 39: Wrong << "Right parenthesis does not match in Line " << Count_line << endl;
	}
}
