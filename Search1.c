#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>

#define PATH_MAX 100000
/***
 *   this program list file from a given directory
 *   Usage: [directory] [-S] [-s] [limit in kb] [-f] [string to find]
 *   the option [-S] shows size in kbs
 *   the option [-s] followed by [size limit kbs] shows files of size greater than the limit
 *   the option [-f] followed by [string] shows files containing the string given
 *   example a.out .. -S -s 100 -f code
 *
 */
typedef struct stringData
{
    char *s;
    struct stringData *next;
} Node;
Node *head = NULL;
Node *tail = NULL;
Node *n;
Node *createNode(char *s)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->s = s;
    newNode->next = NULL;
    return newNode;
}

char *v1[1000];
char v2[1000];
int v;
int eflag;
void insert(Node **link, Node *newNode)
{
    newNode->next = *link;
    *link = newNode;
}

void printList(Node *head, char *command)
{
    FILE *myfilr;
    int ch;

    while (head != NULL)
    {
        strcat(command, " ");
        strcat(command, head->s);
        // printf("exec [%s]\n",command);
        myfilr = popen(command, "r");

        if (myfilr == NULL)
        {
            puts("unable to open the process");
            exit(1);
        }

        while ((ch = fgetc(myfilr)) != EOF)
        {
            putchar(ch);
        }
        pclose(myfilr);

        // printf("%s\n", head->s);
        head = head->next;
    }
}

void tree(char *basePath, const int root, const int showSize, const int sizeLimit, char *findStr);

void arrayCreater(char filePath[4097], char arrayOfFilePaths[]);
int executeLinuxCommands(char *command);
char arrayOfFilePaths[1000][40];

int main(int argc, char *argv[])
{
    // Directory path to list files
    char *path = ".";
    int showSize = 0;
    int sizeLimit = 0;
    char *findStr = 0;
    char *inpitCommand = 0;
    v = 0;
    strcpy(v2, "");
    int opt;
    if (argv[1] != NULL && strcmp(argv[1], "-s") != 0 && strcmp(argv[1], "-S") != 0 && strcmp(argv[1], "-f") != 0)
    {
        path = argv[1];
    }

    while ((opt = getopt(argc, argv, "s:f:Se:")) != -1)
    {
        switch (opt)
        {
        case 's':
            sizeLimit = atoi(optarg);
            printf("Show files above [%d] kbs\n", sizeLimit);
            break;
        case 'e':
            eflag = 1;
            char *v3 = strtok(optarg, " ");
            while (v3 != NULL)
            {
                v1[v++] = v3;
                v3 = strtok(NULL, " ");
            }
            // printf("execute command [%s] after searching the files\n", inpitCommand);
            break;
        case 'S':
            showSize = 1;
            printf("Show file sizes fileName(size)-showsize=[%d]\n", showSize);
            break;
        case 'f':
            findStr = optarg;
            printf("Show filenames & directories with [%s]\n", findStr);
            break;
        default:
            printf("Usage: [directory] [-S] [-s] [limit in kb] [-f] [string to find]\n");
            return -1;
        }
    }

    printf("checking path[%s]\n\n", path);
    tree(path, 0, showSize, sizeLimit, findStr);

    if (eflag == 1)
    {
        if (strcmp(v2, " ") != 0)
        {

        char *v3 = strtok(v2, " ");
        while (v3 != NULL)
        {
            v1[v++] = v3;
            v3 = strtok(NULL, " ");
        }
        v1[v] = NULL;
        // for(int k=0;k<v;k++)
        // {
        //     printf("%s",v1[k]);
        // }
        // printf("%s", v2);
        int ok = fork();
        if (ok == 0)
        {
            int check = execvp(v1[0], v1);
            // if (check == -1)
            // {
            //     printf("no");
            // }
        }
        else if (ok > 0)
        {
            wait(NULL);
        }

        }
    }

    // if (inpitCommand)
    // {
    //     printf("executing command [%s]\n", inpitCommand);
    //     printList(head, inpitCommand);
    // }

    return 0;
}

/**
 * prints all the directories, subdirectory and files.
 */
