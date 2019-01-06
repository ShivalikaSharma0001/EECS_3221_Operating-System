/*

# Family Name: Sharma

# Given Name: Shivalika

# Section: E

# Student Number: 213963608

# CSE Login: ssharma6

*/

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef char string[101];
string str;

struct WordArray
{
        char  word[101];
        int   freq ;
};

struct AnswerArray
{
        char  filename[101];
        int   wordnum;
        char  median_word[101];
};

//void *myThread(void *vargp);
void quickSort(struct WordArray *wa, int low, int high);
int partition (struct WordArray *wa, int low, int high);
void swapInt(int* a, int* b);
void swapString(char a[101], char b[101]);
void printArray(struct WordArray *wa, int size);
void reverse(struct WordArray *wa, int size);
void sortASCII(struct WordArray *wa, int size);
void printAnswerArray(struct AnswerArray *aa, int size);
void quickSortAnswerArray(struct AnswerArray *aa, int low, int high);
int partitionAnswerArray (struct AnswerArray *aa, int low, int high);
void reverseAnswerArray(struct AnswerArray *aa, int size);
int maxUniqWords = 10000;
void *myThread(void *vargp);

int main ( int argc, char *argv[] )
{
	int i;
        struct AnswerArray aa[argc - 1];
        for (i = 0; i < (argc-1); i ++)
        {
                aa[i].filename[0] = '\0';
                aa[i].wordnum = 0;
                aa[i].median_word[0] = '\0';
        }

        if ( argc < 2 ) /* argc should be 2. One for ./a.out and second for filename.txt */
        {
                printf( "%s is provided. But there is no filename to read from!\n", argv[0] );
        }
        else
        {
                pthread_t tid;
                int x;
                for (x = 1; x < argc; x ++)
                {
			char * s;
                        pthread_create(&tid, NULL, myThread, argv[x]);
			pthread_join(tid, (void**)&s);

			string result = "";
			strcpy(result, argv[x]);
			strcat(result,s);
			//printf("%s\n", result);
			int resultLength = strlen(result);

			char *r = strstr(result, ".txt");
                                int indexOfPeriod = r - result;
                                int lengthPeriodOnwards = resultLength - indexOfPeriod;
                                int digitStartIndex = indexOfPeriod + 4;

			int i;
                        int digitCount = 0;
                        string wn = "";
                        for (i = digitStartIndex; i < resultLength; i ++)
                        {
                        	if (isdigit(result[i]))
                                {
                                        digitCount = digitCount + 1;
                                }
                        }
                        strncpy(wn, &result[digitStartIndex], digitCount);

			 int medianWordStartIndex = digitStartIndex + digitCount;
                                int medianWordLength = 0;
                                string mw = "";
                                for (i = medianWordStartIndex; i < resultLength; i ++)
                                {
                                        medianWordLength = medianWordLength + 1;
                                }
                                strncpy(mw, &result[medianWordStartIndex], medianWordLength);

                                int wn_int = atoi(wn);
                                strcpy(aa[x-1].filename, argv[x]);
                                aa[x-1].wordnum = wn_int;
                                strcpy(aa[x-1].median_word, mw);
                }
                pthread_exit(NULL);
        }
	 int fileNum = (argc - 1);
        //printf("\nPrint simple array\n");
        //printAnswerArray(aa, fileNum);

        quickSortAnswerArray(aa, 0, (fileNum-1));
        //printf("\nPrint sorted array\n");
        //printAnswerArray(aa, fileNum);

        reverseAnswerArray(aa, (fileNum - 1));
        //printf("\nPrint reversed array\n");
        printAnswerArray(aa, fileNum);
}

// The function to be executed by all threads
void *myThread(void *vargp)
{
     FILE *file = fopen( vargp, "r" );
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

        reverse(wa, arraySize);

        sortASCII(wa, arraySize);

        int median;
        if ((arraySize%2) == 0)
        {
                median = (arraySize/2);
        }
        else
        {
                median = (arraySize - 1)/2 + 1;
        }

        //printf("%s %d %s\n", vargp, arraySize, wa[median].word);
	sprintf(str, "%d", arraySize);
	strcat(str, wa[median].word);
	return &str;
     }
     fclose(file);
}

void printAnswerArray(struct AnswerArray *aa, int size)
{
        int i;
        for (i = 0; i < size; i ++)
        {
                printf("%s %d %s\n", aa[i].filename, aa[i].wordnum, aa[i].median_word);
        }
}

void quickSortAnswerArray(struct AnswerArray *aa, int low, int high)
{
     if (low < high)
     {
        int pi = partitionAnswerArray(aa, low, high);
        quickSortAnswerArray(aa, low, pi - 1);
        quickSortAnswerArray(aa, pi + 1, high);
     }
}

int partitionAnswerArray (struct AnswerArray *aa, int low, int high)
{
    int pivot = aa[high].wordnum;
    int i = (low - 1);
    int j;

    for (j = low; j <= high - 1; j++)
    {
        if (aa[j].wordnum <= pivot)
        {
            i++;
            swapInt(&aa[i].wordnum, &aa[j].wordnum);
            swapString(aa[i].filename, aa[j].filename);
            swapString(aa[i].median_word, aa[j].median_word);
        }
    }
    swapInt(&aa[i + 1].wordnum, &aa[high].wordnum);
    swapString(aa[i + 1].filename, aa[high].filename);
    swapString(aa[i + 1].median_word, aa[high].median_word);
    return (i + 1);
}

void reverseAnswerArray(struct AnswerArray *aa, int size)
{
    int i;
    for(i = 0; i < size; i ++)
    {
        swapInt(&aa[i].wordnum, &aa[size].wordnum);
        swapString(aa[i].filename, aa[size].filename);
        swapString(aa[i].median_word, aa[size].median_word);
        size --;
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
            swapInt(&wa[i].freq, &wa[j].freq);
            swapString(wa[i].word, wa[j].word);
        }
    }
    swapInt(&wa[i + 1].freq, &wa[high].freq);
    swapString(wa[i + 1].word, wa[high].word);
    return (i + 1);
}

void swapInt(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void swapString(char a[101], char b[101])
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
        swapInt(&wa[i].freq, &wa[size].freq);
        swapString(wa[i].word, wa[size].word);
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
                                                                          swapString(wa[k].word, wa[k-1].word);
                                                                          swapInt(&wa[k].freq, &wa[k-1].freq);
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
                                                                  swapString(wa[k].word, wa[k-1].word);
                                                                  swapInt(&wa[k].freq, &wa[k-1].freq);
                                                  }
                                  }
                  }
                  count = 0;
                }
    }
}
