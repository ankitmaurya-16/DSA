Search Engine (C++ • BM25 • Inverted Index)
A fast C++ search engine with inverted indexing, BM25 ranking, autocomplete, and support for text/code/PDF/image documents.

Features

Inverted index with term positions
BM25 scoring for ranked search
Boolean AND/OR queries
Phrase search
Trie-based autocomplete
Optional PDF parsing (Poppler)
Optional OCR for images (Tesseract)

Requirements
C++17 compiler
(Optional) Poppler for PDF
(Optional) Tesseract for OCR

Installation
Clone:
git clone https://github.com/yourusername/bm25-search-engine.git
cd bm25-search-engine

Optional dependencies:
macOS
xcode-select --install
brew install poppler tesseract

Ubuntu
sudo apt update
sudo apt install clang

sudo apt install poppler-utils tesseract-ocr

compile manually:
For mac/linux:
clang++ -std=c++17 \
 src/index/dependency_checker.cpp \
 src/index/file_reader.cpp \
 src/index/tokenizer.cpp \
 src/index/bloom_filter.cpp \
 src/core/inverted_index.cpp \
 src/core/utils.cpp \
 src/query/trie.cpp \
 src/query/skiplist.cpp \
 src/query/phrase_search.cpp \
 src/ranking/bm25.cpp \
 src/ranking/min_heap.cpp \
 src/clustering/clustering.cpp \
 src/clustering/disjoint_set.cpp \
 src/clustering/avl_tree.cpp \
 main.cpp \
 -o search_engine

For Windows:
g++ -std=c++17 -m64 -static-libgcc -static-libstdc++ src/index/dependency_checker.cpp src/index/file_reader.cpp src/index/tokenizer.cpp src/index/bloom_filter.cpp src/core/inverted_index.cpp src/core/utils.cpp src/query/trie.cpp src/query/skiplist.cpp src/query/phrase_search.cpp src/ranking/bm25.cpp src/ranking/min_heap.cpp src/clustering/clustering.cpp src/clustering/disjoint_set.cpp src/clustering/avl_tree.cpp main.cpp -o search_engine.exe

Usage
Run:
For mac/liux:
./search_engine

For Windows:
.\search_engine.exe

Steps:
Engine checks for Poppler/Tesseract
You provide directory path
It indexes documents
Choose search operation from menu
Supports:
Term search
Boolean AND/OR
Phrase search
Autocomplete
BM25 Top-K
Multi-term search
Document clustering

Project Structure
src/
core/ Inverted index + utilities
index/ Tokenizer, bloom filter, file reader
query/ Trie, skiplist, phrase search
ranking/ BM25, min-heap
clustering/ Cosine similarity + union-find
main.cpp CLI interface
Makefile

Algorithms Used
Inverted Index
Bloom Filter
Trie
Skiplist
BM25 Ranking
Cosine Similarity + Union-Find

Example (CLI)
Enter directory to index: ./docs
Documents indexed: 42
Unique terms: 1337

1. Search
2. AND Search
3. OR Search
4. Phrase Search
5. Autocomplete
6. Top-K BM25
7. Multi-term BM25
8. Cluster Documents
