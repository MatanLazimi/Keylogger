#include "tree.h"

/*
 * tree.c
 *
 *  Created on: May 8, 2019
 *      Author: yahel
 */

char* buildTree(node_t* head){
        current = head;
        if (head == NULL){
		return "Empty Tree Folders";
	}

	while (current != NULL) {
                _mkdir(current);
		current = getNextNode(current);
	}
	return "";
}

static void _mkdir(node_t* head) {
        char tmp[256];
        char nameOfFile[256];
        char *p = NULL;
        size_t len;
        char fName[20];
        int count1 = 0;
        int count2 = 0;
        int i = 0;
        char* strPath =(char*)malloc(strlen(getPath(head))+1);
        strcpy(strPath,"");
        strcat(strPath,getPath(head));
        strcpy(tmp,strPath);
        len = strlen(tmp);
        if(tmp[len - 1] == '.')
                 tmp[len - 1] = 0;
        for(p = tmp ; *p; p++) //counting points
                if(*p == '.') 
                        count1++;

        for(p = tmp ; *p; p++){
                if(*p == '.') {
                        *p = 0;
			//puts(tmp);                        
			mkdir(tmp, S_IRWXU);
                        *p = '/';
                }
        }
        for(p = tmp ; *p; p++){
                if(*p =='/')
                        count2++;
                if(*p == '/' && count2 == count1 ) {
                        count2++;
                        p++;
                        strcpy(nameOfFile,p);
                        char* ptrHelp;
                        // for(ptrHelp = p; *(ptrHelp+1) != NULL ; ptrHelp++){
                        //         *ptrHelp=*(ptrHelp+1);
                        // }                                             
                        *p = '_';
                        p++;
                        *p='\0';
                        strcat(p,nameOfFile);
                        break;
                }
        }
       //if the file is not in folder!
        char* help_1[10];
        if(count1 == 0){
               strcpy(help_1,"_");
               strcat(help_1,tmp);
       }
       else
        help_1[0]='\0';
       
       
        FILE* fptr;
        if(help_1[0] == '\0'){
                fptr = fopen(tmp,"w+");
        }
        else{
                fptr = fopen(help_1,"w+");
        }
        if(fptr == NULL){
                printf("error");
                return;
        } 
        fputs(getValue(head), fptr);
        fclose(fptr);

}
