#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct threads_type{
  int id;
  int state;
  int tickets;
};

struct threads_type list_of_threads[5];
struct threads_type next_thread;

int rand_range(int lower, int upper)
{
  return (rand() %(upper - lower + 1)) + lower;
}

int main()
{


  int j=0;
  int m=0;
  int rand_num = 0;
  int winner_ticket=  0;
  int total_tickets = 0;
  int available_tickets[80];

  list_of_threads[0].id = 0;
  list_of_threads[0].state = 1;
  list_of_threads[0].tickets = 0;

  list_of_threads[1].id = 1;
  list_of_threads[1].state = 1;
  list_of_threads[1].tickets = 5;

  list_of_threads[2].id = 2;
  list_of_threads[2].state = 0;
  list_of_threads[2].tickets = 10;

  list_of_threads[3].id = 3;
  list_of_threads[3].state = 0;
  list_of_threads[3].tickets = 15;

  list_of_threads[4].id = 4;
  list_of_threads[4].state = 0;
  list_of_threads[4].tickets = 2;

  // Get available threads and tickets
  for (int i = 0; i < 5; i++) {
    if(list_of_threads[i].state != 1){
      total_tickets +=list_of_threads[i].tickets;
      // Builds an array of tickets with thread_ids
      for (int k = 0; k < list_of_threads[i].tickets; k++) {
        available_tickets[m]= list_of_threads[i].id;
        m++;
      }
    }
  }

  // Lottery ticket trow
  srand(time(0));
  rand_num = rand_range(0, total_tickets);
  winner_ticket = available_tickets[rand_num];

  // Get next winning thread
  for (int i = 0; i < 5; i++) {
    if (list_of_threads[i].id == winner_ticket) {
      next_thread = list_of_threads[i];
      break;
    }
  }
  printf("next_thread id is: %d\n",next_thread.id );


  return 0;
}
