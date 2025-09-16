// sorting items from the heaviest to lightest
// by selecting the heaviest item at each time  

#include <iostream>
#include <vector>
#include "sort_heaviest.h"

using namespace std;

void sort_heaviest(vector<int>& v, int& comparisons){
    // The index of the heaviest element
    //size_t heaviest;
    size_t temp;
    
    for (size_t j = v.size()-1; j > 0; --j) {
        for (size_t i = 0; i < j; ++i) {
            if (v[i] < v[i+1]) {
                temp = v[i];
                v[i] = v[i+1];
                v[i+1] = temp;
                ++comparisons;
            }
            else {
                ++comparisons;
            }
        }
    }
    // Make sure to update comparisons whenever you compare elements
}

int main() {
    vector<int> v1 = {1,2,3,4,5,6,7,8,9,10};
    
    int compare = 0;

    sort_heaviest(v1, compare);
    cout << compare << endl;
}