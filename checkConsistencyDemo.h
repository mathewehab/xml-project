#pragma once

#include "functions.h"
#ifndef CODE1
//Global variables
string s, tag, temp, text;
string terminals[4] = { "id", "name", "body", "topic" };
string brParents[2] = { "posts", "followers" };
bool childText = false;
bool error;
stack<string> st;
vector<string> errors;
vector<int> lineNum;
bool closingTag;
int line = 0;

//Prototypes
bool isbrParent(string element);
bool isParent(string element);
bool missCtagOverText();
bool missOtagOverText();
bool missCtagOverTags();
bool missOtagOverTags();
void errorHandling();
bool checkDemo(string file);
void correctDemo(string file, Ui::MainWindow* ui);
#endif
