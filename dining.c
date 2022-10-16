// Lab 6 - Dining Philosophers Problem by Group Donuts using C
#include <pthread.h>
#include <semaphore.h> //include semaphore operations
#include <stdio.h>
//create constants representing numbers and expressions
#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (philnum + 4) % N
#define RIGHT (philnum + 1) % N
int state[N]; //declare state array for hungry, thinking, eating
int phil[N] = { 0, 1, 2, 3, 4 }; //philosophers
//Mutex (mutual exclusion) is used such that no two philosophers may access the
pickup or put down forks at the same time.
sem_t mutex;
//A semaphore array for the philosophers
sem_t sphil[N];
// test method to determine whether the philosopher eating or not and picking up
forks or not
void test(int philnum)
{
if (state[philnum] == HUNGRY
&& state[LEFT] != EATING
&& state[RIGHT] != EATING) {
// state that eating
state[philnum] = EATING;
// wait for a current thread for a specified time
sleep(1);
printf("Philosopher %d takes fork %d and %d\n",
philnum + 1, LEFT + 1, philnum + 1);
printf("Philosopher %d is Eating\n", philnum + 1);
// used to wake up hungry philosophers during putfork, has no effect
during takefork
sem_post(&sphil[philnum]);
}
}
// method for hungry state to check if they can pick up forks and eat or not
void take_fork(int philnum)
{
// semaphore lock
sem_wait(&mutex);
// state that hungry
state[philnum] = HUNGRY;
printf("Philosopher %d is Hungry\n", philnum + 1);
// eat if neighboring philosophers are not eating
test(philnum);
// unlock the semaphore
sem_post(&mutex);
// if unable to eat wait for signal
sem_wait(&sphil[philnum]);
sleep(1);
}
// philosophers to put down forks and back to thinking
void put_fork(int philnum)
{
sem_wait(&mutex);
// state that the philosopher is now thinking
state[philnum] = THINKING;
printf("Philosopher %d putting fork %d and %d down\n",
philnum + 1, LEFT + 1, philnum + 1);
printf("Philosopher %d is thinking\n", philnum + 1);
test(LEFT);
test(RIGHT);
sem_post(&mutex);
}
// philosopher method
void* philosopher(void* num)
{
// determine which fork philosopher take and put down
while (1) {
int* i = num;
sleep(1);
take_fork(*i);
sleep(0);
put_fork(*i);
}
}
// main method
int main()
{
int i;
pthread_t thread_id[N]; //thread id based on number of philosophers
// initialize the semaphores
sem_init(&mutex, 0, 1);
for (i = 0; i < N; i++)
sem_init(&sphil[i], 0, 0);
for (i = 0; i < N; i++) {
// create each philosopher processes
pthread_create(&thread_id[i], NULL,
philosopher, &phil[i]);
}
// wait for threads to finish
for (i = 0; i < N; i++)
pthread_join(thread_id[i], NULL);
}
