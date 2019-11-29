#include <leveldb/db.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <vector>

#define EMBEDDING_BASE_NAME (std::string("Tencent_AILab_ChineseEmbedding"))
#define RESOURCE(relpath) (std::string("D:\\Jaysinco\\Cxx\\product\\word2vec\\resources\\")+relpath)

leveldb::DB* db = nullptr;

void setup(int stopLineNo=-1) {
    if (stopLineNo == 0)
        return;
    // create leveldb
    assert(db == nullptr);
    std::cout << "[INFO] setup leveldb..." << std::endl;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, RESOURCE(EMBEDDING_BASE_NAME), &db);
    if (!status.ok()) {
        std::cerr << "[ERROR] failed to open leveldb: " << status.ToString() << std::endl;
        exit(-1);
    }
    // read text file
    std::ifstream infile(RESOURCE(EMBEDDING_BASE_NAME+".txt"));
    std::string line;
    int totalWord = 0, dimension = 0;
    std::getline(infile, line);
    std::istringstream ss(line);
    ss >> totalWord >> dimension;
    std::cout << "[INFO] totalWord=" << totalWord << ", dimension=" << dimension << std::endl;
    if (stopLineNo > 0)
        totalWord = stopLineNo;
    for (int i = 1; i <= totalWord; ++i) {
        if (!std::getline(infile, line)) {
            std::cerr << "[ERROR] failed to read next line=" << i << std::endl;
            exit(-1);
        }
        size_t firstSpaceIndex = line.find_first_of(' ');
        std::string word = line.substr(0, firstSpaceIndex);
        std::istringstream ss(line.substr(firstSpaceIndex+1));
        float dim = 0;
        std::vector<float> vec;
        while (ss >> dim) {
            vec.push_back(dim);
        }
        if (vec.size() != dimension) {
            std::cerr << "[ERROR] invalid dimension size readed for word=\"" << word
                << "\", size=" << vec.size() << ", line=" << i << std::endl;
            exit(-1);
        }
        // write leveldb
        status = db->Put(leveldb::WriteOptions(), word,
            leveldb::Slice((char*)vec.data(), vec.size()*sizeof(float)));
        std::cout << "\r[INFO] line=" << i << std::flush;
    }
    std::cout << "\n[INFO] setup done!" << std::endl;
}

void open() {
    assert(db == nullptr);
    std::cout << "[INFO] open leveldb..." << std::endl;
    leveldb::Status status = leveldb::DB::Open(leveldb::Options(), RESOURCE(EMBEDDING_BASE_NAME), &db);
    if (!status.ok()) {
        std::cerr << "[ERROR] failed to open leveldb: " << status.ToString() << std::endl;
        exit(-1);
    }
}

bool lookup(const std::string &word, std::vector<float> &vec) {
    if (db == nullptr) open();
    std::string value;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), word, &value);
    if (!status.ok()) {
        return false;
    }
    size_t size = value.size() / sizeof(float);
    float *begin = (float*)value.data();
    float *end = begin + size;
    vec = std::vector<float>(begin, end);
    return true;
}

void peek(int n=5) {
    if (db == nullptr) open();
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    int i = 0;
    for (it->Seek(u8"此去经年"); it->Valid() && i++ < n; it->Next()) {
        std::cout << "[INFO] sample" << i << "=" << it->key().ToString() << std::endl;
    }
}

int main() {
    setup(1);
    std::vector<float> vec;
    if (lookup(u8".", vec)) {
        std::cout << "[INFO] lookup_dimension=" << vec.size() << std::endl;
    }
    peek();
    if (db != nullptr) {
        std::cout << "[INFO] leveldb closed!" << std::endl;
        delete db;
    }
}