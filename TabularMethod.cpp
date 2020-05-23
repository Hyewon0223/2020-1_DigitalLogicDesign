#include <iostream>
#include <fstream> // ifstream is; is.open("input.txt"); is.close();
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>
#include <limits.h>
using namespace std;

string Binary(int a,int b){
    char bin[a];
    string binary = "";
    for (int i=0;i<a;i++) bin[i] = 0;
    int idx=0;
    while (b>=1){
        bin[idx++] = '0'+ b%2;
        b/=2;
    }
    for (int j=a-1;j>=0;j--) binary += bin[j];
    return binary;
}

int Count(string n){
    int cnt = 0;
    for (int i=0;i<n.length();i++){
        if (n[i] == '1') cnt++;
    }
    return cnt;
}

int main(){
    int InputVariable,nmin,ndc;
    cout << "Number of Input Variable: ";
    cin >> InputVariable;

    cout << "Number of Minterm, Don't care: ";
    cin >> nmin >> ndc;
    int minterm[nmin];
    int dontcare[ndc];  

    cout << "Minterm: ";
    for (int i=0;i<nmin;i++) cin >> minterm[i];

    cout  << "Don't Care: ";
    for (int i=0;i<ndc;i++) cin >> dontcare[i];

    sort(minterm,minterm+nmin);
    sort(dontcare,dontcare+ndc);

    string PIArr[nmin+ndc][4];
    string numOfOne, binary;
    for (int i=0;i<nmin;i++){
        binary = Binary(InputVariable,minterm[i]);
        numOfOne = Count(binary);
        PIArr[i][0] = numOfOne;
        PIArr[i][1] = "m";
        PIArr[i][2] = minterm[i];
        PIArr[i][3] = binary;
    }
    return 0;
}