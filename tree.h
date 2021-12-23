#include"functions.h"
#define MAX 10

bool leaf;
bool sameprevsibling = false;
int childNumber;
ofstream unindentedjson("unindentedjson.txt");
tree* xml;

void buildXMLtree(string inputfile);
void convert2JSON(node* n);
void prettifyJson(ifstream& f,Ui::MainWindow* ui);
