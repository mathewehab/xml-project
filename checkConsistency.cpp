#include "checkConsistency.h"
#ifdef CODE1
class node {
public:
    string name;
    int child;
    node* lastChild = nullptr;
    node* children[MAX]{};
    node* parent = nullptr;

    node(string s) {
        name = s;
        child = 0;
    }

    node* addNode(string str) {
        node* n = new node(str);
        n->parent = this;
        this->children[child++] = n;
        this->lastChild = n;
        return n;
    }
};

bool isTerminal(string element) {
    for (auto& el : terminals) {
        if (element == el) return true;
    }
    return false;
}

bool hasVarChildren(string element) {
    for (auto& el : varChildren) {
        if (element == el) return true;
    }
    return false;
}

void basicTags(){
    basic_tags = new tree();
    node* users = new node("users");
    basic_tags->setRoot(users);
    node* user = users->addNode("user");

    node* Uid = user->addNode("id");
    node* name = user->addNode("name");

    node* posts = user->addNode("posts");
    node* post = posts->addNode("post");
    node* body = post->addNode("body");
    node* topics = post->addNode("topics");
    node* topic = topics->addNode("topic");

    node* followers = user->addNode("followers");
    node* follower =  followers->addNode("follower");
    node* Fid = follower->addNode("id");
}

void missingClosingTag(string missedTag){
    lineNum.push_back(line);
    missedTags.push_back("</"+missedTag+">");
    st.pop();
}

void missingOpeningTag(string missedTag){
    lineNum.push_back(line);
    missedTags.push_back("<"+missedTag+">");
    st.push(missedTag);
}

void go2child(){
    idx = (ptr->child==1) ? 0 : (ptr->child==4) ? index1++ : index2++;
    ptr = ptr->children[idx];
}

bool check(string file)
{
    ifstream cin(file);

    basicTags();
    ptr = basic_tags->getRoot();

    while (!cin.eof())
    {
        getline(cin, s);
        auto pos = s.find_first_not_of(" \t");
        if (pos != string::npos) s.erase(s.begin(), s.begin() + pos);
        else s = "";
        line++;

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

                if (!closingTag) {
                    if((ptr->name=="post" && index2==2) || (ptr->name=="user" && index1==4)){
                        if(ptr->name=="post") index2 = 0;
                        else if(ptr->name=="user") index1 = 0;

                        missingClosingTag(ptr->name);
                    }
                    else if(tag=="id" && st.top()=="id"){
                        missingClosingTag(ptr->name);
                        ptr = ptr->parent;
                    }

                    while(tag != ptr->name){
                        if(ptr->name=="follower"){
                            if(flag){
                                flag = 0;
                                missingClosingTag(ptr->name);
                            }

                            if(tag=="id"){
                                missingOpeningTag(ptr->name);
                                go2child();
                            }
                            else if(tag=="user"){
                                ptr = ptr->parent;
                                missingClosingTag(ptr->name);
                                ptr = ptr->parent;
                                index1 = 0;
                                missingClosingTag(ptr->name);
                            }
                            else{
                                missedTags.clear();
                                msg = "Incomplete checking ... Non matching tags\n";
                                msg += "Correct tags in line " + to_string(line) + " in the file and check again";
                                missedTags.push_back(msg);
                                return false;
                            }
                        }
                        else if(ptr->name=="post" && tag=="followers"){
                            missingClosingTag(ptr->name);
                            ptr = ptr->parent->parent;
                            go2child();
                        }
                        else if(isTerminal(ptr->name)){
                            if(ptr->name!=st.top()){
                                missingOpeningTag(ptr->name);
                            }
                            missingClosingTag(ptr->name);
                            ptr = ptr->parent;
                            if(ptr->name!="follower") go2child();
                        }
                        else{
                            missingOpeningTag(ptr->name);
                            go2child();
                        }
                    }

                    if(ptr->name=="follower" && flag){
                        flag = 0;
                        missingClosingTag(ptr->name);
                    }

                    st.push(tag);

                    if(!isTerminal(tag)){
                        if(tag=="follower") flag = 1;
                        go2child();
                    }
                }
                else {
                    while(tag != st.top()){
                        missingClosingTag(st.top());
                        if(st.empty()){
                            missedTags.clear();
                            msg = "Incomplete checking ... Non matching tags\n";
                            msg += "Correct tags in line " + to_string(line) + " in the file and check again";
                            missedTags.push_back(msg);
                            return false;
                        }
                    }
                    st.pop();
                    if(isTerminal(tag)){
                        ptr = ptr->parent;
                        if(ptr->name!="follower") go2child();
                    }
                    else if(hasVarChildren(tag)){
                        if(tag=="topics") ptr = ptr->parent->parent;
                        else if(tag=="posts"){
                            ptr = ptr->parent->parent;
                            go2child();
                        }
                        else if(tag=="followers"){
                            while(ptr->name!="user") ptr = ptr->parent;
                        }
                    }
                    else if(tag=="post" || tag=="follower" || tag=="user"){
                        if(tag=="post") index2 = 0;
                        else if(tag=="follower") flag = 0;
                        else index1 = 0;

                        while(ptr->name!=tag) ptr = ptr->parent;
                    }
                    else break;
                }
                i = j;
            }
            else {
                while(st.empty() || st.top()!=ptr->name){
                    missingOpeningTag(ptr->name);
                    if(!isTerminal(ptr->name)) go2child();
                }
                int j = i;
                while (j<(int)s.size() && s[j]!='<') j++;
                i = j - 1;
            }
        }
    }

    // if stack is not empty
    while (!st.empty()){
        line++;
        missingClosingTag(st.top());
    }

    cin.close();
    return (missedTags.size() == 0);
}

void correct(string file, Ui::MainWindow* ui)
{
    if(!missedTags.size()){
        ui->Errors->appendPlainText(QString::fromStdString(missedTags[0]));
    }
    else{
        ifstream correction(file);
        line = 0, idx=0;

        while (!correction.eof()){
            getline(correction, s);
            line++;

            while(idx<(int)missedTags.size() && lineNum[idx]==line)
            {
                tag = missedTags[idx];
                if(missedTags[idx].at(1)!='/'){
                    msg = "in line " + to_string(line);
                    msg += ": missing opening tag: " + tag;
                }
                else{
                    msg = "in line " + to_string(line);
                    msg += ": missing closing tag: " + tag;
                }
                idx++;
                ui->Errors->appendPlainText(QString::fromStdString(msg));
                ui->Display_output->appendPlainText(QString::fromStdString(tag));
            }

            ui->Display_output->appendPlainText(QString::fromStdString(s));
        }
        correction.close();

        for(; idx<(int)missedTags.size(); idx++){
            tag = missedTags[idx];
            msg = "in line " + to_string(line);
            msg += ": missing closing tag: " + tag;

            ui->Errors->appendPlainText(QString::fromStdString(msg));
            ui->Display_output->appendPlainText(QString::fromStdString(tag));
        }
    }
}
#endif
