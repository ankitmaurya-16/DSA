#include "file_reader.h"
#include "dependency_checker.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <cstdlib>

using namespace std;
namespace fs = filesystem;

FileReader::FileReader() : depChecker(nullptr) {}

void FileReader::setDependencyChecker(const DependencyChecker *checker)
{
    depChecker = checker;
}

string FileReader::readTextFile(const string &filepath)
{
    ifstream file(filepath);
    if (!file.is_open())
        return "";
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

string FileReader::readPDFFile(const string &filepath)
{
    if (!depChecker || !depChecker->canProcessPDFs())
    {
        cerr << "Warning: Skipping PDF file (Poppler not available): " << filepath << endl;
        return "";
    }

    string tempOutput = "/tmp/pdf_text.txt";
#ifdef _WIN32
    tempOutput = "pdf_text_temp.txt";
#endif

    string command = "pdftotext \"" + filepath + "\" \"" + tempOutput + "\"";
    int result = system(command.c_str());
    if (result != 0)
        return "";

    string content = readTextFile(tempOutput);

#ifdef _WIN32
    remove(tempOutput.c_str());
#endif

    return content;
}

string FileReader::readImageFile(const string &filepath)
{
    if (!depChecker || !depChecker->canProcessImages())
    {
        cerr << "Warning: Skipping image file (Tesseract not available): " << filepath << endl;
        return "";
    }

    string tempOutput = "/tmp/img_text";
#ifdef _WIN32
    tempOutput = "img_text_temp";
#endif

    string command = "tesseract \"" + filepath + "\" \"" + tempOutput + "\" > ";
#ifdef _WIN32
    command += "NUL 2>&1";
#else
    command += "/dev/null 2>&1";
#endif

    int result = system(command.c_str());
    if (result != 0)
        return "";

    string content = readTextFile(tempOutput + ".txt");

#ifdef _WIN32
    remove((tempOutput + ".txt").c_str());
#endif

    return content;
}

string FileReader::getFileExtension(const string &filepath)
{
    size_t pos = filepath.find_last_of('.');
    if (pos != string::npos)
    {
        string ext = filepath.substr(pos + 1);
        transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        return ext;
    }
    return "";
}

bool FileReader::isSupportedFile(const string &filepath)
{
    string ext = getFileExtension(filepath);

    if (ext == "txt" || ext == "md" || ext == "log" || ext == "cpp" || ext == "h" ||
        ext == "java" || ext == "py" || ext == "js" || ext == "html" || ext == "css" ||
        ext == "json" || ext == "xml")
        return true;
    if (ext == "pdf")
        return depChecker && depChecker->canProcessPDFs();

    if (ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "bmp")
        return depChecker && depChecker->canProcessImages();

    return false;
}

string FileReader::readUniversalFile(const string &filepath)
{
    string ext = getFileExtension(filepath);

    if (ext == "pdf")
        return readPDFFile(filepath);
    else if (ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "bmp")
        return readImageFile(filepath);
    else
        return readTextFile(filepath);
}