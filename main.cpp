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
#include <queue>
#include <thread>
#include <mutex>

using namespace std;

mutex mtx;

struct wordCombination
{
	string currentWord;
	string combinationResults;
	bool validWord;
	wordCombination()
		: currentWord(""),combinationResults("_____"),validWord(false)
	{
	}

	wordCombination(string word, string combResults)
		: currentWord(word),combinationResults(combResults)
	{
		validWord = (currentWord.length() == 5 && combinationResults.length() == 5) ? true : false;
	}
};

// using a struct for this in case more options needed to be added
struct wordPanelManager
{
	bool shouldBePrinting;
	bool close;
	bool acknowledged;
	wordPanelManager()
		: shouldBePrinting(false),close(false),acknowledged(false)
	{
	}
};

vector<string> filterByGuess(vector<string> &wordlist, string guess, string outcome) 
{
  vector<string> filtered;
  for (auto& word : wordlist) 
	{
  	string currentOutcome = ""; // Generate the outcome string for this word
		for (int i = 0; i < 5; ++i) 
		{
			if (word[i] == guess[i]) 
			{
				currentOutcome += 'G'; // Correct position
			} else if (guess.find(word[i]) != string::npos) 
			{
     		currentOutcome += 'Y'; // Wrong position
      } else 
			{
      	currentOutcome += 'B'; // Not in word
      }
    }
    if (currentOutcome == outcome) 
		{
    	filtered.push_back(word);
    }
  }
  return filtered;
}

double calculateInformationGain(vector<string> &wordlist, string guess,queue<wordCombination>* currentCombinations) 
{
	int sizeCount(0);
  map<string, int> outcomeCounts;
  // Simulate outcomes for each word in the list
  for (auto& secret : wordlist) 
	{
    string outcome("");
    for (int i = 0; i < 5; ++i) 
		{
    	if (secret[i] == guess[i]) 
			{
        outcome += 'G';
      } else if (guess.find(secret[i]) != string::npos) 
			{
        outcome += 'Y';
      } else 
			{
        outcome += 'B';
      }
    }
   	outcomeCounts[outcome]++;
		mtx.lock();
		currentCombinations->push(wordCombination(secret,outcome));
		mtx.unlock();
		
		sizeCount++;
  }
	
  double entropy = 0.0;
  for (auto& [outcome, count] : outcomeCounts) 
	{
  	double probability = (double)count / wordlist.size();
  	entropy += probability * log2(probability);
  }
  return -entropy;
}

vector<string> exclude(vector<string>  &wordlist, char c)
{
	vector<string> result;
	for(int i = 0; i < wordlist.size(); i++)
	{
		bool has = false;
		for(int j = 0; j < 5; j++)
		{
			if(wordlist[i][j] == c)
			{
				has = true;
			}
		}
		if(!has)
		{
			result.push_back(wordlist[i]);
		}
	}

  return result;
}

vector<string> includePos(vector<string> &wordlist, char c, int j)
{
  vector<string> result;
  for(int i = 0; i < wordlist.size(); i++)
	{
    if(wordlist[i][j] == c)
		{
    	result.push_back(wordlist[i]);
    }
  }

  return result;
}
vector<string> notPosCon(vector<string> &wordlist, char c, int j)
{
  vector<string> result;
  for(int i = 0; i < wordlist.size(); i++)
	{
    if(wordlist[i][j] != c)
		{
      result.push_back(wordlist[i]);
    }
  }

  return result;
}

vector<string> contains(vector<string> &wordlist, char c)
{
  vector<string> result;
  for(int i = 0; i < wordlist.size(); i++)
	{
  	for(int j = 0; j < 5; j++)
		{
      if(wordlist[i][j] == c)
			{
        result.push_back(wordlist[i]);
      }
    }
  }
  return result;
}


