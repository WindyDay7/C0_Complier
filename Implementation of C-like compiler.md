# Implementation of C-like compiler

## Content Structure

```C++
C0_Complier
├─ 1.txt
├─ C0_Complier.cbp
├─ C0_Complier.cscope_file_list
├─ C0_Complier.depend
├─ C0_Complier.layout
├─ Global_variable.h
├─ Lexical.h
├─ Lexical_analysis.cpp
├─ Lexical_func.h
├─ MIPS_code.txt
├─ New_Quadruple.cpp
├─ New_Quadruple.h
├─ Quaternary.txt
├─ Syntax_Error.cpp
├─ Syntax_analysis.cpp
├─ Syntax_analysis.h
├─ Translate_Mips.cpp
├─ Translate_Mips.h
├─ Wrong.txt
├─ bin
│    └─ Debug
│           ├─ C0_Complier.exe
│           ├─ MIPS_code.txt
│           ├─ result.txt
│           └─ syntax_result.txt
├─ cloc-1.64.exe
├─ main.cpp
├─ obj
│    └─ Debug
│           ├─ Lexical_analysis.o
│           ├─ New_Quadruple.o
│           ├─ Syntax_Error.o
│           ├─ Syntax_analysis.o
│           ├─ Translate_Mips.o
│           ├─ main.o
│           ├─ output_Quaternary.o
│           └─ symbol_table.o
├─ output_Quaternary.cpp
├─ result.txt
├─ source_code.txt
├─ symbol_table.cpp
├─ symbol_table.h
├─ syntax_result.txt
├─ test.txt
├─ test2.txt
└─ test3.txt
```

## The main function

Compile most C language programs correctly, able to compile programs into assembly language, assembly language to machine language, to executable file, just the process of translation, so roughly complete a complete C language compiler.

### lexical analysis

Text segmentation and keyword extraction process, Dealing with keywords is a key step in building a symbol table，

For example：

![1593098151415](C:\Users\wf19981020\AppData\Roaming\Typora\typora-user-images\1593098151415.png)

### Syntax analysis

Determine whether there are grammatical errors in the text program, and analyze the function of each line of the program, for example:

![1593098301428](C:\Users\wf19981020\AppData\Roaming\Typora\typora-user-images\1593098301428.png)

### Semantic Analysis and Generate intermediate code

Analyze whether there are semantic errors in the text, and generate intermediate code according to the semantics of the program, In this step, I introduced a lot of variables, a example of this step as below:

![1593125693996](E:\IMG_4252.jpg)

### Generate MIPS assembly code

Combining the computer architecture of MIPS and the virtual space, we can convert the intermediate code into assembly code of MIPS, This step is also the optimization step

![1593126340862](E:\1593126340862.png)

## 生成代码的优化步骤

