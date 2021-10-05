#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
void rec_fork(int rec_argc, char *rec_argv[]);
int ps_tree[100];
int tree_top = 0;
int main(int argc,char *argv[]){
	if (argc==1) return 0;
	/* Implement the functions here */
	umask(0);
	pid_t pid;
	pid = fork();
	int status;
	if (pid == 0) {
		rec_fork(argc, argv);
	}
	else {
		int ret = mkfifo("./test.fifo",0664);
		int ret1=mkfifo("./test1.fifo",0664);
		int fd=open("./test.fifo",O_RDONLY);
		int fd1=0;
		if (argc>2)
		fd1=open("./test1.fifo",O_RDONLY);
		wait(&status);	
		char buf[1024] = {0};
		ret=read(fd,buf,1023);
		char buf_cp[1024]={0};
		strcpy(buf_cp,buf);
		char * divider = "->";
		char * token;
		char head[1024] = {0};
		char dest[1024] = {0};
		char* pid_ptr[argc-1];
		token = strtok(buf,divider);
		pid_ptr[0]=token;
		strcpy(head,token);
		for (int i=0;i<argc-2;i++) {
			token=strtok(NULL,divider);
			strcpy(dest,token);
			strcat(dest,"->");
			strcat(dest,head);
			strcpy(head,dest);
			pid_ptr[i+1]=token;
		}
		sprintf(head,"%d->",getpid());
		strcat(head,dest);
		if (argc>2)
		printf("process tree: %s\n",head);
		else printf("process tree: %s%s\n",head,token);
		close(fd);
		if (argc>2) {
			char buf1[1024]={0};
			ret1=read(fd1,buf1,1023);
			token = strtok(buf1,divider);
			for (int i=0;i<argc-2;i++) {
				printf("Child process %s",pid_ptr[i]);
				printf(" of parent process %s",pid_ptr[i+1]);
				if (strcmp(token,"0")==0) 
					printf(" terminated normally with exit code 0\n");
				else if (strcmp(token,"5")==0)
					printf(" is termineted by signal 5 (Trap)\n");
				else if (strcmp(token,"1")==0)
					printf(" is terminated by signal 1 (Hangup)\n");
				else if (strcmp(token,"2")==0)
					printf(" is terminated by signal 2 (Interrupt)\n");
				else if (strcmp(token,"3")==0)
					printf(" is terminated by signal 3 (Quit)\n");
				else if (strcmp(token,"4")==0)
					printf(" is terminated by signal 4 (Illeagel)\n");
				else if (strcmp(token,"6")==0)
					printf(" is terminated by signal 6 (Abort)\n");
				else if (strcmp(token,"8")==0) 
					printf(" is terminated by signal 8 (Floating)\n");
				else if (strcmp(token,"9")==0) 
					printf(" is terminated by signal 9 (Kill)\n");
				else if (strcmp(token,"11")==0) 
					printf(" is terminated by signal 11 (Segmentation)\n");
				else if (strcmp(token,"13")==0) 
					printf(" is terminated by signal 13 (Pipe)\n");
				else if (strcmp(token,"14")==0)
					printf(" is terminated by signal 14 (Alarm clock)\n");
				else if (strcmp(token,"15")==0) 
					printf(" is terminated by signal 15 (Termination)\n");
				else if (strcmp(token,"4991")==0)
					printf(" is terminated by signal 4991 (Stop)\n");
				else if (strcmp(token,"7")==0)
					printf(" is terminated by signal 7 (Bus)\n");
				else printf(" is terminated by UNKNOWN signal\n");
				if (i==argc-3) break;
				token=strtok(NULL,divider);
			}
			close(fd1);			
		}
		printf("child process %s of parent process %d",pid_ptr[argc-2],getpid());
		
		if (status==0) 
			printf(" terminated normally with exit code 0\n");
		else if (status==5)
			printf(" is termineted by signal 5 (Trap)\n");
		else if (status==1)
			printf(" is terminated by signal 1 (Hangup)\n");
		else if (status==2)
			printf(" is terminated by signal 2 (Interrupt)\n");
		else if (status==3)
			printf(" is terminated by signal 3 (Quit)\n");
		else if (status==4)
			printf(" is terminated by signal 4 (Illeagel)\n");
		else if (status==6)
			printf(" is terminated by signal 6 (Abort)\n");
		else if (status==8) 
			printf(" is terminated by signal 8 (Floating)\n");
		else if (status==9) 
			printf(" is terminated by signal 9 (Kill)\n");
		else if (status==11) 
			printf(" is terminated by signal 11 (Segmentation)\n");
		else if (status==13) 
			printf(" is terminated by signal 13 (Pipe)\n");
		else if (status==14)
			printf(" is terminated by signal 14 (Alarm clock)\n");
		else if (status==15) 
			printf(" is terminated by signal 15 (Termination)\n");
		else if (status==4991)
			printf(" is terminated by signal 4991 (Stop)\n");
		else if (status==7)
			printf(" is terminated by signal 7 (Bus)\n");
		else printf(" is terminated by UNKNOWN signal\n");
		printf("Myfork process (%d) terminated normally\n",getpid());
	}
	return 0;
	
}
void rec_fork(int rec_argc, char *rec_argv[]) {
	if (rec_argc == 1) return;
	if (rec_argc == 2) {
		umask(0);
		int ret=mkfifo("./test.fifo",0664);
		int fd=open("./test.fifo",O_WRONLY);
		char buf[1024]={0};
		sprintf(buf,"%d->",getpid());
		write(fd,buf,strlen(buf));
		close(fd);
		char * exe_arg[2];
		exe_arg[0] = rec_argv[1];
		exe_arg[1] = NULL;
		execve(exe_arg[0],exe_arg,NULL);
		exit(0);
	}
	// Not root process
	pid_t pid;
	int root_argc = 2;
	int status;
	int sterm_argc = rec_argc - 1;
	char * root_argv[root_argc];
	char * sterm_argv[sterm_argc];
	root_argv[0]=rec_argv[0];
	root_argv[1]=rec_argv[1];
	for (int i=1;i<sterm_argc;i++) {
		sterm_argv[i]=rec_argv[i+1];
	}
	sterm_argv[0]=rec_argv[0];
	pid=fork();
	if (pid != 0) {
		wait(&status);
		int ret1=mkfifo("./test1.fifo",0664);
		int fd1=open("./test1.fifo",O_WRONLY);
		char buf1[1024]={0};
		sprintf(buf1,"%d->",status);
		write(fd1,buf1,strlen(buf1));
		close(fd1);
		rec_fork(root_argc, root_argv);
	}
	else {
		rec_fork(sterm_argc, sterm_argv);
	}
	return;
}
