Aim:To implement Implement a simple shell or command line interpreter.
************************************************************************
Description:
1.To write a simple shell in C called uab_sh.  .
2.The shell will run continuously, and displays a prompt when waiting for input. When the shell is invoked by executing the executable uab_sh, it will provide a prompt uab_sh > and waits for any input from the keyboard and based on the command line input it will the output accordingly as below
***********************************************************************  
To compile :make
To execute: ./myShell.exe
uab_sh > hello 
Hello World!  
uab_sh > fibonacci 10 
The first 10 values: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34 
uab_sh > fibonacci 
How many elements you want to display: 7 
The first 7 value: 0, 1, 1, 2, 3, 5, 8 
uab_sh >help
------------------Help---------------------
Not all the internal commands are supported
cd<path> --chnge directory
hello --- interact with shell
fibonacci --get fibonacci series 
list --list all files and folders
history --list previously used commands
help -- print this help section
quit --exit the shell
uab_sh >ls
Make file myshell.c 
uab_sh> cd New folder
uab_sh> quit
... program finished with exit code 0
Press enter to exit console
*******************************************************************