// <cstdlib>, <cstdio>, <cstring>, <iostream>

#include <cstring>

class SuffixNode;

struct Node{
    SuffixNode *data;
    Node *next;
};

class LinkedList{
public:


    Node *head;

    void insert(SuffixNode suffixNode);
    Node* getMatchingNode(const char* text);

};

class SuffixNode{
public:
    SuffixNode(int startIndx, int suffixIndx){
        edgeStartIndex = startIndx;
        suffixIndex = suffixIndx;
    }

    int edgeStartIndex;
    int suffixIndex;

    // Linked List
    LinkedList *childrenHead;

private:
};

class SuffixTree{
public:
    SuffixTree(char* text);
    void Search(const char* text);
    ~SuffixTree();

    SuffixNode *root;
private:


};

SuffixTree::SuffixTree(char *text) {
    root = new SuffixNode(-1, -1);

    strcat(text, "$");
    int len = (int) strlen(text);
    for (int i = 0; i <len ; ++i) {
        char *suffix = new char[len-i];
        for (int j = i; j < len; ++j) {
            suffix[j-i] = text[j];
        }

        // now i have a suffix
        SuffixNode *currentSuffixNode = root;
        while (currentSuffixNode != nullptr){
            LinkedList *currentLinkedListNode = currentSuffixNode->childrenHead;

            while(currentLinkedListNode != nullptr){
                // if matching prefix


                // len is absolute difference(edgeStart of children)

                Node *childHead = currentLinkedListNode->head;
                int minChildEdgeStart;
                if (childHead!= nullptr){
                    minChildEdgeStart = childHead->data->edgeStartIndex;
                    childHead = childHead->next;
                }

                while (childHead!= nullptr){
                    if(childHead->data->edgeStartIndex < minChildEdgeStart){
                        minChildEdgeStart=childHead->data->edgeStartIndex;
                    }
                    childHead = childHead->next;
                }

                int lenSubstr = minChildEdgeStart - currentLinkedListNode->head->data->edgeStartIndex;


                for (int j = currentLinkedListNode->head->data->edgeStartIndex; j < lenSubstr; ++j) {
                    if (text[]);
                }


            }



        }







        delete[] suffix;
    }



}

void SuffixTree::Search(const char *text) {

}


