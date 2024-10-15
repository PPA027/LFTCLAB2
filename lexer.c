#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "lexer.h"
#include "utils.h"

Token tokens[MAX_TOKENS];
int nTokens;

int line=1;		// the current line in the input file

// adds a token to the end of the tokens list and returns it
// sets its code and line
Token *addTk(int code){
	if(nTokens==MAX_TOKENS)err("too many tokens");
	Token *tk=&tokens[nTokens];
	tk->code=code;
	tk->line=line;
	nTokens++;
	return tk;
	}

// copy in the dst buffer the string between [begin,end)
char *copyn(char *dst,const char *begin,const char *end){
	char *p=dst;
	if(end-begin>MAX_STR)err("string too long");
	while(begin!=end)*p++=*begin++;
	*p='\0';
	return dst;
	}

void tokenize(const char *pch){
	const char *start;
	Token *tk;
	char buf[MAX_STR+1];
	for(;;){
		switch(*pch){
			case ' ':case '\t':pch++;break;
			case '\r':		// handles different kinds of newlines (Windows: \r\n, Linux: \n, MacOS, OS X: \r or \n)
				if(pch[1]=='\n')pch++;
				// fallthrough to \n
			case '\n':
				line++;
				pch++;
				break;
			case '\0':addTk(FINISH);return;
			case ',':addTk(COMMA);pch++;break;
			case '=':
				if(pch[1]=='='){
					addTk(EQUAL);
					pch+=2;
					}else{
					addTk(ASSIGN);
					pch++;
					}
				break;

			case '#':
				while(*pch != '\n' && *pch !='\0'){
					pch++;
				}
				break;
			
			case '(':
				addTk(LPAR);
				pch++;
				break;
			
			case ')':
				addTk(RPAR);
				pch++;
				break;

			case ':':
				addTk(COLON);
				pch++;
				break;

			case ';':
				addTk(SEMICOLON);
				pch++;
				break;

			case '<':
				if(pch[1] == '='){
					addTk(LESSEQ);
					pch += 2;
				}else{
					addTk(LESS);
					pch++;
				}
				break;

			case '>':
				if(pch[1] == '='){
					addTk(GREATEREQ);
					pch += 2;
				}else{
					addTk(GREATER);
					pch++;
				}
				break;

			case '+':
				addTk(ADD);
				pch++;
				break;

			case '"':
				start = ++pch;
				while(*pch != '"' && pch !='\0'){
					pch++;
				}

				if(pch == '\0'){
					err("Unfinished string");
				}
				else{
					char *text = copyn(buf,start,pch);
					tk = addTk(TYPE_STR);
					strcpy(tk->text,text);
					pch++;
				}
				break;

			default:
				if(isalpha(*pch)||*pch=='_'){
					for(start=pch++;isalnum(*pch)||*pch=='_';pch++){}
					char *text=copyn(buf,start,pch);
					if(strcmp(text,"int")==0)addTk(TYPE_INT);
					else{
						tk=addTk(ID);
						strcpy(tk->text,text);
						}
					}else if (isdigit(*pch)){
						start = pch;
						while(isdigit(*pch)) pch++;
						if(*pch == '.'){
							pch++;
							while(isdigit(*pch)) pch++;
							addTk(TYPE_REAL);
						}
						else{
							addTk(TYPE_INT);
						}
						
					}
				else err("invalid char: %c (%d)",*pch,*pch);
			}
		}
	}

const char* getTokenName(int token) {
    switch (token) {
        case ID: return "ID";
        case VAR: return "VAR";
        case FUNCTION: return "FUNCTION";
        case IF: return "IF";
        case ELSE: return "ELSE";
        case WHILE: return "WHILE";
        case END: return "END";
        case RETURN: return "RETURN";
        case TYPE_REAL: return "TYPE_REAL";
        case TYPE_STR: return "TYPE_STR";
        case TYPE_INT: return "TYPE_INT";
        case COMMA: return "COMMA";
        case FINISH: return "FINISH";
        case COLON: return "COLON";
        case SEMICOLON: return "SEMICOLON";
        case LPAR: return "LPAR";
        case RPAR: return "RPAR";
        case ASSIGN: return "ASSIGN";
        case EQUAL: return "EQUAL";
        case ADD: return "ADD";
        case SUB: return "SUB";
        case MUL: return "MUL";
        case DIV: return "DIV";
        case AND: return "AND";
        case OR: return "OR";
        case NOT: return "NOT";
        case NOTEQ: return "NOTEQ";
        case LESS: return "LESS";
        case LESSEQ: return "LESSEQ";
        case GREATER: return "GREATER";
        case GREATEREQ: return "GREATEREQ";
        default: return "UNKNOWN";
    }
}

void showTokens() {
    for (int i = 0; i < nTokens; i++) {
        Token *tk = &tokens[i];
        printf("%d ", tk->line);

        printf("%s", getTokenName(tk->code));

        if (tk->code == ID || tk->code == TYPE_STR) {
            printf(":%s", tk->text); 
        } else if (tk->code == TYPE_INT) {
            printf(":INT:%d", tk->i); 
        } else if (tk->code == TYPE_REAL) {
            printf(":REAL:%f", tk->r);  
        }

        printf("\n");
    }
}


