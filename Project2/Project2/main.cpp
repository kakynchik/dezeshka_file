#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class Node {
public:
    string englishWord;
    string ukrainianTranslation;
    int accessCount;
    Node* left;
    Node* right;

    Node(string english, string ukrainian, int accessCount = 0) {
        this->englishWord = english;
        this->ukrainianTranslation = ukrainian;
        this->accessCount = accessCount;
        this->left = nullptr;
        this->right = nullptr;
    }
};

class BinaryTree {
public:
    Node* root;

    BinaryTree() {
        root = nullptr;
    }

    void addNode(string english, string ukrainian, int accessCount = 0) {
        Node* newNode = new Node(english, ukrainian, accessCount);
        if (root == nullptr) {
            root = newNode;
        }
        else {
            Node* focusNode = root;
            Node* parent;

            while (true) {
                parent = focusNode;
                if (english < focusNode->englishWord) {
                    focusNode = focusNode->left;
                    if (focusNode == nullptr) {
                        parent->left = newNode;
                        return;
                    }
                }
                else if (english > focusNode->englishWord) {
                    focusNode = focusNode->right;
                    if (focusNode == nullptr) {
                        parent->right = newNode;
                        return;
                    }
                }
                else {
                    focusNode->ukrainianTranslation = ukrainian;
                    return;
                }
            }
        }
    }

    Node* findWord(string english) {
        Node* focusNode = root;

        while (focusNode != nullptr) {
            if (english == focusNode->englishWord) {
                focusNode->accessCount++;
                return focusNode;
            }
            else if (english < focusNode->englishWord) {
                focusNode = focusNode->left;
            }
            else {
                focusNode = focusNode->right;
            }
        }

        return nullptr;
    }

    Node* deleteNode(Node* root, string english) {
        if (root == nullptr) return root;

        if (english < root->englishWord) {
            root->left = deleteNode(root->left, english);
        }
        else if (english > root->englishWord) {
            root->right = deleteNode(root->right, english);
        }
        else {
            if (root->left == nullptr) {
                Node* temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr) {
                Node* temp = root->left;
                delete root;
                return temp;
            }

            Node* temp = findMin(root->right);
            root->englishWord = temp->englishWord;
            root->ukrainianTranslation = temp->ukrainianTranslation;
            root->accessCount = temp->accessCount;
            root->right = deleteNode(root->right, temp->englishWord);
        }
        return root;
    }

    Node* findMin(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    void displayWord(string english) {
        Node* result = findWord(english);
        if (result != nullptr) {
            cout << "Word: " << result->englishWord << " - Translation: " << result->ukrainianTranslation << " - Access Count: " << result->accessCount << endl;
        }
        else {
            cout << "Word not found." << endl;
        }
    }

    void inOrderTraversal(Node* focusNode, vector<Node*>& nodes) {
        if (focusNode != nullptr) {
            inOrderTraversal(focusNode->left, nodes);
            nodes.push_back(focusNode);
            inOrderTraversal(focusNode->right, nodes);
        }
    }

    void sortNodes(vector<Node*>& nodes, bool ascending = true) {
        sort(nodes.begin(), nodes.end(), [&](Node* a, Node* b) {
            return ascending ? (a->accessCount < b->accessCount) : (a->accessCount > b->accessCount);
            });
    }

    void displayTopPopular() {
        vector<Node*> nodes;
        inOrderTraversal(root, nodes);
        sortNodes(nodes, false);

        cout << "Top 3 most popular words:" << endl;
        for (int i = 0; i < min(3, static_cast<int>(nodes.size())); i++) {
            cout << nodes[i]->englishWord << " - " << nodes[i]->ukrainianTranslation << " (Access count: " << nodes[i]->accessCount << ")" << endl;
        }
    }

    void displayTopUnpopular() {
        vector<Node*> nodes;
        inOrderTraversal(root, nodes);
        sortNodes(nodes, true);

        cout << "Top 3 least popular words:" << endl;
        for (int i = 0; i < min(3, static_cast<int>(nodes.size())); i++) {
            cout << nodes[i]->englishWord << " - " << nodes[i]->ukrainianTranslation << " (Access count: " << nodes[i]->accessCount << ")" << endl;
        }
    }

    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            saveNode(outFile, root);
            outFile.close();
        }
        else {
            cout << "Unable to open file for writing." << endl;
        }
    }

    void saveNode(ofstream& outFile, Node* node) {
        if (node) {
            outFile << node->englishWord << endl;
            outFile << node->ukrainianTranslation << endl;
            outFile << node->accessCount << endl;
            saveNode(outFile, node->left);
            saveNode(outFile, node->right);
        }
        else {
            outFile << "#\n"; 
        }
    }

    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            root = loadNode(inFile);
            inFile.close();
        }
        else {
            cout << "Unable to open file for reading." << endl;
        }
    }

    Node* loadNode(ifstream& inFile) {
        string english, ukrainian;
        int accessCount;
        if (getline(inFile, english) && english != "#") {
            getline(inFile, ukrainian);
            inFile >> accessCount;
            inFile.ignore(); 
            Node* node = new Node(english, ukrainian, accessCount);
            node->left = loadNode(inFile);
            node->right = loadNode(inFile);
            return node;
        }
        return nullptr; 
    }
};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    BinaryTree dictionary;

    dictionary.addNode("hello", "прив≥т");
    dictionary.addNode("world", "св≥т");
    dictionary.addNode("apple", "€блуко");
    dictionary.addNode("sun", "сонце");
    dictionary.addNode("moon", "м≥с€ць");

    dictionary.displayWord("world");

    dictionary.addNode("computer", "комп'ютер");
    dictionary.addNode("world", "земл€");

    dictionary.root = dictionary.deleteNode(dictionary.root, "sun");

    dictionary.displayTopPopular();
    dictionary.displayTopUnpopular();

   
    dictionary.saveToFile("dictionary.txt");

   
    BinaryTree newDictionary;
    newDictionary.loadFromFile("dictionary.txt");
    newDictionary.displayTopPopular();
    newDictionary.displayTopUnpopular();

    return 0;
}
