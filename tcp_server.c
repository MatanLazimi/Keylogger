/*
 C socket server example, handles multiple clients using threads
 Compile
 gcc server.c -lpthread -o server
 */

#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread

#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>

#include "tree.h"
#include "files.h"
#include "argsList.h"

//the thread function

int isInput(char* c_str);
void *connection_handler(void *);
char* getValuMes(char* input);
char* getPathMes(char* input);
void readFolders();
char** printdir(char *dir, int depth, char* hh, int* size, char** arry_double);
char* delete_memory(char *str, char* arr);
char* memory_str(char *str, char a);
char** memory_dir(char** arry_double,char* name, int* size);
void readfileU();

int flag = 0;
char** arrOfDir;
int arr_size;
node_t* head = NULL;
char* fileNmae = "ServerFile.txt";
int main(int argc, char *argv[]) {
	int socket_desc, client_sock, c;
	struct sockaddr_in server, client;

	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1) {
		printf("Could not create socket");
	}

	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(4444);

	//Bind
	if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");

	//Listen
	listen(socket_desc, 3);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	pthread_t thread_id;

	while ((client_sock = accept(socket_desc, (struct sockaddr *) &client,
			(socklen_t*) &c))) {
		puts("Connection accepted");

		if (pthread_create(&thread_id, NULL, connection_handler,
				(void*) &client_sock) < 0) {
			perror("could not create thread");
			return 1;
		}

		//Now join the thread , so that we dont terminate before the thread
		//pthread_join( thread_id , NULL);
		puts("Handler assigned");
	}

	if (client_sock < 0) {
		perror("accept failed");
		return 1;
	}

	return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc) {
	//Get the socket descriptor
	int sock = *(int*) socket_desc;
	int read_size;
	char *message, client_message[2000];

	//Send some messages to the client

	message = "Greetings! I am your connection handler\n";
	write(sock, message, strlen(message));

	message = "Now type something and i shall repeat what you type \n";
	write(sock, message, strlen(message));

	//Receive a message from client
	int flagReed = 0;

	while ((read_size = recv(sock, client_message, 2000, 0)) > 0) {
		//end of string marker
		client_message[read_size] = '\0';
		if (flag != 0 && client_message[0] == 'e'
				&& strlen(client_message) == 2) {
			write(sock , "Exit:\n" ,strlen("Exit:\n"));
			break;
		}

		//Send the message back to client
		else if (flag == 0) {
			//write(sock, client_message, strlen(client_message));
			if (client_message[0] == 't' && strlen(client_message) == 2) {
				flag = 1;
				write(sock , "Wellcom to trees:\n" ,strlen("Wellcom to trees:\n"));
				//readFolders();
				if(flagReed == 0){
					flagReed = 1;
					readFolders();
				}

			}
			if (client_message[0] == 'f' && strlen(client_message) == 2) {						
				write(sock , "Wellcom to files:\n" ,strlen("Wellcom to files:\n"));
				flag = 2;
				if (flagReed == 0) {
					if (access(fileNmae, F_OK) != -1) {
						flagReed = 1;
						readfileU();

					}

				}
			}
		}  
		else if (flag != 0) //we got 'f' as argument
				{
			
			if (isInput(client_message) == 1) {
				addNode(getPathMes(client_message),getValuMes(client_message), head);
			}
			else{
				char* keeper = activateAction(client_message,head);
				if(keeper != NULL){
					write(sock, keeper, (int) strlen((char*) keeper));
				}
				else{
					write(sock, "", (int) strlen((char*) ""));
				}
			}

		}
		//clear the message buffer
		memset(client_message, 0, 2000);
	}
	if(flag == 1){
		char* temp23 = buildTree(head);
	}
	else if(flag == 2){
		char* temp24 = buildPath(head);
	}
	
	if (read_size == 0) {
		puts("Client disconnected");
		//buildPath(head);
		fflush(stdout);
	} else if (read_size == -1) {
		perror("recv failed");
	}

	return 0;
}

char* getValuMes(char* input) {
	char* help = (char*) malloc(strlen(input));
	strcpy(help, input);
	int i = 0;
	while (help[i] != ' ') {
		i++;
	}
	i++;
	help += i;
	return help;
}

char* getPathMes(char* clientM) {
	char* input =(char*)malloc(sizeof(clientM));
	strcpy(input,clientM);
	char* ans = strtok(input, " ");
	strcat(ans, " ");
	return ans;

}

int isInput(char* c_str) { // return 1 if c_str is input
	char* result = strchr(c_str, ' ');
	int end = strlen(c_str)-2;

	if(c_str[end] == ' ') // if there is ' ' in the end
		return -1;
	if(result != NULL && c_str[0] == '.')
		return 1;

	return 0;
}

char** memory_dir(char** arry_double,char* name, int* size){

	char** temp=NULL; 
	temp=(char**)malloc((*size+1)*sizeof(char*));
	for(int i=0;i<*size;i++){


		(temp)[i]=(char*)malloc(strlen((arry_double)[i])*sizeof(char));

		strcpy((temp)[i],(arry_double)[i]);

	}
	*size=*size+1;
	arry_double=(char**)malloc((*size+1)*sizeof(char*));
	for(int i=0;i<*size-1;i++){

		(arry_double)[i]=(char*)malloc(strlen((temp)[i])*sizeof(char));
		strcpy((arry_double)[i],(temp)[i]);

	}
	(arry_double)[*size-1]=(char*)malloc(strlen(name)*sizeof(char));
	strcpy((arry_double)[*size-1],name);

	return arry_double;

}




