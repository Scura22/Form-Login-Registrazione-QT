#ifndef VIEWUSERLIST_H
#define VIEWUSERLIST_H

#include <QDialog>
#include <QDate>
#include <QFile>
#include <QTextStream>
namespace Ui {
class ViewUserList;
}

class ViewUserList : public QDialog
{
    Q_OBJECT

public:
    explicit ViewUserList(QWidget *parent = nullptr);
    ~ViewUserList();

    void setList(QFile *csv);
    //metodo richiamato dalla classe AdminView utilizzato per inizializzare la lista di utenti e passare il riferimento al file csv
    void setTable();
    //metodo che inizializza la tabella contenente i dati di tutti gli utenti iscritti
private:
    Ui::ViewUserList *ui;
    struct user{//struttura dati utilizzata per memorizzare le informazioni sugli utenti
        QString nome;
        QString cognome;
        QString password;
        QString cellMail;
        QDate dataNascita;
        QChar genere;
    };
    QList<user> userList;
    //lista utilizzata per memorizzare i dati di tutti gli utenti iscritti salvati nel csv
};

#endif // VIEWUSERLIST_H
