#include "Filter.hpp"


// thank youuuu!!!! https://arduino.stackexchange.com/questions/38177/how-to-sort-elements-of-array-in-arduino-code
int Filter::sort_desc(const void *cmp1, const void *cmp2) {
    // Need to cast the void * to int *
    int a = *((int *)cmp1);
    int b = *((int *)cmp2);
    return b - a;
}

void Filter::calcAvg() {
    qsort(measurements, 5, sizeof(measurements[0]), sort_desc);
    int sum = 0;
    for (unsigned int i = 1; i < 4; i++)
    {
        sum += measurements[i];
    }
    average = (sum/3);
    if (last_average == -1)
    {
        last_average = average;
    }
}

float Filter::getValue() {
    calcAvg();
    data_point_count = 0;
    float res = (average + last_average) / 2;
    last_average = average;
    return res;
}

void Filter::addDatapoint(float data) {
    measurements[data_point_count] = data;
    data_point_count++;
}