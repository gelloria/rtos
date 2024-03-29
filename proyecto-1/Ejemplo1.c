#include<stdio.h>
#include<setjmp.h>
jmp_buf buf[3];

void switchThreads(){
  static int curThread = 0;
  int ret_val = sigsetjmp(buf[curThread],1);
  printf("SWITCH: ret_val=%d\n", ret_val);
  if (ret_val == 1) {
    return;
  }
  curThread = 1 - curThread;
  siglongjmp(buf[curThread],1);
}

void func(){
  sigsetjmp(buf[0],1);
  int x = 0;
  for (size_t i = 0; i < 3; i++) {
    x = 1 + x;
  }
  printf("%d\n", x);
}

int main(){
  func();
  return 0;
}
