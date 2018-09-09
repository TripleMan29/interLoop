#include <stdlib.h>
#include "headers/tokenReader.h"
#include "headers/utilities.h"
#include "headers/variables.h"
#include "headers/executor.h"
#include "headers/expressions.h"

#define TOKEN program->currentToken

void calcExpression(int *result, struct Program *program) {
    readToken(program);
    addOrSub(result, program);
}

void addOrSub(int *result, struct Program *program) {
    char operation;
    multOrDiv(result, program);

    int hold;
    while ((operation = *TOKEN.name) == '+' || operation == '-') {
        readToken(program);
        multOrDiv(&hold, program);
        arithmetic(operation, result, &hold);
    }
}

void multOrDiv(int *result, struct Program *program) {
    char operation;
    unary(result, program);

    int hold;
    while ((operation = *TOKEN.name) == '/' || operation == '%' || operation == '*') {
        readToken(program);
        unary(&hold, program);
        arithmetic(operation, result, &hold);
    }
}

void unary(int *result, struct Program *program) {
    char operation;
    if (TOKEN.type == DELIMITER && (operation = *TOKEN.name) == '+' || operation == '-') {
        readToken(program);
    }
    parentheses(result, program);
    if (operation == '-')
        *result = -(*result);
}

void parentheses(int *result, struct Program *program) {
    char operation = *TOKEN.name;
    if (TOKEN.type == DELIMITER && operation == '(') {
        readToken(program);
        addOrSub(result, program);
        if (*TOKEN.name != ')')
            printError("This is not an expression!"); //TODO
        readToken(program);
    } else {
        struct Variable *temp = findVariable(TOKEN.name, program);
        switch (TOKEN.type) {
            case VARIABLE:
                if (temp == NULL || temp->value == NULL){
                    *result = 0;
                }
                *result = temp->value;
                readToken(program);
                return;
            case NUMBER:
                *result = atoi(TOKEN.name);
                readToken(program);
                return;
            default:
                printError("Syntax error!"); //TODO
        }
    }
}

void arithmetic(char operation, int *leftPart, const int *rightPart) {
    int t;
    switch (operation) {
        case '-':
            *leftPart = *leftPart - *rightPart;
            break;
        case '+':
            *leftPart = *leftPart + *rightPart;
            break;
        case '*':
            *leftPart = *leftPart * *rightPart;
            break;
        case '/':
            *leftPart = (*leftPart) / (*rightPart);
            break;
        case '%':
            t = (*leftPart) / (*rightPart);
            *leftPart = *leftPart - (t * (*rightPart));
            break;
        default:
            break;
    }
}