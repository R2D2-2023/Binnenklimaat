#include <iostream>
#include "Filter.hpp"

unsigned int Filter::vectorAvgMiddle(vector<unsigned int> v)
{
    int sum = 0;
    sort(v.begin(), v.end());
    for (unsigned int i = 1; i <= v.size() - 2; i++)
    {
        sum += v[i];
    }
    return sum / (v.size() - 2);
}

unsigned int Filter::compareAverageResults(vector<unsigned int> previous_result, vector<unsigned int> current_result)
{
    return ((vectorAvgMiddle(previous_result) + vectorAvgMiddle(current_result)) / 2);
}

int main()
{
    vector<unsigned int> last_array = {100, 50, 50, 50, 10};
    vector<unsigned int> array_notso_recent = {20, 150, 90, 30, 10};

    Filter f();
    f.compareAverageResults(array_notso_recent, last_array);
}
