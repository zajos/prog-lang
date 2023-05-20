#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_IDENT_SIZE 30
#define MAX_INT_SIZE 10

void toLowerCase(char* str)
{
    int length = strlen(str);
    for (int i = 0; i < length; i++)
    {
        str[i] = tolower(str[i]);
    }
}

char* two_chars(char c1, char c2)
{
    char* comb = malloc(3 * sizeof(char));
    sprintf(comb, "%c%c", c1, c2);
    return comb;
}

void createSubString(char* original, int startIndex, int endIndex, char* sub)
{
    int subSize = endIndex - startIndex;
    for (int i = 0; i < subSize; i++)
    {
        sub[i] = original[startIndex + i];
    }
    sub[subSize] = '\0';
}

bool isOperator(char* str)
{
    if (!strcmp(str, "+") || !strcmp(str, "-") || !strcmp(str, "*") || !strcmp(str, "/") || !strcmp(str, "++") ||
        !strcmp(str, "--") || !strcmp(str, ":="))
    {
        return true;
    }
    return false;
}

bool isKeyword(char* str)
{
    if (!strcasecmp(str, "if") || !strcasecmp(str, "else") || !strcasecmp(str, "while") || !strcasecmp(str, "do") ||
        !strcasecmp(str, "break") || !strcasecmp(str, "enum") || !strcasecmp(str, "continue") || !strcasecmp(str, "int") ||
        !strcasecmp(str, "const") || !strcasecmp(str, "float") || !strcasecmp(str, "return") || !strcasecmp(str, "case") ||
        !strcasecmp(str, "char") || !strcasecmp(str, "long") || !strcasecmp(str, "for") || !strcasecmp(str, "record") ||
        !strcasecmp(str, "static") || !strcasecmp(str, "goto"))
    {
        return true;
    }
    return false;
}

bool isIdentifier(char* str)
{
    if (isalpha(str[0]) && strlen(str) <= MAX_IDENT_SIZE && !isKeyword(str))
    {
        return true;
    }
    return false;
}

