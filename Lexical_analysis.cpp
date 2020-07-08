#include <stdio.h>
#include <iostream>
#include <string.h>
#include "Lexical.h"
#include <fstream>
#include <stdlib.h>
using namespace std;

void initialize()
{
	strcpy(key_word[0], "char");
	strcpy(key_word[1], "const");
	strcpy(key_word[2], "do");
	strcpy(key_word[3], "else");
	strcpy(key_word[4], "for");
	strcpy(key_word[5], "if");
	strcpy(key_word[6], "int");
	strcpy(key_word[7], "main");
	strcpy(key_word[8], "printf");
	strcpy(key_word[9], "return");
	strcpy(key_word[10], "scanf");
	strcpy(key_word[11], "void");
	strcpy(key_word[12], "while");

	ksy_word[0] = char_sy;
	ksy_word[1] = const_sy;
	ksy_word[2] = do_sy;
	ksy_word[3] = else_sy;
	ksy_word[4] = for_sy;
	ksy_word[5] = if_sy;
	ksy_word[6] = int_sy;
	ksy_word[7] = main_sy;
	ksy_word[8] = printf_sy;
	ksy_word[9] = return_sy;
	ksy_word[10] = scanf_sy;
	ksy_word[11] = void_sy;
	ksy_word[12] = while_sy;

	strcpy(special_char, "*/{}()[],;");
	special_char_num[0] = multiply;
	special_char_num[1] = division;
	special_char_num[2] = l_brace;
	special_char_num[3] = r_brace;
	special_char_num[4] = l_parentheses;
	special_char_num[5] = r_parentheses;
	special_char_num[6] = l_bracket;
	special_char_num[7] = r_bracket;
	special_char_num[8] = comma;
	special_char_num[9] = semicolon;
}

int if_special_char(char if_special_char_in)    //
{
	int i = 0;
	for (i = 0; i < 10; ++i)
	{
		if (special_char[i] == if_special_char_in) {
			return i + 1;
		}
	}
	return -1;
}

char next_char(FILE *read_in)
{
	char temp = 0;
	if (Current_location == Current_line_length) {
		Current_location = 0;
		Current_line_length = 0;
		if (feof(read_in) != 0) {
			if (symbol == r_brace || temp_symbol=='}') {
				SaveFile << "The Source program has been read" << endl;
				return 'a';
			}
			else {
				cout << "Something Wrong in the end of the Source code in Line:" << Count_line << endl;
				exit(-1);
			}
		}
		else {
			Count_line += 1;
			fgets(line, line_longest, read_in);
			Current_line_length = strlen(line);
		}
	}
	temp = line[Current_location];
	Current_location += 1;
	return temp;
}

void Read_until(FILE *read_in)
{

    /*( temp_symbol != '}')
    {
        temp_symbol = next_char(read_in);
        if (feof(read_in) != 0)
        {
            exit(0);
            cout << " The wrong program has been read." << endl;
        }
    }
    symbol = r_brace;*/
    while(temp_symbol != ';' &&  temp_symbol != '}')
    {
        temp_symbol = next_char(read_in);
        if (feof(read_in) != 0)
        {
            exit(0);
            cout << " The wrong program has been read." << endl;
        }
    }
    symbol = semicolon;
}

