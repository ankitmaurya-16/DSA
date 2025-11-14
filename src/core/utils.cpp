#include "utils.h"
#include <cstddef>
using namespace std;
Posting::Posting(DocID id) : docId(id) {}
void Posting::addPosition(Position pos) { positions.push_back(pos); }

DocumentInfo::DocumentInfo() : id(0), fileSize(0), termCount(0) {}
DocumentInfo::DocumentInfo(DocID id_, const string &path, size_t size, uint32_t terms, const string &type)
    : id(id_), filepath(path), fileSize(size), termCount(terms), fileType(type)
{
    size_t pos = path.find_last_of("/\\");
    filename = (pos != string::npos) ? path.substr(pos + 1) : path;
}
