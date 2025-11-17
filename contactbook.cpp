#include "contactbook.h"
#include "ui_contactbook.h"

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include<QFileDialog>
QString mFilename = "C:/Users/user/Desktop/EX/file.txt";

void Write(QString Filename,QString str){
    QFile mFile(Filename);
    if(!mFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug()<<"could not open file for write";
        return;
    }
    QTextStream out(&mFile);
    out<<str;
    mFile.flush();
    mFile.close();
}

QString Read(QString Filename){
    QFile mFile(Filename);

    if(!mFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug()<<"could not open file for read";
        return "";
    }

    QTextStream in(&mFile);
    QString text = in.readAll();
    mFile.close();
    return text;
}

ContactBook::ContactBook(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContactBook)
{
    ui->setupUi(this);
    QStringList colTitles;
    ui->tableWidget->setColumnCount(4);
    colTitles<<"學號"<<"班級"<<"姓名"<<"電話";
    ui->tableWidget->setHorizontalHeaderLabels(colTitles);
}

ContactBook::~ContactBook()
{
    delete ui;
}

void ContactBook::on_pushButton_clicked()
{
    QTableWidgetItem *col1, *col2, *col3, *col4;

    col1 = new QTableWidgetItem(QString(ui->lineEdit->text()));
    col2 = new QTableWidgetItem(QString(ui->lineEdit_2->text()));
    col3 = new QTableWidgetItem(QString(ui->lineEdit_3->text()));
    col4 = new QTableWidgetItem(QString(ui->lineEdit_4->text()));
    int rc = ui->tableWidget->rowCount();
    qDebug() << rc <<"\n";
    ui->tableWidget->insertRow(rc);
    ui->tableWidget->setItem(rc,0,col1);
    ui->tableWidget->setItem(rc,1,col2);
    ui->tableWidget->setItem(rc,2,col3);
    ui->tableWidget->setItem(rc,3,col4);
}


void ContactBook::on_pushButton_2_clicked(){
    QString filename = QFileDialog::getSaveFileName(this,
                                                    QStringLiteral("另存新檔"),
                                                    mFilename,
                                                    "Text Files (*.txt);;All Files (*)");

    // If user cancelled the dialog, return
    if(filename.isEmpty()){
        return;
    }

    QString saveFile="";

    for(int i=0;i<ui->tableWidget->rowCount();i++){
        for(int j=0;j<ui->tableWidget->columnCount();j++){
            saveFile+=(j==0)?"":",";
            saveFile+=ui->tableWidget->item(i,j)->text();
        }
        saveFile+="\n";
    }
    Write(filename,saveFile);
}


void ContactBook::on_pushButton_3_clicked()
{
    // Show open file dialog
    QString filename = QFileDialog::getOpenFileName(this,
                                                    QStringLiteral("選擇匯入的檔案"),
                                                    mFilename,
                                                    "Text Files (*.txt);;All Files (*)");

    // If user cancelled the dialog, return
    if(filename.isEmpty()){
        return;
    }

    QString text = Read(filename);
    if(text.isEmpty()){
        qDebug()<<"No data to import or file is empty";
        return;
    }

    // Clear existing table data
    ui->tableWidget->setRowCount(0);

    // Split by lines
    QStringList lines = text.split("\n", Qt::SkipEmptyParts);

    for(const QString &line : lines){
        // Split by comma
        QStringList fields = line.split(",");

        if(fields.size() == 4){
            // Add a new row
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);

            // Set the data for each column
            for(int i = 0; i < 4; i++){
                QTableWidgetItem *item = new QTableWidgetItem(fields[i]);
                ui->tableWidget->setItem(row, i, item);
            }
        }
    }
}


void ContactBook::on_pushButton_4_clicked(){
    qApp->quit();
}
