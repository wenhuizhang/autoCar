/************************************************
 *                     GW KV
 *  https://github.com/gwAdvNet2015/gw-kv-store
 *
 * Copyright 2015 Jongmin Bae, Stephen Monaghan
 *
 * This program is licensed under the MIT license.
 *
 * test.c - Tests the timer utility
 *************************************************/

#include <stdint.h>
#include <sys/time.h>
#include <stdio.h>
#include "timer_util.h"
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

int main(){
        timer_util* tu = malloc(sizeof(timer_util));
        int i, j, k;
        initialize_timer(tu);
        struct histogram* hist = histogram_create(0, 20000, 10);
        for(i = 0; i < NUM_TIMER; i++){
                timer_set_mode(tu, i, 1);
        }
        for(i = 0; i < 100; i++){
                for(j = 0; j < NUM_TIMER; j++){
                        timer_start(tu, j);
                        for(k = 0; k < 1000000; k++){}
                        timer_end_hist(tu, j, hist);
                        printf("Timer #%d min: %"PRIu64"\n", j, timer_min(tu, j));
                        printf("Timer #%d max: %"PRIu64"\n", j, timer_max(tu, j));
                        printf("Timer #%d avg: %"PRIu64"\n\n", j, timer_avg(tu, j));
                }
        }
        histogram_print(hist);
        return 0;
}
