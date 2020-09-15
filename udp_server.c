
// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>

#include "tree.h"
#include "files.h"
#include "argsList.h"

#define PORT 7773
#define MAXLINE 1024 
char* getValuMes(char* input);
char* getPathMes(char* clientM);
int isInput(char* c_str);
void readfileU();
void readFolders();
char** printdir(char *dir, int depth, char* hh, int* size, char** arry_double);
char* delete_memory(char *str, char* arr);
char* memory_str(char *str, char a);
char** memory_dir(char** arry_double,char* name, int* size);
// Driver code 

char** arrOfDir;
int arr_size;
node_t* head = NULL;
char* fileNmae = "ServerFile.txt";
int main() {
	int sockfd;
	char client_messege[MAXLINE];
	char *hello = "Hello from server\n";
	struct sockaddr_in servaddr, cliaddr;

	// Creating socket file descriptor /
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	puts(hello);
	readFolders();
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	// Bind the socket with the server address 
	if (bind(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr))
			< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	int len, n;
	//write to client: printf(messege)
	//write to server: write(sockfd , client_messege , strlen(client_messege));
	int flag = 0;
	int flagReed = 0;
	while (n != NULL && n != 0) {
		n = recvfrom(sockfd, client_messege, MAXLINE, MSG_WAITALL,
				(struct sockaddr *) &cliaddr, &len);

		client_messege[n] = '\0';

		if (flag != 0 && client_messege[0] == 'e' // before exit build file or tree
		&& strlen(client_messege) == 2) {
			if (flag == 1) {
				char* a = buildTree(head);
			} else if (flag == 2) {
				buildPath(head);
			}
		}

		//for the first input 'f' for file 't' for tree
		else if (flag == 0) {
			char* messege;
			if (client_messege[0] == 't' && strlen(client_messege) == 2) {
				messege = "Welcome to tree:\n";
				sendto(sockfd, messege, strlen(messege), MSG_CONFIRM,
						(const struct sockaddr *) &cliaddr, len);
				flag = 1;
			}
			if (client_messege[0] == 'f' && strlen(client_messege) == 2) {
				flag = 2;
				messege = "Welcome to files:\n";
				sendto(sockfd, messege, strlen(messege), MSG_CONFIRM,
						(const struct sockaddr *) &cliaddr, len);
				if (flagReed == 0) {
					if (access(fileNmae, F_OK) != -1) {
						flagReed = 1;
						readfileU();

					}

				}

			}

		} else if (flag != 0) //we got 'f' or 't' as argument
				{

			if (isInput(client_messege) == 1) {
				addNode(getPathMes(client_messege), getValuMes(client_messege),
						head);
			} else {

				char* keeper = activateAction(client_messege, head);
				if (keeper != NULL && strcmp(keeper, "") != 0) {
					sendto(sockfd, keeper, strlen(keeper), MSG_CONFIRM,
							(const struct sockaddr *) &cliaddr, len);

				}
			}
		}
		//clear the message buffer
		memset(client_messege, 0, 2000);

	}

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
	//return "hh";
}
char* getPathMes(char* clientM) {
	char* input = (char*) malloc(sizeof(clientM));
	strcpy(input, clientM);
	char* ans = strtok(input, " ");
	strcat(ans, " ");
	return ans;
}
int isInput(char* c_str) { // return 1 if c_str is input
	char* result = strchr(c_str, ' ');
	int end = strlen(c_str) - 2;

	if (c_str[end] == ' ') // if there is ' ' in the end
			{
		return -1;
	}
	if (result != NULL && c_str[0] == '.') {
		return 1;
	}
	return 0;
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