enumType next_symbol(FILE *read_in)
{

	int temp_ident_length = 0, low = 0, high = 0, mid = 0;
	int number1 = 0;
	while (temp_symbol == ' ' || temp_symbol == 9 || temp_symbol == 10)
	{
		temp_symbol = next_char(read_in);
	}
	temp_ident_length = 0;
	if (('a' <= temp_symbol && temp_symbol <= 'z') || ('A' <= temp_symbol && temp_symbol <= 'Z') || temp_symbol == '_')
	{
		while (('a' <= temp_symbol && temp_symbol <= 'z') || ('A' <= temp_symbol && temp_symbol <= 'Z') || temp_symbol == '_' || ('0' <= temp_symbol && temp_symbol <= '9'))
		{
			temp_ident[temp_ident_length] = temp_symbol;
			temp_ident_length += 1;
			temp_symbol = next_char(read_in);
		}
		temp_ident[temp_ident_length] = '\0';
		low = 0; high = 12;
		while (low <= high)
		{
			mid = (low + high) / 2;
			if (strcmp(temp_ident, key_word[mid]) > 0)
				low = mid + 1;
			else if (strcmp(temp_ident, key_word[mid]) < 0)
				high = mid - 1;
			else {
				SaveFile << key_word[mid] << "_sy" << "  " << key_word[mid] << endl;
				ALL_symbol[symbol_count++] = ksy_word[mid];
				return ksy_word[mid];
			}
		}
		SaveFile << "ident " << temp_ident << endl;
		ALL_symbol[symbol_count++] = ident;
		return ident;
	}
	else if (('1' <= temp_symbol) && (temp_symbol <= '9'))		//
	{
		while (('0' <= temp_symbol) && (temp_symbol <= '9'))
		{
			number1 = number1 * 10 + (temp_symbol - '0');
			temp_symbol = next_char(read_in);
		}
		SaveFile << "un_intcon  " << number1 << endl;
		number = number1;
		ALL_symbol[symbol_count++] = un_intcon;
		return un_intcon;
	}
	else if (temp_symbol == '0') {			//
		SaveFile << "un_intcon  0" << endl;
		temp_symbol = next_char(read_in);
		ALL_symbol[symbol_count++] = un_intcon;
		number = 0;
		return un_intcon;
	}
	else if (temp_symbol == '<') {				//
		temp_symbol = next_char(read_in);
		if (temp_symbol == '=') {
			SaveFile << "ble  <=" << endl;
			temp_symbol = next_char(read_in);
			ALL_symbol[symbol_count++] = ble;
			return ble;
		}
		else {
			SaveFile << "blt  <" << endl;
			ALL_symbol[symbol_count++] = blt;
			return blt;
		}
	}
	else if (temp_symbol == '>') {
		temp_symbol = next_char(read_in);
		if (temp_symbol == '=') {
			temp_symbol = next_char(read_in);
			SaveFile << "bge  >=" << endl;
			ALL_symbol[symbol_count++] = bge;
			return bge;
		}
		else {
			SaveFile << "bgt  >" << endl;
			ALL_symbol[symbol_count++] = bgt;
			return bgt;
		}

	}
	else if (temp_symbol == '+') {				//
		temp_symbol = next_char(read_in);
		if (symbol == ident || symbol == intcon || symbol==un_intcon || symbol==r_bracket || symbol==r_parentheses || symbol==charcon)
		{
			SaveFile << "plus  +" << endl;
			ALL_symbol[symbol_count++] = plus_sign;
			return plus_sign;
		}
		else if (('1' <= temp_symbol) && (temp_symbol <= '9')) {
			while (('0' <= temp_symbol) && (temp_symbol <= '9'))
			{
				number1 = number1 * 10 + (temp_symbol - '0');
				temp_symbol = next_char(read_in);
			}
			SaveFile << "intcon  " << number1 << endl;
			number = number1;
			ALL_symbol[symbol_count++] = intcon;
			return intcon;
		}
		else if (temp_symbol == '0') {
			temp_symbol = next_char(read_in);
			SaveFile << "intcon  0" << endl;
			ALL_symbol[symbol_count++] = intcon;
			number = 0;
			return intcon;
		}
		else {
			SaveFile << "plus  +" << endl;
			ALL_symbol[symbol_count++] = plus_sign;
			return plus_sign;
		}
	}
	else if (temp_symbol == '-') {			//Èç¹ûÊÇ¸ººÅ
		temp_symbol = next_char(read_in);
		if (symbol == ident || symbol == intcon || symbol==un_intcon || symbol==r_bracket || symbol==r_parentheses || symbol == charcon)
		{
			SaveFile << "minus  -" << endl;
			ALL_symbol[symbol_count++] = minus_sign;
			return minus_sign;
		}
		else if (('1' <= temp_symbol) && (temp_symbol <= '9')) {
			while (('0' <= temp_symbol) && (temp_symbol <= '9'))
			{
				number1 = number1 * 10 + (temp_symbol - '0');
				temp_symbol = next_char(read_in);
			}
			SaveFile << "intcon  " << -number1 << endl;
			number = -number1;
			ALL_symbol[symbol_count++] = intcon;
			return intcon;
		}
		else if (temp_symbol == '0') {
			SaveFile << "intcon  0" << endl;
			temp_symbol = next_char(read_in);
			ALL_symbol[symbol_count++] = intcon;
			number = 0;
			return intcon;
		}
		else {
			SaveFile << "minus  -" << endl;
			ALL_symbol[symbol_count++] = minus_sign;
			return minus_sign;
		}
	}
	else if (temp_symbol == '\'') {			//Èç¹ûÊÇ×Ö·û
		temp_symbol = next_char(read_in);
		if ((temp_symbol == '+') || (temp_symbol == '-') || (temp_symbol == '*') || (temp_symbol == '/') || \
		        ('0' <= temp_symbol && temp_symbol <= '9') || ('A' <= temp_symbol && temp_symbol <= 'Z') || \
		        ('a' <= temp_symbol && temp_symbol <= 'z') || (temp_symbol == '_')
		   )
		{
			real_temp_char = temp_symbol;
			temp_symbol = next_char(read_in);
			if (temp_symbol == '\'') {
				SaveFile << "charcon  " << real_temp_char << endl;
				temp_symbol = next_char(read_in);
				ALL_symbol[symbol_count++] = charcon;
				return charcon;
			}
			else {
				cout << "There is more than one character in the char in Source code" << "in Line" << Count_line << endl;
				while (temp_symbol != '\'') {
					temp_symbol = next_char(read_in);
				}
				temp_symbol = next_char(read_in);
				ALL_symbol[symbol_count++] = wrong_sy;
				Read_until(read_in);
				return wrong_sy;
			}
		}
		else {
			while (temp_symbol != '\'') {
				temp_symbol = next_char(read_in);
			}
			temp_symbol = next_char(read_in);
			cout << "Wrong character in char" << "in Line" << Count_line << endl;
			ALL_symbol[symbol_count++] = wrong_sy;
			Read_until(read_in);
			return wrong_sy;
		}
	}
	else if (temp_symbol == '\"') {			//
        char temp_label1[20]={0};
		temp_symbol = next_char(read_in);
		if (temp_symbol == 32 || temp_symbol == 33 || (35 <= temp_symbol && temp_symbol <= 126)) {
			while (temp_symbol == 32 || temp_symbol == 33 || (35 <= temp_symbol && temp_symbol <= 126)) {
			    if(temp_symbol == 92)
			    {
			        temp_ident[temp_ident_length] = temp_symbol;
			        temp_ident_length += 1;
			    }
				temp_ident[temp_ident_length] = temp_symbol;
				temp_symbol = next_char(read_in);
				temp_ident_length += 1;
			}
			temp_ident[temp_ident_length] = '\0';
			// ;
			if (temp_symbol == '\"') {
				SaveFile << "stringcon  " << temp_ident << endl;
				temp_symbol = next_char(read_in);

                sprintf(temp_label1, "%d", sentence_count);
                sentence[sentence_count] = (String_code*)malloc(sizeof(String_code));
                strcpy(sentence[sentence_count]->string_name, "string");

                strcpy(sentence[sentence_count]->string_sce, temp_ident);
                strcat(sentence[sentence_count]->string_name, temp_label1);
                sentence_count += 1;

				return stringcon;
			}
			else {
				while (temp_symbol != '\"') {
					temp_symbol = next_char(read_in);
				}
				temp_symbol = next_char(read_in);
				cout << "The characters in String is wrong in source code" << "in Line" << Count_line << endl;
				ALL_symbol[symbol_count++] = wrong_sy;
				Read_until(read_in);
				return wrong_sy;
			}
		}
		else {
            if(temp_symbol == '\"')
            {
                temp_ident[temp_ident_length] = '\0';
                sprintf(temp_label1, "%d", sentence_count);
                sentence[sentence_count] = (String_code*)malloc(sizeof(String_code));
                strcpy(sentence[sentence_count]->string_name, "string");

                strcpy(sentence[sentence_count]->string_sce, temp_ident);
                strcat(sentence[sentence_count]->string_name, temp_label1);
                sentence_count += 1;
                temp_symbol = next_char(read_in);
                return stringcon;
            }
			while (temp_symbol != '\"') {
				temp_symbol = next_char(read_in);
			}
			temp_symbol = next_char(read_in);
			cout << "The First characters of the String is wrong in source code" << "in Line" << Count_line << endl;
			ALL_symbol[symbol_count++] = wrong_sy;
			Read_until(read_in);
			return wrong_sy;
		}
	}
	else if (temp_symbol == '=') {			//
		temp_symbol = next_char(read_in);
		if (temp_symbol == '=') {
			SaveFile << "beq  ==" << endl;
			temp_symbol = next_char(read_in);
			ALL_symbol[symbol_count++] = beq;
			return beq;
		}
		else {
			SaveFile << "endow  =" << endl;
			ALL_symbol[symbol_count++] = endow;
			return endow;
		}
	}
	else if (temp_symbol == '!') {
		temp_symbol = next_char(read_in);
		if (temp_symbol == '=') {
			SaveFile << "bne  !=" << endl;
			temp_symbol = next_char(read_in);
			ALL_symbol[symbol_count++] = bne;
			return bne;
		}
		else {
			cout << "Something wrong near the '!' in Source_code" << "in Line" << Count_line << endl;
			ALL_symbol[symbol_count++] = wrong_sy;
			Read_until(read_in);
			return wrong_sy;
		}
	}
	else if (if_special_char(temp_symbol) > 0) {
		int number2 = 0;
		number2 = if_special_char(temp_symbol) - 1;
		switch (number2)
		{
		case 0: SaveFile << "multiply  *" << endl; break;
		case 1: SaveFile << "division  /" << endl; break;
		case 2: SaveFile << "l_brace  {" << endl; break;
		case 3: SaveFile << "r_brace  }" << endl; break;
		case 4: SaveFile << "l_parentheses  (" << endl; break;
		case 5: SaveFile << "r_parentheses  )" << endl; break;
		case 6: SaveFile << "l_bracket  [" << endl; break;
		case 7: SaveFile << "r_bracket  ]" << endl; break;
		case 8: SaveFile << "comma  ," << endl; break;
		case 9: SaveFile << "semicolon  ;" << endl; break;
		}
		temp_symbol = next_char(read_in);
		ALL_symbol[symbol_count++] = special_char_num[number2];
		return special_char_num[number2];
	}
	else if (temp_symbol == 0) {
		SaveFile << "Begin lexical analysis" << endl;
		temp_symbol = next_char(read_in);
		ALL_symbol[symbol_count++] = wrong_sy;
		return wrong_sy;
	}
	else {
		temp_symbol = next_char(read_in);
		cout << "Unrecognized character in Line" << Count_line << endl;
		ALL_symbol[symbol_count++] = wrong_sy;
		Read_until(read_in);
		return wrong_sy;
	}
}


