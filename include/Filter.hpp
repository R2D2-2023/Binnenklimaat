#ifndef Filter_HPP
#define Filter_HPP
#include <Arduino.h>

class Filter
{
private:
    void calcAvg();
    float average;
    float last_average = -1;
    float measurements[5];
    unsigned int data_point_count = 0;
    static int sort_desc(const void *cmp1, const void *cmp2);
public:
    float getValue();
    void addDatapoint(float data);
};

#endif //Filter_HPP