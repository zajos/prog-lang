#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#define MAX_IDENT_SIZE 30

typedef enum {
    KEYWORD,
    OPERATOR,
    IDENTIFIER,
    INTEGER,
    STRING,
    END_OF_LINE,
    LEFT_PAR,
    RIGHT_PAR,
    COMMA
} TokenType;
typedef struct {
    TokenType type;
    char* lexeme;
} Token;
typedef enum
{
    INT,
    STR
}VariableType;
typedef struct {
    char* name;
    VariableType type;
    char* value;
} Variable;
size_t varSizes=0;
Variable* variables;
Token* lexTokens;
Token** lines;
size_t tokensSize=0;
int tokensIndex=0;
int lineCount=0;
int* lineSizes;
int isKeyword(char* str)
{
    if(!strcmp(str,"new")||!strcmp(str,"int")||!strcmp(str,"text")||!strcmp(str,"size")||!strcmp(str,"subs")||!strcmp(str,"locate")||!strcmp(str,"insert")||
       !strcmp(str,"override")||!strcmp(str,"read")||!strcmp(str,"write")||!strcmp(str,"from")||!strcmp(str,"to")||!strcmp(str,"input")||!strcmp(str,"output")
       ||!strcmp(str,"asText")||!strcmp(str,"asString")||!strcmp(str,"prompt"))
        {
            return 1;
        }
    return 0;
}
int isVariable(Token token)
{
    for(int i=0;i<varSizes;i++)
        {
            if(token.type==IDENTIFIER&&!strcmp(token.lexeme,variables[i].name))
                {
                    return 1;
                }
        }
    return 0;
}
Variable findVar(Token token)
{
    Variable empty;
    if(isVariable(token))
        {
            for(int i=0;i<varSizes;i++)
                {
                    if(token.lexeme==variables[i].name)
                        {
                            return variables[i];
                        }
                }
        }
    return empty ;
}
int typeCheck(Token token)
{
    for(int i=0;i<varSizes;i++)
        {
            if(token.type==IDENTIFIER&&token.lexeme==variables[i].name&&variables[i].type==STR)
                {
                    return 1;//for String
                }
            else if(token.type==IDENTIFIER&&token.lexeme==variables[i].name&&variables[i].type==INT)
                {
                    return 0;//for Integer
                }
        }
    return -1;//nothing
}
void substring(const char *source, char *destination, int start, int length) {
    strncpy(destination, source + start, length);
    destination[length] = '\0';
}
void overrideTextAtLocation(char* myText, const char* ovrText, int location) {
    int myTextLen = strlen(myText);
    int ovrTextLen = strlen(ovrText);
    int i;

    for (i = 0; i < ovrTextLen; i++) {
        if (location + i >= myTextLen) {
            break;
        }
        myText[location + i] = ovrText[i];
    }
}
void stringSubstract(char *source, const char *item) {
    int sourceLen = strlen(source);
    int itemLen = strlen(item);

    char *finding = strstr(source, item);
    while (finding != NULL) {
        memmove(finding, finding + itemLen, sourceLen - (finding - source) - itemLen + 1);
        finding = strstr(source, item);
    }
}


