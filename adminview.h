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
    //metodo richiamato dalla classe MainWindow utilizzato per inizializzare il grafico e passare il riferimento al file csv
    void setGenderChart();
    //metodo che inizializza il grafico per le statistiche dei generi
    void setAgeChart();
    //metodo che inizializza il grafico per le statistiche dell'età
private slots:
    void on_btnViewTabel_clicked();
    //metodo richiamato al click del pulsante btnViewTabel per visualizzare l'elenco degli iscritti

private:
       Ui::AdminView *ui;
       struct user{ //struttura dati utilizzata per memorizzare i dati relativi agli utenti
           QString nome;
           QString cognome;
           QString password;
           QString cellMail;
           QDate dataNascita;
           QChar genere;
       };
       //variabili contatore per genere
       int m = 0;
       int f = 0;
       //variabili contratore per età
       int over54 = 0;
       int under26 = 0;
       int bet27_35 = 0;
       int bet36_44 = 0;
       int bet45_53 = 0;
       QFile *userListCsv; //variabile per memorizzare il riferimento al file csv

};

#endif // ADMINVIEW_H
