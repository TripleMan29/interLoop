#include <mem.h>
#include <stdlib.h>
#include <ctype.h>

#include "headers/loader.h"
#include "headers/utilities.h"
#include "headers/executor.h"
#include "headers/tokenReader.h"

struct Variable *addVariable(char *name, struct Program *program) {
    program->variablesCounter++;
    program->variablesPointer = (struct Variable *) realloc(program->variablesPointer, sizeof(struct Variable) * program->variablesCounter);

    if (program->variablesPointer == NULL)
        printError("Unable to allocate memory!");

    struct Variable *bufferIndex = program->variablesPointer;

    int i = 1;
    while (i < program->variablesCounter) {
        bufferIndex++;
        i++;
    }

    struct Variable *result = bufferIndex;
    result->name = mallocAndCopy(name, lenght(name));
    result->value = 0;

    return result;
}

struct Variable *findVariable(char *name, struct Program *program) {
    if (isdigit(*name)){
        return NULL;
    }
    int i = 1;
    struct Variable *t = program->variablesPointer;
    while (i <= program->variablesCounter) {
        if (!strcmp(name, t->name)) {
            return t;
        }
        i++;
        t++;
    }
    return addVariable(name, program);
}

