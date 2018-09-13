#include <mem.h>
#include <ctype.h>
#include <stdlib.h>

#include "headers/loader.h"
#include "headers/executor.h"
#include "headers/utilities.h"
#include "headers/tokenReader.h"

#define TOKEN program->currentToken
#define CHAR program->currentChar

struct Command commands[] = {
        "LOOP", LOOP,
        "DO", DO,
        "IF", IF,
        "THEN", THEN,
        "ELSE", ELSE,
        "END", END,
        "ENDIF", ENDIF
};

int isWhite(char c) {
    if (c == ' ' || c == '\t') return 1;
    else return 0;
}

int isDelim(char c) {
    if (strchr(" :+-=/%*()", c) || c == '\r' || c == '\n')
        return 1;
    return 0;
}

void readToken(struct Program *program) {
    free(TOKEN.name);
    TOKEN.id = 0;
    TOKEN.type = 0;

    while (isWhite(*CHAR))
        CHAR++;

    //Проверка конца программы
    if (*CHAR == '\0') {
        TOKEN.id = FINISHED;
        TOKEN.type = DELIMITER;
        return;
    }

    //Проверка конца строки
    if (*CHAR == '\n') {
        CHAR++;
        TOKEN.id = EOL;
        TOKEN.type = DELIMITER;
        return;
    }

    char *tempStart = CHAR;

    //Првоерка разделителя
    if (strchr(":=+-*/%()<>", *CHAR)) {
        //Ищем знак присваивания
        if (*CHAR == ':') {
            CHAR++;
            if (*CHAR == '=') {
                TOKEN.name = mallocAndCopy(tempStart, 2);
                CHAR++;
            } else
                printError("\":=\" expected.");
        } else {
            TOKEN.name = mallocAndCopy(tempStart, 1);
            CHAR++;
        }
        TOKEN.type = DELIMITER;
        return;
    }

    //Проверка на число
    if (isdigit(*CHAR)) {
        int counter = 0;
        while (!isDelim(*CHAR)){
            CHAR++;
            counter++;
        }
        TOKEN.name = mallocAndCopy(tempStart, counter);
        TOKEN.type = NUMBER;
        return;
    }

    //Проверка на букву
    if (isalpha(*CHAR)) {
        int counter = 0;
        while (!isDelim(*CHAR)){
            CHAR++;
            counter++;
        }
        TOKEN.name = mallocAndCopy(tempStart, counter);
        TOKEN.id = getIdCommand(TOKEN.name);
        if (!TOKEN.id)
            TOKEN.type = VARIABLE;
        else
            TOKEN.type = COMMAND;
        return;
    }

    printError("Syntax error! Unknown token.");

}

char *mallocAndCopy(char *source, int steps) {
    char *resultPointer = (char*) malloc((size_t) sizeof(char)*(steps + 1));
    char *tempPointer = resultPointer;
    for (int i = 0; i < steps; i++) {
        *tempPointer = *source;
        tempPointer++;
        source++;
    }
    *tempPointer = '\0';
    return resultPointer;
}

int getIdCommand(char *command) {
    for (int i = 0; *commands[i].name; i++) {
        if (!strcmp(commands[i].name, command))
            return commands[i].id;
    }
    return 0;
}