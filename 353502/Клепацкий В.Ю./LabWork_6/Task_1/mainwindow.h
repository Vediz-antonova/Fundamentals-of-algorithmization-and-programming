#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegularExpressionValidator>
#include <QValidator>
#include "Kinoserial.h"
#include "dialoginputdata.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

public slots:
    void printInfoAboutNew();

private:
    Ui::MainWindow *ui;
    DialogInputData dialog;
};
#endif // MAINWINDOW_H
