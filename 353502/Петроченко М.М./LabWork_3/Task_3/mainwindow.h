#ifndef LAB3_TASK3_MAINWINDOW_H_
#define LAB3_TASK3_MAINWINDOW_H_

#include <QDebug>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "converter.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QWidget {
    Q_OBJECT

   public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

   private:
    QLineEdit* line_edit;
    QLabel* label;
    QPushButton* button;
    QIntValidator* validator;
    QVBoxLayout* vbox;
    Ui::MainWindow* ui;
   private slots:
    void doTask();
};


#endif  //LAB3_TASK3_MAINWINDOW_H_
