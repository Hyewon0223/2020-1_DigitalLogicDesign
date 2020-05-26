#include <iostream>
#include <fstream> // ifstream is; is.open("input.txt"); is.close();
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>
#include <iomanip> //
#include <vector> //
#include <limits.h>
using namespace std;

vector<vector<string>> vecEPI;
vector<string> vecEPI1;
vector<string> vecEPI2;
vector<vector<string>> vecNEPI;
vector<string> vecNEPI1;
vector<string> vecNEPI2;

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
    cout << setw(15) << "binary" << '\n';
    
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

void EPINEPI(int idx, int a, string arr[][4], string EPI[][4]){
    for (int i=0;i<idx;i++){
        if (arr[i][3] == "V") {
            vecEPI1.push_back(EPI[i][1]);
            vecEPI2.push_back(EPI[i][2]);
        }
    }
    for (int i=0;i<a;i++){
        if (arr[i][3] == "") {
            vecNEPI1.push_back(arr[i][1]);
            vecNEPI2.push_back(arr[i][2]);
        }
    }
    vecEPI.push_back(vecEPI1);
    vecEPI.push_back(vecEPI2);
    vecNEPI.push_back(vecNEPI1);
    vecNEPI.push_back(vecNEPI2);
}

//a=nmin+ndc(=arr의 길이), b=inputvariable
void HammingDistance(int a,int b,string arr[100][4]){
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
                        if (EPI[k][2] == binary){
                            state = false;
                            arr[i][3] = "V";
                            arr[j][3] = "V";
                        }
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
    for (int i=0;i<a;i++){
        if (arr[i][3] == "V") vcnt++;
    }
    if (vcnt != 0 && idx>1) HammingDistance(idx,b,EPI);
    else EPINEPI(idx,a, arr, EPI);
}

//arr = table, len=nmin, arr2=minterm
void showDominance(int row, int col, string arr1[][1],int len, int arr2[]){
    cout << setw(20) << "Prime Implicants";
    for (int i=0;i<len;i++) cout << setw(4) << arr2[i];
    cout << '\n';
    
    for (int i=0;i<row;i++){
        cout << setw(20) << arr1[i][0]; 
        for (int j=1;j<col;j++){
            cout << setw(4) << arr1[i][j]; 
        }
        cout << '\n';
    }
    cout << "-----------------------------------------" << '\n';
}

