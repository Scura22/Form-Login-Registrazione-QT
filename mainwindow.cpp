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
    //inizializzazione
    this->setFixedSize(310,340); //imposta la dimensione fissa
    //nasconde i pulsanti relativi alla schermata di accesso
    //all'apertura la registrazione visualizza la schermata di registrazione
    ui->btnToRegistrati->setHidden(true);
    ui->btnRecuperaPassword->setHidden(true);
    ui->btnAccedi->setHidden(true);
    QDate date = QDate::currentDate();//ottengo la data attuale
    ui->dBirthDate->setDate(date);//imposto come data di default la data attuale
    ui->dBirthDate->setMaximumDate(date);//imposto come data massima la data attuale, in modo da non poter inserire date future
    ui->dBirthDate->setMinimumDate(QDate::fromString("19000101", "yyyyMMdd")); //imposto come data minima l'1 gennaio 1900
    setAdminUser();//richiama il metodo per inizializzare l'utente amministratore
    checkDbExist();//richiama il metodo per controllare l'esistenza del file csv contenente i dati degli utenti registrati
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnRegistrazione_clicked()
{
    //quando viene premuto il pulsante btnRegistrazione esegue i controlli sui dati inseriti
    //e, nel caso vadano a buon fine, inserirsce l'utente nel file csv, altrimenti visualizza un messaggio di errore
    QMessageBox msgRegistrazione;
    if (checkName()) //richiama il metodo per il controllo sul nome
        if (checkSurname()) //richiama il metodo per il controllo sul cognome
            if (checkMailAdmin()) //richiama il metodo per controllare che la mail non sia quella dell'utente admin
                if (checkCellMail()) //richiama il metodo per il controllo sulla mail o sul numero di telefono
                    if (checkPassword()) //richiama il metodo per il controllo sulla password
                        if (checkBirthDate()) //richiama il metodo per il controllo sulla data di nascita
                            if (checkGender()) //richiama il metodo per il controllo sul genere
                                if(checkIfReg()) //richiama il metodo per controllare se l'utente è già registrato
                                    msgRegistrazione.setText("Utente già registrato!");
                                else{
                                    insertNewUser(); //richiama il metodo per inserire il nuovo utente nel file csv
                                    msgRegistrazione.setText("Registrazione eseguita!\nBenvenuto/a " + ui->txtNome->text() + " "+ui->txtCognome->text() +"!");
                                    clearDati(); //richiama il metodo per resettare i campi di input
                                }
                            else
                                msgRegistrazione.setText("Selezionare il genere!");
                        else
                            msgRegistrazione.setText("Per registrarsi bisogna essere maggiorenni!");
                    else
                        msgRegistrazione.setText("Il campo password non può essere vuoto e non può contenere il carattere ';'!");
                else
                    msgRegistrazione.setText("Inserire numero di telefono o e-mail in un formato corretto");
            else
                msgRegistrazione.setText("La mail admin@pas.com non può essere usata per la registrazione");
        else
            msgRegistrazione.setText("Il campo cognome non può essere vuoto e non può contenere il carattere ';'");
    else
        msgRegistrazione.setText("Il campo nome non può essere vuoto e non può contenere il carattere ';'!");

    msgRegistrazione.exec();
}



