#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>

using namespace std;

vector<string> filterP(vector<string> &wordlist, char c){
    vector<string> result;
    for(int i = 0; i < wordlist.size(); i++){
        for(int j = 0; j < 5; j++){
            if(wordlist[i][j] == c){
                result.push_back(wordlist[i]);
            }
        }
    }

    return result;
}
vector<string> filterN(vector<string> &wordlist, char c){
    vector<string> result;
    for(int i = 0; i < wordlist.size(); i++){
        bool has = false;
        for(int j = 0; j < 5; j++){
            if(wordlist[i][j] == c){
                has = true;
            }
        }
        if(!has){
            result.push_back(wordlist[i]);
        }
    }

    return result;
}

vector<string> filterQ(vector<string> &wordlist, char c, int j){
    vector<string> result;
    for(int i = 0; i < wordlist.size(); i++){
        if(wordlist[i][j] == c){
            result.push_back(wordlist[i]);
        }
        
            
        
    }

    return result;
}
vector<string> filterQP(vector<string> &wordlist, char c, int j){
    vector<string> result;
    for(int i = 0; i < wordlist.size(); i++){
        if(wordlist[i][j] != c){
            result.push_back(wordlist[i]);
        }
    }

    return result;
}


int main(){
    ifstream input("words.txt");
    vector<string> wordlist;
    unordered_map<string, int> map;
    string word;
    while(input >> word){
        if(!map.count(word)){
            wordlist.push_back(word);
        }
        map[word]++;
        
    }
    sort(wordlist.begin(), wordlist.end());
    int n = 5;
    while(n--){
        char contains = 'y';
        cout << "Contains(y/n): ";
        cin >> contains;
        while(contains == 'y'){
            char c;
            cout << "Letter: ";
            cin >> c;
            wordlist = filterP(wordlist, c);
            cout << "Contains(y/n): ";
            cin >> contains;
        }
        char nott = 'y';
        cout << "Not(y/n): ";
        cin >> nott;
        while(nott == 'y'){
            char c;
            cout << "Letter: ";
            cin >> c;
            wordlist = filterN(wordlist, c);
            cout << "Not(y/n): ";
            cin >> nott;
        }


        char np = 'y';
        cout << "Not Place(y/n): ";
        cin >> np;
        while(np == 'y'){
            char c;
            int n;
            cout << "Letter: ";
            cin >> c;
            cout << "Position: ";
            cin >> n;
            wordlist = filterQP(wordlist, c, n - 1);
            cout << "Not Place(y/n): ";
            cin >> np;
        }

        char place = 'y';
        cout << "Place(y/n): ";
        cin >> place;
        while(place == 'y'){
            char c;
            int n;
            cout << "Letter: ";
            cin >> c;
            cout << "Position: ";
            cin >> n;
            wordlist = filterQ(wordlist, c, n - 1);
            cout << "Place(y/n): ";
            cin >> place;
        }

        ofstream put("possible.txt");
        for(int i = 0; i < wordlist.size(); i++){
            put << wordlist[i] << endl;
        }
    }





    return 0;
}