#include "format.h"

class XMLTag {
public:
    string name;
    Type type;
    int order;

    XMLTag(string name, bool closingTag, int order) {
        this->name = name;
        this->type = (closingTag) ? closeTag : openTag;
        this->order = order;
    }

    XMLTag(string name) {
        this->name = name;
        this->type = value;
    }
};

void prettify(string file, Ui::MainWindow* ui) {
    ifstream f(file);
    vector<XMLTag> XMLTags;
    vector<string> lines;
    string s;

    while (getline(f, s)) {
        auto pos = s.find_first_not_of(" \t");
        if (pos != string::npos) s.erase(s.begin(), s.begin() + pos);
        lines.push_back(s);
        bool closingTag;
        int order = 0;

        if (s[0] != '<') {
            XMLTag *tag = new XMLTag(s);
            XMLTags.push_back(*tag);
            continue;
        }

        for (int i = 0; i < (int)s.size(); i++) {
            string name = "";

            if (s[i] == '<') {
                order++;
                int j = i + 1;

                if (s[j] == '/') {
                    closingTag = true;
                    j++;
                }
                else closingTag = false;

                while (s[j] != '>')
                    name += s[j++];

                XMLTag *tag = new XMLTag(name, closingTag, order);
                XMLTags.push_back(*tag);

                i = j;
            }

        }
    }

    QString qstr = QString::fromStdString(lines[0]);
    ui->Display_output->appendPlainText(qstr);
    ui->Display_output->insertPlainText("\n");

    int tabs = 0;

    for (int i = 1, j = 1;j < (int)lines.size(); i++, j++) {

        if (XMLTags[i].order == 2) {
            j--;
            continue;
        }

        if (XMLTags[i].type == value) {
            tabs++;
            for (int i = 0; i < tabs; i++){
                ui->Display_output->insertPlainText("    ");
            }
            qstr = QString::fromStdString(lines[j]);
            ui->Display_output->insertPlainText(qstr);
            ui->Display_output->insertPlainText("\n");
            tabs--;
            continue;
        }
        else if (XMLTags[i].type == XMLTags[i - 1].type && XMLTags[i - 1].type == openTag) tabs++;
        else if (XMLTags[i].type == XMLTags[i - 1].type && XMLTags[i - 1].type == closeTag) tabs--;

        for (int i = 0; i < tabs; i++){
            ui->Display_output->insertPlainText("    ");
        }
        qstr = QString::fromStdString(lines[j]);
        ui->Display_output->insertPlainText(qstr);
        ui->Display_output->insertPlainText("\n");
    }
}

void minify(string file,Ui::MainWindow* ui){
    ifstream cin(file);
    while (!cin.eof())
    {
        string str;
        getline(cin, str);
        auto pos = str.find_first_not_of(' ');
        if (pos != string::npos) str.erase(str.begin(), str.begin() + pos);
        else str = "";
        QString qstr = QString::fromStdString(str);
        ui->Display_output->moveCursor(QTextCursor::End);
        ui->Display_output->insertPlainText(qstr);
    }
}
