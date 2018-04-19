#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#ifdef DEBUG

void *fun(void *arg)
{
	printf("======[%d]====\n", (int)arg);
	pthread_exit((void*)22);
}


int main(void)
{
	void *val;
	pthread_t tid;
	pthread_create(&tid, NULL, fun, (void *)99);
	pthread_join(tid, &val);
	printf("------[%d]-----\n", (int)val);

	return 0;
}

#else

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct
{
	int a;
	float b;			      
} *LYp, LY;

LYp sheep;
float f = 0;


void *fun(void *arg)
{
	LYp p = (LYp)arg;
	printf("===[%d]===[%f]====\n", p->a, p->b);
	f = 1.234;
	pthread_exit(&f);
}


int main(void)
{
	sheep = malloc(sizeof(LY));
	sheep->a = 100;
	sheep->b = 3.33;
	pthread_t tid;

	pthread_create(&tid, NULL, fun, sheep);
	void *val;
	pthread_join(tid, &val);
	printf("------[%lf]-----\n", *((float*)val));
	free(sheep);

	return 0;
}

#endif

