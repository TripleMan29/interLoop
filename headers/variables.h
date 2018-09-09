#ifndef INTERPRETERLOOPNEW_VARIABLES_H
#define INTERPRETERLOOPNEW_VARIABLES_H

struct Variable *findVariable(char *name, struct Program *program);
struct Variable *addVariable(char *name, struct Program *program);

#endif //INTERPRETERLOOPNEW_VARIABLES_H
