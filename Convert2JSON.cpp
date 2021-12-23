#include "convert2JSON.h"

class node {
public:
    string name;
    string txt;
    int child;
    node* lastChild;
    node* children[MAX]{};
    node* parent;

    node(string s, node* p = nullptr, string t = "") {
        name = s;
        txt = t;
        parent = p;
        child = 0;
    }

    node* addNode(node* n) {
        if (child < MAX) {
            children[child++] = n;
            lastChild = n;
            return n;
        }
        else return nullptr;
    }

    string gettxt() { return txt; }
};

void buildXMLtree(string inputfile){
    string s, text, tag;
    string temp;
    string terminals[4] = { "id", "name", "body", "topic" };
    bool childText = false;
    bool closingTag;
    stack<string> st;
    ifstream cin(inputfile);

    xml = new tree();
    node* ptr = new node("de7k");

    while (!cin.eof())
    {
        getline(cin, s);
        auto pos = s.find_first_not_of(" \t");
        if (pos != string::npos) s.erase(s.begin(), s.begin() + pos);
        else s = "";
        for (int i = 0; i < (int)s.size(); i++) {
            tag = "";
            if (s[i] == '<') {
                int j = i + 1;

                if (s[j] == '/') {
                    closingTag = true;
                    j++;
                }
                else closingTag = false;

                while (s[j] != '>')
                    tag += s[j++];

                if (st.size() && closingTag && st.top() == tag) childText = false;

                leaf = false;

                if (!closingTag) {

                    st.push(tag);
                    //push unconditionally to the stack if st.empty()
                    //add new node to the tree as a child to the st.top()
                    //define a pointer points to the node's tail

                    if (xml->isEmpty()) {
                        xml->setRoot(new node(tag));
                        ptr = xml->root;
                    }
                    else {
                        for (auto& el : terminals)
                            if (el == tag) leaf = true;
                        if (!leaf) ptr = ptr->addNode(new node(tag, ptr));

                    }

                }
                else {

                    st.pop();
                    //get node's parent and point to other tail of it
                    for (auto& el : terminals)
                        if (el == tag) leaf = true;
                    if (leaf) ptr = ptr->addNode(new node(tag, ptr, text));

                    ptr = ptr->parent;
                }
                i = j;
                text = "";

            }
            else {
                childText = true;
                int j = i;
                while (j < (int)s.size() && s[j] != '<') text += s[j++];
                i = j - 1;
            }
        }
    }
}

