#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
using namespace std;
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "kurli/include/curl/curl.h"
#pragma comment(lib, "/kurli/lib/libcurl.lib")




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tallenna_clicked()
{
    int c = ui->listWidget->count(); /* hae tavaroitten lukumäärä listassa */
    std::ofstream hlo("hlot.txt"); /*  avaa hlot.txt */
    for (int i = 0; i < c; i++) {
         hlo << ui->listWidget->item(i)->text().toStdString() << "\n";
}
     QMessageBox::information(this,tr("Ilmoitus"),tr("Tallennettu"));
}

void MainWindow::on_tuo_clicked()
{

        QFile tiedosto("hlot.txt");

        if(!tiedosto.open(QIODevice::ReadOnly | QIODevice::Text))
            return; /* jos tiedosto ei ole auki palaa */

        QTextStream in(&tiedosto);
        /* looppaa kunnes tiedosto loppuu*/
        while(!in.atEnd())
        {
            QString line = in.readLine();
             ui->listWidget->addItem(line);
        }
 }


void MainWindow::on_lisaa_tauluun_clicked()
{
    if(ui->nimi->text() == "" || ui->harrastus->text() == ""|| ui->nro->text() == "") { QMessageBox::critical(this,tr("virhe"),tr("Yksi tai useampi kenttä on tyhjä")); return;}
    /* Lisää tekstikentät nimi + harrastus + nro listaan */
    ui->listWidget->addItem(ui->nimi->text() + " " + ui->harrastus->text() + " " + ui->nro->text());

}

void MainWindow::on_tyhjenna_lista_clicked()
{
    ui->listWidget->clear();
}

/* koko yhdelle tavaralle, nmemb määrä tavaralle */

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
 size_t written;
 written = fwrite(ptr, size, nmemb, stream);
 return written;
}

void MainWindow::on_tuo_kanta_clicked()
{
  //  string nimi = ui->tunnus->text().toStdString() + ":";
    //string salasana = ui->salasana->text().toStdString();
    string tunnukset = ui->tunnus->text().toStdString() + ":" + ui->salasana->text().toStdString();
    CURL* curl;
    FILE* fp;
    CURLcode res;
long http_code = 0;
    curl = curl_easy_init();
    if (curl) {
      fp = fopen("hlot.txt", "wb"); /*wb tee tiedosto kirjoittamiseen*/
      curl_easy_setopt(curl, CURLOPT_URL, "http://cryphic.gq/lataa.php");
      curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
      curl_easy_setopt(curl, CURLOPT_USERPWD, tunnukset.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                       write_data); /*Lähetä kaikki data write_data funktioon*/
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp); /*kirjoita data tiedostoon*/
      res = curl_easy_perform(curl);
      curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
      if (http_code == 200 && res != CURLE_ABORTED_BY_CALLBACK)
      {
                QMessageBox::information(this,tr("Ilmoitus"),tr("Hlot.txt ladattu."));
                curl_easy_cleanup(curl); /*siivoa curl jäljet*/
                fclose(fp);              /*sulje datatiedosto*/
      }
      else
      {
              QMessageBox::critical(this,tr("Ilmoitus"),tr("Palvelin on alhaalla tai käyttäjänimi ja salasana on väärin."));
              return;
      }


    }

}

void MainWindow::on_tallenna_kantaan_clicked()
{
    int maara = 0;
     string c;
     ifstream hlo;

     hlo.open("hlot.txt");
     while (getline(hlo, c)) {
       maara++;
     }
     hlo.close();
     if (maara == 0) {
         QMessageBox::critical(this,tr("virhe"),tr("Hlot.txt on tyhjä"));
       return;
     }


     hlo.open("hlot.txt");
     CURL* curl;
     CURLcode res;
     for (int i = 0; i < maara; i++) {
         string nimi, harrastus;   //määritetään nimi,harr,nro
         int nro;
       hlo >> nimi >> harrastus >> nro; //luetaan niihin txt tiedostosta linja kerrallaan

       curl = curl_easy_init();
       if (curl) {
         string url = "http://cryphic.gq/connect.php?nimi=" + nimi +
                      "&harrastus=" + harrastus + "&nro=" + to_string(nro);

         /*CURL ei halua std::string koska C kirjasto*/
         curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
         /* Käsketään libcurlia seuraamaan osoitetta*/
         curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

         /* Suorita pyyntö */
         res = curl_easy_perform(curl);
         /* Tarkista virheet */
         if (res != CURLE_OK)
            QMessageBox::critical(this,tr("virhe"),tr("Curl virhe"));

         curl_easy_cleanup(curl);
       }
    }
     QMessageBox::information(this,tr("Ilmoitus"),tr("Hlot.txt viety"));
}
