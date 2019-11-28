#include "latex_template.h"

float calculate_u(){
    // The formula is E ci/pi <= n(2^1/n - 1)
    float u = 0;

    // Avoid an indefinition dividing by 0
    if(entry_0_text_period_int == 0) entry_0_text_period_int = 1;
    if(entry_1_text_period_int == 0) entry_1_text_period_int = 1;
    if(entry_2_text_period_int == 0) entry_2_text_period_int = 1;
    if(entry_3_text_period_int == 0) entry_3_text_period_int = 1;
    if(entry_4_text_period_int == 0) entry_4_text_period_int = 1;
    if(entry_5_text_period_int == 0) entry_5_text_period_int = 1;

    u = (float)entry_0_exec_time_int / entry_0_text_period_int +
        (float)entry_1_exec_time_int / entry_1_text_period_int +
        (float)entry_2_exec_time_int / entry_2_text_period_int +
        (float)entry_3_exec_time_int / entry_3_text_period_int +
        (float)entry_4_exec_time_int / entry_4_text_period_int +
        (float)entry_5_exec_time_int / entry_5_text_period_int;

    printf("u: %f\n", u);
    return u;
}

float calculate_U(int n){
  // The formula is n(2^1/n - 1)
  float U = 0;

  U = n*((float)pow((double)2, (double)1/n) - 1);

  printf("N: %d U: %f\n", n, U);

  return U;
}
