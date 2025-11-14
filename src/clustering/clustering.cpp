#include "clustering.h"
#include <cmath>
#include <iostream>
using namespace std;

Clustering::Clustering() : clusters(nullptr), index(nullptr), documents(nullptr) {}
Clustering::~Clustering() { delete clusters; }

void Clustering::setIndex(const unordered_map<string, vector<Posting>> &idx,
                          const map<DocID, DocumentInfo> &docs)
{
    index = &idx;
    documents = &docs;
}

double Clustering::computeCosineSimilarity(DocID d1, DocID d2)
{
    if (!index)
        return 0.0;
    unordered_map<string, int> v1, v2;
    for (const auto &[term, posts] : *index)
    {
        for (const auto &p : posts)
        {
            if (p.docId == d1)
                v1[term] = p.positions.size();
            if (p.docId == d2)
                v2[term] = p.positions.size();
        }
    }
    double dot = 0, mag1 = 0, mag2 = 0;
    for (const auto &[t, f1] : v1)
    {
        if (v2.count(t))
            dot += f1 * v2[t];
        mag1 += f1 * f1;
    }
    for (const auto &[t, f2] : v2)
        mag2 += f2 * f2;
    if (mag1 == 0 || mag2 == 0)
        return 0.0;
    return dot / (sqrt(mag1) * sqrt(mag2));
}

void Clustering::clusterDocuments(double threshold)
{
    if (!documents)
        return;
    size_t n = documents->size();
    delete clusters;
    clusters = new DisjointSet(n);
    for (auto it1 = documents->begin(); it1 != documents->end(); ++it1)
    {
        for (auto it2 = next(it1); it2 != documents->end(); ++it2)
        {
            double sim = computeCosineSimilarity(it1->first, it2->first);
            if (sim > threshold)
                clusters->unite(it1->first, it2->first);
        }
    }
}
