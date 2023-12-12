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
        if (head== nullptr){
            head=node;
            return;
        }

        Node *curr = head;
        while (curr->next!= nullptr){
            curr = curr->next;
        }
        curr->next = node;
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

    char *stringQuery;
//    LinkedList<int> results;


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
            while (nodeCurrent != nullptr){
                current = nodeCurrent->data;
                if (current->substringStartIndex<minSubstrStartIndex){
                    minSubstrStartIndex = current->substringStartIndex;
                }
                nodeCurrent=nodeCurrent->next;
            }
            return minSubstrStartIndex-node.substringStartIndex;
        }
    }

    // return the number of matching characters
    int getMatchingPrefix(const char *suffix, SuffixTreeNode suffixTreeNode){
        int lenOfSuffixNode = getLenOfSubstr(suffixTreeNode);
        // i is a counter for the suffixTreeNode substring while j is a counter for
        // current suffix
        int i = suffixTreeNode.substringStartIndex, j = 0;
        while (i<suffixTreeNode.substringStartIndex+lenOfSuffixNode && j<strlen(suffix)){
            if (originalString[i]==suffix[j]){
                i++;
                j++;
            } else {
                break;
            }
        }

        return j;
    }

    void insertSuffixUtil(const char *suffix, int suffixIndex, Node *currentNode, int parentMatch){
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
            insertSuffixUtil(suffix+prefix, suffixIndex, currentNode->data->children.head, prefix+parentMatch);
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
            newChildNode = new Node(new SuffixTreeNode(suffixIndex+prefix+parentMatch, suffixIndex));
            currentNode->data->children.insert(newChildNode);



        } else if (prefix==0 && currentNode->next == nullptr){
            // add to current linked list
            // TODO: both suffix indices should be different, suffixIndex+prevMatching
            Node *newNode = new Node(new SuffixTreeNode(suffixIndex+parentMatch, suffixIndex));
            currentNode->next = newNode;


        } else if (prefix==0 && currentNode->next != nullptr){
            insertSuffixUtil(suffix, suffixIndex, currentNode->next, parentMatch);
        } else {

            // TODO: REMOVE AFTER TESTING
            std::cout << "ELSE BRANCH - ERROR" << std::endl;
        }
    }



    void insertSuffix(const char *suffix, int suffixIndex){
        // suffix is the same as the edge starting index at this point
        insertSuffixUtil(suffix, suffixIndex, root, 0);

    }

    SuffixTree(char *inString){
        originalString = inString;
        // dummy root node
        SuffixTreeNode *suffixRoot = new SuffixTreeNode(-1, -1);
        root = new Node(suffixRoot);
        len = (int) strlen(originalString);
        for (int i = 0; i < len; i++){
            insertSuffix(originalString + i, i);
        }
    }
    /*
     *  if remainingChars == 0: print all nodes where suffix!=-1;
        if curretNode==NULL and rem>0: not found
        if currNode!=null and prefix==n: recursive: children, remChar-prefix;
        if currNode!=null && prefix==0: recursive(currNode.next, remChar)
     * */

    void searchUtil(int i, Node *currentNode){
        if (i==strlen(stringQuery)){
            Node *tempNode = currentNode;
            while(tempNode!=nullptr){
                if (tempNode->data->suffixIndex!=-1){
//                    results.insert(&(tempNode->data->suffixIndex));
                    std::cout << tempNode->data->suffixIndex << " ";

                } else {
                    searchUtil(i, tempNode->data->children.head);
                }
                tempNode=tempNode->next;
            }
        } else if (currentNode== nullptr){
            std::cout << "NOT FOUND ";
        } else /*if (currentNode!= nullptr)*/{
            int matchingPrefix = getMatchingPrefix(stringQuery+i, *(currentNode->data));
            if (matchingPrefix == getLenOfSubstr(*(currentNode->data))){
                if (currentNode->data->children.head== nullptr){
                    std::cout << currentNode->data->suffixIndex << " ";
                } else {
                    searchUtil(i+matchingPrefix, currentNode->data->children.head);
                }
            } else if (matchingPrefix < getLenOfSubstr((*(currentNode->data))) && matchingPrefix>0){
                if (matchingPrefix == strlen(stringQuery+i)){

                    if (currentNode->data->children.head == nullptr){
                        std::cout << currentNode->data->suffixIndex << " ";
                    } else {
                        searchUtil((int)strlen(stringQuery), currentNode->data->children.head);
                    }


                } else {
                    std::cout << "NOT FOUND ";
                }
            }
            else if (matchingPrefix == 0 /*< getLenOfSubstr((*(currentNode->data)))*/){
                searchUtil(i, currentNode->next);
            }
        }


    }

    void Search(char *string){
        stringQuery = string;
        searchUtil(0, root->data->children.head);
        std::cout << std::endl;
    }

    void printUtil(Node *root){
        Node *currNode = root;


        while (currNode!= nullptr){
            std::cout << "(" << currNode->data->substringStartIndex << ", " <<
                      currNode->data->suffixIndex << ") - ";

            int length = getLenOfSubstr(*(currNode->data));
            char *edgeStr = new char[length + 1];
            strncpy(edgeStr, originalString + currNode->data->substringStartIndex, length);
            edgeStr[length] = '\0';
            std::cout << edgeStr << std::endl;

            if (currNode->data->children.head!= nullptr) {
                printUtil(currNode->data->children.head);
            }

            currNode=currNode->next;
        }
    }

    void print(){
        std::cout << "(" << root->data->substringStartIndex << ", " <<
                  root->data->suffixIndex << ")" << std::endl;
        printUtil(root->data->children.head);

    }

};