//a = nmin, arr[] = minterm;
string Dominance(int a, int arr[]){
    string solution = "";
    //vecNEPI[0] = [8,10 10,11  11,15  12,13  13,15]
    //vecNEPI[1] = [10-0  101-   1-11   110-   11-1]
    int row = vecNEPI[0].size()+vecEPI[0].size();
    string table[row][a+1];
    for (int i=0;i<row;i++){
        for (int j=0; j<a+1; j++) table[i][j] = " ";
    }
    for (int i=0;i<vecNEPI[0].size();i++){
        table[i][0] = vecNEPI[1][i];
        istringstream aa(vecNEPI[0][i]);
        string stringBuffer;
        int k = 0;
        string num[100];
        while(getline(aa,stringBuffer,',')){
            num[k++] = stringBuffer;
        }
        // for (int m=0;m<k;m++){
        //     cout << num[m] << " ";
        // }
        // cout << '\n';
        for (int l=0;l<k;l++){
            for (int j=0; j<a; j++){
                if (num[l] == to_string(arr[j])) table[i][j+1] = "V";
            }
        }
    }
    for (int i=vecNEPI[0].size();i<row;i++){
        table[i][0] = vecEPI[1][i-vecNEPI[0].size()];
        istringstream aa(vecEPI[0][i-vecNEPI[0].size()]);
        string stringBuffer;
        int k = 0;
        string num[100];
        while(getline(aa,stringBuffer,',')){
            num[k++] = stringBuffer;
        }
        for (int l=0;l<k;l++){
            for (int j=0; j<a; j++){
                if (num[l] == to_string(arr[j])) table[i][j+1] = "V";
            }
        }
    }
    // for (int i=0;i<row;i++){
    //     for (int j=0;j<a+1;j++){
    //         cout << table[i][j] << " ";
    //     }
    //     cout << '\n';
    // }
    //showDominance(row,a+1,table,a,arr); //함수로 구현하고 싶은데 인자로 받는 과정에서 에러 발생
    cout << setw(10) << "PI";
    for (int i=0;i<a;i++) cout << setw(4) << arr[i];
    cout << '\n';
    
    for (int i=0;i<row;i++){
        cout << setw(10) << table[i][0]; 
        for (int j=1;j<a+1;j++){
            cout << setw(4) << table[i][j]; 
        }
        cout << '\n';
    }
    for (int i=0;i<a+1;i++) cout << "-----";
    cout << '\n';  

    //column dominance
    for (int j=1;j<a+1;j++){
        int cnt = 0; //V의 수를 세어줌
        int idx = 0; //유일한 V가 있는 위치 저장
        for (int i=0;i<row;i++){
            if (table[i][j] == "V" || table[i][j] == "O"){
                cnt++;
                idx = i;
            }
        }
        if (cnt == 1 && table[idx][j]=="V"){
            table[idx][j] = "O";
            solution += table[idx][0]+"+";
            for (int k=1;k<a+1;k++){
                if (table[idx][k] == "V") {
                    table[idx][k] = "O";
                    for (int l=0;l<row;l++){
                        if (table[l][k] == "V") table[l][k] = "O";
                    }
                }
            }
        }
    }
    cout << "   >> Complete Column Dominance" <<'\n';
    cout << setw(10) << "PI";
    for (int i=0;i<a;i++) cout << setw(4) << arr[i];
    cout << '\n';
        
    for (int i=0;i<row;i++){
        cout << setw(10) << table[i][0]; 
        for (int j=1;j<a+1;j++) cout << setw(4) << table[i][j]; 
        cout << '\n';
    }
    for (int i=0;i<a+1;i++) cout << "-----";
    cout << '\n';

    // row dominance
    while (true){
        int include[row][row];
        int includeSum[row];
        for (int i=0;i<row;i++){
            includeSum[i] = 0;
            for (int j=0;j<row;j++) include[i][j] = 0;
        }
        bool state = false;
        for (int i=0;i<row;i++){
            for (int j=0;j<row;j++){
                if (i!=j){
                    for (int k=1;k<a+1;k++){
                        if (table[i][k] == "V" && table[i][k] == table[j][k]) state = true;
                        else if (table[i][k] == "V" && table[i][k] != table[j][k]) {
                            state = false;
                            break;
                        }
                    }
                    if (state) include[j][i] = 1;
                    state = false;
                }
            }
        }
        int max = INT_MIN;
        int max_idx;
        int totalSum = 0;
        for (int i=0;i<row;i++){
            for (int j=0;j<row;j++){
                includeSum[i] += include[i][j];
            }
            if (max < includeSum[i]){
                max = includeSum[i];
                max_idx = i;
            }
            totalSum += includeSum[i];
        }
        if (totalSum == 0) break;
        solution += table[max_idx][0] + "+";
        for (int i=1;i<a+1;i++){
            if (table[max_idx][i] == "V"){
                for (int j=0;j<row;j++){
                    if (table[j][i] == "V") table[j][i] = "O";
                }
            }
        }
    }

    cout << "   >> Complete Row Dominance" <<'\n';
    cout << setw(10) << "PI";
    for (int i=0;i<a;i++) cout << setw(4) << arr[i];
    cout << '\n';
        
    for (int i=0;i<row;i++){
        cout << setw(10) << table[i][0]; 
        for (int j=1;j<a+1;j++) cout << setw(4) << table[i][j]; 
        cout << '\n';
    }
    for (int i=0;i<a+1;i++) cout << "-----";
    cout << '\n';     

    //row dominance가 없는 경우
    while (true){
        int rowSum[row];
        int rowTotal = 0;
        int max2_idx = 0;
        int max2 = INT_MIN;
        for (int i=0;i<row;i++) rowSum[i] = 0;
        for (int i=0;i<row;i++){
            for (int j=1;j<a+1;j++){
                if (table[i][j] == "V") rowSum[i]++;
            }
            rowTotal += rowSum[i];
            if (max2 < rowSum[i]){
                max2 = rowSum[i];
                max2_idx = i;
            }
        }
        if (rowTotal == 0) return solution;
        for (int i=1;i<a+1;i++){
            if (table[max2_idx][i] == "V"){
                for (int j=0;j<row;j++){
                    if (table[j][i] == "V") table[j][i] = "O";
                }
            }
        }
        solution += table[max2_idx][0] + "+";
    }

    cout << "   >> Petrick's Method Complete" <<'\n';
    cout << setw(10) << "PI";
    for (int i=0;i<a;i++) cout << setw(4) << arr[i];
    cout << '\n';
        
    for (int i=0;i<row;i++){
        cout << setw(10) << table[i][0]; 
        for (int j=1;j<a+1;j++) cout << setw(4) << table[i][j]; 
        cout << '\n';
    }
    for (int i=0;i<a+1;i++) cout << "-----";
    cout << '\n'; 

    return solution;
}

// a = InputVariable
string showSolution(int a, string str){
    string FinalSolution = "f(";
    for (int i=0;i<a-1;i++) FinalSolution += "X"+to_string(i)+",";
    FinalSolution += "X"+to_string(a-1)+") = ";
    istringstream aa(str);
    string stringBuffer;
    int idx = 0;
    string PI_binary[100];
    while(getline(aa,stringBuffer,'+')){
        PI_binary[idx++] = stringBuffer;
    }
    for (int i=0;i<idx;i++){
        for (int j=0;j<PI_binary[i].length();j++){
            if (PI_binary[i][j] != '-') {
                FinalSolution += "X" + to_string(j);
                if (PI_binary[i][j] == '0') FinalSolution +=  "'";
            }
        }
        if (i != idx-1) FinalSolution += " + ";
    }
    return FinalSolution;
}

int main(){
    int InputVariable,nmin,ndc;
    cout << "Number of Input Variable: ";
    cin >> InputVariable;

    cout << "Number of Minterm, Don't care: ";
    cin >> nmin >> ndc;
    int minterm[nmin];
    int dontcare[ndc];  

    if (nmin > 0){
        cout << "Minterm: ";
        for (int i=0;i<nmin;i++)cin >> minterm[i];
    }

    if (ndc > 0){
        cout  << "Don't Care: ";
        for (int i=0;i<ndc;i++) cin >> dontcare[i];
    }

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

    // cout << "EPI" << '\n';
    // for (int i=0;i<vecEPI.size();i++){
    //     for (int j=0;j<vecEPI[i].size();j++){
    //         cout << vecEPI[i][j] << " ";
    //     }
    //     cout << '\n';
    // }
    // cout << "NEPI"<<'\n';
    // for (int i=0;i<vecNEPI.size();i++){
    //     for (int j=0;j<vecNEPI[i].size();j++){
    //         cout << vecNEPI[i][j] << " ";
    //     }
    //     cout << '\n';
    // }
    string solution = Dominance(nmin,minterm); //column/row dominance table
    cout << showSolution(InputVariable, solution); // Final Solution
    return 0;
}