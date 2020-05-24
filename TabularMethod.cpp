#include <iostream>
#include <fstream> // ifstream is; is.open("input.txt"); is.close();
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>
#include <iomanip> //
#include <vector>
#include <limits.h>
using namespace std;


vector<string> EPI;

string Binary(int a,int b){
    char bin[a];
    string binary = "";
    for (int i=0;i<a;i++) bin[i] = '0';
    int idx=0;
    while (b>=1){
        bin[idx++] = '0'+ b%2;
        b/=2;
    }
    for (int j=a-1;j>=0;j--) binary += bin[j];
    return binary;
}

string Count(string n){
    int cnt = 0;
    for (int i=0;i<n.length();i++){
        if (n[i] == '1') cnt++;
    }
    return to_string(cnt);
}

void showPI(int a, int b, string arr[][4]){
    cout << setw(7) << "# of 1s";
    cout << setw(15) << "minterm";
    cout << setw(15) << "binary"<<'\n';
    
    for (int j=0;j<=a;j++){
        for (int i=0;i<b;i++){
            if (arr[i][0] == to_string(j)){
                cout << setw(7) << arr[i][0]; 
                cout << setw(15) << arr[i][1]; 
                cout << setw(15) << arr[i][2] << '\n'; 
            }
        }
    }
    cout << "-----------------------------------------"<< '\n';
}

//a=nmin+ndc, b=inputvariable
void HammingDistance(int a,int b,string arr[][4]){
    string numOfOne;
    vector<int> diff;
    string EPI[100][4];
    int idx=0;
    for (int i=0;i<a-1;i++){
        for (int j=i+1;j<a;j++){
            diff.clear();
            if (abs(atoi(arr[i][0].c_str()) - atoi(arr[j][0].c_str())) == 1){
                int cnt = 0;
                for (int k=0;k<b;k++){
                    if(arr[i][2][k] != arr[j][2][k]){
                        if (arr[i][2][k] != '-' && arr[j][2][k] != '-'){
                            diff.push_back(k);
                        }
                    }
                    else cnt++;
                }
                if (cnt == b-1){
                    string binary = "";
                    for (int k=0;k<b;k++){
                        if (k == diff.front()) binary+='-';
                        else binary += arr[i][2][k];
                    }
                    bool state = true;
                    for (int k=0;k<idx;k++){
                        if (EPI[k][2] == binary) state = false;;
                    }
                    if (state){
                        numOfOne = Count(binary);
                        EPI[idx][0] = numOfOne;
                        EPI[idx][1] = arr[i][1] +","+arr[j][1];
                        EPI[idx][2] = binary;
                        EPI[idx++][3] = "";
                        arr[i][3] = "V";
                        arr[j][3] = "V";
                    }
                }
            }
        }
    }
    if (idx != 0) showPI(b,idx,EPI);
    int vcnt = 0;
    for (int i=0;i<idx;i++){
        if (arr[i][3] == "V") vcnt++; 
    }
    if (vcnt != 0 && idx>1) HammingDistance(idx,b,EPI);
    
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
    for (int i=0;i<nmin;i++){
        cin >> minterm[i];
    }
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
        //PIArr[i][1] = "m";
        PIArr[i][1] = to_string(minterm[i]);
        PIArr[i][2] = binary;
        PIArr[i][3] = "";
        //cout << PIArr[i][0] << " " << PIArr[i][1] << " " << PIArr[i][2] << " " << PIArr[i][3] << '\n';
    }
    for (int i=nmin;i<nmin+ndc;i++){
        binary = Binary(InputVariable,dontcare[i-nmin]);
        numOfOne = Count(binary);
        PIArr[i][0] = numOfOne;
        //PIArr[i][1] = "d";
        PIArr[i][1] = to_string(dontcare[i-nmin]);
        PIArr[i][2] = binary;
        PIArr[i][3] = "";
    }
    showPI(InputVariable, nmin+ndc, PIArr);
    HammingDistance(nmin+ndc,InputVariable,PIArr);

    return 0;
}