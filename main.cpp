#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include "src/core/inverted_index.h"

using namespace std;

void printHeader()
{
    cout << "============================================================" << endl;
    cout << "       ADVANCED SEARCH ENGINE - BM25 & AUTOCOMPLETE        " << endl;
    cout << "============================================================" << endl;
}

void printMenu()
{
    cout << "\n============================================================" << endl;
    cout << "                       SEARCH MENU                          " << endl;
    cout << "============================================================" << endl;
    cout << "1. Simple search" << endl;
    cout << "2. Boolean AND search" << endl;
    cout << "3. Boolean OR search" << endl;
    cout << "4. Phrase search" << endl;
    cout << "5. Autocomplete" << endl;
    cout << "6. Top-K BM25 search" << endl;
    cout << "7. Multi-term BM25 search" << endl;
    cout << "8. Cluster documents" << endl;
    cout << "0. Exit" << endl;
    cout << "\nChoice: ";
}

int main()
{
    printHeader();

    InvertedIndex index;

    index.checkDependencies();

    cout << "\nEnter directory path to index: ";
    string dirPath;
    getline(cin, dirPath);
    if (dirPath.empty())
        dirPath = ".";

    cout << "\nIndexing files in: " << dirPath << endl;
    cout << "Please wait...\n"
         << endl;

    index.crawlDirectory(dirPath);
    index.printStatistics();

    while (true)
    {
        printMenu();

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 0)
            break;

        if (choice == 1)
        {
            cout << "\nEnter term: ";
            string term;
            getline(cin, term);
            transform(term.begin(), term.end(), term.begin(), ::tolower);

            auto results = index.search(term);
            cout << "\nResults: " << results.size() << " documents found" << endl;

            for (auto id : results)
            {
                const auto *doc = index.getDocument(id);
                if (doc)
                    cout << "  - " << doc->filename << endl;
            }
        }
        else if (choice == 2)
        {
            cout << "\nEnter terms (space-separated): ";
            string line;
            getline(cin, line);
            stringstream ss(line);
            vector<string> terms;
            string t;
            while (ss >> t)
            {
                transform(t.begin(), t.end(), t.begin(), ::tolower);
                terms.push_back(t);
            }

            auto results = index.searchAnd(terms);
            cout << "\nDocuments containing ALL terms: " << results.size() << endl;

            for (auto id : results)
            {
                const auto *doc = index.getDocument(id);
                if (doc)
                    cout << "  - " << doc->filename << endl;
            }
        }
        else if (choice == 3)
        {
            cout << "\nEnter terms (space-separated): ";
            string line;
            getline(cin, line);
            stringstream ss(line);
            vector<string> terms;
            string t;
            while (ss >> t)
            {
                transform(t.begin(), t.end(), t.begin(), ::tolower);
                terms.push_back(t);
            }

            auto results = index.searchOr(terms);
            cout << "\nDocuments containing ANY term: " << results.size() << endl;

            for (auto id : results)
            {
                const auto *doc = index.getDocument(id);
                if (doc)
                    cout << "  - " << doc->filename << endl;
            }
        }
        else if (choice == 4)
        {
            cout << "\nEnter phrase: ";
            string line;
            getline(cin, line);
            stringstream ss(line);
            vector<string> terms;
            string t;
            while (ss >> t)
            {
                transform(t.begin(), t.end(), t.begin(), ::tolower);
                terms.push_back(t);
            }

            auto results = index.searchPhrase(terms);
            cout << "\nDocuments containing exact phrase: " << results.size() << endl;

            for (auto id : results)
            {
                const auto *doc = index.getDocument(id);
                if (doc)
                    cout << "  - " << doc->filename << endl;
            }
        }
        else if (choice == 5)
        {
            cout << "\nEnter prefix: ";
            string prefix;
            getline(cin, prefix);
            transform(prefix.begin(), prefix.end(), prefix.begin(), ::tolower);

            auto res = index.autocomplete(prefix);

            if (res.empty())
            {
                cout << "\nNo suggestions found." << endl;
            }
            else
            {
                cout << "\nSuggestions:" << endl;
                for (auto &w : res)
                    cout << "  - " << w << endl;
            }
        }
        else if (choice == 6)
        {
            cout << "\nEnter term: ";
            string term;
            getline(cin, term);
            transform(term.begin(), term.end(), term.begin(), ::tolower);

            cout << "Enter K (number of results): ";
            int k;
            cin >> k;
            cin.ignore();

            auto results = index.topK(term, k);

            if (results.empty())
            {
                cout << "\nNo results found." << endl;
            }
            else
            {
                cout << "\nTop " << results.size() << " results:" << endl;
                for (size_t i = 0; i < results.size(); ++i)
                {
                    const auto *doc = index.getDocument(results[i].first);
                    if (doc)
                        cout << "  " << (i + 1) << ". " << doc->filename
                             << " (Score: " << results[i].second << ")" << endl;
                }
            }
        }
        else if (choice == 7)
        {
            cout << "\nEnter query terms (space-separated): ";
            string line;
            getline(cin, line);
            stringstream ss(line);
            vector<string> terms;
            string t;
            while (ss >> t)
            {
                transform(t.begin(), t.end(), t.begin(), ::tolower);
                terms.push_back(t);
            }

            cout << "Enter K (number of results): ";
            int k;
            cin >> k;
            cin.ignore();

            auto results = index.multiTerm(terms, k);

            if (results.empty())
            {
                cout << "\nNo results found." << endl;
            }
            else
            {
                cout << "\nTop " << results.size() << " results:" << endl;
                for (size_t i = 0; i < results.size(); ++i)
                {
                    const auto *doc = index.getDocument(results[i].first);
                    if (doc)
                        cout << "  " << (i + 1) << ". " << doc->filename
                             << " (Score: " << results[i].second << ")" << endl;
                }
            }
        }
        else if (choice == 8)
        {
            cout << "\nEnter similarity threshold (0.0 to 1.0): ";
            double threshold;
            cin >> threshold;
            cin.ignore();

            cout << "\nClustering documents with threshold " << threshold << "..." << endl;
            index.cluster(threshold);
            cout << "Clustering complete!" << endl;
        }
        else
        {
            cout << "\nInvalid choice. Please try again." << endl;
        }
    }

    cout << "\n============================================================" << endl;
    cout << "                     SESSION ENDED                          " << endl;
    cout << "============================================================" << endl;
    cout << "\nThank you for using the search engine!" << endl;

    return 0;
}