#include <stdio.h>
#include <stdlib.h>
#include <string.h> 	// For strtok() and strcmp()
#include <unistd.h> 	// For fork(), pid_t
#include <sys/wait.h> 	// For waitpid() and associated macros
#include <dirent.h>     //For EXIT codes and error handling
#include <errno.h>

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define MAX_LINE 256
char SHELL_NAME[50] = "uab_sh";
int QUIT = 0;
char history[1000000][MAX_LINE]; //history array to store history commands(can store maximum 100000 commands)
int count = 0;//count denotes total no.of commands executed until now
int exc=0;//to check the exit command 

// Function to read a line from command into the buffer
char *readLine()
{
	char *line = (char *)malloc(sizeof(char) * 1024); // Dynamically Allocate Buffer
	char c;
	int pos = 0, bufsize = 1024;
	if (!line) // Buffer Allocation Failed
	{
		printf("\nBuffer Allocation Error.");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		c=getchar();
		if (c == EOF || c == '\n') // If End of File or New line, replace with Null character
		{
			line[pos] = '\0';
			return line;
		}
		else
		{
			line[pos] = c;
		}
		pos ++;
		// If we have exceeded the buffer
		if (pos >= bufsize)
		{
			bufsize += 1024;
			line = realloc(line, sizeof(char) * bufsize);
			if (!line) // Buffer Allocation Failed
			{
			printf("\nBuffer Allocation Error.");
			exit(EXIT_FAILURE);
			}
		}
	}

}

