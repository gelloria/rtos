#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

void timer_handler (int signum)
{
 static int count = 0;
 printf ("timer expired %d times\n", ++count);
}

int main ()
{
 struct sigaction sa;
 struct itimerval timer;
 int quantum_ms = 250;

 /* Install timer_handler as the signal handler for SIGVTALRM. */
 //memset (&sa, 0, sizeof (sa));
 sa.sa_handler = &timer_handler;
 sigaction (SIGALRM, &sa, NULL);

 /* Configure the timer to expire after 250 msec... */
 timer.it_value.tv_sec = 0;
 timer.it_value.tv_usec = quantum_ms*1000;
 timer.it_interval.tv_sec = 0;
 timer.it_interval.tv_usec = quantum_ms*1000;
 /* Start a virtual timer. It counts down whenever this process is
   executing. */
 setitimer (ITIMER_REAL, &timer, NULL);

 /* Do busy work. */
 while (1);
}