void tree(char *basePath, const int root, const int showSize, const int sizeLimit, char *findStr)
{
    int i;
    char path[1000];
    struct dirent *directoryPath;
    struct stat filestats;
    char filePathsToExecComandOn[1000];
    char filepath[100];
    char buffer1[PATH_MAX + 1];


    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((directoryPath = readdir(dir)) != NULL)
    {
        if (strcmp(directoryPath->d_name, ".") != 0 && strcmp(directoryPath->d_name, "..") != 0)
        {
            stat(directoryPath->d_name, &filestats);

            /**
             *   here we are going to check if the entry is a file
             *   If it is a file, we check it against arguments given
             */
            if (!S_ISDIR(filestats.st_mode))
            {
                // if(eflag ==1)
                // {
                //     char buffer[PATH_MAX + 1];
                //     char *p = realpath(directoryPath->d_name, buffer);
                //     // printf("%s",p);

                //     strcat(v2, directoryPath->d_name);
                //     strcat(v2, " ");
                //     printf("%s",v2);
                // }
                if (showSize != 0)
                {
                    if (sizeLimit > 0)
                    {
                        if ((filestats.st_size / 1024) >= sizeLimit)
                        {
                            if (findStr != 0)
                            {
                                char *xCOmpare = strstr(directoryPath->d_name, findStr);
                                if (xCOmpare)
                                {
                                    if (eflag == 1)
                                    {

                                        strcat(v2, realpath(directoryPath->d_name, buffer1));
                                        strcat(v2, " ");
                                    }
                                    else
                                    {
                                        char buffer[PATH_MAX + 1];

                                        printf(" -----> %s ( %ld )\n", directoryPath->d_name, filestats.st_size);
                                        realpath(directoryPath->d_name, buffer);
                                        printf("hoooo%s", realpath(directoryPath->d_name, buffer));
                                        n = createNode(buffer);
                                        insert(&head, n);
                                        tail = n;
                                    }
                                }
                            }
                            else
                            {
                                if (eflag == 1)
                                {
                                    // char buffer[PATH_MAX + 1];

                                    strcat(v2, realpath(directoryPath->d_name,buffer1));
                                    strcat(v2, "    ");
                                    printf("%shlooooo",v2);
                                }
                                else
                                {
                                    char buffer[PATH_MAX + 1];
                                    printf(" -----> %s ( %ld )\n", directoryPath->d_name, filestats.st_size);
                                    realpath(directoryPath->d_name, buffer);
                                    n = createNode(buffer);
                                    insert(&head, n);
                                    tail = n;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (findStr != 0)
                        {
                            char *xKompare = strstr(directoryPath->d_name, findStr);
                            if (xKompare)
                            {
                                if (eflag == 1)
                                {
                                    // char buffer[PATH_MAX + 1];

                                    strcat(v2, realpath(directoryPath->d_name, buffer1));
                                    strcat(v2, " ");
                                }
                                else
                                {
                                    char buffer[PATH_MAX + 1];
                                    printf(" -----> %s ( %ld )\n", directoryPath->d_name, filestats.st_size);
                                    realpath(directoryPath->d_name, buffer);
                                    n = createNode(buffer);
                                    insert(&head, n);
                                    tail = n;
                                }
                            }
                        }
                        else
                        {
                            if (eflag == 1)
                            {
                                // char buffer[PATH_MAX + 1];

                                strcat(v2, realpath(directoryPath->d_name, buffer1));
                                strcat(v2, " ");
                            }
                            else
                            {
                                char buffer[PATH_MAX + 1];
                                printf(" -----> %s ( %ld )\n", directoryPath->d_name, filestats.st_size);
                                realpath(directoryPath->d_name, buffer);
                                n = createNode(buffer);
                                insert(&head, n);
                                tail = n;
                            }
                        }
                    }
                }
                else
                {

                    if (sizeLimit > 0)
                    {
                        if (filestats.st_size >= sizeLimit)
                        {
                            if (findStr != 0)
                            {
                                char *xKompare = strstr(directoryPath->d_name, findStr);
                                if (xKompare)
                                {
                                    if (eflag == 1)
                                    {
                                        // char buffer[PATH_MAX + 1];

                                        strcat(v2, realpath(directoryPath->d_name, buffer1));
                                        strcat(v2, " ");
                                    }
                                    else
                                    {
                                        char buffer[PATH_MAX + 1];
                                        printf(" -----> %s ( %ld )\n", directoryPath->d_name, filestats.st_size);
                                        realpath(directoryPath->d_name, buffer);
                                        // printf("filename[>%s]",buffer);
                                        n = createNode(buffer);
                                        insert(&head, n);
                                        tail = n;
                                    }
                                }
                            }
                            else
                            {
                                if (eflag == 1)
                                {
                                    // char buffer[PATH_MAX + 1];

                                    strcat(v2, realpath(directoryPath->d_name, buffer1));
                                    strcat(v2, " ");
                                }
                                else
                                {
                                    char buffer[PATH_MAX + 1];
                                    printf(" -----> %s ( %ld )\n", directoryPath->d_name, filestats.st_size);
                                    realpath(directoryPath->d_name, buffer);
                                    n = createNode(buffer);
                                    insert(&head, n);
                                    tail = n;
                                }
                            }
                        }
                    }
                    else
                    {
                        // printf("..checking for a tag");
                        if (findStr != 0)
                        {
                            char *xCompare = strstr(directoryPath->d_name, findStr);
                            if (xCompare)
                            {
                                if (eflag == 1)
                                {
                                    // char buffer[PATH_MAX + 1];

                                    strcat(v2, realpath(directoryPath->d_name, buffer1));
                                    strcat(v2, " ");
                                }
                                else
                                {
                                    char buffer[PATH_MAX + 1];
                                    printf(" -----> %s ( %ld )\n", directoryPath->d_name, filestats.st_size);
                                    realpath(directoryPath->d_name, buffer);
                                    n = createNode(buffer);
                                    insert(&head, n);
                                    tail = n;
                                }
                            }
                        }
                        else
                        {
                            if (eflag == 1)
                            {
                                // char buffer[PATH_MAX + 1];

                                strcat(v2, realpath(directoryPath->d_name, buffer1));
                                strcat(v2, " ");
                            }
                            else
                            {
                                char buffer[PATH_MAX + 1];
                                printf(" -----> %s \n", directoryPath->d_name);
                                realpath(directoryPath->d_name, buffer);
                                n = createNode(buffer);
                                insert(&head, n);
                                tail = n;
                            }
                        }
                    }
                }
            }
            /**
             *   Given that this is a directory,
             *   we check if there is a name/string to check against
             */
            else
            {
                if (findStr != 0)
                {
                    // if (eflag == 1)
                    // {
                    //     // char buffer[PATH_MAX + 1];

                    //     strcat(v2, realpath(directoryPath->d_name, buffer1));
                    //     strcat(v2, " ");
                    // }
                    // else
                    // {

                        char *x = strstr(directoryPath->d_name, findStr);
                        if (x)
                        {
                            printf(" -----> %s ( %ld )\n", directoryPath->d_name, filestats.st_size);
                        }
                    // }
                }
                else
                {
                    printf(" ##- %s\n", directoryPath->d_name);
                }
            }

            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, directoryPath->d_name);
            tree(path, root + 2, showSize, sizeLimit, findStr);
            
        }
    }

    closedir(dir);
    // if (eflag == 1)
    // {
    //     // if (strcmp(v2, " ") != 0)
    //     // {
    //     char *v3 = strtok(v2, " ");
    //     while (v3 != NULL)
    //     {
    //         v1[v++] = v3;
    //         v3 = strtok(NULL, " ");
    //     }
    //     v1[v] = NULL;
    //     int ok = fork();
    //     if (ok == 0)
    //     {
    //         int check = execvp(v1[0], v1);
    //         // if (check == -1)
    //         // {
    //         //     printf("no");
    //         // }
    //     }
    //     else if (ok > 0)
    //     {
    //         wait(NULL);
    //     }
    //     // }
    // }
}
