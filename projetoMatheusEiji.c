#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include<linux/limits.h>
#include <limits.h>
#include <sys/wait.h>

/* Spawn a child process running a new program.  PROGRAM is the name
   of the program to run; the path will be searched for this program.
   ARG_LIST is a NULL-terminated list of character strings to be
   passed as the program's argument list.  Returns the process id of
   the spawned process.  */

int spawn (char* program, char** arg_list)
{
  pid_t child_pid;
  /* Duplicate this process.  */
  child_pid = fork ();

  if (child_pid == 0){
    /* Now execute PROGRAM, searching for it in the path.  */
    execvp (program, arg_list);
    /* The execvp function returns only if an error occurs.  */
    fprintf (	stderr, "an error occurred in execvp\n");
  }else if (child_pid < 0){
  perror("Erro ao criar processo filho");
  } else {
  	int status;
  	waitpid(child_pid, &status,0);
  }
}

int main (int argc, char* argv[])
{
 	char *path_or_name;
	do{	
	  	char* arg_list[15];    /* The argument list must end with a NULL.  */
		char cmd[100];
		char* token;
		
		char cwd[PATH_MAX];
		getcwd(cwd,sizeof(cwd));
		char *user;
		user = getenv("USER");
		
		char hostname[HOST_NAME_MAX + 1];
		gethostname(hostname, HOST_NAME_MAX + 1);
		fflush(stdin);
		printf("[MyShell] %s@%s:~%s ",user,hostname,cwd);
		fgets(cmd,100, stdin);// Contains the path or name, with the arguments
		
		cmd[strcspn(cmd, "\n")] = '\0';
		if (!strcmp(cmd,"exit"))
			return 0;
			
		token = strtok(cmd," ");
		
		path_or_name = token; // Points to the path or name of the program
		

		int i=0;
		while(token != NULL){
			arg_list[i] = token;
			i++;
			token = strtok(NULL, " ");
		};
		arg_list[i] = NULL;
	  /* Spawn a child process running the "ls" command.  Ignore the
	     returned child process id.  */
	     
	     	// Treating relative path
	     	if ((strstr(path_or_name,"/")!= NULL) && (strstr(path_or_name,"/home") == NULL)){
	     		char aux[100];
	     		strcpy(aux, cwd);
	     		strcat(aux, "/");
	     		strcat(aux, path_or_name);
	     		strcpy(path_or_name,aux);
	     	}
	     	printf ("%s\n", path_or_name);
	 	spawn (path_or_name, arg_list); 
 	}while(strcmp(path_or_name,"exit"));

  return 0;
}
