#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H
#include <vector>
using namespace std;
class DisjointSet
{
private:
    vector<int> parent;
    vector<int> rank;

public:
    DisjointSet(int n);
    int find(int x);
    void unite(int x, int y);
    bool connected(int x, int y);
};

#endif
