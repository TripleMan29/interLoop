#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <mem.h>
#include <ctype.h>

#include "headers/loader.h"
#include "headers/tokenReader.h"
#include "headers/utilities.h"
#include "headers/expressions.h"
#include "headers/loop.h"
#include "headers/variables.h"

void putBack(struct Program *program);
void executeToken(struct Program *program);
void endProgram(struct Program *program);
void executeLoop(struct Program *program);
void executeEnd(struct Program *program);
void executeIf(struct Program *program);
void executeToElseOrEndif(struct Program *program);
void skipToElseOrEndif(struct Program *program);
void skipToEndif(struct Program *program);
void assignment(struct Program *program);


void execute(struct Program program) {

    struct LoopStack loopStack;
    loopStack.maxNesting = MAX_NESTING;
    loopStack.nestingLevel = 0;

    program.loopStack = loopStack;

    while (program.currentToken.id != FINISHED){
        readToken(&program);
        executeToken(&program);
    }

    endProgram(&program);

}

void endProgram(struct Program *program) {

    if (program->loopStack.nestingLevel != 0){
        printError("Loop is not closed with \"END\"");
    }

    writeResult(program->fileNameOutVariables, program);

    free(program->currentToken.name);
    free(program->loopStack.stack);
    free(program->startChar);
    //free(program->variablesPointer);
}

void executeToken(struct Program *program) {
    if (program->currentToken.id == FINISHED){
        endProgram(program);
    }

    if (program->currentToken.type == VARIABLE) {
        assignment(program);
    }

    if (program->currentToken.type == COMMAND) {
        switch (program->currentToken.id) {
            case LOOP:
                executeLoop(program);
                break;
            case IF:
                executeIf(program);
                break;
            case END:
                executeEnd(program);
            default:
                break;
        }
    }
}

void assignment(struct Program *program) {
    int value;
    struct Variable *var;
    if ((var = findVariable(program->currentToken.name, program)) == NULL)
        var = addVariable(program->currentToken.name, program);
    readToken(program); //Считываем равно
    calcExpression(&value, program); //Вычисляем выражение
    var->value = value;
}

void executeIf(struct Program *program) {
    int leftExp, rightExp, condition;
    char operation;
    calcExpression(&leftExp, program); //Получаем левое выражение

    if (!strchr("=<>", *program->currentToken.name)) {
        printError("Syntax error: \"=, <, >\" expected."); //Недопустимый оператор
        return;
    }

    operation = *program->currentToken.name;
    calcExpression(&rightExp, program);  //Получаем правое выражение

    putBack(program);
    condition = 0;

    switch (operation) {
        case '=':
            if (leftExp == rightExp) condition = 1;
            break;
        case '<':
            if (leftExp < rightExp) condition = 1;
            break;
        case '>':
            if (leftExp > rightExp) condition = 1;
            break;
        default:
            break;
    }
    if (condition) {  //Если значение IF "истина"
        if (program->currentToken.id != THEN) {
            printError("Syntax error: THEN expected.");
        }
        executeToElseOrEndif(program);
    } else {
        skipToElseOrEndif(program);
    }
}

void skipToElseOrEndif(struct Program *program) {
    readToken(program);
    int count = 0;
    do {
        if (program->currentToken.id == FINISHED){
            printError("ENDIF expected.");
        }
        if (program->currentToken.id == IF)
            count++;
        if (program->currentToken.id == ENDIF)
            count--;
        if (count < 0)
            printError("Syntax error: ENDIF doesn't match closing construction");
        readToken(program);
    } while ((program->currentToken.id != ENDIF || count!= 0) && (program->currentToken.id != ELSE || count != 0));
    if (program->currentToken.id == ENDIF)
        return;
    do {
        readToken(program);
        executeToken(program);
    } while (program->currentToken.id != ENDIF);
}

void executeToElseOrEndif(struct Program *program) {
    readToken(program);
    do {
        if (program->currentToken.id == FINISHED){
            printError("ENDIF expected.");
        }
        executeToken(program);
        readToken(program);
    } while (program->currentToken.id != ENDIF && program->currentToken.id != ELSE);
    if (program->currentToken.id == ELSE){
        skipToEndif(program);
    }
}

void skipToEndif(struct Program *program) {
    int count = 0;
    while (program->currentToken.id != ENDIF){
        if (program->currentToken.id == IF)
            count++;
        if (program->currentToken.id == ENDIF)
            count--;
        if (count < 0)
            printError("Syntax error: ENDIF doesn't match closing construction");
        readToken(program);
    }
}

void putBack(struct Program *program) {
    char *t;
    t = program->currentToken.name;
    for (; *t; t++) program->currentChar--;
}

void executeEnd(struct Program *program) {
    struct Loop loop;

    loop = loopPop(program);

    loop.source++;
    if (loop.source > loop.target)
        return;
    loopPush(loop, program);
    program->currentChar = loop.cycleStart;
}

void executeLoop(struct Program *program) {
    struct Loop currentLoop;

    //readToken(); //Чтение переменной-счётчика

    currentLoop.source = 1;

    int loop_iterations = 0;

    calcExpression(&loop_iterations, program);

    currentLoop.target = loop_iterations;

    if (currentLoop.target < currentLoop.source){
        int counter = 0;

        do {

            switch (program->currentToken.id){
                case FINISHED:
                    printError("Syntax error.");
                    break;
                case LOOP:
                    counter++;
                    break;
                case END:
                    counter--;
                    break;
                default:
                    break;
            }
            readToken(program);

        } while (program->currentToken.id != END || counter != 0);

        return;
    }

    //readToken();
    if (program->currentToken.id != DO)
        printError("Expected \"DO\" in LOOP construction");
    currentLoop.cycleStart = program->currentChar;
    loopPush(currentLoop, program);

}

