#include "files.h"

//typedef struct node_t;
//struct node_t * current;
char* buildPath(node_t * head) {
	FILE* fPointer;
	current = head;
	fPointer = fopen("ServerFile.txt", "w");
	
	if (head == NULL){
		fclose((fPointer));
		return "Empty";
	}

	while (current != NULL) {
		char* str = (char*) malloc(strlen(getPath(current)));
		strcpy(str, getPath(current));
		strcat(str, " ");
		strcat(str, getValue(current));
		fputs(str, fPointer);
		current = getNextNode(current);
	}
	fclose((fPointer));
	return "OK!";
}


char* activateAction(char* str_c, node_t* head) {
	char* p = str_c;
	p[strlen(p)-1] = 0; 
	if (str_c == NULL || head == NULL) {
		return NULL;
	}
	int count;
	int num;
	int temp = 0;
	char* pathKeeper;
	char* valueKeeper;
	int valueSize;
	int oldPathSize;
	switch (str_c[0]) {
		case '.':
			return getValueByPath(str_c+1, head);
		case '+':
			updateValue(str_c+1, head);
			return "";
			break;
		case '*':
			num=0;
			num = sumOfValues(str_c+1, head);
			temp = num;
			count = 0;
			while (temp != 0) {
				temp /= 10;
				++count;
			}
			char* str =(char*)malloc(count + 1);
			sprintf(str, "%d\n", num);
			return str;

		case '-':
			pathKeeper = str_c+1;
			valueKeeper = getValueByPath(pathKeeper, head);
			oldPathSize = strlen(pathKeeper);
			char arr[20];
			sprintf(arr,"%d",oldPathSize);			
			valueSize = strlen(valueKeeper);
			sprintf(arr,"%d",valueSize);
			////////////////////////////////////////////////
			char* newPath = (char*)malloc(valueSize + oldPathSize);
			strcpy(newPath,pathKeeper);
			strcat(newPath,".");
			strcat(newPath, valueKeeper);
			char* p = newPath;
			p[strlen(p)-1] = '\0';
			return getValueByPath(newPath, head);
		default:
			return "";
	}

}