Aim:To implement a search program in C program using system calls for files and 
directories. 
************************************************************************
Description:
1.The c program that takes the directory name as command-line arguments and reads the directory and displays the hierarchy (files and sub folders) in it
2.If there is no input given the program dispalys all the current directory file hirarchy
3.specified format when executed with -S option which lists the files and directory's along with the file size
4.specified format when executed with -s 1024 option which List all files with size >= 1024 bytes
5.specified format when executed with -f option which list all files with the substring provided
6.specified format when executed with -f and -s options which list all files with the substring provided and whose file 
7.Also a makefile to build the program
Algorithm :
**********************************************************************
1.main function to pass the command line arguements (directory name,inputs options(-s,-f,-S)
2.Function to dispaly the type of the files inside the directory while printing
3.Function to perform reading the directory and displaying the hierachy of it according to the input provided 

***********************************************************************  
To compile :$ gcc -o dr dr.c
while executing give the directory name 
To execute: $./dr ../Test
Output:
Displaying the hierarchy of the directory
Test
Sample
Test/TestSub
Sample1
*******************************************************************
To execute : $./dr 
Output :
lists all the current directory files and folders 
***********************************************************************
To execute: $./dr -s 1024
Output:
lists all the directory files and folders of file size >=1024
***********************************************************************
To execute:$./dr -f txt
Output:
lists all the directory files having substring txt
*********************************************************************
To execute:$./dr -f txt -s 1024
Output:
Lists all the directory files having substring txt and whose size is >=1024
*********************************************************