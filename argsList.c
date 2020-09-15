
#include "argsList.h"



struct node {
	char* val;
	char* path;
	struct node* next;
	int size;
};
typedef struct node node_t;

node_t* head;


node_t* createArgsLIst()
{
	head = malloc(sizeof(node_t));
	if (head == NULL)
	{
		return NULL;
	}
	head->next = NULL;
	head->size = 1;
}
node_t* createArgsLIstWithArguments(char* path, char* val)
{
	head = malloc(sizeof(node_t));
	if (head == NULL)
	{
		return NULL;
	}
	head->next = NULL;
	head->size = 1;
	head->path = (char*)malloc(strlen(path));
	head->val = (char*)malloc(strlen(val));
	strcpy(head->val, val);
	strcpy(head->path, path);
	return head;
}

int contains_bool(char* path, node_t* head) // 0 - not exist   1 - exist
{
	if (head == NULL)
	{
		return 0;
	} 
	else
	{
		node_t* current = head;
		while (current != NULL)
		{
			//if(strncmp(a, b, strlen(b)) == 0)
			if (strcmp(current->path, path) == 0)
			{
				return 1;
			}
			current = current->next;
		}
		return 0;
	}
}






void updateValue(char* path, node_t* head)
{
	if (head != NULL)
	{
		node_t* current = head;
		while (current != NULL)
		{
			if (my_strcmp(current->path, path) == 0)
			{
				char* p1 = current->val;
				p1[strlen(p1)-1] = '\0'; 
				if (isANumber(current->val))
				{
					int keeper = atoi(current->val);
					keeper++;
					sprintf(current->val, "%d\n", keeper); 
					return;
				}
				p1[strlen(p1)] = '\n'; 
			}
			current = current->next;
		}
	}
}


void addNode(char* path, char* value, node_t* head)
{
	if(path[0] == '.')
		path=path+1;//cancel the point in the start
	char* p = path;
	p[strlen(p)-1] = 0; 
	if (head == NULL)
	{
		createArgsLIstWithArguments(path, value);
		return;
	}
	node_t* current = head;
	if (contains_bool(path, head) == 1)
	{
		while (current != NULL )
		{

			if (strcmp(current->path, path) == 0){
				strcpy(current->val, value);
				return;
			}
			current = current->next;
		}
	}
	else
	{
		
		while (current->next != NULL)
		{
			current = current->next;
		}
		node_t* newNode = malloc(sizeof(node_t));
		newNode->next = NULL;
		newNode->val = (char*)malloc(strlen(value));
		newNode->path = (char*)malloc(strlen(path));
		strcpy(newNode->val, value);
		strcpy(newNode->path, path);
		current->next = newNode;
		head->size++;
	}

}

char* getValue(node_t* node)
{
	if (node == NULL)
	{
		return NULL;
	}
	return node->val;
}

char* getPath(node_t* node)
{
	if (node == NULL)
	{
		return NULL;
	}
	return node->path;
}

int getSize(node_t* node)
{
	if (node == NULL)
	{
		return NULL;
	}
	return node->size;
}
node_t* getNextNode(node_t* node)
{
	if (node == NULL)
	{
		return NULL;
	}
	return node->next;
}
char* getValueByPath(char* path, node_t* head)
{
	if (head == NULL)
	{
		return "";
	}
	node_t* current = head;
	while (current != NULL)
	{		
		if (my_strcmp(current->path, path) == 0)
		{
			return current->val;
		}
		current = current->next;
	}
	return "";
}

void destroy()
{
	if (head == NULL)
	{
		return;
	}
	current = head;
	while (current != NULL)
	{
		node_t* keeper = current;
		current = current->next;
		keeper->next = NULL;
		//free(keeper->val);
		//free(keeper->path);
		free(keeper);
	}
	head = NULL;
}

void printNode(node_t* node)
{
	if (head == NULL)
	{
		printf("list is empty");
		return;
	}
	printf(node->path);
	printf(" ");
	printf(node->val);

}


void printLIst(node_t* first)
{
	if (first == NULL)
	{
		printf("the list is empty");
		return;
	}
	node_t* runner = first;
	while (runner != NULL)
	{
		printNode(runner);
		runner = runner->next;
		printf("\n");
	}
	printf("\n");
}


int isANumber(char* input) // return 0 if not a number
{
	int size = strlen(input);
	int i = 0;
	for (i = 0; i<size; i++)
	{
		if (!isdigit(input[i])) {
			return 0;
		}
	}
	return 1;
}

int sumOfValues(char* path, node_t* head){
	if(path==NULL || head==NULL)
	{
		return 0;
	}
	int sum=0;
	node_t* current=head;
	while(current != NULL)
	{
		if(strncmp(current->path, path, strlen(path)) == 0)
		{
			if(current->path[strlen(path)]=='.' || current->path[strlen(path)]=='\0'||current->path[strlen(path)]==0){
				char* p1 = current->val;
				p1[strlen(p1)-1] = '\0'; 
				if(isANumber(current->val))
					sum += atoi(current->val);
				p1[strlen(p1)] = '\n'; 
			}
		}
		current = current->next;
	}
	return sum;
}

int my_strcmp(char *a, char *b)
{
    while (*a && *b && *a == *b) { ++a; ++b; }
    return (unsigned char)(*a) - (unsigned char)(*b);
}
void addNodeFromFoldes(char* path, char* value, node_t* head){
	path=path+1;//cancel the point in the start
	char* p = path;
	p[strlen(p)-1] = 0; 
	if (head == NULL)
	{
		createArgsLIstWithArguments(path, value);
		return;
	}
	node_t* current = head;
	if (contains_bool(path, head) == 1)
	{
		while (current != NULL )
		{

			if (strcmp(current->path, path) == 0){
				strcpy(current->val, value);
				return;
			}
			current = current->next;
		}
	}
	else
	{
		
		while (current->next != NULL)
		{
			current = current->next;
		}
		node_t* newNode = malloc(sizeof(node_t));
		newNode->next = NULL;
		newNode->val = (char*)malloc(strlen(value));
		newNode->path = (char*)malloc(strlen(path));
		strcpy(newNode->val, value);
		strcpy(newNode->path, path);
		current->next = newNode;
		head->size++;
	}



}
