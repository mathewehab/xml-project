#include "checkConsistencyDemo.h"
#ifndef CODE1

bool isbrParent(string element) {
   for (auto& el : brParents)
       if (element == el) return true;
   return false;
}

bool isParent(string element) {
   for (auto& el : terminals) {
       if (element == el) return false;
   }
   return true;
}

bool missCtagOverText() {
   lineNum.push_back(line-1);
   string msg = "in line " + to_string(line - 1);
   msg += ": missing closing tag over text";
   msg += ", error in tag: </" + st.top() + ">";
   errors.push_back(msg);
   childText = false;
   st.pop();
   if (!closingTag) st.push(tag);
   else if (st.top() == tag) st.pop();
   else return true;
   return false;
}

bool missOtagOverText() {
   lineNum.push_back(line-1);
   string msg = "in line " + to_string(line - 1);
   msg += ": missing opening tag over text";
   msg += ", error in tag: <" + tag + ">";
   errors.push_back(msg);
   childText = false;
   return false;
}

bool missCtagOverTags() {
   lineNum.push_back(line-1);
   string msg = "in line " + to_string(line - 1);
   msg += ": missing closing tag over tags";
   msg += ", error in tag: </" + temp + ">";
   errors.push_back(msg);
   if (closingTag) st.pop();
   else st.push(tag);
   return false;
}

bool missOtagOverTags() {
   lineNum.push_back(line);
   string msg = "in line " + to_string(line);
   msg += ": closing tag has no opening tag over tags";
   msg += ", error in tag: <" + tag + ">";
   errors.push_back(msg);
   if ((st.size() > 1 && temp != "") || temp == "user") st.push(temp);
   return false;
}

void errorHandling() {
   if (childText) {
       if (tag != st.top() || (tag == st.top() && !closingTag)) {
           if (isParent(st.top())) error = missOtagOverText();
           else error = missCtagOverText();
       }
   }
   else {
       if (st.size() > 1) {
           temp = st.top();
           st.pop();

           if ((tag != temp && closingTag && st.top() == tag) || (!closingTag && tag == st.top()) || (isbrParent(temp) && isbrParent(tag)))
               error = missCtagOverTags();
           else if (closingTag && tag != temp && st.top() != tag)
               error = missOtagOverTags();
           else if (tag == temp && !closingTag) error = missCtagOverTags();
       }
       else { //if st.size() <= 1
           error = missOtagOverTags();

       }
   }
}

bool checkDemo(string file)
{
   ifstream cin(file);

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

               while (s[j] != '>') {
                   tag += s[j++];
               }

               if (st.size() && closingTag && st.top() == tag) childText = false;

               if (!closingTag) {
                   if (st.empty() || (!childText && !(isbrParent(st.top()) && isbrParent(tag))) && st.top() != tag) { //hena!
                       st.push(tag);
                       error = false;
                   }
                   else error = true;
               }
               else {
                   if (st.size() && st.top() == tag) {
                       st.pop();
                       error = false;
                   }
                   else error = true;
               }
               i = j;
               text = "";

               //handling errors
               while (error) errorHandling();

           }
           else {
               childText = true;
               int j = i;
               while (j < (int)s.size() && s[j] != '<') text += s[j++];
               i = j - 1;
           }
       }
   }

   // if stack is not empty
   while (!st.empty()) {
       lineNum.push_back(line);
       string msg = "in line " + to_string(line);
       msg += ": missing closing tag over tags";
       msg += ", error in tag: </" + st.top() + ">";
       errors.push_back(msg);
       st.pop();
   }

   return (errors.size()==0);
}

#endif
