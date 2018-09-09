#ifndef INTERPRETERLOOPNEW_TOKENREADER_H
#define INTERPRETERLOOPNEW_TOKENREADER_H

//Типы лексем
#define DELIMITER  1 //Разделитель
#define VARIABLE   2 //Переменная
#define NUMBER     3 //Число
#define COMMAND    4 //Команда

//Внутренние представления лексем
#define LOOP 10
#define DO 11
#define IF 12
#define THEN 13
#define ELSE 14
#define END 15
#define EOL 16
#define FINISHED 17
#define ENDIF 18

int isWhite(char ch);
int isDelim(char ch);
void readToken(struct Program *program);
char *mallocAndCopy(char *start, int name);

#endif //INTERPRETERLOOPNEW_TOKENREADER_H