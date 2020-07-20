/**
 * TODO: file header
 *
 * Author:
 */
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"

/* TODO: add pseudo compression with ascii encoding and naive header
 * (checkpoint) */
void pseudoCompression(const string& inFileName, const string& outFileName) {
    ifstream inFile;
    ofstream outFile;

    vector<unsigned int> freqs(256, 0);

    inFile.open(inFileName, ios::binary);
    outFile.open(outFileName);

    unsigned char nextChar = inFile.get();
    while (nextChar != 255) {  // EOF BIT!!
        freqs[nextChar]++;
        nextChar = inFile.get();
    }

    HCTree* tree = new HCTree();
    tree->build(freqs);

    for (int i = 0; i < 256; i++) {
        outFile << freqs[i] << "\n";
    }

    inFile.close();
    inFile.open(inFileName, ios::binary);
    nextChar = inFile.get();
    // cout << "after nextChar" << endl;
    while (nextChar != 255) {  // EOF BIT!!
        tree->encode(nextChar, outFile);
        // cout << "after encode" << endl;
        nextChar = inFile.get();
    }

    inFile.close();
    outFile.close();
}

/* TODO: True compression with bitwise i/o and small header (final) */
void trueCompression(const string& inFileName, const string& outFileName) {}

/* Main program that runs the compression */
int main(int argc, char* argv[]) {
    cxxopts::Options options(argv[0],
                             "Compresses files using Huffman Encoding");
    options.positional_help("./path_to_input_file ./path_to_output_file");

    bool isAsciiOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Write output in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAsciiOutput))(
        "input", "", cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        return 0;
    }

    // if original file is empty, output empty file
    if (FileUtils::isEmptyFile(inFileName)) {
        ofstream outFile;
        outFile.open(outFileName, ios::out);
        outFile.close();
        return 0;
    }

    if (isAsciiOutput) {
        pseudoCompression(inFileName, outFileName);
    } else {
        trueCompression(inFileName, outFileName);
    }

    return 0;
}