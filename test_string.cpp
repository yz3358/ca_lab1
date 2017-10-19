//
// Created by ZhangYutong on 10/16/17.
//

#include<iostream>
#include<string>
using namespace std;

int main () {

    string s = "ABCDEFG";

    // substring
    cout << s.substr(0,2) << endl;
    cout << s.substr(0,3) << endl;
    cout << s << endl;

    int i = 1;
    int j = 3;
    if (i == 1 && j == 2)
        printf("Testing successful!\n");

    if (s=="ABCDEFG")
        printf("Testing successful for comparing equal str.\n");
    return 0;
}
