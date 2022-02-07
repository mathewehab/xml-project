#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <sstream>
#include <bitset>
#include "gvc.h"
#include "cdt.h"
#include "cgraph.h"
#include "functions.h"

string filepath;
string compressedfile;
ofstream compressed("compressedfile.txt");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_browse_clicked()
{
    QString filter = "XML File(*.xml) ;; JSON File(*.json)";
    QString filename = QFileDialog::getOpenFileName(this, "Choose XML file", "C://",filter);
    filepath=filename.toStdString();
    QFile file(filename);

    if (!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "WARNING", "file not open");
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    ui->Display_inputfile->setPlainText(text);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-3);
    file.close();
}

void MainWindow::on_pushButton_save_clicked()
{
    QString filter = "Text File(*.txt);;XML File(*.xml) ;; JSON File(*.json)";
    QString filename = QFileDialog::getSaveFileName(this, "Save as", "C://", filter);
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot save file: "+file.errorString());
        return;
    }
    QTextStream out(&file);
    QString text = ui->Display_output->toPlainText();
    out << text;
    file.close();
}

void MainWindow::on_pushButton_minify_clicked()
{
    ui->Display_output->setPlainText(QString());
    if(filepath == ""){
        QMessageBox::warning(this, "Warning", "No file selected!");
        return;
    }
    minify(filepath,ui);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-2);
}


void MainWindow::on_pushButton_check_clicked()
{
    ui->Display_output->setPlainText(QString());
    if(filepath == ""){
        QMessageBox::warning(this, "Warning", "No file selected!");
        return;
    }
#ifndef CODE1
    bool error_free = checkDemo(filepath);
#else
        bool error_free = check(filepath);
#endif

    if(error_free)
        QMessageBox::about(this, "SUIIIIIIIII", "This file has no errors");
    else{
#ifndef CODE1
        correctDemo(filepath,ui);
#else
        correct(filepath,ui);
#endif

        }
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-3);
}


void MainWindow::on_pushButton_compress_clicked()
{
    ui->Display_output->setPlainText(QString());
    if(filepath == ""){
        QMessageBox::warning(this, "Warning", "No file selected!");
        return;
    }
    buildHuffmanTree(filepath);
    Compress(root, EMPTY_STRING, huffmanCode);
    ifstream lines(filepath);
     while (!lines.eof()) {
            compressed << huffmanCode[lines.get()];
        }
    lines.close();
    compressed.close();
    compressed.clear();

    ifstream compressedfile("compressedfile.txt");
    while (!compressedfile.eof()) {
         char parsed = 0;
         for (int i = 0; i < 8 && !compressedfile.eof(); i++) {
         if (compressedfile.get() == '1' ) {
                parsed |= 1 << (7 - i);
                }
            }
         QChar qchar=QChar(parsed);
         ui->Display_output->insertPlainText(qchar);
    }
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-2);
    compressedfile.close();
    compressedfile.clear();
}


void MainWindow::on_pushButton_decompress_clicked()
{
      ui->Display_output->setPlainText(QString());
      if(filepath == ""){
          QMessageBox::warning(this, "Warning", "No file selected!");
          return;
      }
      string binary=TextToBinaryString(ui);
      ifstream decompressedfile("compressedfile.txt");

    if (isLeaf(root))
        {
            // Special case: For input like a, aa, aaa, etc.
            while (root->freq--) {
                QChar qchar=QChar(root->ch);
                ui->Display_output->insertPlainText(qchar);
            }
        }
        else {
            // Traverse the Huffman Tree again and this time,
            // decode the encoded string
            while (!decompressedfile.eof()) {
                Decompress(root,decompressedfile,ui);
            }
        }
     ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-2);
     decompressedfile.close();
     remove("compressedfile.txt");

}


void MainWindow::on_pushButton_format_clicked()
{
    ui->Display_output->setPlainText(QString());
    if(filepath == ""){
        QMessageBox::warning(this, "Warning", "No file selected!");
        return;
    }
    prettify(filepath,ui);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-2);

}


void MainWindow::on_pushButton_json_clicked()
{
    ui->Display_output->setPlainText(QString());
    if(filepath == ""){
        QMessageBox::warning(this, "Warning", "No file selected!");
        return;
    }
    buildXMLtree(filepath);
    convert2JSON(xml->root);
    unindentedjson.close();
    ifstream file("unindentedjson.txt");
    prettifyJson(file,ui);
    file.close();
    remove("unindentedjson.txt");
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-2);
}

void MainWindow::on_pushButton_Graph_clicked()
{
    int real_user_count=toGraph(filepath);

    // convert adjacency matrix to dot language to pass to graphviz functions
    string str="digraph main{";
    for (int i = 0; i < real_user_count; i++)
     {
         str=str+"node[shape =oval label=\""+to_string(i+1)+"\"]id"+to_string(i+1) +' ';
     }

    for (int i = 0; i <real_user_count; i++)
     {

         for (int j = 0; j < real_user_count; j++)
         {
             if(Dgraph[i][j]==1){
                 str=str+"id"+to_string(i+1)+"->"+"id"+to_string(j+1)+" ";
             }
         }

     }

    str.append("}");
    Agraph_t* G;
    GVC_t* gvc;
    gvc = gvContext();

    string dotLang=str;
    char * y= &dotLang[0];

    G = agmemread(y);
    gvLayout (gvc, G, "dot");

    //saving generated graph image
    string pic_path="graph.png";
    const char *c = pic_path.c_str();

    gvRenderFilename(gvc,G,"png",c);
    gvFreeLayout(gvc, G); /* library function */
    agclose(G); /* library function */
    gvFreeContext(gvc);

    // to display graph image on QT
    QPixmap pix(c);
    ui->label_pic->setPixmap(pix);
    ui->label_pic->setScaledContents(true);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);

}
