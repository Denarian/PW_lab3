#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)


int zmienna_wspolna=0;

#define WYMIAR 1000
#define ROZMIAR WYMIAR*WYMIAR

#define ILOSC 10

double a[ROZMIAR],b[ROZMIAR],c[ROZMIAR];


double czasozajmowacz(){
  int i, j, k;
  int n=WYMIAR;
  for(i=0;i<ROZMIAR;i++) a[i]=1.0*i;
  for(i=0;i<ROZMIAR;i++) b[i]=1.0*(ROZMIAR-i);
  for(i=0;i<n;i++){
    for(j=0;j<n;j++){
      c[i+n*j]=0.0;
      for(k=0;k<n;k++){
	c[i+n*j] += a[i+n*k]*b[k+n*j];
      }
    }
  }
  return(c[ROZMIAR-1]);
}

static void  display_pthread_attr(pthread_attr_t *attr, char *prefix)
{
    int s, i;
    size_t v;
    void *stkaddr;
    struct sched_param sp;

    s = pthread_attr_getdetachstate(attr, &i);
    if (s != 0)
      handle_error_en(s, "pthread_attr_getdetachstate");
    printf("%sDetach state        = %s\n", prefix,
        (i == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" :
        (i == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" : "???");

    s = pthread_attr_getinheritsched(attr, &i);
    if (s != 0)
    handle_error_en(s, "pthread_attr_getinheritsched");
    printf("%sInherit scheduler   = %s\n", prefix,
      (i == PTHREAD_INHERIT_SCHED)  ? "PTHREAD_INHERIT_SCHED" :
      (i == PTHREAD_EXPLICIT_SCHED) ? "PTHREAD_EXPLICIT_SCHED" : "???");

      s = pthread_attr_getschedpolicy(attr, &i);
    if (s != 0)
    handle_error_en(s, "pthread_attr_getschedpolicy");
    printf("%sScheduling policy   = %s\n", prefix,
     (i == SCHED_OTHER) ? "SCHED_OTHER" :
     (i == SCHED_FIFO)  ? "SCHED_FIFO" :
     (i == SCHED_RR)    ? "SCHED_RR" : "???");

    s = pthread_attr_getschedparam(attr, &sp);
    if (s != 0)
      handle_error_en(s, "pthread_attr_getschedparam");
    printf("%sScheduling priority = %d\n", prefix, sp.sched_priority);

    s = pthread_attr_getguardsize(attr, &v);
    if (s != 0)
      handle_error_en(s, "pthread_attr_getguardsize");
    printf("%sGuard size          = %zu bytes\n", prefix, v);

    s = pthread_attr_getstack(attr, &stkaddr, &v);
    if (s != 0)
      handle_error_en(s, "pthread_attr_getstack");
    printf("%sStack address       = %p\n", prefix, stkaddr);
    printf("%sStack size          = 0x%zx bytes\n", prefix, v);

}


void * watek (void * arg)
{  
    size_t size;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_getattr_np(pthread_self(), &attr);
    char str[20];
    sprintf(str, "%d\t", arg);
    display_pthread_attr(&attr,str);
	  czasozajmowacz();
    printf("%d - zakonczonono\n",arg);
	return(NULL);
}
int main()
{
pthread_t tid[ILOSC];
int wyn[] = {0,0,0};
size_t stack_size = 16384;
pthread_attr_t attr;
int random = 0;
for( int i = 0; i < ILOSC; i++)
{   
  random =  (int)(rand() / (RAND_MAX + 1.0) * 10.0) ;
  wyn[0] = pthread_attr_init(&attr);
  switch(random)
  {
    case 0:
      wyn[1] = pthread_attr_setstacksize(&attr, stack_size*getpagesize()*4 );// ustawianie stosu
      break;
    case 1:
      wyn[1] = pthread_attr_setstacksize(&attr, stack_size*getpagesize()*2 );// ustawianie stosu
      break;
    case 2:
      wyn[1] = pthread_attr_setstacksize(&attr, stack_size*getpagesize()*10 );// ustawianie stosu
      break;
    case 3:
      wyn[1] = pthread_attr_setstacksize(&attr, stack_size*getpagesize()*6 );// ustawianie stosu
      break;
    case 4:
      wyn[1] = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
      break;
    case 5:
      wyn[1] = pthread_attr_setguardsize(&attr, 0);
      break;
    case 6:
      wyn[1] = pthread_attr_setguardsize(&attr, getpagesize()*6);
      break;
    case 7:
      wyn[1] = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
      wyn[1] |= pthread_attr_setguardsize(&attr, getpagesize()*3);
      wyn[1] |= pthread_attr_setstacksize(&attr, stack_size*getpagesize()*10 );
      wyn[1] |=  pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
      break;
    case 8:
      wyn[1]  = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
      break;
    case 9:
      wyn[1]  = pthread_attr_setschedpolicy(&attr,SCHED_RR );
      break;
    default:
      break;
	
	
  }
  wyn[2] = pthread_create(&tid[i], &attr, watek, i);
    
  
  pthread_attr_destroy(&attr);
    
  if((wyn[0] | wyn[1] | wyn[2]) != 0)
  {
        printf("ERRROR - %d - %d - %d | %d | %d\n", i, random, wyn[0] , wyn[1], wyn[2]);
      
  }
 // random++;

  pthread_join(tid[i],NULL);
}
/*
for( int i = 0; i < ILOSC; i++)
{
	pthread_join(tid[i],NULL);
}
*/
return 0;
}