void MainWindow::on_btnToAccedi_clicked()
{
    //quando viene premuto il pulsante btnToAccedi si passa dalla visualizzazione della schermata
    //di registrazione a quella dell'accesso
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
    //quando viene premuto il pulsante btnToRegistrati si passa dalla visualizzazione della schermata
    //di accesso a quella della registrazione
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

bool MainWindow::checkName() //controllo che il campo nome non sia vuoto e non contenga ; per evitare problemi di gestione del csv
{
    if (ui->txtNome->text() == "" || ui->txtNome->text().contains(';'))
        return false;
    else
        return true;
}
bool MainWindow::checkSurname() //controllo che il campo cognome non sia vuoto e non contenga ; per evitare problemi di gestione del csv
{
    if (ui->txtCognome->text() == "" || ui->txtCognome->text().contains(';'))
        return false;
    else
        return true;
}
bool MainWindow::checkMailAdmin() //controllo che l'indirizzo mail inserito non corrisponda all'indirizzo mail dell'admin
{
    if (ui->txtCellMail->text() == admin.cellMail)
        return false;
    else
        return true;
}

bool MainWindow::checkCellMail()
{
    //controllo che il campo mail o numero di cellulare non sia vuoto e che sia in un formato corretto
    QString cellMail = ui->txtCellMail->text();
    QString nomeutente, dominio = "";
    QStringList check;
    //CONTROLLO MAIL
    //controllo che la mail sia nel formato <nomeutente>@<dominio>
    if (cellMail.contains('@') && cellMail.contains('.')){//controllo se sono prensenti @ e .

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
    //controllo che il numero inserito sia di 10 cifre, e che tutte siano cifre e non siano presenti altri tipi di carattere
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
bool MainWindow::checkPassword() //controllo che il campo password non sia vuoto e non contenga ; per evitare problemi di gestione del csv
{
    if (ui->txtPassword->text() == "" || ui->txtPassword->text().contains(';'))
        return false;
    else
        return true;
}
bool MainWindow::checkBirthDate() //controllo che l'utente sia maggiorenne
{
    QDate date = QDate::currentDate();
    QDate dataNascita = ui->dBirthDate->date();

    if (dataNascita.addYears(18)>date) //aggiungo 18 anni alla data di nascita, se è una data futura, allora l'utente non è maggiorenne
        return false;
    return true;
}
bool MainWindow::checkGender() //controllo che almeno uno dei due generi sia selezionato
{
    if (ui->rbtn_male->isChecked() || ui->rbt_female->isChecked())
        return true;
    return false;
}

void MainWindow::checkDbExist() //controllo l'esistenza del file csv
{
    QString path = QApplication::applicationDirPath()+"/db/";
    QDir dir(path);

    if (!dir.exists()) //controllo l'esistenza della directory, se non esiste la creo
        dir.mkpath(path);

    userListCsv.setFileName(path+"user_list.csv"); //imposto il nome del file, all'apertura, se non esiste il file viene creato
    userListCsv.open(QIODevice::Append);
    userListCsv.close();

}

void MainWindow::setAdminUser() //inizializzo i dati dell'utente amministratore
{
    admin.nome = "Luca";
    admin.cognome = "Scurati";
    admin.dataNascita = QDate::fromString("20000122", "yyyyMMdd");
    admin.cellMail = "admin@pas.com";
    admin.password = "admin";
    admin.genere = 'M';
}

bool MainWindow::checkIfReg() //controllo se l'utente è già registrato
{
    bool find = false;
    userListCsv.open(QIODevice::ReadOnly);
    QTextStream inputStream(&userListCsv); //apro lo stream per leggere all'interno del file
    while (!inputStream.atEnd() && !find) {
        //finchè non arriva alla fine del file e non ho gia trovato l'utente, leggo una nuova riga e effettuo il controllo
        QStringList userString = inputStream.readLine().split(';');
        if (ui->txtCellMail->text() == userString[0]) //0 perchè nel csv la mail è il primo parametro
            find = true; //se ho trovato l'utente nel csv allora setto a true il flag che indica che l'utente è già registrato
    }
    inputStream.flush();
    userListCsv.close();
    return find;
}

bool MainWindow::checkCredential()//controllo se le credenziali inserite per l'accesso sono corrette
{
    QMessageBox msgAccedi;
    user userLogin;
    bool find = false;
    userListCsv.open(QIODevice::ReadOnly);
    QTextStream inputStream(&userListCsv); //apro lo stream per leggere all'interno del file
    while (!inputStream.atEnd() && !find) {
        //finchè non arriva alla fine del file e non ho gia trovato l'utente, leggo una nuova riga e effettuo il controllo
        QStringList userString = inputStream.readLine().split(';');
        //se trovo l'utente procedo con il controllo della password
        if (ui->txtCellMail->text() == userString[0]) //0 perchè nel csv la mail è il primo parametro
        {
            //se la password è corretta visualizzo il messaggio di benvenuto, altrimenti visualizzo il messaggio di errore
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

bool MainWindow::checkIfAdmin() //controllo se l'utente ha inserito le credenziali dell'amministratore
{
    if (ui->txtCellMail->text() == admin.cellMail)
        return true;
    else
        return false;

}

void MainWindow::insertNewUser()//metodo per l'insermento di un nuovo utente
{
    //memorizzo i dati del nuovo utente
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
    QTextStream outputStream(&userListCsv);//apro lo stream per scrivere alla fine del file
    outputStream << u.cellMail << ";" << u.password << ";" << u.nome << ";" << u.cognome << ";" << u.dataNascita.toString("yyyyMMdd")<< ";" << u.genere <<"\n";
    //aggiungo una nuova riga al file con i dati dell'utente
    outputStream.flush();
    userListCsv.close();
}

void MainWindow::clearDati(){
    //setto i campi testo vuoti, inizializzo la data di nascita alla data corrente e rimuovo la scelta del genere
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

void MainWindow::on_btnRecuperaPassword_clicked()//metodo utilizzato per il reset della password
{
    QMessageBox msgRecuperoPassword;

    if (!checkCellMail()) //controllo sul formato della mail o del numero di cellulare
        msgRecuperoPassword.setText("Inserire numero di telefono o e-mail in un formato corretto");
    else{
        //se il controllo va a buon fine controllo se l'utente è registrato e, in caso positivo visualizzo
        //il messaggio di mail o sms inviati correttamente, altrimenti visualizzo il messaggio di utente non registrato
        if (checkIfReg())
            msgRecuperoPassword.setText("E-Mail/SMS di recupero password inviata/o correttamente");
        else
            msgRecuperoPassword.setText("Utente " + ui->txtCellMail->text() +" non registrato");
    }

    msgRecuperoPassword.exec();
}

void MainWindow::on_btnAccedi_clicked() //metodo utilizzato per l'accesso
{
    QMessageBox msgAccedi;
    if (checkCellMail()) //controllo sul formato della mail o del numero di cellulare
        if (checkPassword()){ //controllo sul formato della password
            if (!checkIfAdmin()){ //controllo se la mail inserita è quella dell'utente admin
                if (!checkCredential()){
                    //se l'utente non è amministratore richiamo il metodo per verificare la correttezza delle credenaizli inserite
                    msgAccedi.setText("Utente "+ ui->txtCellMail->text() +" non registrato");
                    msgAccedi.exec();
                }
            }
            else
            {//controllo password amministratore
                if (ui->txtPassword->text() == admin.password){
                    //se la password è corretta visualizzo i grafici relativi al genere e all'età
                    //richiamando il metodo setChart e passando il riferimento al file
                    AdminView aw(this);
                    aw.setModal(true);
                    this->hide();
                    aw.setChart(&userListCsv);
                    aw.exec();
                    this->show();
                }
                else
                {
                    //se la password è errata visualizzo un messaggio di errore
                    msgAccedi.setText("Password Errata!");
                    msgAccedi.exec();
                }
            }
        }
        else{
            //se il campo password è vuoto visualizzo un messaggio di errore
            msgAccedi.setText("Il campo password non può essere vuoto!");
            msgAccedi.exec();
        }
    else
    {
        //se il campo mail o cell è vuoto o non è stato inserito un'indirizzo mail o un numero di telefono valido
        //visualizzo un messaggio di errore
        msgAccedi.setText("Inserire numero di telefono o e-mail in un formato corretto");
        msgAccedi.exec();
    }
}
