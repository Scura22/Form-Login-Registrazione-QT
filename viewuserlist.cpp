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
    QTextStream inputStream(csv); //apro lo stream per leggere all'interno del file
    while (!inputStream.atEnd()) { //finchè non arriva alla fine del file leggo una nuova riga e memorizzo i dati degli utenti registrati
        QStringList userString = inputStream.readLine().split(';');
        u.cellMail = userString[0]; //0 perchè nel csv la mail / il numero di telefono è il primo parametro
        u.password = userString[1]; //1 perchè nel csv la password è il secondo parametro
        u.nome = userString[2]; //2 perchè nel csv il nome è il terzo parametro
        u.cognome = userString[3]; //3 perchè nel csv il cognome è il quarto parametro
        u.dataNascita = QDate::fromString(userString[4], "yyyyMMdd"); //4 perchè nel csv la data di nascita è il quinto parametro
        u.genere = static_cast<QChar>(userString[5][0]);//5 perchè nel csv il genere è il sesto parametro

        userList.append(u);//inserisco nella lista di utenti i dati dell'utente letti dal csv

    }
    inputStream.flush();
    csv->close();

    setTable();//richiamo il metodo per l'inizializzazione della tabella

}

void ViewUserList::setTable(){

    ui->tbUserList = new QTableWidget(userList.count(), 6, this); //inizializzo la tabella con 6 colonne e tante righe quanti gli utenti registrati
    ui->tbUserList->setEditTriggers(QAbstractItemView::NoEditTriggers);//disabilito la modifica delle celle della tabella
    ui->tbUserList->setFixedSize(800,600);//imposto una dimensione fissa per la tabella
    //inizializzo una lista di stringhe che contiene gli header delle colonne della tabella
    QStringList header;
    header.append("EMail/Cellulare");
    header.append("Password");
    header.append("Nome");
    header.append("Cognome");
    header.append("Data di Nascita");
    header.append("Genere");
    QTableWidgetItem *item;

    ui->tbUserList->setHorizontalHeaderLabels(header); //imposto gli header della tabella con la lista di stringhe definita in precedenza

    for (int row=0; row<userList.count(); row++){ //per ogni utente registrato inserisco i dati nella tabella
        for (int column=0; column<6; column++){ //per ogni colonna prendo il dato dell'utente relativo a quella colonna
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
            ui->tbUserList->setItem(row,column,item); //inserisco il dato "column" nella riga corrispondente all'utente "row",
        }
    }
    //per ogni colonna imposto una dimensione fissa dipendente dalla dimensione della tabella
    for (int column=0; column<ui->tbUserList->columnCount(); column++){
        ui->tbUserList->setColumnWidth(column,ui->tbUserList->width()/ui->tbUserList->columnCount());
    }

}