// Function to split a line into constituent commands
char **splitLine(char *line)
{
	char **tokens = (char **)malloc(sizeof(char *) * 64);
	char *token;
	char delim[10] = " \t\n\r\a";
	int pos = 0, bufsize = 64;
	if (!tokens)
	{
		printf("\nBuffer Allocation Error.");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, delim);
	while (token != NULL)
	{
		tokens[pos] = token;
		pos ++;
		if (pos >= bufsize)
		{
			bufsize += 64;
			line = realloc(line, bufsize * sizeof(char *));
			if (!line) // Buffer Allocation Failed
			{
			printf("\nBuffer Allocation Error.");
			exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, delim);
	}
	tokens[pos] = NULL;
	return tokens;
}

// Section Dealing with Built-in Commands

// Function Declarations
int myShell_cd(char **args);
int myShell_exit();
int hello(char **args);
int fib(char **args);
int help(char ** args);
int list(char **args);
// int hist();

// Definitions
char *builtin_cmd[] = {"cd", "quit", "hello", "fibonacci", "help", "list"};

int (*builtin_func[]) (char **) = {&myShell_cd, &myShell_exit, &hello, &fib, &help, &list}; // Array of function pointers for call from execShell

int numBuiltin() // Function to return number of builtin commands
{
	return sizeof(builtin_cmd)/sizeof(char *);
}

int list(char **args)
{
		//Here we will list the directory
	int op_a, op_l;
	struct dirent *d;
	DIR *dh = opendir(args[0]);
	if (!dh)
	{
		if (errno = ENOENT)
		{
			//If the directory is not found
			perror("Directory doesn't exist");
		}
		else
		{
			//If the directory is not readable then throw error and exit
			perror("Unable to read directory");
		}
		exit(EXIT_FAILURE);
	}
	//While the next entry is not readable we will print directory files
	while ((d = readdir(dh)) != NULL)
	{
		//If hidden files are found we continue
		if (!op_a && d->d_name[0] == '.')
			continue;
		printf("%s  ", d->d_name);
		if(op_l) printf("\n");
	}
	if(!op_l)
	printf("\n");

}

int help(char **args)
{
	printf(ANSI_COLOR_GREEN   "----------Help--------"   ANSI_COLOR_RESET "\n");
	printf(ANSI_COLOR_GREEN   "Not all the internal commands are supported."   ANSI_COLOR_RESET "\n");
	printf(ANSI_COLOR_GREEN   "Supported internal commands: cd, help, history, quit, list "   ANSI_COLOR_RESET "\n");
	printf(ANSI_COLOR_GREEN   "cd <path>  --change directory"   ANSI_COLOR_RESET "\n");
	printf(ANSI_COLOR_GREEN   "hello -- interact with shell "   ANSI_COLOR_RESET "\n");
	printf(ANSI_COLOR_GREEN   "fibonacci -- get fibonacci series of given number"   ANSI_COLOR_RESET "\n");printf(ANSI_COLOR_GREEN   "list -- list all files and folders"   ANSI_COLOR_RESET "\n");
	printf(ANSI_COLOR_GREEN   "history -- list previously used commands"   ANSI_COLOR_RESET "\n");
	printf(ANSI_COLOR_GREEN   "help -- print this help section"   ANSI_COLOR_RESET "\n");
	printf(ANSI_COLOR_GREEN   "quit -- exit the shell"   ANSI_COLOR_RESET "\n");
	
}
int myShell_cd(char **args)
{
	if (args[1] == NULL) 
	{
		printf("Shell: expected argument to \"cd\"\n");
	} 
	else 
	{
		if (chdir(args[1]) != 0) 
		{
			perror("Shell: ");
		}
	}
	return 1;
}

int myShell_exit()
{
	QUIT = 1;
	return 0;
}

int hello(char **args)
{
	printf("Hello World!\n");
}

int fib(char **args)
{
	int n1=0,n2=1,n3,i,number;    
 	printf("Enter the number of elements:");    
 	scanf("%d",&number); 
 	printf("\n%d %d",n1,n2);//printing 0 and 1    
 	for(i=2;i<number;++i)//loop starts from 2 because 0 and 1 are already printed    
 	{    
		n3=n1+n2;    
		printf(" %d",n3);    
		n1=n2;    
		n2=n3;    
 	}
	printf("\n");  
  return 0;
}


// Function to create child process and run command
int myShellLaunch(char **args)
{
	pid_t pid, wpid;
	int status;
	pid = fork();
	if (pid == 0)
	{
		// The Child Process
		if (execvp(args[0], args) == -1)
		{
			perror("Shell: ");
		}
	exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		//Forking Error
		perror("Shell: ");
	}
	else
	{
		// The Parent Process
	do 
	{
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}

// Function to execute command from terminal
int execShell(char **args)
{
	int ret;
	if (args[0] == NULL)
	{
		// Empty command
		return 1;
	}
	// Loop to check for builtin functions
	for (int i=0; i< numBuiltin(); i++) // numBuiltin() returns the number of builtin functions
	{
		if(strcmp(args[0], builtin_cmd[i])==0) // Check if user function matches builtin function name
			return (*builtin_func[i])(args); // Call respective builtin function with arguments
	}
	ret = myShellLaunch(args);
	return ret;
}

// Read and Parse from Config File
int readConfig()
{
	FILE *fptr;
	char line[200];
	char **args;
	fptr = fopen("config", "r");
	if (fptr == NULL)
	{
		printf("Unable to find config file.\n");
		return 1;
	}
	else
	{
		while(fgets(line, sizeof(line), fptr) != NULL)
		{
			printf("\n%s", line);
			args=splitLine(line);
			if(strcmp(args[0], "export")==0)
				strcpy(SHELL_NAME, args[1]);
		}
	}
	free(args);
	fclose(fptr);
	return 1;
}

// When myShell is called Interactively
int myShellInteract()
{
	char *line;
	char **args;
	while(QUIT == 0)
	{
		printf("%s> ", SHELL_NAME);
		line=readLine();
		args=splitLine(line);
		execShell(args);
		free(line);
		free(args);
	}
	return 1;
}

// When myShell is called with a Script as Argument
int myShellScript(char filename[100])
{
	printf("Received Script. Opening %s", filename);
	FILE *fptr;
	char line[200];
	char **args;
	fptr = fopen(filename, "r");
	if (fptr == NULL)
	{
		printf("\nUnable to open file.");
		return 1;
	}
	else
	{
		printf("\nFile Opened. Parsing. Parsed commands displayed first.");
		while(fgets(line, sizeof(line), fptr)!= NULL)
		{
			printf("\n%s", line);
			args=splitLine(line);
			execShell(args);
		}
	}
	free(args);
	fclose(fptr);
	return 1;
}

int main(int argc, char **argv)
{
	// Read from myShell Configuration Files
	readConfig();
	// hist();
	// Parsing commands Interactive mode or Script Mode
	if (argc == 1)
		myShellInteract();
	else if (argc == 2)
		myShellScript(argv[1]);
	else
		printf("\nInvalid Number of Arguments");

	// Exit the Shell
	return EXIT_SUCCESS;
}