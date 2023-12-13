#include <cstring>
#include <iostream>

class SuffixTreeNode;

class Node
{
public:
    SuffixTreeNode *data;
    Node *next;

    // Constructor initializing Node with a SuffixTreeNode
    explicit Node(SuffixTreeNode *suffixTreeNode)
    {
        data = suffixTreeNode;
        next = nullptr;
    }
};

// LinkedList class representing a linked list of SuffixTreeNodes
class LinkedList
{
public:
    Node *head;

    // Constructor initializing an empty linked list
    LinkedList()
    {
        head = nullptr;
    }

    void insert(Node *node)
    {
        if (head == nullptr)
        {
            head = node;
            return;
        }

        Node *curr = head;
        while (curr->next != nullptr)
        {
            curr = curr->next;
        }
        curr->next = node;
    }
};


// SuffixTreeNode class representing a node in the suffix tree
class SuffixTreeNode
{
public:
    int substringStartIndex; // Starting index of the substring represented by the node
    int suffixIndex; // Index of the suffix (leaf nodes have a valid suffix index otherwise -1)

    LinkedList children; // Linked list of child nodes

    // Constructor initializing SuffixTreeNode
    SuffixTreeNode(int substrIndx, int suffixIndx)
    {
        substringStartIndex = substrIndx;
        suffixIndex = suffixIndx;
    }
};


class SuffixTree
{
public:
    Node *root; // Pointer to root
    char *originalString; // Pointer to the original string
    int len; // Length of the original string
    char *stringQuery; // String to be queried for search

    // Method to get the length of the substring represented by a SuffixTreeNode
    int getLenOfSubstr(SuffixTreeNode node)
    {
        // len is min(children substr start index) - node substr start index
        // if no children THEN len of originalString - node substr start index
        // a node is a leaf if node->children is null

        // Check if the node is a leaf (no children)
        if (node.children.head == nullptr)
        {
            return len - node.substringStartIndex;
        } else
        {
            // If it's an internal node, find the minimum substring start index among its children

            // Initialize variables to traverse the children linked list
            Node *nodeCurrent = node.children.head;
            SuffixTreeNode *current = nodeCurrent->data;

            // Initialize the minimum substring start index with the first child's value
            int minSubstrStartIndex = current->substringStartIndex;

            // Traverse the children linked list to find the minimum substring start index
            while (nodeCurrent != nullptr)
            {
                current = nodeCurrent->data;

                // Update the minimum substring start index if a smaller value is found
                if (current->substringStartIndex < minSubstrStartIndex)
                {
                    minSubstrStartIndex = current->substringStartIndex;
                }

                // Move to the next node in the linked list
                nodeCurrent = nodeCurrent->next;
            }

            // Calculate and return the length of the substring represented by the internal node
            return minSubstrStartIndex - node.substringStartIndex;
        }
    }

    // Return the number of matching characters between a given suffix and the substring represented by a SuffixTreeNode
    int getMatchingPrefix(const char *suffix, SuffixTreeNode suffixTreeNode)
    {
        // Get the length of the substring represented by the SuffixTreeNode
        int lenOfSuffixNode = getLenOfSubstr(suffixTreeNode);

        // Initialize counters for traversing the substring of the SuffixTreeNode and the given suffix
        int i = suffixTreeNode.substringStartIndex, j = 0;

        // Iterate while there are characters remaining in both the substring and the given suffix
        while (i < suffixTreeNode.substringStartIndex + lenOfSuffixNode && j < strlen(suffix))
        {

            // Compare characters at the current positions in the substring and the given suffix
            if (originalString[i] == suffix[j])
            {
                // If characters match, increment both counters
                i++;
                j++;
            } else
            {
                // If characters do not match, break out of the loop
                break;
            }
        }

        // Return the number of matching characters
        return j;
    }


