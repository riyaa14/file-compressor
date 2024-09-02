#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <map>
#include <queue>

#include "huffman_utility.h"

using namespace std;

map<char, int> getFreqMap(ifstream &sourceFileHeader)
{
    map<char, int> freqMap;

    string line;
    while (getline(sourceFileHeader, line))
    {
        if (line == "EOH")
            break;

        // if (line == "")
        //     continue; // this is kind of a bug, will handle later

        if (line.substr(0, 3) == "EOL")
        {

            freqMap['\n'] = stoi(line.substr(3));
            continue;
        }

        char character = '\0';
        string frequency = "";

        if (isdigit(line[1]))
        {
            if (line[0] != '\0')
                character = line[0];
            frequency = line.substr(1);
        }

        cout << character << " " << frequency << endl;
        // if (frequency == "")
        //     continue;
        freqMap[character] = stoi(frequency); // this is kind of a bug, will handle later
    }

    return freqMap;
}

void decompressFile(ifstream &sourceFile, ofstream &destinationFile, huffmanNode *huffmanTreeRoot)
{
    // read bit by bit not char (whih is 8 bits)
    char ch;
    huffmanNode *currNode = huffmanTreeRoot;
    cout << "decompressing file\n";
    while (sourceFile.get(ch))
    {
        // bits have reverse ordering
        // 1 1 0 1 0 1 0 0 <--- bits as placed in file
        // 7 6 5 4 3 2 1 0 <--- ordering
        cout << ch << " at this character.\n";
        for (int i = 7; i >= 0; i--)
        {
            bool bit = ch & (1 << i); // check if ith bit is set or unset
            cout << bit << " ";
            if (bit)
                currNode = currNode->right;
            else
                currNode = currNode->left;

            if (currNode == NULL)
            {
                cout << "Error: Reached a null node while decompressing";
                return;
            }

            if (currNode->character != '\0')
            {
                cout << currNode->character;
                destinationFile << currNode->character;
                currNode = huffmanTreeRoot;
            }
        }
    }

    return;
}

void printTree(huffmanNode *root)
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

void decompress()
{

    string sourceFileName, destinationFileName;

    cout << "Please enter the name of file to be decompressed: ";
    cin >> sourceFileName;

    cout << "Please give a name for decompressed file: ";
    cin >> destinationFileName;

    ifstream sourceFileHeader(sourceFileName, ios::in | ios::binary);
    map<char, int> freqMap = getFreqMap(sourceFileHeader);
    streampos actualDataPosition = sourceFileHeader.tellg();
    cout << "actualDataPosition: " << actualDataPosition << endl;
    sourceFileHeader.close();

    huffmanNode *huffmanTreeRoot = buildHuffmanTree(freqMap);
    huffmanTreeRoot == NULL ? cout << "root is null" : cout << "root is fine";

    cout << "Printing HuffMan Tree while decompressing\n";
    printTree(huffmanTreeRoot);

    ifstream sourceFile(sourceFileName, ios::in | ios::binary);
    ofstream destinationFile(destinationFileName, std::ios::out | ios::binary | ios::app);

    if (!sourceFile.is_open())
    {
        std::cerr << "Error: Could not open source file " << endl;
        return;
    }

    if (!destinationFile.is_open())
    {
        std::cerr << "Error: Could not open destination file " << endl;
        return;
    }

    sourceFile.seekg(actualDataPosition, ios::beg);
    decompressFile(sourceFile, destinationFile, huffmanTreeRoot);
    sourceFile.close();
    destinationFile.close();
}