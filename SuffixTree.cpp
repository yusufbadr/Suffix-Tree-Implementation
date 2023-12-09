#include <cstring>
#include <iostream>


class SuffixTreeNode;

class Node{
public:
    SuffixTreeNode *data;
    Node *next;

    explicit Node(SuffixTreeNode *suffixTreeNode){
        data = suffixTreeNode;
        next = nullptr;
    }
};

class LinkedList{
public:
    Node *head;
    // insert

    LinkedList(){
        head = nullptr;
    }

    void insert(Node *node){
        node->next=head;
        head = node;
    }
};

class SuffixTreeNode{
public:
    int substringStartIndex;
    int suffixIndex;

    LinkedList children;

    SuffixTreeNode(int substrIndx, int suffixIndx){
        substringStartIndex = substrIndx;
        suffixIndex = suffixIndx;
    }
};


class SuffixTree{
public:
    Node *root;
    char *originalString;
    // banana
    int len;


    int getLenOfSubstr(SuffixTreeNode node){
        // len is min(children substr start index) - node substr start index
        // if no children len of originalString - node substr start index
        // a node is a leaf if node->children is null
        if (node.children.head == nullptr){
            return len - node.substringStartIndex;
        } else {
            // else node is internal node, then get min of children node
            Node *nodeCurrent = node.children.head;
            SuffixTreeNode *current = nodeCurrent->data;
            int minSubstrStartIndex = current->substringStartIndex;
            while (nodeCurrent->next != nullptr){
                if (current->substringStartIndex<minSubstrStartIndex){
                    minSubstrStartIndex = current->substringStartIndex;
                }
                nodeCurrent=nodeCurrent->next;
                current = nodeCurrent->data;
            }
            return minSubstrStartIndex;
        }
    }

    // return the number of matching characters
    int getMatchingPrefix(const char *suffix, SuffixTreeNode suffixTreeNode){
        int lenOfSuffixNode = getLenOfSubstr(suffixTreeNode);
        // i is a counter for the suffixTreeNode substring while j is a counter for
        // current suffix
        int i = suffixTreeNode.substringStartIndex, j = 0;
        while (i<suffixTreeNode.substringStartIndex+lenOfSuffixNode, j<strlen(suffix)){
            if (originalString[i]==suffix[j]){
                i++;
                j++;
            } else {
                break;
            }
        }

        return j;
    }

    void insertSuffixUtil(const char *suffix, int suffixIndex, Node *currentNode){
        // handling the case when the root is empty

        if (currentNode==root && currentNode->data->children.head == nullptr){
            currentNode->data->children.insert(new Node(new SuffixTreeNode(suffixIndex, suffixIndex)));
            return;
        }

        // assert(currentNode)!=nullptr AND root has atleast one child

        // if root descend one level to root->children
        if (currentNode==root && currentNode->data->substringStartIndex == -1 && currentNode->data->suffixIndex == -1){
            currentNode=currentNode->data->children.head;
        }

        int prefix = getMatchingPrefix(suffix, *(currentNode->data));

        if (prefix == getLenOfSubstr(*(currentNode->data))){
            insertSuffixUtil(suffix+prefix, suffixIndex, currentNode->data->children.head);
        } else if (prefix > 0){
            // split the current node


            // dealing with the rest of the current suffix tree node substring
            // if current node to be split have children, suffixIndex of newNode will be -1
            // else will take suffixIndex

            Node *newChildNode;

            if (currentNode->data->children.head == nullptr){
                newChildNode = new Node(new SuffixTreeNode(currentNode->data->substringStartIndex+prefix,
                                                            currentNode->data->suffixIndex));

            } else {
                newChildNode = new Node(new SuffixTreeNode(currentNode->data->substringStartIndex+prefix,
                                                           -1));
            }


            currentNode->data->suffixIndex = -1;
            newChildNode->data->children = currentNode->data->children;
            currentNode->data->children.head = newChildNode;



            // dealing with the rest of the current suffix query (string to be inserted)
            newChildNode = new Node(new SuffixTreeNode(suffixIndex+prefix, suffixIndex));
            currentNode->data->children.insert(newChildNode);



        } else if (prefix==0 && currentNode->next == nullptr){
            // add to current linked list
            Node *newNode = new Node(new SuffixTreeNode(suffixIndex, suffixIndex));
            currentNode->next = newNode;


        } else if (prefix==0 && currentNode->next != nullptr){
            insertSuffixUtil(suffix, suffixIndex, currentNode->next);
        } else {

            // TODO: REMOVE AFTER TESTING
            std::cout << "ELSE BRANCH - ERROR" << std::endl;
        }
    }



    void insertSuffix(const char *suffix, int suffixIndex){
        // suffix is the same as the edge starting index at this point
        insertSuffixUtil(suffix, suffixIndex, root);

    }

    SuffixTree(char *inString){
        originalString = inString;
        // dummy root node
        SuffixTreeNode *suffixRoot = new SuffixTreeNode(-1, -1);
        root = new Node(suffixRoot);
        strcat(originalString, "$");
        len = (int) strlen(originalString);
        for (int i = 0; i < len; i++){
            insertSuffix(originalString + i, i);
        }
    }


};


int main(){
//    char string[] = "banana";
    char string[] = "acacba";
    SuffixTree suffixTree(string);


    return 0;
}