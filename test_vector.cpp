//
// Created by ZhangYutong on 10/16/17.
//

using namespace std;
#include <vector>
#include <iostream>




int main () {
    vector<int> powersOfTwo;
    powersOfTwo.push_back(1);
    powersOfTwo.push_back(22);
    powersOfTwo.push_back(4);
    powersOfTwo.push_back(8);

    cout << "2 ^ 3 = " << powersOfTwo[3] << endl;

    powersOfTwo[1] = 2;

    cout << "2 ^ 1 = " << powersOfTwo[1] << endl;

    return 0;
}