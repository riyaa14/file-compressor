#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <map>
#include <queue>

#include "huffman_utility.h"

using namespace std;

map<char, int> buildFreqMap(ifstream &sourceFile)
{

    // build frequency map
    map<char, int> freqMap;
    char character;
    while (sourceFile.get(character))
    {
        if (character != '\0')
            freqMap[character]++;
    }

    return freqMap;
}

void getCodeTable(huffmanNode *huffmanTreeRoot, string code, map<char, string> &codeTable)
{

    if ((huffmanTreeRoot->character) != '\0')
    {
        codeTable[huffmanTreeRoot->character] = code;
        return;
    }

    if (huffmanTreeRoot->left)
        getCodeTable(huffmanTreeRoot->left, code + "0", codeTable);
    if (huffmanTreeRoot->right)
        getCodeTable(huffmanTreeRoot->right, code + "1", codeTable);

    return;
}

void compressFile(ifstream &sourceFileToEncode, ofstream &destinationFile, map<char, string> &codeTable)
{
    char byte = 0;
    int bitsInCurrByte = 0;
    cout << "compressing the file..." << endl;
    char ch;
    while (sourceFileToEncode.get(ch))
    {
        string code = codeTable[ch];

        for (auto b : code)
        {
            if (b == '1')
                byte |= (1 << (7 - bitsInCurrByte));

            bitsInCurrByte++;

            if (bitsInCurrByte == 8)
            {
                destinationFile << byte;
                byte = 0;
                bitsInCurrByte = 0;
            }
        }
    }
}

void printTreeComp(huffmanNode *root)
{
    if (root == NULL)
    {
        return;
    }

    queue<huffmanNode *> q;
    q.push(root);

    while (!q.empty())
    {
        int size = q.size();
        while (size--)
        {
            (q.front()->character == '\0') ? cout << "null" << " " : cout << q.front()->character << " ";
            if (q.front()->left)
                q.push(q.front()->left);
            if (q.front()->right)
                q.push(q.front()->right);
            q.pop();
        }

        cout << endl;
    }
}

void compress()
{
    // read file
    // TODO read file in chunks of 5KB
    string sourceFileName = "sourceSample.txt", destinationFileName = "destinationSample.txt";
    cout << "Please provide the name of file you want to compress: ";
    cin >> sourceFileName;
    cout << "Please give a name for your compressed file: ";
    cin >> destinationFileName;

    ifstream sourceFile(sourceFileName);

    if (!sourceFile.is_open())
    {
        cerr << "Error: Can't open this file" << endl;
        return;
    }

    // build freq map
    map<char, int> freqMap = buildFreqMap(sourceFile);
    sourceFile.close();

    // Tree Implementation
    huffmanNode *huffmanTreeRoot = buildHuffmanTree(freqMap);
    cout << huffmanTreeRoot->character;

    cout << "Printing HuffMan Tree while compressing\n";
    printTreeComp(huffmanTreeRoot);

    // get codes from the tree
    map<char, string> codeTable;
    getCodeTable(huffmanTreeRoot, "", codeTable);
    for (auto c : codeTable)
    {
        cout << c.first << " " << c.second << endl;
    }

    // open a stream for destination file, open in binary and trunc mode, add header to destination file
    ofstream destinationFileHeader(destinationFileName, ios::binary | ios::trunc);
    for (auto elem : freqMap)
    {
        if (elem.first == '\n')
            destinationFileHeader << "EOL" << to_string(elem.second) << endl;
        else
            destinationFileHeader << elem.first << to_string(elem.second) << endl;
    }

    destinationFileHeader << "EOH\n";

    destinationFileHeader.close();

    // Encode the file
    ofstream destinationFile(destinationFileName, std::ios::out | std::ios::binary | std::ios::app);
    ifstream sourceFileToEncode(sourceFileName);

    if (!sourceFileToEncode.is_open())
    {
        cerr << "Error: Can't open this file" << endl;
        return;
    }

    if (!destinationFile.is_open())
    {
        cerr << "Error: Can't open this file" << endl;
        return;
    }

    compressFile(sourceFileToEncode, destinationFile, codeTable);

    sourceFileToEncode.close();
    destinationFile.close();

    return;
}