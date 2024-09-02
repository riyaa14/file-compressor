#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <queue>
#include "huffman_utility.h"

huffmanNode *buildHuffmanTree(map<char, int> &freqMap)
{
    priority_queue<huffmanNode *, vector<huffmanNode *>, bool (*)(huffmanNode *, huffmanNode *)> maxHeap(huffmanNode::compareHuffmanNodes);

    // add all elements of freqMap to priority queue as huffmanNodes
    for (pair<char, int> element : freqMap)
    {

        char character = element.first;
        int frequency = element.second;

        huffmanNode *node = new huffmanNode(character, frequency);
        maxHeap.push(node);
    }

    while (maxHeap.size() > 1)
    {
        huffmanNode *rightNode = maxHeap.top();
        maxHeap.pop();

        huffmanNode *leftNode = maxHeap.top();
        maxHeap.pop();

        int totalFreq = leftNode->frequency + rightNode->frequency;
        huffmanNode *node = new huffmanNode('\0', totalFreq);
        node->left = leftNode;
        node->right = rightNode;

        maxHeap.push(node);
    }

    return maxHeap.top();
}