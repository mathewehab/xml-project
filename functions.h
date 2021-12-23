#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <stack>
#include <QApplication>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#define EMPTY_STRING ""
#define CODE1
using namespace std;

class node;
class tree;

// A Huffaman Tree node
struct Node
{
    char ch;
    int freq;
    Node* left, * right;
};

class tree {
public:
    node* root;
    node* getRoot() { return root; }
    void setRoot(node* n) { root = n; }
    bool isEmpty() { return root == nullptr; }
};

extern Node* root;
extern unordered_map<char, string> huffmanCode;
extern unordered_map<char, int> freq;
extern ofstream unindentedjson;
extern tree* xml;

// Comparison object to be used to order the heap
struct comp
{
    bool operator()(const Node* l, const Node* r) const
    {
        // the highest priority item has the lowest frequency
        return l->freq > r->freq;
    }
};

bool isbrParent(string);
bool isParent(string);
bool checkDemo(string);
void correctDemo(string file, Ui::MainWindow*);
bool check(string);
void correct(string file, Ui::MainWindow*);
void minify(string,Ui::MainWindow*);
void prettify(string,Ui::MainWindow*);
void Compress(Node*, string, unordered_map<char, string>&);
void Decompress(Node*,ifstream&,Ui::MainWindow*);
string TextToBinaryString(Ui::MainWindow*);
void buildHuffmanTree(string);
void buildXMLtree(string);
void convert2JSON(node*);
void prettifyJson(ifstream&,Ui::MainWindow*);
bool isLeaf(Node*);

#endif /* FUNCTIONS_H_ */
