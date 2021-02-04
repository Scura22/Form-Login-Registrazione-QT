#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDate>
#include "adminview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnRegistrazione_clicked();
    //metodo richiamato quando si preme sul pulsante btnRegistrazione, utilizzato per registrarsi
    void on_btnToAccedi_clicked();
    //metodo richiamato quando si preme sul pulsante btnToAccedi, utilizzato per passare dalla finestra di registrazione a quella di accesso
    void on_btnToRegistrati_clicked();
    //metodo richiamato quando si preme sul pulsante btnToRegistrato, utilizzato per passare dalla finestra di accesso a quelal di registrazione
    void on_btnRecuperaPassword_clicked();
    //metodo richiamato quando si preme sul pulsante btnRecupera password, utilizzata per richiamare la procedura di recupero password
    void on_btnAccedi_clicked();
    //metodo richiamato quando si preme sul pulsante btnAccedi, utilizzato per accedere
private:
    Ui::MainWindow *ui;

    QFile userListCsv;
    struct user{//struttura dati utilizzata per memorizzare le informazioni sugli utenti
        QString nome;
        QString cognome;
        QString password;
        QString cellMail;
        QDate dataNascita;
        QChar genere;
    };
    user admin;
    bool checkName(); //metodo utilizzato per effettuare i controlli sul nome inserito
    bool checkSurname(); //metodo utilizzato per effettuare i controlli sul cognome inserito
    bool checkCellMail(); //metodo utilizzato per effettuare i controlli sulla mail o sul numero di telefono inserito
    bool checkPassword(); //metodo utilizzato per controllare se la password è stata inserita
    bool checkBirthDate(); //metodo utilizzato per controllare se l'utente è maggiorenne
    bool checkGender(); //metodo utilizzato per controllare se è stato selezionato il genere
    void checkDbExist(); //metodo utilizzato verificare l'esistenza del file csv
    void setAdminUser(); //metodo utilizzato in fase di inizializzazione per memorizzare le informazioni dell'utente amministratore
    bool checkIfReg(); //metodo utilizzato per controllare se l'utente è registrato
    bool checkIfAdmin();
    //metodo utilizzato per controllare se le credenziali inserite per l'accesso corrispondono con quelle dell'amministratore
    //in caso positivo, visualizza i grafici, altrimenti procede con la verifica delle credenziali dell'utente
    bool checkCredential(); //metodo utilizzato per effettuare i controlli sulle credenziali inserite in fase di accesso
    void insertNewUser(); //metodo utilizzato per inserire un nuovo utente all'interno del file csv
    void clearDati(); //metodo utilizzato per effettuare il reset dei campi di inserimento dei dati, utilizzato dopo una registrazione o un accesso
};
#endif // MAINWINDOW_H
