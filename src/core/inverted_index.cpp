#include "inverted_index.h"
#include "../clustering/clustering.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <set>

using namespace std;
namespace fs = filesystem;

InvertedIndex::InvertedIndex() : nextDocId(0), totalTerms(0), clustering(nullptr)
{
    fileReader.setDependencyChecker(&depChecker);
}

void InvertedIndex::checkDependencies()
{
    depChecker.printStatus();
}

void InvertedIndex::updateBM25Ref()
{
    bm25.setIndex(hashIndex, documents);
    if (clustering)
        clustering->setIndex(hashIndex, documents);
}

void InvertedIndex::addDocument(const string &filepath, const string &content)
{
    DocID id = nextDocId++;
    auto tokens = tokenizer.tokenize(content);
    totalTerms += tokens.size();
    string type = fs::path(filepath).extension().string();

    documents[id] = DocumentInfo(id, filepath, content.size(), tokens.size(), type);

    for (Position pos = 0; pos < tokens.size(); ++pos)
    {
        const string &term = tokens[pos];
        bloomFilter.insert(term);
        termTrie.insert(term);
        auto &postings = hashIndex[term];
        if (postings.empty() || postings.back().docId != id)
            postings.emplace_back(id);
        postings.back().addPosition(pos);
    }

    tokens = tokenizer.tokenize(filepath);
    totalTerms += tokens.size();

    for (Position pos = 0; pos < tokens.size(); ++pos)
    {
        const string &term = tokens[pos];
        bloomFilter.insert(term);
        termTrie.insert(term);
        auto &postings = hashIndex[term];
        if (postings.empty() || postings.back().docId != id)
            postings.emplace_back(id);
        postings.back().addPosition(pos);
    }

    updateBM25Ref();
}

void InvertedIndex::crawlDirectory(const string &dirPath)
{
    stack<fs::path> stack;
    stack.push(dirPath);
    int skippedFiles = 0;
    int processedFiles = 0;

    while (!stack.empty())
    {
        fs::path p = stack.top();
        stack.pop();
        if (fs::is_directory(p))
        {
            for (const auto &entry : fs::directory_iterator(p))
            {
                if (fs::is_directory(entry))
                    stack.push(entry.path());
                else if (fs::is_regular_file(entry.path()))
                {
                    string path = entry.path().string();

                    if (fileReader.isSupportedFile(path))
                    {
                        string content = fileReader.readUniversalFile(path);
                        if (!content.empty())
                        {
                            addDocument(path, content);
                            processedFiles++;
                        }
                    }
                    else
                    {
                        skippedFiles++;
                    }
                }
            }
        }
    }

    if (skippedFiles > 0)
    {
        cout << "\n[i] Info: Skipped " << skippedFiles << " unsupported files." << endl;
        if (!depChecker.canProcessPDFs() || !depChecker.canProcessImages())
        {
            cout << "    (Install missing dependencies to support more file types)" << endl;
        }
    }
}

vector<DocID> InvertedIndex::search(const string &term)
{
    string t = term;
    transform(t.begin(), t.end(), t.begin(), ::tolower);
    if (!bloomFilter.possiblyContains(t))
        return {};
    auto it = hashIndex.find(t);
    if (it == hashIndex.end())
        return {};
    vector<DocID> result;
    for (auto &p : it->second)
        result.push_back(p.docId);
    return result;
}

vector<string> InvertedIndex::autocomplete(const string &prefix, int limit)
{
    return termTrie.prefixSearch(prefix, limit);
}

vector<pair<DocID, double>> InvertedIndex::topK(const string &term, int k)
{
    return bm25.topKForTerm(term, k);
}

vector<pair<DocID, double>> InvertedIndex::multiTerm(const vector<string> &terms, int k)
{
    return bm25.scoreMultiTerm(terms, k);
}

vector<DocID> InvertedIndex::searchAnd(const vector<string> &terms)
{
    if (terms.empty())
        return {};
    vector<SkipList<DocID>> lists;
    for (const auto &term : terms)
    {
        auto it = hashIndex.find(term);
        if (it == hashIndex.end())
            return {};
        SkipList<DocID> s;
        for (auto &p : it->second)
            s.insert(p.docId);
        lists.push_back(s);
    }
    auto result = lists[0].toVector();
    for (size_t i = 1; i < lists.size(); i++)
    {
        auto other = lists[i].toVector();
        vector<DocID> inter;
        set_intersection(result.begin(), result.end(), other.begin(), other.end(), back_inserter(inter));
        result = inter;
        if (result.empty())
            break;
    }
    return result;
}

vector<DocID> InvertedIndex::searchOr(const vector<string> &terms)
{
    set<DocID> result;
    for (const auto &t : terms)
    {
        auto it = hashIndex.find(t);
        if (it != hashIndex.end())
            for (auto &p : it->second)
                result.insert(p.docId);
    }
    return vector<DocID>(result.begin(), result.end());
}

vector<DocID> InvertedIndex::searchPhrase(const vector<string> &terms)
{
    return PhraseSearch::searchPhrase(terms, hashIndex);
}

void InvertedIndex::cluster(double threshold)
{
    if (!clustering)
        clustering = new Clustering();
    clustering->setIndex(hashIndex, documents);
    clustering->clusterDocuments(threshold);
}

const DocumentInfo *InvertedIndex::getDocument(DocID id) const
{
    auto it = documents.find(id);
    return (it != documents.end()) ? &(it->second) : nullptr;
}

void InvertedIndex::printStatistics() const
{
    cout << "\n============================================================" << endl;
    cout << "                   INDEXING COMPLETE                        " << endl;
    cout << "============================================================" << endl;
    cout << "Documents indexed: " << documents.size() << endl;
    cout << "Unique terms: " << hashIndex.size() << endl;
    cout << "Total terms: " << totalTerms << endl;
}