int lexicalAnalysis(char* sourceText, size_t sourceSize) {
    lexTokens = (Token*)malloc((tokensSize + 1) * sizeof(Token));

    for (int i = 0; i < sourceSize + 1; i++) {
        if (sourceText[i] == ';') {
            tokensSize++;
            lexTokens = (Token*)realloc(lexTokens, (tokensSize + 1) * sizeof(Token));
            lexTokens[tokensIndex].type = END_OF_LINE;
            lexTokens[tokensIndex].lexeme = NULL;
            tokensIndex++;
        } else if (sourceText[i] == '(') {
            tokensSize++;
            lexTokens = (Token*)realloc(lexTokens, (tokensSize + 1) * sizeof(Token));
            lexTokens[tokensIndex].type = LEFT_PAR;
            lexTokens[tokensIndex].lexeme = NULL;
            tokensIndex++;
        } else if (sourceText[i] == ')') {
            tokensSize++;
            lexTokens = (Token*)realloc(lexTokens, (tokensSize + 1) * sizeof(Token));
            lexTokens[tokensIndex].type = RIGHT_PAR;
            lexTokens[tokensIndex].lexeme = NULL;
            tokensIndex++;
        } else if (sourceText[i] == ',') {
            tokensSize++;
            lexTokens = (Token*)realloc(lexTokens, (tokensSize + 1) * sizeof(Token));
            lexTokens[tokensIndex].type = COMMA;
            lexTokens[tokensIndex].lexeme = NULL;
            tokensIndex++;
        } else if ((sourceText[i] == ':' && sourceText[i + 1] == '=') || sourceText[i] == '+' || sourceText[i] == '-') {
            if (sourceText[i] == ':') {
                tokensSize++;
                lexTokens = (Token*)realloc(lexTokens, (tokensSize + 1) * sizeof(Token));
                lexTokens[tokensIndex].type = OPERATOR;
                lexTokens[tokensIndex].lexeme = (char*)malloc(3 * sizeof(char));
                strcpy(lexTokens[tokensIndex].lexeme, ":=");
                tokensIndex++;
            } else if (sourceText[i] == '+') {
                tokensSize++;
                lexTokens = (Token*)realloc(lexTokens, (tokensSize + 1) * sizeof(Token));
                lexTokens[tokensIndex].type = OPERATOR;
                lexTokens[tokensIndex].lexeme = (char*)malloc(2 * sizeof(char));
                strcpy(lexTokens[tokensIndex].lexeme, "+");
                tokensIndex++;
            } else {
                tokensSize++;
                lexTokens = (Token*)realloc(lexTokens, (tokensSize + 1) * sizeof(Token));
                lexTokens[tokensIndex].type = OPERATOR;
                lexTokens[tokensIndex].lexeme = (char*)malloc(2 * sizeof(char));
                strcpy(lexTokens[tokensIndex].lexeme, "-");
                tokensIndex++;
            }
        } else if (sourceText[i] == '/' && sourceText[i + 1] == '*') {
            while (1) {
                if (sourceText[i + 1] == '\0') {
                    printf("Lexical error!\n");
                    return 1;
                } else if (sourceText[i] == '*' && sourceText[i + 1] == '/' && sourceText[i - 1] != '/') {
                    break;
                }
                i++;
            }
        } else if (sourceText[i] == '"') {
            char* exp;
            size_t expSize = 0;
            exp = (char*)malloc((expSize + 1) * sizeof(char));
            int index = 0;
            while (1) {
                i++;
                if (sourceText[i] == '"') {
                    i++;
                    break;
                } else if (sourceText[i] == '\0') {
                    printf("Lexical error!\n");
                    return 1;
                }
                expSize++;
                exp = (char*)realloc(exp, (expSize + 1) * sizeof(char));
                exp[index] = sourceText[i];
                index++;
            }
            exp[index + 1] = '\0';
            tokensSize++;
            lexTokens = (Token*)realloc(lexTokens, (tokensSize + 1) * sizeof(Token));
            lexTokens[tokensIndex].type = STRING;
            lexTokens[tokensIndex].lexeme = (char*)malloc((strlen(exp) + 1) * sizeof(char));
            strcpy(lexTokens[tokensIndex].lexeme, exp);
            tokensIndex++;
            free(exp);
        } else if (isdigit(sourceText[i])) {
            char* number;
            size_t numSize = 0;
            number = (char*)malloc((numSize + 1) * sizeof(char));
            int index = 0;
            while (isdigit(sourceText[i])) {
                int numInt = atoi(number);
                if (numInt >= UINT_MAX) {
                    printf("Maximum size of integer exceeded!\n");
                    return 1;
                }
                numSize++;
                number = (char*)realloc(number, (numSize + 1) * sizeof(char));
                number[index] = sourceText[i];
                i++;
                index++;
            }
            number[index] = '\0';
            tokensSize++;
            lexTokens = (Token*)realloc(lexTokens, (tokensSize + 1) * sizeof(Token));
            lexTokens[tokensIndex].type = INTEGER;
            lexTokens[tokensIndex].lexeme = (char*)malloc((strlen(number) + 1) * sizeof(char));
            strcpy(lexTokens[tokensIndex].lexeme, number);
            tokensIndex++;
            free(number);
        } else if (isalpha(sourceText[i])) {
            char* exp;
            size_t expSize = 0;
            int index = 0;
            expSize++;
            exp = (char*)malloc((expSize + 1) * sizeof(char));
            exp[index] = sourceText[i];
            index++;
            i++;
            while (isalnum(sourceText[i]) || sourceText[i] == '_') {
                if (expSize >= MAX_IDENT_SIZE) {
                    printf("Maximum size of identifier exceeded!\n");
                    return 1;
                }
                expSize++;
                exp = (char*)realloc(exp, (expSize + 1) * sizeof(char));
                exp[index] = sourceText[i];
                index++;
                i++;
            }
            exp[index] = '\0';

            if (isKeyword(exp)) {
                tokensSize++;
                lexTokens = (Token*)realloc(lexTokens, (tokensSize + 1) * sizeof(Token));
                lexTokens[tokensIndex].type = KEYWORD;
                lexTokens[tokensIndex].lexeme = (char*)malloc((strlen(exp) + 1) * sizeof(char));
                strcpy(lexTokens[tokensIndex].lexeme, exp);
                tokensIndex++;
            } else {
                tokensSize++;
                lexTokens = (Token*)realloc(lexTokens, (tokensSize + 1) * sizeof(Token));
                lexTokens[tokensIndex].type = IDENTIFIER;
                lexTokens[tokensIndex].lexeme = (char*)malloc((strlen(exp) + 1) * sizeof(char));
                strcpy(lexTokens[tokensIndex].lexeme, exp);
                tokensIndex++;
            }
            i--;
            free(exp);
        }
    }
    return 0;
}

