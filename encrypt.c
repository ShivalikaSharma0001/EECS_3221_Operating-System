/*

# Family Name: Sharma

# Given Name: Shivalika

# Section: E

# Student Number: 213963608

# CSE Login: ssharma6

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

typedef struct
{
    char data;
    off_t offset;
    char state;
} BufferItem;

// global variables
BufferItem *bi; // creating an instance of BUfferItem global
void *key;
FILE *file_in;
FILE *file_out;
int bufSize = 0;
int IN_tip; //IN thread in progress
int WORK_tip; //WORK thread in progress

// create mutex threads for IN, WORK, OUT
pthread_mutex_t mIN;
pthread_mutex_t mWORK;
pthread_mutex_t mOUT;

// functions
void t_sleep(void);
int is_empty();
int findIndex();
int findIndexOfChar(char c);
void *IN_THREAD(void *p);
void *WORK_THREAD(void *p);
void *OUT_THREAD(void *p);

int main(int argc, char *argv[])
{
  // local variables
  int nIN;
	int nOUT;
	int nWORK;

  // the number of arguements supplied should be 7 excluding encrypt, KEY... file_out bufSize
  if (argc < 8 || argc > 8)
  {
    // error if number of arguements are not 7, print error message and exit
    printf("Please enter these 7 inputs - <KEY> <nIN> <nWORK> <nOUT> <file_in> <file_out> <bufSize>\n");
    exit(-1);
  }
  // when number of argurements are 7
  else
  {
    key = argv[1];
    // key_temp to distinguish value from key
    // converting the arguement into an int
    int key_temp = atoi(key);
    if (key_temp < -127 || key_temp > 127)
    {
      printf("Key value should be in between -127 to +127. Please input the correct value.\n");
      exit(-1);
    }

    // converting the arguement into an int
    nIN = atoi(argv[2]);
    if (nIN < 1)
    {
      printf("The number of IN threads to create should be atleast 1. Please input the correct value.\n");
      exit(-1);
    }

    nWORK = atoi(argv[3]);
    if (nWORK < 1)
    {
      printf("The number of WORK threads to create should be atleast 1. Please input the correct value.\n");
      exit(-1);
    }

    nOUT = atoi(argv[4]);
    if (nOUT < 1)
    {
      printf("The number of OUT threads to create should be atleast 1. Please input the correct value.\n");
      exit(-1);
    }

    // open the file to read from
    file_in = fopen(argv[5], "r");
    // error if the file does not exist
    if(file_in == NULL)
    {
      printf("The file to be converted does not exist. PLease make sure the file exists.\n");
      exit(-1);
    }

    // open the file to write the output result to
	  file_out = fopen(argv[6], "w");
    // error if file does not exist
    if (file_out == NULL)
    {
      printf("The target file does not exist. PLease make sure the file exists.\n");
      exit(-1);
	  }

    // converting the arguement into an int
    bufSize = atoi(argv[7]);
    if (bufSize < 1)
    {
      printf("The capacity of the shared buffer should be atleast 1. Please input the correct value.\n");
      exit(-1);
    }
  }

  // initializing the global variables IN_tip, WORK_tip
  IN_tip = nIN;
  WORK_tip = nWORK;

  // creating an instance of BUfferItem
  bi = (BufferItem*)malloc(sizeof(BufferItem)*bufSize);

  // initializing the BUfferItem bi's state to e, empty
  int i;
  for (i = 0; i < bufSize; i ++)
  {
    bi[i].state = 'e';
  }

  // threads
	pthread_t INthreads[nIN];
	pthread_t OUTthreads[nOUT];
	pthread_t WORKthreads[nWORK];

  // initialize mutexe threads
	pthread_mutex_init(&mIN, NULL);
	pthread_mutex_init(&mWORK, NULL);
	pthread_mutex_init(&mOUT, NULL);

  pthread_attr_t attr;
  // initialize the attr
	pthread_attr_init(&attr);

  // create the number of threads as asked by the user
	for (i = 0; i < nIN; i++)
  {
		pthread_create(&INthreads[i], &attr, (void *) IN_THREAD, file_in);
	}
	for (i = 0; i < nWORK; i++){
		pthread_create(&WORKthreads[i], &attr, (void *) WORK_THREAD, key);
	}
	for (i = 0; i < nOUT; i++){
		pthread_create(&OUTthreads[i], &attr, (void *) OUT_THREAD, file_out);
	}

  // join all the threads
	for (i = 0; i < nIN; i++)
  {
		pthread_join(INthreads[i], NULL);
	}
	for (i = 0; i < nWORK; i++)
  {
		pthread_join(WORKthreads[i], NULL);
	}
	for (i = 0; i < nOUT; i++)
  {
		pthread_join(OUTthreads[i], NULL);
	}

  // destory mutex threads
	pthread_mutex_destroy(&mIN);
	pthread_mutex_destroy(&mOUT);
	pthread_mutex_destroy(&mWORK);

  // close the files
	fclose(file_in);
	fclose(file_out);

  //  free the buffer
	free(bi);

  return 0;
}

void t_sleep(void)
{
	struct timespec ts;
  int seedp = 0;
  ts.tv_sec = 0;

  ts.tv_nsec = rand_r((unsigned int*)&seedp)%(10000000+1);

	nanosleep(&ts, NULL);
}

// finds if buffer is empty or not
int is_empty()
{
    int i;
    // returns 1 if the buffer is empty and 0 if it not empty
    for (i = 0; i < bufSize; i ++)
    {
      if (bi[i].state == 'e')
      {
              return 1;
      }
    }
    return 0;
}

// find the index of first empty element in buffer, i.e. the index of 'e'
int findIndex()
{
    if (is_empty() == 1)
    {
          int i;
          for (i = 0; i < bufSize; i ++)
          {
            if (bi[i].state == 'e')
            {
              // rturn the index of e
                    return i;
            }
          }
    }
    // if 'e' is not found, return -1
    return -1;
}

// similar to findIndex, this function finds the index of given character c
int findIndexOfChar(char c)
{
  int i;
  for (i = 0; i < bufSize; i ++)
  {
    if (bi[i].state == c)
    {
            return i;
    }
  }
  // if char c is not found, return -1
  return -1;
}

void *IN_THREAD(void *p)
{
        // thread sleeps
        t_sleep();

        int index = 0;
        char ch;
        off_t offset;

        do
        {
                // critical section starts for mWORK
                pthread_mutex_lock(&mWORK);

                // find the index of 'e'
                index = findIndex();

                while (index > -1)
                {
                        // if buffer is empty, then sleep
                        if (is_empty() == 1)
                        {
                                t_sleep();
                        }

                        // critical section starts for mIN
                        pthread_mutex_lock(&mIN);

                        // ftell gives file_in position
                        offset = ftell(file_in);

                        // gets the char from file_in
                        ch = fgetc(file_in);

                        // critical section ends for mIN
                        pthread_mutex_unlock(&mIN);

                        // if char is EOF, break
                        if (ch == EOF)
                        {
                            break;
                        }

                        else
                        {
                            bi[index].offset = offset;
                            bi[index].data = ch;

                            // set the state to w, working
                            bi[index].state = 'w';
                            index = findIndex();
                        }
                }
                // critical section ends for mWORK
                pthread_mutex_unlock(&mWORK);
        } while (!feof(file_in)); // keep running until EOF is reached

        // thread sleeps
        t_sleep();

        // another critical section for mWORK to update IN_tip value
        pthread_mutex_lock(&mWORK);
        IN_tip--;
        pthread_mutex_unlock(&mWORK);

        return NULL;
}

void *WORK_THREAD(void *p)
{
        // thread sleeps
        t_sleep();

        int index = 0;
        char ch;

        // convert key to an int value
        int key = atoi(p);

        do
        {
                // critical section starts for mWORK
                pthread_mutex_lock(&mWORK);

                // find the index of char 'w'
                index = findIndexOfChar('w');

                if (index > -1)
                {
                        ch = bi[index].data;

                        if (is_empty() == 1)
                        {
                                // if buffer is empty, then sleep
                                t_sleep();
                        }

                        // break if EOF or '\0' is reached
                        if (ch == EOF || ch == '\0')
                        {
                                break;
                        }

                        if (ch > 31 && ch < 127)
                        {
                                // for encryption
                                if (key >= 0)
                                {
                                  ch = (((int)ch-32)+2*95+key)%95+32;
                                }

                                // for decryption
                                else if (key < 0)
                                {
                                  ch = (((int)ch-32)+2*95-(-1*key))%95+32;
                                }
                        }

                        bi[index].data = ch;

                        // set the state to 'o', means ready to output to the target file
                        bi[index].state = 'o';
                }

                // critical section ends for mWORK
                pthread_mutex_unlock(&mWORK);

        } while (index > -1 || IN_tip > 0);

        // thread sleeps
        t_sleep();

        // critical section starts for mWORK to update value for WORK_tip
        pthread_mutex_lock(&mWORK);
        WORK_tip--;
        pthread_mutex_unlock(&mWORK);

        return NULL;
}

void *OUT_THREAD(void *p)
{
        // thread sleeps
        t_sleep();

        int index = 0;
        char ch;
        off_t offset;

        do
         {
                // critical section starts for mWORK
                pthread_mutex_lock(&mWORK);

                // find index of 'o'
                index = findIndexOfChar('o');

                if (index > -1)
                {
                        offset = bi[index].offset;
                        ch = bi[index].data;

                        // if buffer empty, sleep
                        if (is_empty() == 1)
                        {
                                t_sleep();
                        }

                        // critical section starts for mOUT
                        pthread_mutex_lock(&mOUT);

                        // gives error if unable to set position of offset in file_out
                        if (fseek(file_out, offset, SEEK_SET) == -1)
                        {
                          printf("Unable to find file position.\n");
                          exit(-1);
                        }

                        // EOF returned if any problem while writing ch to file_out
                        // gives the error message
                        if (fputc(ch, file_out) == EOF)
                         {
                           printf("Unable to write to the target file.\n");
                           exit(-1);
                        }
                        // critical section ends for mOUT
                        pthread_mutex_unlock(&mOUT);

                        bi[index].data = '\0';
                        // state set back to 'e', as it is empty again
                        bi[index].state = 'e';
                        bi[index].offset = 0;
                }

                // critical section ends for mWORK
                pthread_mutex_unlock(&mWORK);

        } while (index > -1 || WORK_tip > 0);

        // thread sleeps
        t_sleep();

        return NULL;
}
