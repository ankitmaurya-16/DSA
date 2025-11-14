#ifndef CLUSTERING_H
#define CLUSTERING_H

#include "../core/utils.h"
#include "../core/inverted_index.h"
#include "disjoint_set.h"
#include <map>
#include <unordered_map>
using namespace std;

class Clustering;

class Clustering
{
private:
    DisjointSet *clusters;
    const unordered_map<string, vector<Posting>> *index;
    const map<DocID, DocumentInfo> *documents;
    double computeCosineSimilarity(DocID d1, DocID d2);

public:
    Clustering();
    ~Clustering();
    void setIndex(const unordered_map<string, vector<Posting>> &idx,
                  const map<DocID, DocumentInfo> &docs);
    void clusterDocuments(double similarityThreshold = 0.3);
};

#endif
