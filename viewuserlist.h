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
    void setTable();
private:
    Ui::ViewUserList *ui;
    struct user{
        QString nome;
        QString cognome;
        QString password;
        QString cellMail;
        QDate dataNascita;
        QChar genere;
    };
    QList<user> userList;
};

#endif // VIEWUSERLIST_H
