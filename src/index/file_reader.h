#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
using namespace std;
class DependencyChecker;

class FileReader
{
private:
    const DependencyChecker *depChecker;
    string readTextFile(const string &filepath);
    string readPDFFile(const string &filepath);
    string readImageFile(const string &filepath);
    string getFileExtension(const string &filepath);

public:
    FileReader();
    void setDependencyChecker(const DependencyChecker *checker);
    bool isSupportedFile(const string &filepath);
    string readUniversalFile(const string &filepath);
};

#endif