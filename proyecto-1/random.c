#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int rand_range(int lower, int upper, int count)
{
  return (rand() %(upper - lower + 1)) + lower;
}

// Driver code
int main()
{
    int lower = 1, upper = 20, count = 10, rand_num = 0;
    srand(time(0));
    for (int i = 0; i < count; i++) {
      rand_num = rand_range(lower, upper, count);
      printf("Num: %d\n", rand_num);
    }
    return 0;
}