    // Utility method for inserting a suffix into the suffix tree
    void insertSuffixUtil(const char *suffix, int suffixIndex, Node *currentNode, int parentMatch)
    {

        // Handling the case when the root is empty
        if (currentNode == root && currentNode->data->children.head == nullptr)
        {
            currentNode->data->children.insert(new Node(new SuffixTreeNode(suffixIndex, suffixIndex)));
            return;
        }

        // Assert that currentNode is not nullptr and the root has at least one child

        // If the root is the current node, descend one level to root->children
        if (currentNode == root && currentNode->data->substringStartIndex == -1 && currentNode->data->suffixIndex == -1)
        {
            currentNode = currentNode->data->children.head;
        }

        // Calculate the matching prefix between the current suffix and the substring represented by the current node
        int prefix = getMatchingPrefix(suffix, *(currentNode->data));

        if (prefix == getLenOfSubstr(*(currentNode->data)))
        {
            // If the entire suffix matches the current node's substring, recursively insert into its children
            insertSuffixUtil(suffix + prefix, suffixIndex, currentNode->data->children.head, prefix + parentMatch);
        } else if (prefix > 0)
        {
            // If there is a partial match, split the current nod

            // Dealing with the rest of the current suffix tree node substring

            // Create a new node to represent the rest of the current node's substring
            Node *newChildNode;

            // if current node to be split have children, suffixIndex of newNode will be -1
            // else will take current node's suffixIndex
            if (currentNode->data->children.head == nullptr)
            {
                newChildNode = new Node(new SuffixTreeNode(currentNode->data->substringStartIndex + prefix,
                                                           currentNode->data->suffixIndex));
            } else
            {
                newChildNode = new Node(new SuffixTreeNode(currentNode->data->substringStartIndex + prefix,
                                                           -1));
            }

            // Updating nodes data in accordance to the splitting process
            currentNode->data->suffixIndex = -1;
            newChildNode->data->children = currentNode->data->children;
            currentNode->data->children.head = newChildNode;

            // Create a new node for the remaining part of the current suffix and insert it into the children
            newChildNode = new Node(new SuffixTreeNode(suffixIndex + prefix + parentMatch, suffixIndex));
            currentNode->data->children.insert(newChildNode);

        } else if (prefix == 0 && currentNode->next == nullptr)
        {
            // If there is no match and no next node, add the new node to the current linked list
            Node *newNode = new Node(new SuffixTreeNode(suffixIndex + parentMatch, suffixIndex));
            currentNode->next = newNode;

        } else if (prefix == 0 && currentNode->next != nullptr)
        {
            // If there is no match and there is a next node, recursively insert into the next node
            insertSuffixUtil(suffix, suffixIndex, currentNode->next, parentMatch);
        }
    }

    void insertSuffix(const char *suffix, int suffixIndex)
    {
        // Suffix is the same as the edge starting index at this point
        insertSuffixUtil(suffix, suffixIndex, root, 0);
    }

    // Constructor for the SuffixTree class, initializing the tree with the input string
    SuffixTree(char *inString)
    {
        originalString = inString;
        // dummy root node
        SuffixTreeNode *suffixRoot = new SuffixTreeNode(-1, -1);
        root = new Node(suffixRoot);

        // Calculate the length of the original string
        len = (int) strlen(originalString);

        // Iterate through each suffix of the original string and insert it into the suffix tree
        for (int i = 0; i < len; i++)
        {
            insertSuffix(originalString + i, i);
        }
    }

    // Utility method for searching the suffix tree with a query (sub)string

    void searchUtil(int i, Node *currentNode)
    {
        // Check if the end of the query string has been reached
        if (i == strlen(stringQuery))
        {
            Node *tempNode = currentNode;

            // Iterate through nodes of subtree
            while (tempNode != nullptr)
            {
                // If the node represents a valid suffix (leaf) then print its suffix index
                if (tempNode->data->suffixIndex != -1)
                {
                    std::cout << tempNode->data->suffixIndex << " ";
                } else
                {
                    // Recursively search in the children of the current node for leaves
                    searchUtil(i, tempNode->data->children.head);
                }

                // Move to the next node at the same level
                tempNode = tempNode->next;
            }
        } else if (currentNode == nullptr)
        {
            std::cout << "NOT FOUND ";
        } else /*if (currentNode!= nullptr)*/{
            // Calculate the matching prefix between the remaining query string and the substring represented by the current node
            int matchingPrefix = getMatchingPrefix(stringQuery + i, *(currentNode->data));

            // If the entire remaining query matches the current node's substring, recursively search in its children
            if (matchingPrefix == getLenOfSubstr(*(currentNode->data)))
            {
                if (currentNode->data->children.head == nullptr)
                {
                    std::cout << currentNode->data->suffixIndex << " ";
                } else
                {
                    searchUtil(i + matchingPrefix, currentNode->data->children.head);
                }
            } else if (matchingPrefix < getLenOfSubstr((*(currentNode->data))) && matchingPrefix > 0)
            {
                // If there is a partial match, and it is equal to the length of the remaining query
                if (matchingPrefix == strlen(stringQuery + i))
                {
                    // if currentNode->children is nullptr (no children) print the suffix index
                    // else recursively search
                    if (currentNode->data->children.head == nullptr)
                    {
                        std::cout << currentNode->data->suffixIndex << " ";
                    } else
                    {
                        searchUtil((int) strlen(stringQuery), currentNode->data->children.head);
                    }

                } else
                {
                    std::cout << "NOT FOUND ";
                }
            } else if (matchingPrefix == 0)
            {
                // If there is no match, recursively search in the next node at the same level
                searchUtil(i, currentNode->next);
            }
        }

    }

