#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/time.h>
#include <pthread.h>

int x = 0;

void capture()
{
	exit(0);
}

int get()
{
	return x;
}

void put()
{
	x++;
}

void node(int* p)
{
	int name;
	int seq1,seq2,i = 0;
	long t;
	struct timeval tv;
	struct timezone tz;
	name = p;
	while(1)
	{
		seq1 = get();
		seq2 = get();
		if(seq1 == seq2)
		{
			put();
			seq1 = get();
			gettimeofday(&tv,&tz);
			printf("station %d transmitting frame %d at %ld \n", name, ++i, tv, tz);
			usleep(6);
			seq2 = get();
			if(seq1 != seq2)
			{
				printf("station %d collision occurred %d \n", name, i--);
				usleep(51.2);
			}
			else
			{
				printf("station %d transmission of frame %d success \n", name, i);
			}
		}
		usleep(200000);
	}
}

void main()
{
	int n;
	printf("number of stations: ");
	scanf("%d", &n);
	signal(SIGINT,capture);
	pthread_t* threads = (pthread_t*) malloc (n * sizeof(pthread_t));
	for (int i = 0; i < n; ++i)
	{
		pthread_create(&threads[i], 0, (void*)node, i + 1);
	} 
	while(1);
}
