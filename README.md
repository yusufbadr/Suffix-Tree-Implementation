# Suffix Tree Implementation

## Overview

This C++ project implements a compact Suffix Tree data structure, with an efficient Search() operation for any substring. The suffix tree is constructed with a time complexity of O(n^2) and a space complexity of O(n), where n is the length of the input string. The search operation has a time complexity of O(m+occ), where m is the length of the searched pattern, and occ is the number of occurrences of the pattern inside the string.

## Features

- **Compact Suffix Tree**: Each node in the tree contains a linked list of children nodes, the starting index of the characters on the edge going to this node (substringStartIndex), and the starting index of the suffix ending at this node (used for leaf nodes only, otherwise it is -1).

- **Search Operation**: The tree supports a search operation with a time complexity of O(m+occ), making it efficient for finding occurrences of substrings within the original string.

## Code Structure

The project is organized into several classes:

- **SuffixTreeNode**: Represents a node in the suffix tree, containing information about the substring it represents and its children nodes.

- **Node**: Represents a node in the linked list used to store children nodes in each SuffixTreeNode.

- **LinkedList**: Represents a linked list of SuffixTreeNodes.

- **SuffixTree**: The main class encapsulating the suffix tree construction and search functionalities.

## Usage

To use the Suffix Tree implementation, create an instance of the `SuffixTree` class by providing the input string. After construction, you can perform searches by calling the `Search` method with the desired pattern.

```cpp
// Example Usage
SuffixTree suffixTree("example$");
suffixTree.Search("ple");   // Prints: 4
suffixTree.Search("exam");  // Prints: 0
suffixTree.Search("ing");   // Prints: NOT FOUND
```

## Examples

The project includes several test cases in the main function, demonstrating the usage of the suffix tree for different input strings and search patterns.

## Installation

Clone the repository to your local machine and compile the C++ code using a compatible compiler. No additional dependencies are required.

## Contributors

- [Yusuf Badr](https://github.com/yusufbadr)
- [Abdelrahman Wael](https://github.com/abwael)
- [Adham Mansour](https://github.com/Mansour266)
- [Maya Ayman](https://github.com/MayaZayn)
- [Your Name](https://github.com/your-username)
