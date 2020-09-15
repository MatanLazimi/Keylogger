/*
* files.h
*
*  Created on: May 8, 2019
*      Author: yahel
*/

#ifndef FILES_H_
#define FILES_H_

#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen

#include "argsList.h"


char* buildPath(node_t * head);
char* activateAction(char* str_c,node_t* head);

node_t* current;


#endif /* FILES_H_ */


