/*
Copyright 2021 Bruno VERCHÈRE

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"treestruct.h"


Tree createNode(int id, char name[], int value){
    Tree tmp;
    if((tmp=(Person*)malloc(sizeof(Person)))==NULL)
       return NULL;
    tmp->id = id;
    strcpy(tmp->name, name);
    tmp->value = value;
    tmp->lc=tmp->rc=NULL;
    return tmp;
}

int buildTree(Tree *t, int id, char name[], int value, FILE* logs){
    char logMessage[255] = " - ERROR: Cannot allocate to a new individual\n";
    if(id == 1 && *t == NULL){
        if((*t=createNode(id, name, value))==NULL){
                writeLogs(logs, logMessage);
                return 1;
        }
        return 1;
    }

    if(id!=1 && *t==NULL){
        return 1;
    }
    if((*t)->id == (int)(id/2)){
        if(id%2 == 0){
            if(((*t)->lc = createNode(id, name, value))==NULL){
                writeLogs(logs, logMessage);
                return 1;
            }
        }else{
            if (((*t)->rc = createNode(id, name, value))==NULL){
                writeLogs(logs, logMessage);
                return 1;
            }
        }
        return 1;
    }
    return buildTree(&((*t)->lc), id, name, value, logs) && buildTree(&((*t)->rc), id, name, value, logs);
}

void getFields(char line[], char* tokens[]){
    char* token;
	int count = 0;
	token = strtok(line, CSVSEP);
    while(token != NULL){
		tokens[count] = token;
		count++;
		token = strtok(NULL, CSVEOL);
	}
}

int toInt(char* str){
	int  i, len;
	int result = 0;
	len = strlen(str);
	for(i=0; i<len; i++){
		result = result * 10 + ( str[i] - '0' );
	}
	return result;
}

int readCSV(FILE* file, Tree *t, FILE* logs){
	char line[1024];
	char* tokens[4];
	while (lcets(line, 1024, file)){
        char* tmp = strdup(line);
		getFields(tmp, tokens);
        if(buildTree(&(*t), toInt(tokens[0]), tokens[1], toInt(tokens[2]), logs))
            return 1;
        free(tmp);
	}
    return 1;
}

void displayPerson(Tree t){
    if(t==NULL){
        return ;
    }
    printf("Individual %d: %d\n", t->id, t->value);
    displayPerson(t->lc);
    displayPerson(t->rc);
}

void rowMissingChildren(FILE* file, int children){
    for(int i=0; i < children; i++){
        fprintf(file, "\nchild [missing]");     
    }
}

void missingChildren(int id, FILE* file, int rootId){
    if(id >= rootId*2 && id < rootId*4){
        if(id%2 != 0){ rowMissingChildren(file, 10); }
    }
    else if(id >= rootId*4 && id < rootId*8){
        if(id%2!=0){ rowMissingChildren(file, 4); }
    }
    else if(id >= rootId*8 && id < rootId*16){
        if(id%2!=0){ rowMissingChildren(file, 2); }
    }
}

void color(int value, char strColor[]){
	switch(value){
		case 0: strcat(strColor, "white");break;
		case 1: strcat(strColor, "lightblue");break;
		case 2: strcat(strColor, "blue");break;
		default: strcat(strColor, "white");
	}
}

void writeLatexPerson(FILE *file, Tree t, int depth, int rootId){
    if(t==NULL || depth >= MAXDEPTH){
        return ;
    }
    char s[MAXLEN]="";
    char strColor[11]="";
    char strId[10];
    if(t->id == rootId){
        strcat(s, "\n\\node [bottom color=");
    }else{
        strcat(s, "\nchild {node[bottom color=");
    }
    missingChildren(t->id, file, rootId);
    color(t->value, strColor);
    strcat(s, strColor);
    strcat(s,"]{");
    
    snprintf(strId, 10, "%d", t->id);
    strcat(s, strId);
    strcat(s, "\\pdfcomment[icon=Note, color={0.9 0.9 0.9}, opacity=0.5, borderstyle=dashed]{");
    strcat(s, t->name);
    strcat(s, "}}");
    fputs(s, file);
    writeLatexPerson(file, t->lc, depth+1, rootId);
    writeLatexPerson(file, t->rc, depth+1, rootId);
	if(depth > 0)
		fputs("}", file);
}

void writeLatexFile(FILE* file, Tree t, int rootId){
    fputs("\\documentclass{article}\\usepackage{tikz}\\usepackage{color}\n\\usepackage[legalpaper, landscape, margin=1in]{geometry}\n\\usepackage{pdfcomment}\n", file);
    fputs("\\definecolor{blue}{RGB}{0,0,255}\n", file);
    fputs("\\definecolor{lightblue}{RGB}{175,175,255}\n", file);
    fputs("\\definecolor{white}{RGB}{255,255,255}\n", file);
  
    fputs("\\begin{document}\n\\begin{tikzpicture}[nodes = {circle, fill, scale=0.5}, <-,top color=white]\n", file);
    writeLatexPerson(file, t, 0, rootId);
    fputs(";\n\\end{tikzpicture}\n\\end{document}", file);
}

void writeLogs(FILE* logs, char message[]){
    time_t now = time (NULL);
    struct tm tm_now = *localtime (&now);
    char s_now[sizeof "JJ/MM/AAAA HH:MM:SS"];
    strftime (s_now, sizeof s_now, "%d/%m/%Y %H:%M:%S", &tm_now);
    fputs(s_now, logs);
    fputs(message, logs);
}

void formateInteger(int val, char ch[], char addOn[]){
    char strInt[15];
    snprintf(strInt, 10, "%d", val);
    strcat(ch, addOn);
    strcat(ch, strInt);
}


int main(int argc, char* argv[]){
    Tree t = NULL;
    int id;
    char logMessage[255] = " - ";
    // Open files
    FILE* data = fopen(SRC, "r");
    FILE* logs = fopen(LOGS, "a");
    if(data!=NULL){
        if(readCSV(data, &t, logs)){
            id = t->id;
            strcpy(logMessage, " - INFO: Write TEX file with ");
            formateInteger(id, logMessage, "root=");
            strcat(logMessage, "\n");
            writeLogs(logs, logMessage);
			printf("Build Latex file...");
			FILE* tex = fopen(TEX, "w");
			writeLatexFile(tex, t, id);
			fclose(tex);
			free(t);
			printf("Done\n");
        }else{
            strcpy(logMessage, " - ERROR: Cannot read the CSV file properly: error(s) occurred\n");
            writeLogs(logs, logMessage);
        }
    }else{
        strcpy(logMessage, " - ERROR : cannot open \"");
        strcat(logMessage, SRC);
        strcat(logMessage, "\"\n");
        writeLogs(logs, logMessage);
    }
    fclose(data);
    fclose(logs);
    return 0;
}
