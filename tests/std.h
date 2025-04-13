///
/// @file std.h
/// @brief 外部或内置函数头文件
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-09-29
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-09-29 <td>1.0     <td>zenglj  <td>新做
/// </table>
///

#ifndef MINIC_STD_H
#define MINIC_STD_H

/* Input & output functions */
int getint();
int getch();
int getarray(int a[]);

void putint(int a);
void putch(int a);
void putarray(int n, int a[]);

void putstr(char * str);

float getfloat();
int getfarray(float a[]);
void putfloat(float a);
void putfarray(int n, float a[]);
void putf(char a[], ...);

#endif // MINIC_STD_H
