#include <array>
#include <iostream>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <random>

using namespace std;

const static int N = 4;
const std::array<char, N> Letters = {
        'A', 'T', 'G', 'C'};

struct Nucleotide {
    char symbol;
    size_t position;
    int chromosome_num;
    int gene_num;
    bool is_marked;
    char service_info;
};

struct CompactNucleotide {
    uint32_t position;
    uint32_t gene_num:15;
    uint32_t is_marked:1;
    uint32_t chromosome_num:6;
    uint32_t symbol:2;
    uint32_t service_info:8;
};

CompactNucleotide Compress(const Nucleotide& n) {
    CompactNucleotide result;
    result.position = n.position;
    result.gene_num = n.gene_num;
    result.chromosome_num = n.chromosome_num;
    result.service_info = n.service_info;
    result.is_marked = n.is_marked;
    result.symbol = distance(Letters.begin(), std::find(Letters.begin(), Letters.end(), n.symbol));
    return result;
}

Nucleotide Decompress(const CompactNucleotide& cn) {
    Nucleotide result;
    result.position = cn.position;
    result.gene_num = cn.gene_num;
    result.chromosome_num = cn.chromosome_num;
    result.service_info = cn.service_info;
    result.is_marked = cn.is_marked;
    result.symbol = Letters[cn.symbol];
    return result;
}

static_assert(sizeof(CompactNucleotide) <= 8, "Your CompactNucleotide is not compact enough");
static_assert(alignof(CompactNucleotide) == 4, "Don't use '#pragma pack'!");
bool operator==(const Nucleotide& lhs, const Nucleotide& rhs) {
    return (lhs.symbol == rhs.symbol) && (lhs.position == rhs.position) && (lhs.chromosome_num == rhs.chromosome_num)
           && (lhs.gene_num == rhs.gene_num) && (lhs.is_marked == rhs.is_marked) && (lhs.service_info == rhs.service_info);
}
void TestSize() {
    assert(sizeof(CompactNucleotide) <= 8);
    cout << sizeof(CompactNucleotide);
}
void TestCompressDecompress() {
    Nucleotide source;
    source.symbol = 'T';
    source.position = 1'000'000'000;
    source.chromosome_num = 48;
    source.gene_num = 1'000;
    source.is_marked = true;
    source.service_info = '!';

    CompactNucleotide compressedSource = Compress(source);
    Nucleotide decompressedSource = Decompress(compressedSource);

    assert(source == decompressedSource);
}

//void Test1() {
//    for (int i = 0; i < 10000000; ++i) {
//        // generate vector with random numbers
//        std::random_device rd;
//        std::mt19937 rng(rd());
//        std::uniform_int_distribution<int> symbol(0, 3);
//        std::uniform_int_distribution<long long> position(0, 3'300'000'000);
//        std::uniform_int_distribution<int> chromosome_num(1, 46);
//        std::uniform_int_distribution<int> gene_num(0, 25000);
//        std::uniform_int_distribution<int> is_marked(0, 1);
//        std::uniform_int_distribution<int> service_info(0, 127);
//        Nucleotide source;
//        source.symbol = Letters[symbol(rng)];
//        source.position = position(rng);
//        source.chromosome_num = chromosome_num(rng);
//        source.gene_num = gene_num(rng);
//        source.is_marked = is_marked(rng);
//        source.service_info = service_info(rng);
//
//        CompactNucleotide compressedSource = Compress(source);
//        Nucleotide decompressedSource = Decompress(compressedSource);
//
//        assert(source == decompressedSource);
//    }
//}

int main() {
    TestSize();
    TestCompressDecompress();
    //Test1();
}