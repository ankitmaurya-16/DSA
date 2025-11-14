#ifndef DEPENDENCY_CHECKER_H
#define DEPENDENCY_CHECKER_H

#include <string>
using namespace std;
class DependencyChecker
{
private:
    bool tesseractAvailable;
    bool popplerAvailable;

    bool checkCommand(const string &command);

public:
    DependencyChecker();
    bool isTesseractAvailable() const { return tesseractAvailable; }
    bool isPopplerAvailable() const { return popplerAvailable; }
    bool canProcessImages() const { return tesseractAvailable; }
    bool canProcessPDFs() const { return popplerAvailable; }
    void printStatus() const;
};

#endif