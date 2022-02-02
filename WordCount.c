#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct keyword // structure keyword with two entries word and count
{
   char* word;
   int count;

};
int get_key_count(char *s,char *w) //To compare the keyword and count the occurences of the keyword count in the given input stream 
{
    int found=0;
    char *wordbreak=" ";
    wordbreak=strtok(s," ");
    while(wordbreak!=NULL){
        if (strcmp(wordbreak,w)==0){
            found++;
        }
        wordbreak=strtok(NULL," ,.\n");
    }
    /*for(i=0;s[i];i++)
    {
    	if(s[i]==' ')
    	{
    		a[k++]=i;
		}
	}
	a[k++]=i;
	j=0;
	for(i=0;i<k;i++)
	{
		n=a[i]-j;
		if(n==strlen(w))
		{
			t=0;
			for(l=0;w[l];l++)
			{
				if(s[l+j]==w[l])
				{
					t++;
				}
			}
			if(t==strlen(w))
		    {
				found++;
				 
		    }
		}
	
		j=a[i]+1;
	}*/
	 
    return found;
}
void search_update(int argc, struct keyword *keys, char* str) //To search keyword in the given input and updating struct array.
{
   int i;
   int len = strlen(str);
    char *string = malloc((len + 1)* sizeof(char));
   for (i = 0; i < argc - 1; i++)
   {
       strcpy(string,str);
       keys[i].count = keys[i].count + get_key_count(str, keys[i].word);
        strcpy(str,string);
        
   }

}

void displaykeywordcount_table(int argc, struct keyword *keys) //Display the keyword/count table 
{
   int i;
   for (i = 0; i < argc-1; i++) 
   {
       printf("%s\t", keys[i].word);
       printf("%d\n", keys[i].count);

   }
}

int main(int argc, char* argv[])
{
int i;
    size_t size =100;
    char* line;
    int char_num=0;
    int count_num = argc; 
   struct keyword keys[count_num];
   for (i = 1; i < argc; i++) 
   {
       keys[i - 1].word = argv[i];
       keys[i - 1].count = 0;
   }
   line = (char*)malloc(size);

   while((char_num=getline(&line, &size, stdin))>0)//To read the text input using getline() method
   search_update(argc, keys, line);//function to perform the search and update the table
   displaykeywordcount_table(argc, keys);// function to print the table    
  
}
//To compile gcc -o WordCount WordCount.c
//To execute ./WordCount