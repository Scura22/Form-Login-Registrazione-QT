#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "adminview.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(310,340);
    ui->btnToRegistrati->setHidden(true);
    ui->btnRecuperaPassword->setHidden(true);
    ui->btnAccedi->setHidden(true);
    QDate date = QDate::currentDate();
    ui->dBirthDate->setDate(date);
    ui->dBirthDate->setMaximumDate(date);
    ui->dBirthDate->setMinimumDate(QDate::fromString("19000101", "yyyyMMdd"));
    setAdminUser();
    checkDbExist();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnRegistrazione_clicked()
{
    QMessageBox msgRegistrazione;
    if (checkName())
        if (checkSurname())
            if (checkCellMail())
                if (checkPassword())
                    if (checkBirthDate())
                        if (checkGender())
                            if(checkIfReg())
                                msgRegistrazione.setText("Utente già registrato!");
                            else{
                                insertNewUser();
                                msgRegistrazione.setText("Registrazione eseguita!\nBenvenuto/a " + ui->txtNome->text() + " "+ui->txtCognome->text() +"!");
                                clearDati();
                            }
                        else
                            msgRegistrazione.setText("Selezionare il genere!");
                    else
                        msgRegistrazione.setText("Per registrarsi bisogna essere maggiorenni!");
                else
                    msgRegistrazione.setText("Il campo password non può essere vuoto!");
            else
                msgRegistrazione.setText("Inserire numero di telefono o e-mail in un formato corretto");
        else
            msgRegistrazione.setText("Il campo cognome non può essere vuoto!");
    else
        msgRegistrazione.setText("Il campo nome non può essere vuoto!");

    msgRegistrazione.exec();
}



void MainWindow::on_btnToAccedi_clicked()
{
    clearDati();
    ui->btnRegistrazione->setHidden(true);
    ui->btnToAccedi->setHidden(true);
    ui->txtNome->setHidden(true);
    ui->txtCognome->setHidden(true);
    ui->dBirthDate->setHidden(true);
    ui->rbtn_male->setHidden(true);
    ui->rbt_female->setHidden(true);
    ui->btnAccedi->setHidden(false);
    ui->btnToRegistrati->setHidden(false);
    ui->btnRecuperaPassword->setHidden(false);
}

void MainWindow::on_btnToRegistrati_clicked()
{
    clearDati();
    ui->btnRegistrazione->setHidden(false);
    ui->btnToAccedi->setHidden(false);
    ui->txtNome->setHidden(false);
    ui->txtCognome->setHidden(false);
    ui->dBirthDate->setHidden(false);
    ui->rbtn_male->setHidden(false);
    ui->rbt_female->setHidden(false);
    ui->btnAccedi->setHidden(true);
    ui->btnToRegistrati->setHidden(true);
    ui->btnRecuperaPassword->setHidden(true);
}

bool MainWindow::checkName()
{
    if (ui->txtNome->text() == "")
        return false;
    else
        return true;
}
bool MainWindow::checkSurname()
{
    if (ui->txtCognome->text() == "")
        return false;
    else
        return true;
}
bool MainWindow::checkCellMail()
{
    QString cellMail = ui->txtCellMail->text();
    QString nomeutente, dominio = "";
    QStringList check;
    //CONTROLLO MAIL
    if (cellMail.contains('@') && cellMail.contains('.')){

        check=cellMail.split('@');
        nomeutente=check[0];
        dominio=check[1];

        if(nomeutente.length()!=0 && dominio.length()>=4 && dominio.contains('.')){
            for(int i=0;i<nomeutente.length();++i)
                if(!nomeutente[i].isLetter() && !nomeutente[i].isDigit() && nomeutente[i]!='.' && nomeutente[i]!='-' && nomeutente[i]!='_')
                    return false;

            check=dominio.split('.');
            nomeutente=check[0];
            dominio=check[1];

            if(nomeutente.length()>=1 && dominio.length()>=2){
                for(int i=0;i<nomeutente.length();++i)
                    if(!nomeutente[i].isLetter() && !nomeutente[i].isDigit())
                        return false;
                for(int i=0;i<dominio.length();++i)
                    if(!dominio[i].isLetter())
                        return false;
            }
            else
                return false;

        }
        else
            return false;
        return true;
    }
    //CONTROLLO NUMERO DI TELEFONO
    if (cellMail.length() == 10){
        for (int i=0; i<cellMail.length(); i++){
            if (!cellMail[i].isDigit())
                return false;
        }
        return true;
    }
    else
        return false;

}
bool MainWindow::checkPassword()
{
    if (ui->txtPassword->text() == "")
        return false;
    else
        return true;
}
bool MainWindow::checkBirthDate()
{
    QDate date = QDate::currentDate();
    QDate dataNascita = ui->dBirthDate->date();

    if (dataNascita.addYears(18)>date)
        return false;
    return true;
}
bool MainWindow::checkGender()
{
    if (ui->rbtn_male->isChecked() || ui->rbt_female->isChecked())
        return true;
    return false;
}

