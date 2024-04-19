#include <fstream>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <string>


using namespace std;
void fillVector(string filename, vector<string>& wordlist, unordered_map<string, int> visited){
    ifstream input(filename);
    string word;
    while(input >> word){
        if(word.size() > 3 && !visited.count(word)){
            wordlist.push_back(word);
        }
    }
    input.close();
}
vector<string> filterWord(vector<string>& wordlist, unordered_map<char, int>& allowed, char c){
    vector<string> filter;
    vector<string> result;
    for(auto i : wordlist){
        bool found = false;
        for(auto j : i){
            if(j == c){
                found = true;
            }
        }
        if(found){
            filter.push_back(i);
        }
    }

    for(auto i: filter){
        bool impure = false;
        for(auto j: i){
            if(!allowed.count(j)){
                impure = true;
            }
        }

        if(!impure){
            result.push_back(i);
        }
    }
    return result;
}
int main(){
    vector<string> wordlist;
    
    unordered_map<string, int> map;
    

    fillVector("enable1.txt", wordlist, map);
    fillVector("ospd.txt", wordlist, map);
    fillVector("popular.txt", wordlist, map);
    fillVector("words.txt", wordlist, map);
    fillVector("unix-words", wordlist, map);

    cout << wordlist.size() <<  endl;


    unordered_map<char, int> allowed;
    char shouldHave;
    cout << "Character that it should have: ";
    cin >> shouldHave;
    int n = 7;
    while(n--){
        char c;
        cin >> c;
        allowed[c]++;
    }

    vector<string> filter = filterWord(wordlist, allowed, shouldHave);

    ofstream out("possible.txt");
    for(auto i : filter){
        out << i << endl;
    }
}