void lineSplitter() {
    lines = (Token**)malloc((lineCount + 1) * sizeof(Token*));
    lineSizes = (int*)malloc((lineCount + 1) * sizeof(int));
    int j = 0;
    int lineIndex = 0;

    for (int i = 0; i < tokensSize; i++) {
        if (lexTokens[i].type == END_OF_LINE) {
            Token* line = (Token*)malloc((i - j + 1) * sizeof(Token));
            for (int k = j, x = 0; k < i; k++, x++) {
                line[x].lexeme = strdup(lexTokens[k].lexeme);
                line[x].type = lexTokens[k].type;
            }
            lineSizes[lineIndex] = i - j;
            lines[lineIndex] = line;
            lineIndex++;
            j = i + 1;
        }
    }
    lineCount = lineIndex;
    lines = (Token**)realloc(lines, lineCount * sizeof(Token*));
    lineSizes = (int*)realloc(lineSizes, lineCount * sizeof(int));
}

void deleteElement(int index, int lineIndex) {
    if (index >= lineSizes[lineIndex]) {
        return;
    }

    Token* line = lines[lineIndex];
    free(lines[lineIndex][index].lexeme);

    for (int i = index; i < lineSizes[lineIndex] - 1; i++) {
        lines[lineIndex][i].lexeme = strdup(line[i + 1].lexeme);
        lines[lineIndex][i].type = line[i + 1].type;
    }

    lineSizes[lineIndex]--;

    lines[lineIndex] = (Token*)realloc(lines[lineIndex], lineSizes[lineIndex] * sizeof(Token));
}



