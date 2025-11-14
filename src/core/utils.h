#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <cstdint>
using namespace std;
using DocID = uint32_t;
using Position = uint32_t;

struct Posting
{
    DocID docId;
    vector<Position> positions;
    Posting(DocID id);
    void addPosition(Position pos);
};

struct DocumentInfo
{
    DocID id;
    string filepath;
    string filename;
    size_t fileSize;
    uint32_t termCount;
    string fileType;
    DocumentInfo();
    DocumentInfo(DocID id, const string &path, size_t size, uint32_t terms, const string &type);
};

#endif
