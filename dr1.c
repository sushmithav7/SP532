#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

void tree_hirarchy(char *dr, const int root, const int showvalue,const int showcheck, const int showSize, const int sizeLimit, const char *findStr);

int main(int argc, char *argv[]) // to pass the command line arguements (directory name,inputs options(-s,-f,-S)
{
    // Directory path to list files
    char drpath[100];
    int showSize = 0,showcheck=0,showvalue=0;
    int sizeLimit = 0;
    char *findStr;

    int opt;
    if (argv[1] != NULL)
    {
        tree_hirarchy(drpath, 0, showvalue,showcheck,showSize, sizeLimit, findStr);
    }

    while ((opt = getopt(argc, argv, "s:f:S")) != -1) //To display according to the input option given accrdingly
    {
        switch (opt)
        {
        case 's':                       
            showcheck =1;
            sizeLimit = atoi(optarg);
            printf("Show files above [%d] kbs\n", sizeLimit);

            break;
        case 'S':
            showSize = 1;
            printf("Show file sizes fileName(size)\n");
            break;
        case 'f':
            showvalue=1;
            findStr = optarg;
            printf("option (f) find[%s]\n", findStr);
            break;
        default:
            printf("Usage: [directory] [-S] [-s] [limit in kb] [-f] [string to find]\n");
            return -1;
        }
    }

    tree_hirarchy(".", 0, showvalue,showcheck,showSize, sizeLimit, findStr);

    return 0;
}

/**
 * prints all the directories, subdirectory and files.

*/
int FilecheckingSize(char *checksize)
{
    struct stat value;
    if (stat(checksize, &value) == -1)
    {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    return value.st_size;
}
void tree_hirarchy(char *dr, const int root, const int showvalue,const int showcheck,const int showSize, const int sizeLimit, const char *findStr)
{
    int i;
    char drpath[1000];
    struct dirent *dp;
    struct stat filestats;

    DIR *dir = opendir(dr);

    if (!dir)
        return;

   
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            // for (i = 0; i < root; i++)
            // {
            //     if (i % 2 == 0 || i == 0)
            //         printf("|");
            //     else
            //         printf("--");
            // }
          
            strcpy(drpath, dr);
            strcat(drpath, "/");
            strcat(drpath, dp->d_name);

            if (showSize != 0)
            {
                printf(" ->%s ( %lld )\n", dp->d_name, filestats.st_size);
            }
            else if(showcheck !=0)
            {
                if(FilecheckingSize(drpath) >= sizeLimit)
                {
                    printf(" ->%s\n", dp->d_name);
                }
            }
            else if(showvalue !=0)
            {
                if(strstr(dp->d_name,findStr))
                {
                    printf(" ->%s\n", dp->d_name);
                }
            }
            else
            {
                printf(" ->%s\n", dp->d_name);
            }

            if (S_ISDIR(filestats.st_mode))
            {

                tree_hirarchy(drpath, root + 2,showvalue,showcheck, showSize, sizeLimit, findStr);
            }
        }
    }

    closedir(dir);
}