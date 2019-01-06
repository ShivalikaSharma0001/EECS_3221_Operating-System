/*

# Family Name: Sharma

# Given Name: Shivalika

# Section: E

# Student Number: 213963608

# CSE Login: ssharma6

*/

/*
 * Purpose of this program-
 * correctly count frequencies of all distinct words
 * and print out the median words for all text documents provided in the command line
 *
 * The output result should look like-
 * Filename1 wordnum1 median_word1
 * where Filename1 is the name of the input file,
 * wordnum1 is the number of distinct words in the Filename1
 * and median_word1 is the median word
 */

#include <stdio.h>
#include <string.h>
typedef char string[101];

struct WordArray
{
        char  word[101];
        int   freq ;
};

void quickSort(struct WordArray *wa, int low, int high);
int partition (struct WordArray *wa, int low, int high);
void swapFreq(int* a, int* b);
void swapWord(char a[101], char b[101]);
void printArray(struct WordArray *wa, int size);
void reverse(struct WordArray *wa, int size);
void sortASCII(struct WordArray *wa, int size);
int maxUniqWords = 10000;

int main ( int argc, char *argv[] )
{
    if ( argc < 2 ) /* argc should be 2. One for ./a.out and second for filename.txt */
    {
        printf( "%s is provided. But there is no filename to read from!\n", argv[0] );
    }
    else
    {
	int x;
	for (x = 1; x < argc; x ++)
	{
        FILE *file = fopen( argv[x], "r" );
        if ( file == 0 )
        {
            printf( "Could not open file\n" );
        }
        else
        {    
            struct WordArray wa[maxUniqWords];
	    int i;
	    int j = 1;
	    string temp;
            int distWordsCount = 0;
            int newWordFound;
	    int initialJValue;

	    for (i = 0; i < maxUniqWords; i ++)
            {
	    	wa[i].freq = 0;
		wa[i].word[0] = '\0';
	    }	 
 
	    while (fscanf(file, "%s", temp) == 1) 
	    {
	    	newWordFound = 0; // 0 means not found, 1 means found
		for (i = 1; (i <= j) && (newWordFound == 0); i++)
		{
			if ((strcmp(wa[i].word, temp)) == 0)
			{
				wa[i].freq ++;
				newWordFound = 1;
			}
		}

		if (newWordFound == 0)
		{
			strcpy(wa[j].word, temp);
			wa[j].freq ++;
			j++;
		}
	    }

	    int arraySize = (j-1);
	    quickSort(wa, 0, (arraySize));
	    //printArray(wa, arraySize);
	    reverse(wa, arraySize);
	    //printf("========\n");
	    //printArray(wa, arraySize);
	    //printf("===========\n");
	    sortASCII(wa, arraySize);
	    //printArray(wa, arraySize);

	    int median;
	    if ((arraySize%2) == 0)
	    {
		median = (arraySize/2);
	    }
	    else
	    {
		median = (arraySize - 1)/2 + 1;
	    }

	    printf("%s %d %s\n", argv[x], arraySize, wa[median].word);

	}
        fclose(file);
    }
  }
}

void quickSort(struct WordArray *wa, int low, int high)
{
     if (low < high) 
     { 
        int pi = partition(wa, low, high); 
        quickSort(wa, low, pi - 1); 
        quickSort(wa, pi + 1, high); 
     }
}

int partition (struct WordArray *wa, int low, int high)
{
    int pivot = wa[high].freq;    
    int i = (low - 1);  
    int j;

    for (j = low; j <= high- 1; j++)
    {
        if (wa[j].freq <= pivot)
        {
            i++;    
            swapFreq(&wa[i].freq, &wa[j].freq);
	    swapWord(wa[i].word, wa[j].word);
        }
    }
    swapFreq(&wa[i + 1].freq, &wa[high].freq);
    swapWord(wa[i + 1].word, wa[high].word);
    return (i + 1);
}

void swapFreq(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void swapWord(char a[101], char b[101])
{
    char t[101];
    int i; 
    for (i = 0; i < 101; i ++)
    {
	t[i] = a[i];
	a[i] = b[i];
	b[i] = t[i];
    }
}

void printArray(struct WordArray *wa, int size)
{
    int i;
    for (i=1; i <= size; i++)
    {
        printf("%d| %s:%d\n", i, wa[i].word, wa[i].freq);
    }
}

void reverse(struct WordArray *wa, int size)
{
    int i;
    for(i = 1; i <= size; i ++)
    {
        swapFreq(&wa[i].freq, &wa[size].freq);
	swapWord(wa[i].word, wa[size].word);
        size --;
    }
}

void sortASCII(struct WordArray *wa, int size)
{
    int i;
    int count = 0;
    for (i = 1; i <= size; i ++)
    {
	if (i == size) // last index
	{
          int j, k;
	//at the end of each iteration, the smallest ascii word will be at j;
          for (j = (i - count); j < i; j ++)
          {
           	
		for (k = i; k > j; k --)
                  {
                          if (strcmp(wa[k].word, wa[k-1].word)<0) // wa[k] < wa[k-1]
                          {
                                  swapWord(wa[k].word, wa[k-1].word);
                                  swapFreq(&wa[k].freq, &wa[k-1].freq);
                          }
                  }
          }
	}
	else if (wa[i].freq == wa[i+1].freq)
        {
                count ++;
        }
 	else   
        {
	
          int j, k;
	//at the end of each iteration, the smallest ascii word will be at j;
          for (j = (i - count); j < i; j ++)
          {
           	
		for (k = i; k > j; k --)
                  {
                          if (strcmp(wa[k].word, wa[k-1].word)<0) // wa[k] < wa[k-1]
                          {
                                  swapWord(wa[k].word, wa[k-1].word);
                                  swapFreq(&wa[k].freq, &wa[k-1].freq);
                          }
                  }
          }
	  count = 0;
        }
    }
}
