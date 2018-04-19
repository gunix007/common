#include <stdio.h>
#include <stdlib.h>

typedef struct data
{
	long *a;
	long *b;
} data_t;

void *thread_handle(void *args)
{
	data_t *rev = (data_t *)args;
	data_t *ret = (data_t *)malloc(sizeof(data_t));
	ret->a = (long *)malloc(sizeof(long));
	ret->b = (long *)malloc(sizeof(long));
	*(ret->a) = 2 * (*(rev->a));
	*(ret->b) = 2 * (*(rev->b));
	return (void *)ret;
}


int main()
{
	pthread_t pid;
	void *ret;

	data_t *tmp = (data_t *)malloc(sizeof(data_t));
	tmp->a = (long *)malloc(sizeof(long));
	tmp->b = (long *)malloc(sizeof(long));
	*(tmp->a) = 5;
	*(tmp->b) = 6;
	pthread_create(&pid, NULL, thread_handle, (void *)tmp);
	pthread_join(pid, &ret);
	printf("a=%ld\n", *(((data_t *)ret)->a));
	printf("b=%ld\n", *(((data_t *)ret)->b));
}

