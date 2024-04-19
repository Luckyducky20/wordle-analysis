#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <map>
#include <cmath>
#include <cmath>
#include <chrono>
#include <limits>
using namespace std;

vector<string> filterByGuess(vector<string> &wordlist, string guess, string outcome) {
    vector<string> filtered;
    for (auto& word : wordlist) {
        string currentOutcome = ""; // Generate the outcome string for this word
        for (int i = 0; i < 5; ++i) {
            if (word[i] == guess[i]) {
                currentOutcome += 'G'; // Correct position
            } else if (guess.find(word[i]) != string::npos) {
                currentOutcome += 'Y'; // Wrong position
            } else {
                currentOutcome += 'B'; // Not in word
            }
        }
        if (currentOutcome == outcome) {
            filtered.push_back(word);
        }
    }
    return filtered;
}

double calculateInformationGain(vector<string> &wordlist, string guess) {
    map<string, int> outcomeCounts;
    // Simulate outcomes for each word in the list
    for (auto& secret : wordlist) {
        string outcome = "";
        for (int i = 0; i < 5; ++i) {
            if (secret[i] == guess[i]) {
                outcome += 'G';
            } else if (guess.find(secret[i]) != string::npos) {
                outcome += 'Y';
            } else {
                outcome += 'B';
            }
        }
        outcomeCounts[outcome]++;
    }
    double entropy = 0.0;
    for (auto& [outcome, count] : outcomeCounts) {
        double probability = (double)count / wordlist.size();
        entropy += probability * log2(probability);
    }
    return -entropy;
}
vector<string> exclude(vector<string>  &wordlist, char c){
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

vector<string> includePos(vector<string> &wordlist, char c, int j){
    vector<string> result;
    for(int i = 0; i < wordlist.size(); i++){
        if(wordlist[i][j] == c){
            result.push_back(wordlist[i]);
        }
    }

    return result;
}
vector<string> notPosCon(vector<string> &wordlist, char c, int j){
    vector<string> result;
    for(int i = 0; i < wordlist.size(); i++){
        if(wordlist[i][j] != c){
            result.push_back(wordlist[i]);
        }
    }

    return result;
}

vector<string> contains(vector<string> &wordlist, char c){
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
int main() {
    ifstream input("words.txt");
    ofstream put("possible.txt");
    vector<string> wordlist;
    unordered_map<string, int> map;
    string word;
    while (input >> word) {
        if (!map.count(word)) {
            wordlist.push_back(word);
        }
        map[word]++;
    }
    input.close();  // Close the file after reading

    if (wordlist.empty()) {
        cout << "No words loaded. Check the words file." << endl;
        return 1;
    }

    for (int round = 0; round < 5; round++) {  // Assume 5 rounds of guessing
        string bestGuess;
        double bestGain = -INFINITY;

        // Evaluate each word as a potential guess
        auto start = chrono::high_resolution_clock::now();
        for (auto& guess : wordlist) {
            double gain = calculateInformationGain(wordlist, guess);
            if (gain > bestGain) {
                bestGain = gain;
                bestGuess = guess;
            }
        }

        cout << "Best guess: " << bestGuess << " with information gain: " << bestGain << endl;
        auto stop = chrono::high_resolution_clock::now();  // End timing
        auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
        auto hours = duration.count() / 3600;
        auto minutes = (duration.count() % 3600) / 60;
        auto seconds = duration.count() % 60;

        cout << "Iteration took " << hours << " hours, " << minutes << " minutes, and " << seconds << " seconds." << endl;
  

        vector<int> result(5);
        cout << "Enter feedback for each letter (0 = not in word, 1 = wrong place, 2 = correct): ";
        for (int i = 0; i < 5; i++) {
            cin >> result[i];
            if (result[i] == 0) {
                wordlist = exclude(wordlist, bestGuess[i]);
            } else if (result[i] == 2) {
                wordlist = includePos(wordlist, bestGuess[i], i);
            } else if (result[i] == 1) {
                wordlist = contains(wordlist, bestGuess[i]);
                wordlist = notPosCon(wordlist, bestGuess[i], i);
            }
        }

        for(auto i : wordlist){
            put << i << endl;
        }
    }

    return 0;
}