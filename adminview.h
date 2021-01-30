#ifndef ADMINVIEW_H
#define ADMINVIEW_H

#include <QDialog>
#include <QtCharts>
#include <QPieSeries>
#include <QPieSlice>
#include "viewuserlist.h"
#include <QTextStream>
namespace Ui {
class AdminView;
}

class AdminView : public QDialog
{
    Q_OBJECT

public:
    explicit AdminView(QWidget *parent = nullptr);
    ~AdminView();

    void setChart(QFile *csv);
    void setGenderChart();
    void setAgeChart();

private slots:
    void on_btnViewTabel_clicked();

private:
       Ui::AdminView *ui;
       struct user{
           QString nome;
           QString cognome;
           QString password;
           QString cellMail;
           QDate dataNascita;
           QChar genere;
       };
       //variabile per genere
       int m = 0;
       int f = 0;
       //variabile per età
       int over54 = 0;
       int under26 = 0;
       int bet27_35 = 0;
       int bet36_44 = 0;
       int bet45_53 = 0;
        QFile *userListCsv;

};

#endif // ADMINVIEW_H
