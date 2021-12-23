#include "compressXML.h"

// Function to allocate a new tree node
Node* getNode(char ch, int freq, Node* left, Node* right)
{
    Node* node = new Node();

    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

// Utility function to check if Huffman Tree contains only a single node
bool isLeaf(Node* root) {
    return root->left == nullptr && root->right == nullptr;
}

string TextToBinaryString(Ui::MainWindow* ui) {
    QString text = ui->Display_output->toPlainText();
    string words=text.toStdString();
    string returnedstr;
    char bit;
    for(int i=0;i<(int)words.length();i++){
        for(int j=0; j<8; j++){
            bit = words[i] & (1<<(7-j));
            if(bit) returnedstr.push_back('1');
            else returnedstr.push_back('0');
        }
    }
    return returnedstr;

}

// Traverse the Huffman Tree and store Huffman Codes in a map.
void Compress(Node* root, string str, unordered_map<char, string>& huffmanCode)
{
    if (root == nullptr) {
        return;
    }

    // found a leaf node
    if (isLeaf(root)) {
        huffmanCode[root->ch] = (str != EMPTY_STRING) ? str : "1";
    }

    Compress(root->left, str + "0", huffmanCode);
    Compress(root->right, str + "1", huffmanCode);
}

void Decompress(Node* root, ifstream& str,Ui::MainWindow* ui)
{
    if (root == nullptr) {
        return;
    }

    // found a leaf node
    if (isLeaf(root))

    {

        QChar qchar=QChar(root->ch);
        ui->Display_output->insertPlainText(qchar);

        return;
    }

    if(!str.eof()){
        if (str.get() == '0') {
            Decompress(root->left, str,ui);
        }
        else {
            Decompress(root->right, str,ui);
        }
    }

}

// Traverse the Huffman Tree and decode the encoded string
void buildHuffmanTree(string filename)
{
    ifstream lines;
    lines.open(filename);


    // count the frequency of appearance of each character
    // and store it in a map
    while (!lines.eof()) {
        freq[lines.get()]++;
    }

    lines.close();
    // Create a priority queue to store live nodes of the Huffman tree
    priority_queue<Node*, vector<Node*>, comp> pq;

    // Create a leaf node for each character and add it
    // to the priority queue.
    for (auto pair : freq) {
        pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
    }

    // do till there is more than one node in the queue
    while (pq.size() != 1)
    {
        // Remove the two nodes of the highest priority
        // (the lowest frequency) from the queue

        Node* left = pq.top(); pq.pop();
        Node* right = pq.top();pq.pop();

        // create a new internal node with these two nodes as children and
        // with a frequency equal to the sum of the two nodes' frequencies.
        // Add the new node to the priority queue.

        int sum = left->freq + right->freq;
        pq.push(getNode('\0', sum, left, right));
    }

    // `root` stores pointer to the root of Huffman Tree
    root = pq.top();


}