void MainWindow::checkDbExist()
{
    QString path = QApplication::applicationDirPath()+"/db/";
    QDir dir(path);

    if (!dir.exists())
        dir.mkpath(path);

    if (!userListCsv.exists()){
        userListCsv.setFileName(path+"user_list.csv");
        userListCsv.open(QIODevice::ReadOnly);
        userListCsv.close();
    }

}

void MainWindow::setAdminUser()
{
    admin.nome = "Luca";
    admin.cognome = "Scurati";
    admin.dataNascita = QDate::fromString("20000122", "yyyyMMdd");
    admin.cellMail = "admin@pass.com";
    admin.password = "admin";
    admin.genere = 'M';
}

bool MainWindow::checkIfReg()
{
    bool find = false;
    userListCsv.open(QIODevice::ReadOnly);
    QTextStream inputStream(&userListCsv);
    while (!inputStream.atEnd() && !find) {
        QStringList userString = inputStream.readLine().split(';');
        if (ui->txtCellMail->text() == userString[0]) //0 perchè nel csv la mail è il primo parametro
            find = true;
    }
    inputStream.flush();
    userListCsv.close();
    return find;
}

bool MainWindow::checkCredential()
{
    QMessageBox msgAccedi;
    user userLogin;
    bool find = false;
    userListCsv.open(QIODevice::ReadOnly);
    QTextStream inputStream(&userListCsv);
    while (!inputStream.atEnd() && !find) {
        QStringList userString = inputStream.readLine().split(';');
        if (ui->txtCellMail->text() == userString[0]) //0 perchè nel csv la mail è il primo parametro
        {
            if (ui->txtPassword->text() == userString[1]) //1 perchè nel csv la password è il secondo parametro
                msgAccedi.setText("Accesso eseguito!\nBenvenuto/a " + userString[2] + " " + userString[3] +"!"); //2 e 3 perchè nel csv nome e cognome sono terzo e quarto parametro
            else
                msgAccedi.setText("Password Errata!");
            find = true;
            msgAccedi.exec();
        }
    }
    inputStream.flush();
    userListCsv.close();
    return find;
}

bool MainWindow::checkIfAdmin()
{
    if (ui->txtCellMail->text() == admin.cellMail)
        return true;
    else
        return false;

}

void MainWindow::insertNewUser()
{
    user u;
    u.nome = ui->txtNome->text();
    u.cognome = ui->txtCognome->text();
    u.cellMail = ui->txtCellMail->text();
    if (ui->rbtn_male->isChecked())
        u.genere = 'M';
    else
        if (ui->rbt_female->isChecked())
            u.genere = 'F';
    u.password = ui->txtPassword->text();
    u.dataNascita = ui->dBirthDate->date();

    userListCsv.open(QIODevice::Append);
    QTextStream outputStream(&userListCsv);
    outputStream << u.cellMail << ";" << u.password << ";" << u.nome << ";" << u.cognome << ";" << u.dataNascita.toString("yyyyMMdd")<< ";" << u.genere <<"\n";
    outputStream.flush();
    userListCsv.close();
}

void MainWindow::clearDati(){
    ui->txtNome->setText("");
    ui->txtCognome->setText("");
    ui->txtCellMail->setText("");
    ui->txtPassword->setText("");
    ui->dBirthDate->setDate(QDate::currentDate());
    if(ui->rbtn_male->isChecked())
    {this->ui->rbtn_male->setAutoExclusive(false);
        this->ui->rbtn_male->setChecked(false);
    this->ui->rbtn_male->setAutoExclusive(true);
    }
    else
    {this->ui->rbt_female->setAutoExclusive(false);
        this->ui->rbt_female->setChecked(false);
    this->ui->rbt_female->setAutoExclusive(true);}
}

void MainWindow::on_btnRecuperaPassword_clicked()
{
    QMessageBox msgRecuperoPassword;

    if (!checkCellMail())
        msgRecuperoPassword.setText("Inserire numero di telefono o e-mail in un formato corretto");
    else{
        if (checkIfReg())
            msgRecuperoPassword.setText("E-Mail/SMS di recupero password inviata/o correttamente");
        else
            msgRecuperoPassword.setText("Utente " + ui->txtCellMail->text() +" non registrato");
    }

    msgRecuperoPassword.exec();
}

void MainWindow::on_btnAccedi_clicked()
{
    QMessageBox msgAccedi;
    if (checkCellMail())
        if (checkPassword()){
            if (!checkIfAdmin()){
                if (!checkCredential()){
                    msgAccedi.setText("Utente "+ ui->txtCellMail->text() +" non registrato");
                    msgAccedi.exec();
                }
            }
            else
            {//controllo password amministratore
                if (ui->txtPassword->text() == admin.password){
                    AdminView aw(this);
                    aw.setModal(true);
                    this->hide();
                    aw.setChart(&userListCsv);
                    aw.exec();
                    this->show();
                }
                else
                {
                    msgAccedi.setText("Password Errata!");
                msgAccedi.exec();
                }
            }
        }
        else{
            msgAccedi.setText("Il campo password non può essere vuoto!");
            msgAccedi.exec();
        }
    else
    {
        msgAccedi.setText("Inserire numero di telefono o e-mail in un formato corretto");
        msgAccedi.exec();
    }
}
