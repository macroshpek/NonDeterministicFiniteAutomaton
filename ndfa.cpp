#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#define ENG_LETTERS 26

using namespace std;

class NDFA {
public:
	NDFA(vector<size_t> Q1, vector<char> E1, size_t s1, vector<size_t> F1, vector<vector<vector<char>>> delta1) {
		for (int i = 0; i < Q1.size(); i++) {
			Q[i] = Q1[i];
		}
		for (int i = 0; i < E1.size(); i++) {
			E[i] = E1[i];
		}
		s = s1;
		for (int i = 0; i < F1.size(); i++) {
			F[i] = F1[i];
		}
		for (int i = 0; i < delta1.size(); i++) {
			for (int j = 0; j < delta1.size(); j++) {
				for(int k = 0; k < ENG_LETTERS; k++)
				delta[i][j][k] = delta1[i][j][k];
			}
		}
	}

	void belong(vector<string> words, vector<size_t> solve) {
		size_t init = s;
		size_t current;
		for (size_t i = 0; i < words.size(); i++) {
			current = s;
			int check = step_check(current, delta, words[i], 0);
			if (check >= 0) {
				current = check;
				if (step_check(current, delta, words[i], words[i].length())) {
					for (size_t k = 0; k < F.size(); k++) {
						if (current == F[k]) {
							solve[i] = 1;
						}
					}
				}
			}
		}
	}

	int step_check(size_t from, std::vector<std::vector<std::vector<char>>> tensor, std::string word, size_t current) {
		int check = 0;
		for (size_t to = 0; to < tensor.size(); to++) {
			for (size_t i = 0; i < ENG_LETTERS; i++) {
				if (tensor[from][to][i] == '\0') {
					break;
				}
				if (tensor[from][to][i] == word[current]) {
					if (current + 1 < word.length()) {
						check = step_check(to, tensor, word, current + 1);
						if (check >= 0) {
							return check;
						}
					}
					if (current + 1 == word.length()) {
						return to;
					}
				}
			}
		}
		return -1;
	}
private:
	vector<size_t> Q;
	vector<char> E;
	size_t s;
	vector<size_t> F;
	vector<vector<vector<char>>> delta;
};

int main() {
	std::ifstream input;
	input.open("input.txt");
	if (input.is_open()) {
		size_t numOfStates;
		size_t s;
		size_t numOfFinalStates;
		input >> numOfStates >> s >> numOfFinalStates;

		vector<vector<vector<char>>> delta(numOfStates);
		vector<char> E(ENG_LETTERS);
		vector<size_t> Q(numOfStates);
		vector<size_t> F(numOfFinalStates);
		for (int i = 0; i < numOfFinalStates; i++) {
			input >> F[i];
		}

		if(numOfStates){
			for (int i = 0; i < numOfStates; i++) {
				(delta[i]).resize(numOfStates);
				for (int j = 0; j < numOfStates; j++) {
					(delta[i][j]).resize(ENG_LETTERS);
				}
			}
			vector<size_t> letters(ENG_LETTERS, 0);
			size_t P;
			input >> P;
			size_t from;
			size_t to;
			char value;
			for (size_t i = 0; i < P; i++) {
				input >> from >> to >> value;
				(delta[from][to]).push_back(value);
				if (!(letters[int(value) - 97])) {
					E.push_back(value);
				}
			}
			sort(E.begin(), E.end());
			NDFA ndfa(Q, E, s, F, delta);
			
			size_t numOfWords;
			input >> numOfWords;
			vector<string> words(numOfWords);
			for (size_t i = 0; i < numOfWords; i++) {
				input >> words[i];
			}
			input.close();
			vector<size_t> solve(numOfWords);
			ndfa.belong(words, solve);
			ofstream output;
			output.open("output.txt");
			for (int i = 0; i < numOfWords; i++) {
				if (solve[i]) {
					output << "YES" << std::endl;
				}
				else {
					output << "NO" << std::endl;
				}
			}
		}
	}
	return 0;
}