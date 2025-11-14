#include "bloom_filter.h"
using namespace std;
size_t BloomFilter::hash1(const string &str) const
{
    size_t hash = 5381;
    for (char c : str)
        hash = ((hash << 5) + hash) + c;
    return hash % BLOOM_FILTER_SIZE;
}

size_t BloomFilter::hash2(const string &str) const
{
    size_t hash = 0;
    for (char c : str)
        hash = hash * 31 + c;
    return hash % BLOOM_FILTER_SIZE;
}

size_t BloomFilter::hash3(const string &str) const
{
    size_t hash = 0;
    for (size_t i = 0; i < str.length(); i++)
    {
        hash = (hash << 4) + str[i];
        size_t g = hash & 0xF0000000L;
        if (g != 0)
            hash ^= g >> 24;
        hash &= ~g;
    }
    return hash % BLOOM_FILTER_SIZE;
}

void BloomFilter::insert(const string &term)
{
    bits[hash1(term)] = 1;
    bits[hash2(term)] = 1;
    bits[hash3(term)] = 1;
}

bool BloomFilter::possiblyContains(const string &term) const
{
    return bits[hash1(term)] && bits[hash2(term)] && bits[hash3(term)];
}

void BloomFilter::clear() { bits.reset(); }
