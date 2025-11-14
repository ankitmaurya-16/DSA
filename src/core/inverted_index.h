#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <filesystem>
#include "../index/tokenizer.h"
#include "../index/bloom_filter.h"
#include "../index/file_reader.h"
#include "../index/dependency_checker.h"
#include "../query/trie.h"
#include "../query/skiplist.h"
#include "../query/phrase_search.h"
#include "../ranking/bm25.h"
#include "../ranking/min_heap.h"
#include "utils.h"
using namespace std;
class Clustering;

class InvertedIndex
{
private:
    unordered_map<string, vector<Posting>> hashIndex;
    Trie termTrie;
    BloomFilter bloomFilter;
    map<DocID, DocumentInfo> documents;
    Tokenizer tokenizer;
    FileReader fileReader;
    DependencyChecker depChecker;
    BM25 bm25;
    unordered_map<string, vector<DocID>> filenameIndex;
    Clustering *clustering;
    DocID nextDocId;
    size_t totalTerms;

    void updateBM25Ref();

public:
    InvertedIndex();
    void addDocument(const string &filepath, const string &content);
    void crawlDirectory(const string &dirPath);
    void checkDependencies();

    vector<DocID> search(const string &term);
    vector<string> autocomplete(const string &prefix, int limit = 10);
    vector<pair<DocID, double>> topK(const string &term, int k = 10);
    vector<pair<DocID, double>> multiTerm(const vector<string> &terms, int k = 10);

    vector<DocID> searchAnd(const vector<string> &terms);
    vector<DocID> searchOr(const vector<string> &terms);
    vector<DocID> searchPhrase(const vector<string> &terms);

    void cluster(double threshold = 0.3);

    const DocumentInfo *getDocument(DocID id) const;
    void printStatistics() const;
};

#endif