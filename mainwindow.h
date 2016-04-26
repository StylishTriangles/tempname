#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "options.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_exitButton_clicked();

    void on_newGameButton_clicked();

    void on_optionsButton_clicked();

private:
    Ui::MainWindow *ui;
    Options* opt;
};

#endif // MAINWINDOW_H
