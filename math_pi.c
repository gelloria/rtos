#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "mythreads.c"
#include <stdlib.h>
#include <math.h>

// function to find factorial of given number
long double factorial(int n)
{
    if (n == 0)
      return 1;

    return n * factorial(n - 1);
}

/*
long double arcsen(int x) {
  long double result = 0, numerador = 0, denominador = 0;

    // TODO: Este numero deberia de ser variable
    for (int n = 0; n < 878; n++) {
      //result+=(factorial(2*n) * 2*x**(2*n+1))/(   (2**(2*n))   *   (factorial(n)**2)  *  (2*n + 1));
      //result+=(factorial(2*n) * 2*pow(x, 2*n+1))/(   (pow(2, 2*n))   *   (pow(factorial(n), 2))  *  (2*n + 1));
      numerador = factorial(2*n) * pow(x, 2*n+1);
      denominador = pow(2, 2*n) * pow(factorial(n), 2) * (2*n + 1);
      //printf("NUMERADOR %Lf \n", numerador);
      //printf("DENOMINADOR %Lf \n", denominador);
      result += numerador / denominador;
    }

  return result;
}
*/

int main(int argc, char const *argv[]) {

  long double result = 2*arcsen(1);
  //printf("Result %f\n", result);
  fprintf(stderr, "Result %.30Lf\n", result);

  return 0;
}