    // Public method to initiate a search in the suffix tree with a given query string
    void Search(char *string)
    {
        stringQuery = string; // Set the query string for the search
        searchUtil(0, root->data->children.head); // Initiate the search starting from the children of the root
        std::cout << '\n';
    }

};


int main()
{
    // Case 0
    SuffixTree t("bananabanaba$");
    t.Search("ana"); // Prints: 1 3 7
    t.Search("naba"); // Prints: 4 8
    t.Search("banana0"); // NOT FOUND
    std::cout << '\n';

    // Case 1
    SuffixTree t1("programming$");
    t1.Search("gram");  // Prints: 3
    t1.Search("pro");   // Prints: 0
    t1.Search("ming");  // Prints: 7
    std::cout << '\n';

    // Case 2
    SuffixTree t2("abracadabra$");
    t2.Search("bra");   // Prints: 1 8
    t2.Search("ada");   // Prints: 5
    t2.Search("abr");   // Prints: 0 7
    std::cout << '\n';

    // Case 3
    SuffixTree t3("mississippi$");
    t3.Search("ssi");   // Prints: 2 5
    t3.Search("iss");   // Prints: 1 4
    t3.Search("ssippi");// Prints: 5
    std::cout << '\n';

    // Case 4
    SuffixTree t4("algorithmabcbacacca$");
    t4.Search("rithm"); // Prints: 4
    t4.Search("algo");  // Prints: 0
    t4.Search("ca"); // 14 17
    std::cout << '\n';

    // Case 5
    SuffixTree t5("datastructures$");
    t5.Search("data"); // Prints: 0
    t5.Search("a");  // Prints: 1 3
    t5.Search("xyz"); // not found
    std::cout << '\n';

    // Case 6
    SuffixTree t6("abcdefgh$");
    t6.Search("def");   // Prints: 3
    t6.Search("gh");    // Prints: 6
    t6.Search("abc");   // Prints: 0
    std::cout << '\n';

    // Case 7
    SuffixTree t7("xyxxy$");
    t7.Search("yx");    // Prints: 1
    t7.Search("xx");    // Prints: 2
    t7.Search("x"); // 0 2 3
    std::cout << '\n';

    // Case 8
    SuffixTree t8("example$");
    t8.Search("mpl");   // Prints: 3
    t8.Search("exam");  // Prints: 0
    t8.Search("ple");   // Prints: 4
    std::cout << '\n';

    // Case 9
    SuffixTree t9("testing$");
    t9.Search("ing");   // Prints: 4
    t9.Search("test");  // Prints: 0
    t9.Search("ting");  // Prints: 3
    std::cout << '\n';

    // Case 10
    SuffixTree t10("abcdefgabcdefgabcdefg$");
    t10.Search("cd");   // Prints: 2 9 16
    t10.Search("ab");   // Prints: 0 7 14
    t10.Search("fg");   // Prints: 5 12 19
    std::cout << '\n';

    // Case 11
    SuffixTree t11(
            "abracadabraabrakadabraabracadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabraabrakadabra$");
    t11.Search("brak"); // 12 34 45 56 67 78 89 100 111 122 133 144 155 166 177 188
    t11.Search("abrakadabra"); // 11 33 44 55 66 77 88 99 110 121 132 143 154 165 176 187
    t11.Search("000");
    std::cout << '\n';

    return 0;
}
