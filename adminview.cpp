#include "adminview.h"
#include "ui_adminview.h"
#include "mainwindow.h"
AdminView::AdminView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminView)
{
    ui->setupUi(this);
    this->setFixedSize(600,600); //imposta la dimensione fissa della finestra

}

AdminView::~AdminView()
{
    delete ui;
}

void AdminView::setChart(QFile *csv){

    userListCsv = csv; //salva il riferimento al file csv

    user u;

    csv->open(QIODevice::ReadOnly);
    QTextStream inputStream(csv); //inizializzo lo stream per leggere dal file csv
    while (!inputStream.atEnd()) { //finchè non arriva alla fine del file legge i dati degli utenti registrati
        QStringList userString = inputStream.readLine().split(';'); //crea una lista di dati leggendo riga per riga il csv, con separatore ;
        u.cellMail = userString[0]; //0 perchè nel csv la mail / il numero di telefono è il primo parametro
        u.password = userString[1]; //1 perchè nel csv la password è il secondo parametro
        u.nome = userString[2]; //2 perchè nel csv il nome è il terzo parametro
        u.cognome = userString[3]; //3 perchè nel csv il cognome è il quarto parametro
        u.dataNascita = QDate::fromString(userString[4], "yyyyMMdd"); //4 perchè nel csv la data di nascita è il quinto parametro
        u.genere = static_cast<QChar>(userString[5][0]);//5 perchè nel csv il genere è il sesto parametro

        //incremento contarore genere
        if (u.genere == 'M') //se l'utente è maschio incremento il contatore dei maschi
            m++;
        else
            if (u.genere == 'F')//se l'utente è maschio incremento il contatore delle femmine
                f++;
        //incremento contatore età
        QDate date = QDate::currentDate(); //salvo la data attuale per controllare la fascia d'età dell'utente

        //in base all'età, incremento il relativo contatore
        if (u.dataNascita.addYears(54)<=date)
            over54++;
        else
            if (u.dataNascita.addYears(45)<=date)
                bet45_53++;
            else
                if (u.dataNascita.addYears(36)<=date)
                    bet36_44++;
                else
                    if (u.dataNascita.addYears(27)<=date)
                        bet27_35++;
                    else
                        under26++;

    }
    //chiudo il file
    inputStream.flush();
    csv->close();

    setGenderChart(); //richiama il metodo che esegue il set del grafico per genere
    setAgeChart(); //richiama il metodo che esegue il set del grafico per età
}


void AdminView::setGenderChart(){
    QPieSeries *seriesGender = new QPieSeries(); //crea una serie per il pie chart
    //inizializzo  dati che vanno a creare il grafico
    seriesGender->append("Maschi", m);
    seriesGender->append("Femmine", f);

    seriesGender->slices().at(0)->setLabel("Maschi, " + QString::number(m));
    seriesGender->slices().at(1)->setLabel("Femmine, " + QString::number(f));

    //creo il grafico e gli assegno la serie e il nome
    QChart *chartGender = new QChart();
    chartGender->addSeries(seriesGender);
    chartGender->setTitle("Grafico per Genere");
    //imposto il parent e la dimensione del grafico
    ui->pieChartGender->setFixedSize(580,273);
    QChartView *chartGenderview = new QChartView(chartGender);
    chartGenderview->setParent(ui->pieChartGender);
    chartGenderview->setFixedSize(ui->pieChartGender->width(),ui->pieChartGender->height());
}

void AdminView::setAgeChart(){
    QPieSeries *seriesAge = new QPieSeries();//crea una serie per il pie chart
    //inizializzo  dati che vanno a creare il grafico
    seriesAge->append("18-26", under26);
    seriesAge->append("27-35", bet27_35);
    seriesAge->append("36-44", bet36_44);
    seriesAge->append("45-53", bet45_53);
    seriesAge->append("54+", over54);

    seriesAge->slices().at(0)->setLabel("18-26, " + QString::number(under26));
    seriesAge->slices().at(1)->setLabel("27-35, " + QString::number(bet27_35));
    seriesAge->slices().at(2)->setLabel("36-44, " + QString::number(bet36_44));
    seriesAge->slices().at(3)->setLabel("45-53, " + QString::number(bet45_53));
    seriesAge->slices().at(4)->setLabel("54+, " + QString::number(over54));
    //creo il grafico e gli assegno la serie e il nome
    QChart *chartAge = new QChart();
    chartAge->addSeries(seriesAge);
    chartAge->setTitle("Grafico per Età");
    //imposto il parent e la dimensione del grafico
    ui->pieChartAge->setFixedSize(580,273);
    QChartView *chartAgeview = new QChartView(chartAge);
    chartAgeview->setParent(ui->pieChartAge);

    chartAgeview->setFixedSize(ui->pieChartAge->width(),ui->pieChartAge->height());
}

void AdminView::on_btnViewTabel_clicked()
{
    //al click del pulsante btnViewTabel nascondo la visualizzazione dei grafici e visualizzo quella
    //dell'elenco degli utenti, richiamando il metodo setList e passando il riferimento al file
    ViewUserList vul(this);
    vul.setModal(true);
    this->hide();
    vul.setList(userListCsv);
    vul.exec();
    this->show();
}
