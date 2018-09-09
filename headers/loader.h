#ifndef INTERPRETERLOOPNEW_LOADER_H
#define INTERPRETERLOOPNEW_LOADER_H

#define INITIALIZED 1
#define EXECUTING 2
#define ENDED 3

#include "executor.h"

struct Program {
    struct Lexem currentToken;
    char *currentChar;
    char *startChar;
    char *fileNameOutVariables;
    struct LoopStack loopStack;
    struct Variable *variablesPointer;
    int variablesCounter;
};

void readFile(char *fileNameProgram, char *memory);
void execute(struct Program program);

#endif //INTERPRETERLOOPNEW_LOADER_H