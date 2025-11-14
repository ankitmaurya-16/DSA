#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <bitset>
#include <string>
using namespace std;
const int BLOOM_FILTER_SIZE = 10000;

class BloomFilter
{
private:
    bitset<BLOOM_FILTER_SIZE> bits;
    size_t hash1(const string &str) const;
    size_t hash2(const string &str) const;
    size_t hash3(const string &str) const;

public:
    void insert(const string &term);
    bool possiblyContains(const string &term) const;
    void clear();
};

#endif
