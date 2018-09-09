#include "headers/loader.h"
#include "headers/executor.h"
#include "headers/utilities.h"

void loopPush(struct Loop loop, struct Program *program) {
    if (program->loopStack.nestingLevel > program->loopStack.maxNesting)
        printError("The nesting level of the loop is too large.");

    program->loopStack.stack[program->loopStack.nestingLevel] = loop;
    program->loopStack.nestingLevel++;
}

struct Loop loopPop(struct Program *program) {
    program->loopStack.nestingLevel--;
    if (program->loopStack.nestingLevel < 0)
        printError("END does not match closing construction.");
    return (program->loopStack.stack[program->loopStack.nestingLevel]);
}