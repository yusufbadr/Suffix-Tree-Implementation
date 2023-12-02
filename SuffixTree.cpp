//
// Created by Yusuf Badr on 02/12/2023.
//


class SuffixTree{
public:
    SuffixTree(const char* text);
    void Search(const char* text);
    ~SuffixTree();

private:

    class Node{
    public:
        Node(int startIndx, int suffixIndx){
            edgeStartIndex = startIndx;
            suffixIndex = suffixIndx;
        }

    private:
        int edgeStartIndex;
        int suffixIndex;

        // Linked List
        class ChildNode{
            Node* data;
            ChildNode* next;
            ChildNode(Node* inData){
                data = inData;
                next = nullptr;
            }
        };
        ChildNode* head;
    };

};


