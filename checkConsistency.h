#include "functions.h"
#ifdef CODE1

class node;
class tree;

tree *basic_tags;
node *ptr;

string s, tag, msg;
string terminals[4] = {"id", "name", "body", "topic"};
string varChildren[4] = {"users", "posts", "topics", "followers"};
bool flag = 0;
int idx = 0, index1 = 0, index2 = 0;
stack<string> st;
vector<string> missedTags;
vector<int> lineNum;
bool closingTag;
int line = 0;

bool isTerminal(string element);
bool hasVarChildren(string element);
void basicTags();
void missingClosingTag(string missedTag);
void missingOpeningTag(string missedTag);
void go2child();
bool check(string file);
void correct(string file, Ui::MainWindow* ui);

#endif
