#include <vector>
#include <iostream>

using namespace std;

bool divisible_N(vector<int> vector) {
    bool by6, by4, by3, by2 = false; // Factors of 12

    if (vector.empty() || vector.size() == 1) { // Edge cases
        return false;
    }

    for (int i = 0; i < vector.size(); ++i) {
        if (vector[i] % 6 == 0) { // Checking if divisible by the factors
            by6 = true;
        }
        else if (vector[i] % 2 == 0) { // Has to be else if for this one Ex: {6,1} would return true w/o it
            by2 = true;
        }
        if (vector[i] % 4 == 0) {
            by4 = true;
        }
        if (vector[i] % 3 == 0) {
            by3 = true;
        }

        if ((by6 && by2) || (by4 && by3)) { // Checking if there are numbers divisible by both
            return true;
        }

    }

    return false;

}

bool divisible_N2(vector<int> vector) {
    if (vector.empty() || vector.size() == 1) { // Edge cases
        return false;
    }

    for (int i = 0; i < vector.size(); ++i) {
        for (int j = i+1; j < vector.size(); ++j)
            if ((vector[i] * vector[j]) % 12 == 0) {
                return true;
            }
    }
    return false;
}

template <class T>
    

int Binary_Search(vector<T> &v, T x, int& num_comp) {
    num_comp = 0;
    int mid, low = 0;     
    int high = (int) v.size()-1;     
    while (low < high) {         
        mid = (low+high)/2;                 
        if (num_comp++, v[mid] < x) low = mid+1;         
        else high = mid;     
    }

    if (num_comp++, x == v[low]) return low; //OK: found    

   return -1; //not found
}

int main() {
    
    vector<int> v1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,16};
    
    int num_comp = 0;


    Binary_Search(v1,15,num_comp);
    cout << num_comp << endl;

    //Binary_Search(v1,16,num_comp);
    // cout << num_comp << endl;
    // Binary_Search(v1,8,num_comp);
    // cout << num_comp << endl;
    //cout << Binary_Search(v2,'c',num_comp) << endl;
    // vector<int> v1 = {};
    // vector<int> v2 = {12};
    // vector<int> v3 = {1,12};
    // vector<int> v4 = {1, 6};
    // vector<int> v5 = {2, 6};
    // vector<int> v6 = {1, 3, 6, 5};
    // vector<int> v7 = {6, 1, 4};

    // cout << "The output for each test case of the O(n) function is: " << endl;
    // cout << divisible_N(v1) << endl;
    // cout << divisible_N(v2) << endl;
    // cout << divisible_N(v3) << endl;
    // cout << divisible_N(v4) << endl;
    // cout << divisible_N(v5) << endl;
    // cout << divisible_N(v6) << endl;
    // cout << divisible_N(v7) << endl;

    // cout << "The output for each test case of the O(n^2) function is: " << endl;
    // cout << divisible_N2(v1) << endl;
    // cout << divisible_N2(v2) << endl;
    // cout << divisible_N2(v3) << endl;
    // cout << divisible_N2(v4) << endl;
    // cout << divisible_N2(v5) << endl;
    // cout << divisible_N2(v6) << endl;
    // cout << divisible_N2(v7) << endl;

    return 1;
}