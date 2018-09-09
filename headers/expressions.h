#ifndef INTERPRETERLOOPNEW_EXPRESSIONS_H
#define INTERPRETERLOOPNEW_EXPRESSIONS_H

#include "loader.h"

void calcExpression(int *result, struct Program *program);
void multOrDiv(int *result, struct Program *program);
void addOrSub(int *result, struct Program *program);
void parentheses(int *result, struct Program *program);
void unary(int *result, struct Program *program);
void arithmetic(char operation, int *leftPart, const int *rightPart);

#endif //INTERPRETERLOOPNEW_EXPRESSIONS_H
