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

    void on_btnToAccedi_clicked();

    void on_btnToRegistrati_clicked();

    void on_btnRecuperaPassword_clicked();

    void on_btnAccedi_clicked();

private:
    Ui::MainWindow *ui;

    QFile userListCsv;
    struct user{
        QString nome;
        QString cognome;
        QString password;
        QString cellMail;
        QDate dataNascita;
        QChar genere;
    };
    user admin;
    bool checkName();
    bool checkSurname();
    bool checkCellMail();
    bool checkPassword();
    bool checkBirthDate();
    bool checkGender();
    void checkDbExist();
    void setAdminUser();
    bool checkIfReg();
    bool checkIfAdmin();
    bool checkCredential();
    void insertNewUser();
    void clearDati();
};
#endif // MAINWINDOW_H
