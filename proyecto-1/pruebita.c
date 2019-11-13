#include<stdio.h>
#include<setjmp.h>
jmp_buf buf[2];
int first_time = 1;

void compute_result(int actual_thread){
  int x = 0;

  if(first_time){
      sigsetjmp(buf[actual_thread], 1);
      printf("On thread %d \n", actual_thread);
  }

  if(first_time){
    first_time = 0;
    siglongjmp(buf[actual_thread], 1);
  }

  for (size_t i = 0; i < 3; i++) {
    x++;
    printf("On thread %d, X: %d\n", actual_thread, x);
  }

  sigsetjmp(buf[actual_thread], 1);
}

void scheduler(){
  for (size_t i = 0; i < 2; i++) {
    compute_result(0);
    compute_result(1);
  }

}

int main(){
  scheduler();
  return 0;
}
