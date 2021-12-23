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

//without indentation
void convert2JSON(node* n) {

    node* travptr = n;

    if (travptr->parent == nullptr){
        unindentedjson << '{' << endl;
    }
    if (!sameprevsibling) {
        unindentedjson << '"' << travptr->name << "\": ";
    }

    else {
        if (travptr->child != 0) {
            unindentedjson << ',' << endl;
        }
    }

    if (travptr->parent != nullptr && travptr->parent->child > 1 && !sameprevsibling) {
        if (travptr->parent->children[1]->name == travptr->parent->children[0]->name) {
            unindentedjson << '[' << endl;
        }
    }

    if (travptr->child > 0)
    {
        unindentedjson << '{' << endl;
        sameprevsibling = false;
    }
    else {
        unindentedjson << '"' << travptr->txt << '"';
        if (travptr->parent != nullptr && travptr->parent->child == 1) {
            unindentedjson << endl;
        }
        else if (travptr->parent != nullptr && childNumber == travptr->parent->child - 1) {
            unindentedjson << endl << ']' << endl;
            sameprevsibling = false;
        }
        else {
            unindentedjson << ',' << endl;
        }
    }

    for (int i = 0; i < travptr->child; i++)
    {
        travptr = travptr->children[i];
        if (i > 0 && travptr->name == travptr->parent->children[i - 1]->name) sameprevsibling = true;
        childNumber = i;
        convert2JSON(travptr);
        travptr = n;
    }

    if (travptr->child != 0)
    {
        unindentedjson << '}' << endl;
    }

    if (travptr->name == "post" || travptr->name == "follower" || travptr->name == "user")
    {
        if (travptr == travptr->parent->lastChild && travptr->parent->child > 1)
        {
            unindentedjson << ']' << endl;
        }
    }
    if (travptr->name == "posts" && travptr != travptr->parent->lastChild)
    {
        unindentedjson << "," << endl;
    }

    if (travptr->name == "users")
    {
        unindentedjson << "}" << endl;
    }


}

void prettifyJson(ifstream& f,Ui::MainWindow* ui) {

    vector<string> lines;
    string s;

    while (getline(f, s)) {
        auto pos = s.find_first_not_of(" \t");
        if (pos != string::npos) s.erase(s.begin(), s.begin() + pos);
        lines.push_back(s);
    }

    int tabs = 0;
    QString qstr;

    for (int i = 0; i < lines.size(); i++) {

        if (lines[i][0] == ']' || lines[i][0] == '}') {
            tabs--;

            for (int i = 0; i < tabs; i++){
                ui->Display_output->insertPlainText("    ");
            }
            qstr = QString::fromStdString(lines[i]);
            ui->Display_output->insertPlainText(qstr);

            if (i + 1 != lines.size()) {
                if (lines[i][0] == '}') {
                    if (lines[i + 1][0] == ',') {
                        ui->Display_output->insertPlainText(",");
                        i++;
                    }
                }
            }

            ui->Display_output->insertPlainText("\n");
            continue;
        }

        for (int i = 0; i < tabs; i++){
            ui->Display_output->insertPlainText("    ");
        }
        qstr = QString::fromStdString(lines[i]);
        ui->Display_output->insertPlainText(qstr);
        ui->Display_output->insertPlainText("\n");

        if (lines[i][0] == '{' || lines[i][0] == '[' || lines[i][lines[i].length() - 1] == '{' || lines[i][lines[i].length() - 1] == '[')
            tabs++;

        if (lines[i][lines[i].length() - 1] == '}' || lines[i][lines[i].length() - 1] == ']')
            tabs--;
    }
}