int interpretToken(int index, int lineIndex, Token** lines)
{
    Token* line = lines[lineIndex];
    int count = lineSizes[lineIndex];

    variables = (Variable*)malloc((varSizes + 1) * sizeof(Variable));
    if (line[index].type == KEYWORD) {
        if (index + 2 < count) {
            if (!strcmp(line[index].lexeme, "new")) {
                if (line[index + 1].type == KEYWORD && !strcmp(line[index + 1].lexeme, "int") && line[index + 2].type == IDENTIFIER)
                {
                    varSizes++;
                    variables = (Variable*)realloc(variables, (varSizes + 1) * sizeof(Variable));
                    variables[varSizes - 1].name = (char*)malloc((strlen(line[index + 2].lexeme) + 1) * sizeof(char));
                    strcpy(variables[varSizes - 1].name, line[index + 2].lexeme);
                    variables[varSizes - 1].type = INTEGER;
                }
                else if (line[index + 1].type == KEYWORD && strcmp(line[index + 1].lexeme, "text") && line[index + 2].type == IDENTIFIER)
                {
                    varSizes++;
                    variables = (Variable*)realloc(variables, (varSizes + 1) * sizeof(Variable));
                    variables[varSizes - 1].name = (char*)malloc((strlen(line[index + 2].lexeme) + 1) * sizeof(char));
                    strcpy(variables[varSizes - 1].name, line[index + 2].lexeme);
                    variables[varSizes - 1].type = STRING;
                }
                else
                {
                    printf("Invalid syntax!\n");
                    return 1;
                }
            }
            else if (!strcmp(line[index].lexeme, "size") && index + 3 < count) {
                if (line[index + 1].type == LEFT_PAR && (line[index + 2].type == STRING || (isVariable(line[index + 2]) && typeCheck(line[index + 2])))
                    && line[index + 3].type == RIGHT_PAR)
                {
                    if (isVariable(line[index + 2]))
                    {
                        line[index].type = INTEGER;
                        line[index].lexeme = (char*)malloc(12 * sizeof(char)); // Assuming max integer size is 12 characters
                        sprintf(line[index].lexeme, "%d", strlen(findVar(line[index + 2]).value));
                    }
                    else
                    {
                        line[index].type = INTEGER;
                        line[index].lexeme = (char*)malloc(12 * sizeof(char)); // Assuming max integer size is 12 characters
                        sprintf(line[index].lexeme, "%d", strlen(line[index + 2].lexeme));
                    }
                    deleteElement(index + 1, lineIndex);
                    deleteElement(index + 2, lineIndex);
                    deleteElement(index + 3, lineIndex);
                }
                else {
                    printf("Invalid syntax!\n");
                    return 1;
                }
            }
        }
        else if (index + 7 < count) {
            if (!strcmp(line[index].lexeme, "subs")) {
                if (line[index + 1].type == LEFT_PAR && (line[index + 2].type == STRING || (isVariable(line[index + 2]) && typeCheck(line[index + 2])))
                    && line[index + 3].type == COMMA && (line[index + 4].type == INTEGER || (isVariable(line[index + 4]) && typeCheck(line[index + 4])))
                    && line[index + 5].type == COMMA && (line[index + 6].type == INTEGER || (isVariable(line[index + 6]) && typeCheck(line[index + 6])))
                    && line[index + 7].type == RIGHT_PAR)
                {
                    if (isVariable(line[index + 2]))
                    {
                        int startIndex = atoi(findVar(line[index + 4]).value);
                        int endIndex = atoi(findVar(line[index + 6]).value);
                        line[index].type = STRING;
                        substring(findVar(line[index+2]).value,line[index].lexeme,startIndex,endIndex-startIndex+1);
                    }
                    else
                    {
                        int startIndex = atoi(line[index + 4].lexeme);
                        int endIndex = atoi(line[index + 6].lexeme);
                        line[index].type = STRING;
                        substring(line[index + 2].lexeme,line[index].lexeme, startIndex, endIndex-startIndex+1);
                    }
                    deleteElement(index + 1, lineIndex);
                    deleteElement(index + 2, lineIndex);
                    deleteElement(index + 3, lineIndex);
                    deleteElement(index + 4, lineIndex);
                    deleteElement(index + 5, lineIndex);
                    deleteElement(index + 6, lineIndex);
                    deleteElement(index + 7, lineIndex);
                }
                else {
                    printf("Invalid syntax!\n");
                    return 1;
                }
                }
                else if(!strcmp(line[index].lexeme,"locate")){
                    if(line[index+1].type==LEFT_PAR&&(line[index+2].type==STRING||(isVariable(line[index+2])&&typeCheck(line[index+2])))
                       &&line[index+3].type==COMMA&&(line[index+4].type==STRING||(isVariable(line[index+4])&&typeCheck(line[index+4])))&&
                       line[index+5].type==COMMA&&
                       (line[index+6].type==INTEGER||(isVariable(line[index+6])&&!typeCheck(line[index+6])))&&line[index+7].type==RIGHT_PAR)
                       {
                            char* bigText;
                            char* smallText;
                            if(isVariable(line[index+2]))
                                {
                                    bigText=(char*)malloc((strlen(findVar(line[index+2]).value)+1)*sizeof(char));
                                    bigText=findVar(line[index+2]).value;
                                }
                            else{
                                    bigText=(char*)malloc((strlen(line[index+2].lexeme)+1)*sizeof(char));
                                    bigText=findVar(line[index+2]).value;
                                }
                            if(isVariable(line[index+4]))
                                {
                                    smallText=(char*)malloc((strlen(findVar(line[index+4]).value)+1)*sizeof(char));
                                    smallText=findVar(line[index+4]).value;
                                }
                            else{
                                    smallText=(char*)malloc((strlen(line[index+4].lexeme)+1)*sizeof(char));
                                    smallText=findVar(line[index+4]).value;
                                }
                            if(strlen(bigText)<strlen(smallText))
                                {
                                    printf("Invalid text values!\n");
                                    return 1;
                                }
                            else if(strstr(bigText,smallText)!=NULL)
                                {
                                    line[index].type=INTEGER;
                                    sprintf(line[index].lexeme,"%d",(strstr(bigText,smallText)-bigText));
                                }
                            else{
                                    line[index].type=INTEGER;
                                    sprintf(line[index].lexeme,"%d",0);
                            }
                            deleteElement(index+1,lineIndex);
                            deleteElement(index+2,lineIndex);
                            deleteElement(index+3,lineIndex);
                            deleteElement(index+4,lineIndex);
                            deleteElement(index+5,lineIndex);
                            deleteElement(index+6,lineIndex);
                            deleteElement(index+7,lineIndex);
                       }
                }
                else if(!strcmp(line[index].lexeme,"insert")){
                    if(line[index+1].type==LEFT_PAR&&(line[index+2].type==STRING||(isVariable(line[index+2])&&typeCheck(line[index+2])))
                       &&line[index+3].type==COMMA&&(line[index+4].type==INTEGER||(isVariable(line[index+4])&&!typeCheck(line[index+4])))&&
                       line[index+5].type==COMMA&&
                       (line[index+6].type==STRING||(isVariable(line[index+6])&&typeCheck(line[index+6])))&&line[index+7].type==RIGHT_PAR)
                       {
                            char* myText;
                            char* insertText;
                            int location;
                            char* result;
                            if(isVariable(line[index+2]))
                                {
                                    myText=(char*)malloc((strlen(findVar(line[index+2]).value)+1)*sizeof(char));
                                    myText=findVar(line[index+2]).value;
                                }
                            else
                                {
                                    myText=(char*)malloc((strlen(line[index+2].lexeme)+1)*sizeof(char));
                                    myText=line[index+2].lexeme;
                                }
                            if(isVariable(line[index+4]))
                                {
                                    location=atoi(findVar(line[index+4]).value);
                                }
                            else
                                {
                                    location=atoi(line[index+4].lexeme);
                                }
                            if(isVariable(line[index+6]))
                                {
                                    insertText=(char*)malloc((strlen(findVar(line[index+6]).value)+1)*sizeof(char));
                                    insertText=findVar(line[index+6]).value;
                                }
                            else
                                {
                                    insertText=(char*)malloc((strlen(line[index+6].lexeme)+1)*sizeof(char));
                                    insertText=line[index+6].lexeme;
                                }
                            result=(char*)malloc((strlen(myText)+strlen(insertText)+1)*sizeof(char));
                            strncpy(result,myText,location);
                            strncpy(result+location,insertText,strlen(insertText));
                            strncpy(result + location + strlen(insertText), myText + location, strlen(myText) - location);
                            line[index].lexeme=result;
                            deleteElement(index+1,lineIndex);
                            deleteElement(index+2,lineIndex);
                            deleteElement(index+3,lineIndex);
                            deleteElement(index+4,lineIndex);
                            deleteElement(index+5,lineIndex);
                            deleteElement(index+6,lineIndex);
                            deleteElement(index+7,lineIndex);
                       }

                }
                else if(!strcmp(line[index].lexeme,"override")){
                    if(line[index+1].type==LEFT_PAR&&(line[index+2].type==STRING||(isVariable(line[index+2])&&typeCheck(line[index+2])))
                       &&line[index+3].type==COMMA&&(line[index+4].type==INTEGER||(isVariable(line[index+4])&&!typeCheck(line[index+4])))&&
                       line[index+5].type==COMMA&&
                       (line[index+6].type==STRING||(isVariable(line[index+6])&&typeCheck(line[index+6])))&&line[index+7].type==RIGHT_PAR)
                       {
                            char* myText;
                            char* ovrText;
                            int location;
                            if(isVariable(line[index+2]))
                                {
                                    myText=(char*)malloc((strlen(findVar(line[index+2]).value)+1)*sizeof(char));
                                    myText=findVar(line[index+2]).value;
                                }
                            else
                                {
                                    myText=(char*)malloc((strlen(line[index+2].lexeme)+1)*sizeof(char));
                                    myText=line[index+2].lexeme;
                                }
                            if(isVariable(line[index+4]))
                                {
                                    location=atoi(findVar(line[index+4]).value);
                                }
                            else
                                {
                                    location=atoi(line[index+4].lexeme);
                                }
                            if(isVariable(line[index+6]))
                                {
                                    ovrText=(char*)malloc((strlen(findVar(line[index+6]).value)+1)*sizeof(char));
                                    ovrText=findVar(line[index+6]).value;
                                }
                            else
                                {
                                    ovrText=(char*)malloc((strlen(line[index+6].lexeme)+1)*sizeof(char));
                                    ovrText=line[index+6].lexeme;
                                }
                            overrideTextAtLocation(myText,ovrText,location);
                            line[index].type=STRING;
                            line[index].lexeme=myText;
                            deleteElement(index+1,lineIndex);
                            deleteElement(index+2,lineIndex);
                            deleteElement(index+3,lineIndex);
                            deleteElement(index+4,lineIndex);
                            deleteElement(index+5,lineIndex);
                            deleteElement(index+6,lineIndex);
                            deleteElement(index+7,lineIndex);
                       }
                    else
                        {
                            printf("Invalid parameters!\n");
                            return 1;
                        }
                }}
                else if(index+3<count){
                if(!strcmp(line[index].lexeme,"read")){
                    if(isVariable(line[index+1])&&typeCheck(line[index+1])&&line[index+2].type==KEYWORD
                       &&line[index+2].lexeme=="from"&&line[index+3].type==IDENTIFIER)
                        {
                            FILE* file=fopen(strcat(line[index+3].lexeme,".txt"),"r");
                            fscanf(file,"%s",findVar(line[index+1]).value);
                            fclose(file);
                        }
                    else{
                        printf("An exception thrown!\n");
                        return 1;
                    }
                }
                else if(!strcmp(line[index].lexeme,"write")){
                    if((line[index+1].type==STRING||(isVariable(line[index+1])&&typeCheck(line[index+1])))&&line[index+2].type==KEYWORD
                       &&!strcmp(line[index+2].lexeme,"to")&&line[index+3].type==IDENTIFIER)
                       {
                            FILE* file=fopen(strcat(line[index+3].lexeme,".txt"),"w");
                            if(isVariable(line[index+1]))
                                {
                                    fprintf(file,"%s",findVar(line[index+1]).value);
                                }
                            else
                                {
                                    fprintf(file,"%s",line[index+1].lexeme);
                                }
                            fclose(file);
                       }
                       else
                        {
                            printf("An exception thrown!\n");
                            return 1;
                        }
                }
                else if(!strcmp(line[index].lexeme,"input")){
                    if(isVariable(line[index+1])&&typeCheck(line[index+1])&&line[index+2].type==KEYWORD&&!strcmp(line[index+2].lexeme,"prompt")
                       &&(line[index+3].type==STRING||(isVariable(line[index+3])&&typeCheck(line[index+3]))))
                       {
                            if(isVariable(line[index+3]))
                                {
                                    printf("%s\n",findVar(line[index+3]).value);
                                }
                            else
                                {
                                    printf("%s\n",line[index+3].lexeme);
                                }
                            scanf("%s",findVar(line[index+1]).value);
                       }
                    else{
                        printf("Invalid syntax!\n");
                        return 1;
                    }
                }}
                else if(index+1<count){
                if(!strcmp(line[index].lexeme,"output")){
                    if(line[index+1].type==STRING||(isVariable(line[index+1])&&typeCheck(line[index+1])))
                        {
                            if(isVariable(line[index+1]))
                                {
                                    printf("%s\n",findVar(line[index+1]).value);
                                }
                            else
                                {
                                    printf("%s\n",line[index+1].lexeme);
                                }
                        }
                    else
                        {
                            printf("Invalid syntax!\n");
                            return 1;
                        }
                }}
               else if(index+2<count){
               if(!strcmp(line[index].lexeme,"asText")){
                    if(line[index+1].type==LEFT_PAR&&(line[index+2].type==STRING||(isVariable(line[index+2])&&typeCheck(line[index+2])))
                       &&line[index+3].type==RIGHT_PAR)
                       {
                            line[index].type=INTEGER;
                            if(isVariable(line[index+2]))
                                {
                                    if(isdigit(findVar(line[index+2]).value)){
                                        line[index].lexeme=findVar(line[index+2]).value;
                                    }
                                    else{
                                        printf("Invalid parameter!\n");
                                        return 1;
                                    }
                                }
                            else
                                {
                                    if(isdigit(line[index+2].lexeme)){
                                        line[index].lexeme=line[index+2].lexeme;
                                    }
                                    else{
                                        printf("Invalid parameter!\n");
                                        return 1;
                                    }
                                }
                            deleteElement(index+1,lineIndex);
                            deleteElement(index+2,lineIndex);
                            deleteElement(index+3,lineIndex);
                       }
                       else
                        {
                            printf("Invalid parameter!\n");
                            return 1;
                        }
               }
               else if(!strcmp(line[index].lexeme,"asString")){
                    if(line[index+1].type==LEFT_PAR&&(line[index+2].type==INTEGER||(isVariable(line[index+2])&&!typeCheck(line[index+2])))
                       &&line[index+3].type==RIGHT_PAR)
                       {
                            line[index].type=STRING;
                            if(isVariable(line[index+2]))
                                {
                                    line[index].lexeme=findVar(line[index+2]).value;
                                }
                            else
                                {
                                    line[index].lexeme=line[index+2].lexeme;
                                }
                            deleteElement(index+1,lineIndex);
                            deleteElement(index+2,lineIndex);
                            deleteElement(index+3,lineIndex);

                       }
                       else
                        {
                            printf("Invalid parameter!\n");
                            return 1;
                        }
               }}
        }
        else if(line[index].type==OPERATOR){
                if(index+1<count&&index-1>=0){
                if(!strcmp(line[index].lexeme ,":=")){
                    if(isVariable(line[index-1])&&!typeCheck(line[index-1]))
                        {
                            if(line[index+1].type==INTEGER)
                                {
                                    strcpy(findVar(line[index-1]).value,line[index+1].lexeme);
                                }
                            else if(isVariable(line[index+1])&&!typeCheck(line[index+1]))
                                {
                                    strcpy(findVar(line[index-1]).value,findVar(line[index+1]).value);
                                }
                            else{
                                printf("Invalid assignment!\n");
                                return 1;
                            }
                        }
                    else if(isVariable(line[index-1])&&typeCheck(line[index-1]))
                        {
                            if(line[index+1].type==STRING)
                                {
                                    strcpy(findVar(line[index-1]).value,line[index+1].lexeme);
                                }
                            else if(isVariable(line[index+1])&&typeCheck(line[index+1]))
                                {
                                    strcpy(findVar(line[index-1]).value,findVar(line[index+1]).value);
                                }
                            else{
                                printf("Invalid assignment!\n");
                                return 1;
                            }
                        }
                    else{
                        printf("Syntax error!\n");
                        return 1;
                    }
                }
                else if(!strcmp(line[index].lexeme ,"+")){
                    if((line[index-1].type==INTEGER||(isVariable(line[index-1])&&!typeCheck(line[index-1])))
                       &&(line[index+1].type==INTEGER||(isVariable(line[index+1])&&!typeCheck(line[index+1]))))
                        {
                            int num1;
                            int num2;
                            int result;
                            if(isVariable(line[index-1]))
                                {
                                    num1=atoi(findVar(line[index-1]).value);
                                }
                            else{
                                num1=atoi(line[index-1].lexeme);
                            }
                            if(isVariable(line[index+1]))
                                {
                                    num2=atoi(findVar(line[index+1]).value);
                                }
                            else{
                                num2=atoi(line[index+1].lexeme);
                            }
                            result=num1+num2;
                            sprintf(line[index].lexeme,"%d",result);
                            line[index].type=INTEGER;
                            deleteElement(index+1,lineIndex);
                            deleteElement(index-1,lineIndex);

                        }
                    else if((line[index-1].type==STRING||(isVariable(line[index-1])&&typeCheck(line[index-1])))
                       &&(line[index+1].type==STRING||(isVariable(line[index+1])&&typeCheck(line[index+1]))))
                        {
                            char* text1;
                            char* text2;
                            if(isVariable(line[index-1]))
                                {
                                    text1=atoi(findVar(line[index-1]).value);
                                    text1=(char*)malloc((strlen(text1)+1)*sizeof(char));
                                }
                            else{
                                text1=atoi(line[index-1].lexeme);
                                text1=(char*)malloc((strlen(text1)+1)*sizeof(char));
                            }
                            if(isVariable(line[index+1]))
                                {
                                    text2=atoi(findVar(line[index+1]).value);
                                    text2=(char*)malloc((strlen(text2)+1)*sizeof(char));
                                }
                            else{
                                text2=atoi(line[index+1].lexeme);
                                text2=(char*)malloc((strlen(text2)+1)*sizeof(char));
                            }
                            sprintf(line[index].lexeme,"%s",strcat(text1,text2));
                            line[index].type=STRING;
                            deleteElement(index+1,lineIndex);
                            deleteElement(index-1,lineIndex);
                        }
                    else
                        {
                            printf("Incompatible type of values!\n");
                            return 1;
                        }
                }
                else if(!strcmp(line[index].lexeme ,"-")){
                    if((line[index-1].type==INTEGER||(isVariable(line[index-1])&&!typeCheck(line[index-1])))
                       &&(line[index+1].type==INTEGER||(isVariable(line[index+1])&&!typeCheck(line[index+1]))))
                        {
                            int num1;
                            int num2;
                            int result;
                            if(isVariable(line[index-1]))
                                {
                                    num1=atoi(findVar(line[index-1]).value);
                                }
                            else{
                                num1=atoi(line[index-1].lexeme);
                            }
                            if(isVariable(line[index+1]))
                                {
                                    num2=atoi(findVar(line[index+1]).value);
                                }
                            else{
                                num2=atoi(line[index+1].lexeme);
                            }
                            if(num1>=num2)
                                {
                                    result=num1-num2;
                                }
                            else{
                                printf("Operation failed!\n");
                                return 1;
                            }
                            sprintf(line[index].lexeme,"%d",result);
                            line[index].type=INTEGER;
                            deleteElement(index+1,lineIndex);
                            deleteElement(index-1,lineIndex);
                        }
                    else if((line[index-1].type==STRING||(isVariable(line[index-1])&&typeCheck(line[index-1])))
                       &&(line[index+1].type==STRING||(isVariable(line[index+1])&&typeCheck(line[index+1]))))
                        {
                            char* text1;
                            char* text2;
                            if(isVariable(line[index-1]))
                                {
                                    text1=atoi(findVar(line[index-1]).value);
                                    text1=(char*)malloc((strlen(text1)+1)*sizeof(char));
                                }
                            else{
                                text1=atoi(line[index-1].lexeme);
                                text1=(char*)malloc((strlen(text1)+1)*sizeof(char));
                            }
                            if(isVariable(line[index+1]))
                                {
                                    text2=atoi(findVar(line[index+1]).value);
                                    text2=(char*)malloc((strlen(text2)+1)*sizeof(char));
                                }
                            else{
                                text2=atoi(line[index+1].lexeme);
                                text2=(char*)malloc((strlen(text2)+1)*sizeof(char));
                            }
                            if(strlen(text1)>=strlen(text2))
                                {
                                    stringSubstract(text1,text2);
                                    sprintf(line[index].lexeme,"%s",text1);
                                    line[index].type=STRING;
                                    deleteElement(line,index+1);
                                    deleteElement(line,index-1);
                                }
                            else{
                                printf("Operation failed!\n");
                                return 1;
                            }
                        }
                    else
                        {
                            printf("Incompatible type of values!\n");
                            return 1;
                        }
                }}
        }
for (int i = 0; i < varSizes; i++) {
        free(variables[i].name);
    }
    free(variables);
return 0;
    }


