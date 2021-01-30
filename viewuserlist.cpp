#include "viewuserlist.h"
#include "ui_viewuserlist.h"

ViewUserList::ViewUserList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewUserList)
{
    ui->setupUi(this);
    this->setFixedSize(800,600);
}

ViewUserList::~ViewUserList()
{
    delete ui;
}

void ViewUserList::setList(QFile *csv){

    user u;

    csv->open(QIODevice::ReadOnly);
    QTextStream inputStream(csv);
    while (!inputStream.atEnd()) {
        QStringList userString = inputStream.readLine().split(';');
        u.cellMail = userString[0]; //0 perchè nel csv la mail / il numero di telefono è il primo parametro
        u.password = userString[1]; //1 perchè nel csv la password è il secondo parametro
        u.nome = userString[2]; //2 perchè nel csv il nome è il terzo parametro
        u.cognome = userString[3]; //3 perchè nel csv il cognome è il quarto parametro
        u.dataNascita = QDate::fromString(userString[4], "yyyyMMdd"); //4 perchè nel csv la data di nascita è il quinto parametro
        u.genere = static_cast<QChar>(userString[5][0]);//5 perchè nel csv il genere è il sesto parametro

        userList.append(u);

    }
    inputStream.flush();
    csv->close();

    setTable();

}

void ViewUserList::setTable(){

    ui->tbUserList = new QTableWidget(userList.count(), 6, this);
    ui->tbUserList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tbUserList->setFixedSize(800,600);
    QStringList header;
    header.append("EMail/Cellulare");
    header.append("Password");
    header.append("Nome");
    header.append("Cognome");
    header.append("Data di Nascita");
    header.append("Genere");
    QTableWidgetItem *item;

    ui->tbUserList->setHorizontalHeaderLabels(header);

    for (int row=0; row<userList.count(); row++){
        for (int column=0; column<6; column++){
            switch (column){
            case 0: item = new QTableWidgetItem(userList[row].cellMail);
                break;
            case 1: item = new QTableWidgetItem(userList[row].password);
                break;
            case 2: item = new QTableWidgetItem(userList[row].nome);
                break;
            case 3: item = new QTableWidgetItem(userList[row].cognome);
                break;
            case 4: item = new QTableWidgetItem(userList[row].dataNascita.toString("dd/MM/yyyy"));
                break;
            case 5: item = new QTableWidgetItem(userList[row].genere);
                break;
            }
            ui->tbUserList->setItem(row,column,item);
        }
    }

    for (int column=0; column<ui->tbUserList->columnCount(); column++){
        ui->tbUserList->setColumnWidth(column,ui->tbUserList->width()/ui->tbUserList->columnCount());
    }

}
