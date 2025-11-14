#include "dependency_checker.h"
#include <iostream>
#include <cstdlib>
using namespace std;
#ifdef _WIN32
#define NULL_DEVICE "NUL"
#else
#define NULL_DEVICE "/dev/null"
#endif

DependencyChecker::DependencyChecker()
    : tesseractAvailable(false), popplerAvailable(false)
{
    tesseractAvailable = checkCommand("tesseract --version");

    popplerAvailable = checkCommand("pdftotext -v");
}

bool DependencyChecker::checkCommand(const string &command)
{
    string fullCommand = command + " > " + NULL_DEVICE + " 2>&1";
    int result = system(fullCommand.c_str());

#ifdef _WIN32
    return (result == 0);
#else
    return (result == 0 || result == 256);
#endif
}

void DependencyChecker::printStatus() const
{
    cout << "\n============================================================" << endl;
    cout << "              DEPENDENCY CHECK RESULTS                      " << endl;
    cout << "============================================================" << endl;

    cout << "\nTesseract OCR: "
         << (tesseractAvailable ? "[OK] Installed" : "[X] Not Found") << endl;
    cout << "Poppler (PDF): "
         << (popplerAvailable ? "[OK] Installed" : "[X] Not Found") << endl;

    cout << "\n------------------------------------------------------------" << endl;
    cout << "Supported file types:" << endl;
    cout << "  * Text files (.txt, .md, .log, etc.) - Always supported" << endl;
    cout << "  * Code files (.cpp, .h, .py, .js, etc.) - Always supported" << endl;

    if (popplerAvailable)
        cout << "  * PDF files (.pdf) - [OK] Supported" << endl;
    else
        cout << "  * PDF files (.pdf) - [X] Requires Poppler" << endl;

    if (tesseractAvailable)
        cout << "  * Image files (.png, .jpg, .jpeg, .bmp) - [OK] Supported" << endl;
    else
        cout << "  * Image files (.png, .jpg, .jpeg, .bmp) - [X] Requires Tesseract" << endl;

    if (!tesseractAvailable || !popplerAvailable)
    {
        cout << "\n------------------------------------------------------------" << endl;
        cout << "Installation Instructions:" << endl;

        if (!popplerAvailable)
        {
            cout << "\nPoppler (for PDF support):" << endl;
#ifdef _WIN32
            cout << "  Windows: Download from https://github.com/oschwartz10612/poppler-windows/releases" << endl;
            cout << "           Extract and add bin folder to PATH" << endl;
#elif __APPLE__
            cout << "  macOS:   brew install poppler" << endl;
#else
            cout << "  Linux:   sudo apt-get install poppler-utils" << endl;
            cout << "           or: sudo yum install poppler-utils" << endl;
#endif
        }

        if (!tesseractAvailable)
        {
            cout << "\nTesseract (for image OCR support):" << endl;
#ifdef _WIN32
            cout << "  Windows: Download from https://github.com/UB-Mannheim/tesseract/wiki" << endl;
            cout << "           Install and add to PATH" << endl;
#elif __APPLE__
            cout << "  macOS:   brew install tesseract" << endl;
#else
            cout << "  Linux:   sudo apt-get install tesseract-ocr" << endl;
            cout << "           or: sudo yum install tesseract" << endl;
#endif
        }
    }
    cout << "------------------------------------------------------------\n"
         << endl;
}