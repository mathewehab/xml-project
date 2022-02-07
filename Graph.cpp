#include "graph.h"
vector<vector<int>> Dgraph(MAX, vector<int>(MAX, 0));

int toGraph(string file) {
    int real_user_count = 0;
    vector<string> helper;
    ifstream f(file);
    string s;

    while (getline(f, s)) {
        auto pos = s.find_first_not_of(" \t");

        if (pos != string::npos)
            s.erase(s.begin(), s.begin() + pos);

        for (int i = 0; i < (int)s.size(); i++) {
            string tag = "";

            if (s[i] == '<') {
                int j = i + 1;

                while (s[j] != '>')
                    tag += s[j++];

                helper.push_back(tag);

                if (tag == "id") {
                    string id_num = "";

                    while (s[j + 1] != '<') {
                        id_num += s[j + 1];
                        j++;
                    }

                    helper.push_back(id_num);
                }

                i = j;
            }
        }
    }

    int host = 0;

    for (int i = 0; i < (int)helper.size(); i++) {
        if (helper[i] == "id" && helper[i - 1] == "user") {
            real_user_count++;
            host = stoi(helper[i + 1]);
        }
        else if (helper[i] == "id" && helper[i - 1] == "follower")
            Dgraph[host - 1][stoi(helper[i + 1]) - 1] = 1;
    }
    return real_user_count;
}
