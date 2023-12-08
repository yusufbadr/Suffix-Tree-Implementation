#include <cstring>

class SuffixTreeNode{
public:
    int substringStartIndex;
    int suffixIndex;

    SuffixTreeNode *next;
    SuffixTreeNode *children;

    SuffixTreeNode(int substrIndx, int suffixIndx){
        substringStartIndex = substrIndx;
        suffixIndex = suffixIndx;
    }
};


class SuffixTree{
public:
    SuffixTreeNode *root;
    char *string;
    int len;


    int getLenOfSubstr(SuffixTreeNode *node){
        // len is min(children substr start index) - node substr start index
        // if no children len of string - node substr start index
        // a node is leaf is node->children is null
        if (node->children == nullptr){
            return len - node->substringStartIndex;
        } else {
            // else node is internal node, then get min of children node
            SuffixTreeNode *current = node->children;
            int minSubstrStartIndex = current->substringStartIndex;
            while (current!= nullptr){
                if (current->substringStartIndex<minSubstrStartIndex){
                    minSubstrStartIndex = current->substringStartIndex;
                }
                current = current->next;
            }
            return minSubstrStartIndex;
        }
    }


    void insertSuffix(const char *suffix, int suffixIndex){
        // suffix is the same as the edge starting index at this point




    }

    SuffixTree(char *inString){
        string = inString;
        // dummy root node
        root = new SuffixTreeNode(-1, -1);
        strcat(string, "$");
        len = (int) strlen(string);
        for (int i = 0; i < len; i++){
            insertSuffix(string + i, i);
        }
    }


};