char* memory_str(char *str, char a){


	char* temper=NULL;
	int len=strlen(str);
	temper=(char*)malloc((len+1)*sizeof(char));
	strcpy(temper,str);
	temper[len]='\0';
	if(str!=NULL){

		free(str);
	}
	len=strlen(temper)+1;
	str=(char*)malloc((len+1)*sizeof(char));
	strcpy(str,temper);
	str[len-1]=a;
	str[len]='\0';
	if(temper!=NULL){

		free(temper);
	}
	return str;
	
}

char* delete_memory(char *str, char* arr){

	char* temp=NULL;
	temp=(char*)malloc(0*sizeof(char));
	int s=strlen(str)-strlen(arr);
	for(int i=0;i<s;i++){

		temp=memory_str(temp,str[i]);
	}
	temp[s]='\0';
	if(str!=NULL){

		free(str);

	}

	str=(char*)malloc(s+1*sizeof(char));
	strcpy(str,temp);
	str[s]='\0';
	if(temp!=NULL){
		free(temp);
	}
	return str;	

}

char** printdir(char *dir, int depth, char* hh, int* size, char** arry_double)
{
    
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return NULL;
    }
    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            /* Found a directory, but ignore . and .. */
            if(strcmp(".",entry->d_name) == 0 ||
                strcmp("..",entry->d_name) == 0)
                continue;

		if(hh==NULL){

			hh=(char*)malloc(strlen(entry->d_name)*sizeof(char));
			strcpy(hh,entry->d_name);
			//printf("start: %s\n", hh);
		}
		else{
			for(int i=0;i<strlen(entry->d_name);i++){
				hh=memory_str(hh,entry->d_name[i]);
			}

		}

		hh=memory_str(hh,'.');

		//printf("middle: %s\n", hh);
            /* Recurse at a new indent level */
        arry_double=printdir(entry->d_name,depth+4,hh,size,arry_double);
		if(hh!=NULL){
			hh=NULL;
			hh=malloc(1*sizeof(char));
			hh[0] = '\0';
		}
        }
        else{
		char* arr;
		for(int p=0;p<strlen(entry->d_name);p++){

			arr=memory_str(hh,entry->d_name[p]);
		}
		arry_double=memory_dir(arry_double,arr,size);
		arr=delete_memory(arr,entry->d_name);
	     }
    }
    chdir("..");
    closedir(dp);
	return arry_double;

}



void readFolders(){
	char temp1[PATH_MAX];
	char* arr_hi;
	arr_size=0;
	int i;
	arr_hi=(char*)malloc(0*sizeof(char));
	if (getcwd(temp1, sizeof(temp1)) == NULL) {
       puts("getcwd() error");
       return;
   }
	arrOfDir=printdir(temp1,0,arr_hi,&arr_size,arrOfDir);
	// for(i = 0 ; i<arr_size; i ++){
	// 			puts(arrOfDir[i]);

	// }
	char charty[100];
	getcwd(charty, 100);
	strcat(charty,"/prj");
	chdir(charty);
	int oldSize = 0;	
	char* copyDir;
	for(i = 0 ; i < arr_size ; i++ ){
		copyDir = (char*)malloc(strlen(arrOfDir[i]));
		strcpy(copyDir,arrOfDir[i]);
		char* placeChar = strchr(copyDir,'.');
		int indexAfterPoint = placeChar - copyDir;
		if (copyDir[0] == '_' ||(strchr(copyDir,'.') && strchr(&(copyDir[indexAfterPoint]),'_'))){	
			
			char* helper = (char*)malloc(strlen(copyDir));
			char* helper2 = (char*)malloc(strlen(copyDir));
			strcpy(helper,copyDir);
			strcpy(helper2,copyDir);
			char* ans = strtok(helper, "_");
			char *string = copyDir;
			char *e;
			int index;
			e = strchr(string, '_');
			index = (int)(e - string);
			index++;
			if(strchr(ans, '.'))
				strcat(ans,&(string[index]));

			///////////////////Replace A character////////////////////////////
			char *str_tm, ch = '.', Newch = '/';
  			int i;
			str_tm=(char*)malloc(strlen(helper2));
			//strcpy(str_tm,"/");
			strcpy(str_tm,helper2);
			for(i = 0; i <= strlen(str_tm); i++){
  					if(str_tm[i] == ch)  
					{
  						str_tm[i] = Newch;
 					}
			}
			FILE * fp2;
			char * line2 = NULL;
			size_t len2 = 0;
			ssize_t read;			

			fp2 = fopen(str_tm, "r");	
			//oldSize+=6;		
			if (fp2 == NULL){	 
				puts("Wrong input");
				return;
			}
			read = getline(&line2, &len2, fp2);			
			char help[1024];
			strcpy(help, line2);
			
			char * str_tmstr;
			str_tmstr=(char*)malloc(strlen(ans)+2);
			strcpy(str_tmstr,".");
			strcat(str_tmstr,ans);
			strcat(str_tmstr," ");

			addNodeFromFoldes(str_tmstr,help,head);

			fclose(fp2);
		}
	}

	return;
}

void readfileU() {
	FILE * fp;
		char * line = NULL;
		size_t len = 0;
		ssize_t read;

		fp = fopen("ServerFile.txt", "r");
		if (fp == NULL)
			return;
		while ((read = getline(&line, &len, fp)) != -1) {

			char help[1024];
			strcpy(help, line);
			addNode(getPathMes(help),getValuMes(help) ,head);
		}
		fclose(fp);
		if (line)
			free(line);

}