int main()
{
//    SuffixTree t("bananabanaba$");
//    t.Search("ana"); // Prints: 1 3 7
//    t.Search("naba"); // Prints: 4 8
//    t.Search("banana0"); // NOT FOUND
//    std::cout << std::endl;
//
//
//
//    SuffixTree t1("programming$");
//    t1.Search("gram");  // Prints: 3
//    t1.Search("pro");   // Prints: 0
//    t1.Search("ming");  // Prints: 7
//    std::cout << std::endl;
//
//
//
//    // Case 2
//    SuffixTree t2("abracadabra$");
//    t2.Search("bra");   // Prints: 1 8
//    t2.Search("ada");   // Prints: 5
//    t2.Search("abr");   // Prints: 0 7
//    std::cout << std::endl;
//
//
//    // Case 3
//    SuffixTree t3("mississippi$");
//    t3.Search("ssi");   // Prints: 2 5
//    t3.Search("iss");   // Prints: 1 4
//    t3.Search("ssippi");// Prints: 5
//    std::cout << std::endl;
//
//
//
//    // Case 4
//    SuffixTree t4("algorithmabcbacacca$");
//    t4.Search("rithm"); // Prints: 4
//    t4.Search("algo");  // Prints: 0
//    t4.Search("ca"); // 14 17
//    std::cout << std::endl;
//
//    // Case 5
//    SuffixTree t5("datastructures$");
//    t5.Search("data"); // Prints: 0
//    t5.Search("a");  // Prints: 1 3
//    t5.Search("xyz");
//    std::cout << std::endl;
//
//
//    // Case 6
//    SuffixTree t6("abcdefgh$");
//    t6.Search("def");   // Prints: 3
//    t6.Search("gh");    // Prints: 6
//    t6.Search("abc");   // Prints: 0
//    std::cout << std::endl;
//
//    // Case 7
//    SuffixTree t7("xyxxy$");
//    t7.Search("yx");    // Prints: 1
//    t7.Search("xx");    // Prints: 2
//    t7.Search("x"); // 0 2 3
//    std::cout << std::endl;
//
//    // Case 8
//    SuffixTree t8("example$");
//    t8.Search("mpl");   // Prints: 4
//    t8.Search("exam");  // Prints: 0
//    t8.Search("ple");   // Prints: 6
//    std::cout << std::endl;
//
//    // Case 9
//    SuffixTree t9("testing$");
//    t9.Search("ing");   // Prints: 5
//    t9.Search("test");  // Prints: 0
//    t9.Search("ting");  // Prints: 6
//    std::cout << std::endl;
//
//    // Case 10
//    SuffixTree t10("abcdefgabcdefgabcdefg$");
//    t10.Search("cd");   // Prints: 2
//    t10.Search("ab");   // Prints: 0
//    t10.Search("fg");   // Prints: 5
//    std::cout << std::endl;


    SuffixTree t1("abracadabraabrakadabraabracadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabra$");

    // Test Case 4
    t1.Search("brak"); // Prints: 8 33 58 83
    std::cout << std::endl;

    // Test Case 7
    t1.Search("abrakadabra"); // Prints: 11 36 61
    std::cout << std::endl;


    return 0;
}