int main()
{
    char* sourceText;
    size_t sourceSize=0;
    sourceText=(char*)malloc((sourceSize+1)* sizeof(char));
    char* fileName;
    size_t fileNameSize=0;
    fileName=(char*)malloc((fileNameSize+1)* sizeof(char));
    printf("Please enter the name/path of the file you want to execute (---.tj):\n");
    while(1)
        {
            fileName=(char* )realloc(fileName,(fileNameSize+1)* sizeof(char));
            char ch;
            scanf("%c", &ch);
            if(ch=='\n')
                {
                    fileName[fileNameSize]='\0';
                    break;
                }
            fileName[fileNameSize]=ch;
            fileNameSize++;
        }
    fileNameSize=strlen(fileName);
    fileName=(char*)realloc(fileName,(fileNameSize+1)* sizeof(char));
    FILE* source=fopen(fileName,"r");
    if(source==NULL)
        {
            printf("File not found!\nMake sure the file has the extension '.tj'!\n");
            free(fileName);
            return 1;
        }
    char ch;
    while((ch=fgetc(source))!=EOF)
        {
            sourceText=(char*)realloc(sourceText,(sourceSize+1)* sizeof(char));
            sourceText[sourceSize]=ch;
            sourceSize++;
        }
    sourceText[sourceSize]='\0';
    fclose(source);
    lexicalAnalysis(sourceText,sourceSize);
    if(lexicalAnalysis(sourceText,sourceSize)==0){
        lineSplitter();
        for(int i=0;i<lineCount;i++){
            for(int j=0;j<lineSizes[lineCount];j++){
                if(interpretToken(j,i,lines)==0){
                    interpretToken(j,i,lines);
                }
                else if(interpretToken(j,i,lines)==1){
                    return 1;
                }
            }

        }
    }
    else if(lexicalAnalysis(sourceText,sourceSize)==1){
        return 1;
    }
    free(lines);
    free(lineSizes);
    free(sourceText);
    free(fileName);
    free(lexTokens);
    free(variables);
    return 0;
}