void printCombinationResults(int width,queue<wordCombination>* wordCombinations,wordPanelManager* controller)
{
	if(width == 0)
	{
		controller->acknowledged = true;
		return;
	}
	wordCombination currentWord;
	bool hasWord(false);

	int currentWidth(0);
	while(true)
	{
		// checking if we should close the current thread
		if(controller->close)
		{
			controller->acknowledged = true;
			return;
		}

		// checking if we shouldn't be printing anything out
		while(!controller->shouldBePrinting)
		{
			if(controller->close)
			{
				controller->acknowledged = true;
				return;
			}
			if(controller->acknowledged == false)
			{
				cout << "\n";
				currentWidth = 0;
			}
			controller->acknowledged = true;
		}
		controller->acknowledged = false;

		if(controller->shouldBePrinting && !wordCombinations->empty())
		{
			currentWord = wordCombinations->front();
			hasWord     = true;

			mtx.lock();
			wordCombinations->pop();
			mtx.unlock();
		}

		// making sure that we aren't trying to print out 
		// any info that doesn't exist
		if(controller->shouldBePrinting && hasWord) 
		{
			if(currentWord.validWord)
			{
				cout << currentWord.currentWord << ": " << currentWord.combinationResults << " ";
				currentWidth++;
			}

			// checking if the amount of information that we have 
			// printed is to the specified width, if it is
			// then we want to return to the beginning of the line
			if(currentWidth % width == 0) 
			{
				cout << "\r";
				currentWidth = 0;
			}
		}
	}
}

void findMostOptimalOption(int printWidth)
{
	
	// getting our utility files ready
	ifstream input("words.txt");
	ofstream put  ("possible.txt");

	vector<string> wordlist;
	unordered_map<string, int> map;
	string word;
	while (input >> word) 
	{
		if (!map.count(word)) 
		{
    	wordlist.push_back(word);
    }
    map[word]++;
  }
  input.close();  // Close the file after reading

  if (wordlist.empty()) 
	{
    cout << "No words loaded. Check the words file." << endl;
    exit(1);
  }
	queue<wordCombination>* currentCombinations = new queue<wordCombination>{};
	wordPanelManager* controller                = new wordPanelManager();

	thread t1(printCombinationResults,printWidth,currentCombinations,controller);
	

	// Assume 5 rounds of guessing
  for (int round = 0; round < 5; round++) 
	{  
		cout << "total guess options rn: " << wordlist.size() << "\n";
		string bestGuess;
		double bestGain = -INFINITY;
		auto 	 start    = chrono::high_resolution_clock::now();

		// Evaluate each word as a potential guess
		controller->shouldBePrinting = true;
		for (auto& guess : wordlist) 
		{
      double gain = calculateInformationGain(wordlist, guess,currentCombinations);
			
      if (gain > bestGain) 
			{
        bestGain  = gain;
        bestGuess = guess;
      }
    }
		controller->shouldBePrinting = false;
		auto stop     = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
	
		// waiting till the other thread is done with its current task
		while(!controller->acknowledged) {}

		// calculating out the timing difference for 
    // how long it took to calculate information gain
		cout << "Best guess: " << bestGuess << " with information gain: " << bestGain << endl;
		auto hours    =  duration.count() / 3600;
		auto minutes  = (duration.count() % 3600) / 60;
		auto seconds  =  duration.count() % 60;

    cout << "Iteration took " << hours << " hours, " << minutes << " minutes, and " << seconds << " seconds." << endl;
  

    vector<int> result(5);
    cout << "Enter feedback for each letter (0 = not in word, 1 = wrong place, 2 = correct): ";
    for (int i = 0; i < 5; i++) 
		{
			cin >> result[i];
			if (result[i] == 0) 
			{
    		wordlist = exclude(wordlist, bestGuess[i]);
    	} else if (result[i] == 2) 
			{
        wordlist = includePos(wordlist, bestGuess[i], i);
      } else if (result[i] == 1) 
			{
        wordlist = contains (wordlist, bestGuess[i]);
        wordlist = notPosCon(wordlist, bestGuess[i], i);
      }
    }

    for(auto j : wordlist)
		{
    	put << j << endl;
    }

		// checking if the current word is correct or if 
		// we should continue narrowing down our options
		bool allTwo(true);
		for(int j=0;j<5;++j)
		{
			if(result[j] != 2)
			{
				allTwo = false;
			}
		}

		if(allTwo)
		{
			cout << "Successfuly found word " << bestGuess << "!\n";
			break;
		}
  }

	// closing our thread 
	controller->close = true;
	t1.join();
	
	delete currentCombinations;
	delete controller;
}

int main(int argc, char** argv)
{
	int printWidth(0);
	if(argc >= 2)
	{
		printWidth = stoi(argv[1]);
	}
	findMostOptimalOption(printWidth);

  return 0;
}
