/************************************************
 *                     GW KV
 *  https://github.com/gwAdvNet2015/gw-kv-store
 *
 * Copyright 2015 Jongmin Bae, Stephen Monaghan
 *
 * This program is licensed under the MIT license.
 *
 * timer_util.h - Implements a timer utility to
 *    test performance
 *************************************************/

#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include "../histogram/hist.h"

#define TIMER_CYCLES
#define TIMER_USEC
#define NUM_TIMER 10

struct timerStruct{
        struct timeval begin, end;
        double elapsed_time;
        unsigned int mode, numUsed;
        long int min, max, sum;
};

typedef struct timer_util{
        struct timerStruct structList[NUM_TIMER];
        //struct histogram* hist;
} timer_util;

void timer_set_mode(timer_util *tu, int timer_id, int mode_flag);
void timer_start(timer_util *tu, int timer_id);
uint64_t timer_end(timer_util *tu, int timer_id);
uint64_t timer_min(timer_util *tu, int timer_id);
uint64_t timer_max(timer_util *tu, int timer_id);
uint64_t timer_avg(timer_util *tu, int timer_id);
uint64_t timer_end_hist(timer_util *tu, int timer_id, struct histogram* hist);

void initialize_timer(timer_util *tu){
        int i = 0;
        //tu->hist = histogram_create(0, 20000000, 10);
        for(i = 0; i < NUM_TIMER; i++){
                tu->structList[i].elapsed_time = 0;
                tu->structList[i].mode = 0;
                tu->structList[i].numUsed = 0;
                tu->structList[i].min = 100000000;
                tu->structList[i].max = 0;
                tu->structList[i].sum = 0;
                gettimeofday(&tu->structList[i].begin, NULL);
                gettimeofday(&tu->structList[i].end, NULL);
        }
}

void timer_set_mode(timer_util *tu, int timer_id, int mode_flag){
        tu->structList[timer_id].mode = mode_flag;
}

void timer_start(timer_util *tu, int timer_id){
        gettimeofday(&tu->structList[timer_id].begin, NULL);
        tu->structList[timer_id].numUsed = tu->structList[timer_id].numUsed + 1;
}

uint64_t timer_end(timer_util *tu, int timer_id){
        gettimeofday(&tu->structList[timer_id].end, NULL);
        long int temp;
        temp = (tu->structList[timer_id].end.tv_sec - tu->structList[timer_id].begin.tv_sec)*1000000L + tu->structList[timer_id].end.tv_usec - tu->structList[timer_id].begin.tv_usec;
        if(temp > tu->structList[timer_id].max){ tu->structList[timer_id].max = temp; }
        if(temp < tu->structList[timer_id].min){ tu->structList[timer_id].min = temp; }
        tu->structList[timer_id].sum = tu->structList[timer_id].sum + temp;
        return temp;
}

uint64_t timer_min(timer_util *tu, int timer_id){
        return tu->structList[timer_id].min;
}

uint64_t timer_max(timer_util *tu, int timer_id){
        return tu->structList[timer_id].max;
}

uint64_t timer_avg(timer_util *tu, int timer_id){
        return (tu->structList[timer_id].sum / (tu->structList[timer_id].numUsed));
}

uint64_t timer_end_hist(timer_util *tu, int timer_id, struct histogram* hist ){
        gettimeofday(&tu->structList[timer_id].end, NULL);
        long int temp;
        temp = (tu->structList[timer_id].end.tv_sec - tu->structList[timer_id].begin.tv_sec)*1000000L + tu->structList[timer_id].end.tv_usec - tu->structList[timer_id].begin.tv_usec;
        if(temp > tu->structList[timer_id].max){ tu->structList[timer_id].max = temp; }
        if(temp < tu->structList[timer_id].min){ tu->structList[timer_id].min = temp; }
        tu->structList[timer_id].sum = tu->structList[timer_id].sum + temp;
        histogram_inc(hist, temp);
        return temp;
}
