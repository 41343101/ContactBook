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
    
    // Apply modern styling to the main widget
    this->setStyleSheet(
        "QWidget {"
        "    background-color: #f5f5f5;"
        "    font-family: 'Microsoft YaHei', 'SimHei', Arial, sans-serif;"
        "    font-size: 12pt;"
        "}"
        
        // Table widget styling
        "QTableWidget {"
        "    background-color: white;"
        "    border: 2px solid #e0e0e0;"
        "    border-radius: 8px;"
        "    gridline-color: #e0e0e0;"
        "    selection-background-color: #4CAF50;"
        "    selection-color: white;"
        "}"
        "QTableWidget::item {"
        "    padding: 8px;"
        "    border-bottom: 1px solid #f0f0f0;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "}"
        "QHeaderView::section {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    padding: 10px;"
        "    border: none;"
        "    font-weight: bold;"
        "    font-size: 13pt;"
        "}"
        
        // Label styling
        "QLabel {"
        "    color: #333333;"
        "    font-weight: bold;"
        "    font-size: 12pt;"
        "    padding: 5px;"
        "}"
        
        // LineEdit styling
        "QLineEdit {"
        "    background-color: white;"
        "    border: 2px solid #e0e0e0;"
        "    border-radius: 6px;"
        "    padding: 8px 12px;"
        "    font-size: 12pt;"
        "    color: #333333;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid #2196F3;"
        "    background-color: #f8f9fa;"
        "}"
        
        // Button styling
        "QPushButton {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 6px;"
        "    padding: 10px 24px;"
        "    font-size: 12pt;"
        "    font-weight: bold;"
        "    min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #0D47A1;"
        "}"
        "QPushButton#pushButton {"  // Add button
        "    background-color: #4CAF50;"
        "}"
        "QPushButton#pushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton#pushButton_2 {"  // Export button
        "    background-color: #FF9800;"
        "}"
        "QPushButton#pushButton_2:hover {"
        "    background-color: #FB8C00;"
        "}"
        "QPushButton#pushButton_3 {"  // Import button
        "    background-color: #9C27B0;"
        "}"
        "QPushButton#pushButton_3:hover {"
        "    background-color: #7B1FA2;"
        "}"
        "QPushButton#pushButton_4 {"  // Exit button
        "    background-color: #f44336;"
        "}"
        "QPushButton#pushButton_4:hover {"
        "    background-color: #d32f2f;"
        "}"
    );
    
    // Enable alternating row colors for better readability
    ui->tableWidget->setAlternatingRowColors(true);
    
    // Set better column widths
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnWidth(0, 120);  // Student ID
    ui->tableWidget->setColumnWidth(1, 120);  // Class
    ui->tableWidget->setColumnWidth(2, 150);  // Name
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
