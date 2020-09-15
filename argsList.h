/*
* argsLIst.h
*
*  Created on: May 14, 2019
*      Author: yahel
*/

#ifndef ARGSLIST_H_
#define ARGSLIST_H_
#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen



typedef struct node node_t;

node_t* current;

node_t* createArgsLIst();
node_t* createArgsLIstWithArguments(char* path, char* val);
char* getValueByPath(char* path, node_t* head);
int contains_bool(char* path,node_t* head);
void updateValue(char* path, node_t* head);
int sumOfValues(char* path, node_t* head);
void addNode(char* path, char* value, node_t* head);
char* getValue(node_t* node);
void destroy();
int isANumber(char* input);
void printNode(node_t* node);
void printLIst(node_t* first);
char* getPath(node_t* node);
int my_strcmp(char *a, char *b);
node_t* getNextNode(node_t* node);
void addNodeFromFoldes(char* path, char* value, node_t* head);




#endif /* ARGSLIST_H_ */