int main()
{
    char delimiter[17] = ";()[]{}+-*/: \n\t";

    char* target_text = malloc(100 * sizeof(char));
    target_text[0] = '\0';
    int target_size = 0;

    FILE* source_file;
    source_file = fopen("code.psi", "r");
    if (source_file == NULL) {
        printf("File opening error!\n");
        exit(1);
    }
    fseek(source_file, 0, SEEK_END);
    long size = ftell(source_file);
    fseek(source_file, 0, SEEK_SET);

    char* source_text = malloc((size + 1) * sizeof(char));
    if (source_text == NULL) {
        printf("Could not allocate memory!");
        fclose(source_file);
        return 1;
    }
    size_t readSize = fread(source_text, sizeof(char), size, source_file);
    source_text[readSize] = '\0';
    fclose(source_file);

    for (int i = 0; i < readSize ; i++)
    {

        if (source_text[i] == ';')
        {
            strcat(target_text, "EndOfLine\n");
            target_size += strlen("EndOfLine\n");
            if (target_size + 1 >= strlen(target_text))
            {
                target_text = realloc(target_text, (target_size + 1) * 2);
            }
        }
        else if (source_text[i] == '(')
        {
            strcat(target_text, "LeftPar\n");
            target_size += strlen("LeftPar\n");
            if (target_size + 1 >= strlen(target_text))
            {
                target_text = realloc(target_text, (target_size + 1) * 2);
            }
        }
        else if (source_text[i] == ')')
        {
            strcat(target_text, "RightPar\n");
            target_size += strlen("RightPar\n");
            if (target_size + 1 >= strlen(target_text))
            {
                target_text = realloc(target_text, (target_size + 1) * 2);
            }
        }
        else if (source_text[i] == '[')
        {
            strcat(target_text, "LeftSquareBracket\n");
            target_size += strlen("LeftSquareBracket\n");
            if (target_size + 1 >= strlen(target_text))
            {
                target_text = realloc(target_text, (target_size + 1) * 2);
            }
        }
        else if (source_text[i] == ']')
        {
            strcat(target_text, "RightSquareBracket\n");
            target_size += strlen("RightSquareBracket\n");
            if (target_size + 1 >= strlen(target_text))
            {
                target_text = realloc(target_text, (target_size + 1) * 2);
            }
        }
        else if (source_text[i] == '{')
        {
            strcat(target_text, "LeftCurlyBracket\n");
            target_size += strlen("LeftCurlyBracket\n");
            if (target_size + 1 >= strlen(target_text))
            {
                target_text = realloc(target_text, (target_size + 1) * 2);
            }
        }
        else if (source_text[i] == '}')
        {
            strcat(target_text, "RightCurlyBracket\n");
            target_size += strlen("RightCurlyBracket\n");
            if (target_size + 1 >= strlen(target_text))
            {
                target_text = realloc(target_text, (target_size + 1) * 2);
            }
        }
        else if (source_text[i]=='/'&&source_text[i+1]=='*' && i >= 0)
        {

            for (int j = i; j < strlen(source_text) ; j++)
            {
                if (source_text[j]=='*'&&source_text[j+1]=='/' && (j + 1) < strlen(source_text))
                {
                    i=j+2;
                    break;
                }
                else if (j == strlen(source_text) )
                {
                    printf("Lexical error occurred!\n");
                    exit(1);
                }


            }

        }
        else if (isOperator(two_chars(source_text[i], source_text[i + 1])) && i > 0)
        {
            char* temp = malloc(14 * sizeof(char));
            sprintf(temp, "Operator(%s)\n", two_chars(source_text[i], source_text[i + 1]));
            strcat(target_text, temp);
            target_size += strlen(temp);
            if (target_size + 1 >= strlen(target_text))
            {
                target_text = realloc(target_text, (target_size + 1) * 2);
            }
            i=i+1;

        }
        else if (isOperator(two_chars(source_text[i],'\0')))
        {
            char* temp = malloc(14 * sizeof(char));
            sprintf(temp, "Operator(%c)\n", source_text[i]);
            strcat(target_text, temp);
            target_size += strlen(temp);
            if (target_size + 1 >= strlen(target_text))
            {
                target_text = realloc(target_text, (target_size + 1) * 2);
            }
        }
        else if (source_text[i] == '"')
        {
            int count;
            char* str_cons;
            char* sub;
            for (int j = i+1; j < strlen(source_text) ; j++)
            {
                if (source_text[j] == '"')
                {
                    count = j+1;
                    break;
                }
                else if (j == strlen(source_text) )
                {
                    printf("Lexical error occurred!\n");
                    exit(1);
                }
            }
            sub=malloc((count-i)*sizeof(char));
            createSubString(source_text, i, count, sub);
            str_cons=malloc((count-i+11)*sizeof(char));
            sprintf(str_cons,"StrConst(%s)\n",sub);
            target_size += strlen(str_cons);
            if (target_size + 1 >= strlen(target_text))
            {
                target_text = realloc(target_text, (target_size + 1) * 2);
            }
            strcat(target_text, str_cons);

            i=count;
        }

        else if (isalpha(source_text[i]))
        {
            int j = i;
            char *exp = malloc(MAX_IDENT_SIZE * sizeof(char));
            int k = 0;
            while (strchr(delimiter, source_text[j]) == NULL)
            {
                exp[k++] = source_text[j++];
            }
            exp[k] = '\0';

            if (isKeyword(exp))
            {
                char *temp = malloc((11 + strlen(exp)) * sizeof(char));
                sprintf(temp, "Keyword(%s)\n", exp);
                strcat(target_text, temp);
                target_size += strlen(temp);
                if (target_size + 1 >= strlen(target_text))
                {
                    target_text = realloc(target_text, (target_size + 1) * 2);
                }
            }
            else if (isIdentifier(exp))
            {
                char *temp = malloc((14 + strlen(exp)) * sizeof(char));
                sprintf(temp, "Identifier(%s)\n", exp);
                strcat(target_text, temp);
                target_size += strlen(temp);
                if (target_size + 1 >= strlen(target_text))
                {
                    target_text = realloc(target_text, (target_size + 1) * 2);
                }
            }
            else
            {
                printf("Identifier exceeded maximum size!");
                exit(1);
            }

            free(exp);
            i=j-1;
        }
        else if (isdigit(source_text[i]))
        {
            int j = i;
            char *exp = malloc(MAX_INT_SIZE * sizeof(char));
            int k = 0;
            while (isdigit(source_text[j]))
            {
                exp[k++] = source_text[j++];
            }
            exp[k] = '\0';

            if (strlen(exp) > MAX_INT_SIZE)
            {
                printf("Integer exceeded maximum digits!");
                exit(1);
            }
            else
            {
                char *temp = malloc((11 + strlen(exp)) * sizeof(char));
                sprintf(temp, "IntConst(%s)\n", exp);
                strcat(target_text, temp);
                target_size += strlen(temp);
                if (target_size + 1 >= strlen(target_text))
                {
                    target_text = realloc(target_text, (target_size + 1) * 2);
                }
            }

            free(exp);
            i=j-1;
        }

    }

    target_text[target_size] = '\0';


    FILE *lex_file = fopen("code.lex", "w");
    if (lex_file == NULL)
    {
        printf("File error occurred!");
        exit(1);
    }
    fprintf(lex_file, "%s", target_text);
    fclose(lex_file);
    printf("code.lex dosyasi olusturulmustur.\n");
    free(source_text);
    free(target_text);

    return 0;
}







