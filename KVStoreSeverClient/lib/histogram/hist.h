/************************************************
 *                     GW KV
 *  https://github.com/gwAdvNet2015/gw-kv-store
 *
 * Copyright 2015 Jongmin Bae, Stephen Monaghan
 *
 * This program is licensed under the MIT license.
 *
 * hist.h - Implements a historgram to be used with
 *    timer utility
 *************************************************/

#include <stdlib.h>
#include <stdio.h>

struct histogram {
        int min;
        int max;
        int num_buckets;
        double bucket_size; // For memo-izing the division
        unsigned int* buckets;
        int missed_counter;
};

/* Creates and returns the histogram datastructure
 */
struct histogram* histogram_create(int min, int max, int num_buckets)
{
        int i;
        struct histogram* hist = (struct histogram*) malloc(sizeof(struct histogram));

        hist->num_buckets = num_buckets;
        hist->min = min;
        hist->max = max;

        hist->bucket_size = (double) max / (double) num_buckets;

        // Create and zero the histogram's array
        hist->buckets = (unsigned int*) malloc(sizeof(unsigned int) * num_buckets);
        for (i = 0; i < num_buckets; i++) {
                hist->buckets[i] = 0;
        }

        return hist;
}

/* Increments the respective bucket in the histogram
 */
void histogram_inc(struct histogram* hist, int data_point)
{
        int i = (int) (((double) data_point - hist->min) / hist->bucket_size);

        // Deal with the edge case of when data_point is max
        if (i == hist->num_buckets) {
                hist->buckets[i - 1]++;
        }
        else if( i > hist->num_buckets){
                hist->buckets[hist->num_buckets-1]++;
        }
        else{
                hist->buckets[i]++;
        }
}

/* Prints the histogram to the console
 */
void histogram_print(struct histogram* hist)
{
        int i;
        double value = (double) hist->min;

        // Figure out how much we should right justify the buckets
        int diff = hist->max - hist->min;

        if (diff < 10000) {
                for (i = 0; i < hist->num_buckets; i++) {
                        printf("%8.2f\t%d\n", value, hist->buckets[i]);
                        value += hist->bucket_size;
                }
        } else if (diff < 10000000) {
                for (i = 0; i < hist->num_buckets; i++) {
                        printf("%11.2f\t%d\n", value, hist->buckets[i]);
                        value += hist->bucket_size;
                }
        } else {
                for (i = 0; i < hist->num_buckets; i++) {
                        printf("%14.2f\t%d\n", value, hist->buckets[i]);
                        value += hist->bucket_size;
                }
        }
}

/* Frees the histogram from memory
 */
void histogram_free(struct histogram* hist)
{
        free(hist->buckets);
        free(hist);
}
