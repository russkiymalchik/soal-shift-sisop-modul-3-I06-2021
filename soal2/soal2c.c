#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<fcntl.h>
#include<errno.h>

int main() 
{ 
	// We use two pipes 
	// First pipe to connect the result from ps aux -> sort -nrk 3,3
	// Second pipe to connect the result from sort -nrk 3,3 -> head -5

	int fd1[2]; // Used to store two ends of first pipe 
	int fd2[2]; // Used to store two ends of second pipe 

	char input_str[100]; 
	pid_t p; 

	if (pipe(fd1)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
	if (pipe(fd2)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
 
	p = fork(); 
	if (p < 0) 
	{ 
		fprintf(stderr, "fork Failed" ); 
		return 1; 
	} 

	// Parent process 

	if (p == 0) 
    {
		close(fd1[0]); // Close reading end of first pipe 
		
        dup2(fd1[1], STDOUT_FILENO);
        char *argv[] = {"ps", "aux", NULL};
		execv("/bin/ps", argv);
        exit(EXIT_SUCCESS);
	} 

    // Child process
    else 
    {
		wait(NULL);
		p = fork();
		if (p < 0) 
        {
			exit(EXIT_FAILURE);
	    }

	    if (p == 0)
        {
		    close(fd1[1]); // Close writing end of first pipe 
            dup2(fd1[0], STDIN_FILENO);
		    
            close(fd2[0]); // Close reading end of second pipe 
		    dup2(fd2[1], STDOUT_FILENO);
		    
            char *argv[] = {"sort", "-nrk 3,3", NULL};
		    execv("/usr/bin/sort", argv);
            exit(EXIT_SUCCESS);
	    }

        else
        {
		    close(fd2[1]); // Close writing end of second pipe 
            close(fd1[1]); // Close writing end of first pipe
            close(fd1[0]); // Close reading end of first pipe
		    
            wait(NULL);
		    
            dup2(fd2[0], STDIN_FILENO);
		    char *argv[] = {"head", "-5", NULL};
		    execv("/usr/bin/head", argv);
            exit(EXIT_SUCCESS);
	    }
	}
}
