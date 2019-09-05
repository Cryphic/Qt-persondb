#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_tallenna_clicked();

    void on_tuo_clicked();

    void on_lisaa_tauluun_clicked();

    void on_tyhjenna_lista_clicked();

    void on_tuo_kanta_clicked();

    void on_tallenna_kantaan_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
