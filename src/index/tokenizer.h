#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <unordered_set>
using namespace std;
class Tokenizer
{
private:
    static const unordered_set<string> STOPWORDS;
    string stem(const string &word);
    bool endsWith(const string &str, const string &suffix);

public:
    vector<string> tokenize(const string &text);
};

#endif
