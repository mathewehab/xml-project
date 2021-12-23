#include"functions.h"

using namespace std;

Node* root;
unordered_map<char, string> huffmanCode;
unordered_map<char, int> freq;

Node* getNode(char ch, int freq, Node* left, Node* right);
bool isLeaf(Node* root);
string TextToBinaryString(Ui::MainWindow* ui);
void Compress(Node* root, string str, unordered_map<char, string>& huffmanCode);
void Decompress(Node* root, ifstream& str,Ui::MainWindow* ui);
void buildHuffmanTree(string filename);
