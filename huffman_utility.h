#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

// Tree Nodes Class for Huffman Tree
class huffmanNode
{
public:
    char character;
    int frequency;
    huffmanNode *left;
    huffmanNode *right;

    huffmanNode(char ch, int freq)
    {
        character = ch;
        frequency = freq;
        left = nullptr;
        right = nullptr;
    }

    static bool compareHuffmanNodes(huffmanNode *a, huffmanNode *b)
    {
        if (a->frequency == b->frequency)
            return a->character > b->character;
        return a->frequency > b->frequency;
    }
};

extern huffmanNode *buildHuffmanTree(map<char, int> &freqMap);
