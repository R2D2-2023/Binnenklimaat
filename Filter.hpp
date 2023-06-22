#ifndef Filter_HPP
#define Filter_HPP

#include <vector>
#include <algorithm>
using namespace std;

class Filter
{
private:
    unsigned int vectorAvgMiddle(vector<unsigned int> v);

public:
    unsigned int compareAverageResults(vector<unsigned int> previous_result, vector<unsigned int> current_result);
};

#endif //Filter_HPP