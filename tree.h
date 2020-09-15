/*
 * tree.h
 *
 *  Created on: May 8, 2019
 *      Author: yahel
 */

#ifndef TREE_H_
#define TREE_H_
#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <sys/stat.h>
#include <sys/types.h>
#include <stddef.h>

#include "argsList.h"

char* buildTree(node_t* head);
static void _mkdir(node_t* head);
#endif /* TREE_H_ */
