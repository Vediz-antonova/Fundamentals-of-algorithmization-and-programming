#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QScrollBar>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->lineEdit, &QLineEdit::returnPressed, &loop, &QEventLoop::quit);

    QTimer::singleShot(50, [&]{
        printMenu();
    });

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    scrollArea->setWidget(ui->label);

    ui->gridLayout->removeWidget(ui->label);
    ui->gridLayout->addWidget(scrollArea, 0, 0);

    Qt::WindowFlags flags = Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::CustomizeWindowHint;
    setWindowFlags(flags);

    setFixedSize(size());
    setWindowTitle("Задание 2");
}

MainWindow::~MainWindow()
{
    delete ui;
    QApplication::quit();
}

QString MainWindow::input()
{
    loop.exec();
    QString ret = ui->lineEdit->text();
    ui->lineEdit->clear();
    ui->label->setText(ui->label->text() + "\n");
    return ret;
}

void MainWindow::print(QString newStr)
{
    ui->label->setText(ui->label->text() + newStr);
}

struct Ship {
    int shipId;
    double shipLength;
    char shipType;
    bool isDocked;
    static const int maxNameLength = 20;
    char shipName[maxNameLength];
    int crewSize;
};

void MainWindow::printMenu()
{
    bool isRunning = true;
    while (isRunning) {
        print("\nМеню:\n"
              "1. Записать параграф в файл\n"
              "2. Прочитать и распечатать параграф из файла\n"
              "3. Записать массив в файл\n"
              "4. Прочитать и распечатать массив из файла\n"
              "5. Записать структуру в файл\n"
              "6. Прочитать и распечатать структуры из файла\n"
              "7. Записать предложение в файл\n"
              "8. Прочитать и распечатать предложения из файла\n"
              "9. Записать структуру в бинарный файл\n"
              "10. Прочитать и распечатать структуры из бинарного файла\n"
              "11. Выход\n"
              "Введите ваш выбор:\n");
        switch (input().toInt()) {
        case 1:
            writeParagraphToFile(paragraphFile);
            break;
        case 2:
            readAndPrintParagraphFromFile(paragraphFile);
            break;
        case 3:
            writeArrayToFile(arrayFile, '-');
            break;
        case 4:
            readAndPrintArrayFromFile(arrayFile, '-');
            break;
        case 5:
            writeStructToFile(structFile);
            break;
        case 6:
            readAndPrintStructsFromFile(structFile);
            break;
        case 7:
            writeSentenceToFile(sentenceFile);
            break;
        case 8:
            readAndPrintSentencesFromFile(sentenceFile);
            break;
        case 9:
            saveShipToBinaryFile(binaryFile);
            break;
        case 10:
            printShipsFromBinaryFile(binaryFile);
            break;
        case 11:
            print("Завершение программы...\n");
            isRunning = false;
            break;
        default:
            print("Неверный выбор. Пожалуйста, введите число от 1 до 11.\n");
        }
    }
    QApplication::quit();
}

void MainWindow::writeParagraphToFile(const std::string &filename)
{
    std::wofstream file(filename, std::ios::out);
    if (!file) {
        QMessageBox::critical(this, "Ошибка!", "Ошибка открытия файла!");
        return;
    }
    print("Введите параграф (нажмите Enter для завершения):\n");
    QString paragraph = input();
    file << paragraph.toStdWString();
    print("Параграф введён!\n");
    file.close();
}

void MainWindow::readAndPrintParagraphFromFile(const std::string &filename)
{
    std::wifstream file(filename, std::ios::in);
    if (!file) {
        QMessageBox::critical(this, "Ошибка!", "Ошибка открытия файла!");
        return;
    }
    QString fileName(filename.c_str());
    print(QString("Параграф из файла " + fileName + ":" + "\n"));
    wchar_t ch;
    while (file.get(ch)) {
        print(QString(QChar(ch)));
    }
    file.close();
}

void MainWindow::writeArrayToFile(const std::string &filename, char delimiter)
{
    int size;
    print("Введите размер массива:\n");
    size = input().toInt();

    std::ofstream file(filename, std::ios::out);
    if (!file) {
        QMessageBox::critical(this, "Ошибка!", "Ошибка открытия файла!");
        return;
    }

    print("Введите элементы массива, разделенные '" + QString(delimiter) + "': ");
    QString inp = input();
    for (int i = 0; i < size; ++i) {
        char value;
        value = inp[2 * i].toLatin1();
        file << value;
        if (i != size - 1) {
            file << delimiter;
        }
    }

    file.close();

    print("Массив введён!");
}

void MainWindow::readAndPrintArrayFromFile(const std::string &filename, char delimiter)
{
    std::ifstream file(filename, std::ios::in);
    if (!file) {
        QMessageBox::critical(this, "Ошибка!", "Ошибка открытия файла!");
        return;
    }
    QString fileName(filename.c_str());
    print(QString("Массив из файла " + fileName + ":" + "\n"));
    char value;
    while (file.get(value)) {
        if (value != delimiter) {
            print(QString(value) + " ");
        }
    }

    file.close();
}

void MainWindow::writeStructToFile(const std::string &filename)
{
    std::ofstream file(filename, std::ios::app);
    if (!file) {
        QMessageBox::critical(this, "Ошибка!", "Ошибка открытия файла!");
        return;
    }

    Ship obj;
    print("Введите индентификационный номер самолета: ");
    obj.shipId = input().toInt();
    print("Введите массу самолета: ");
    obj.shipLength = input().toDouble();
    print("Введите тип самолета: ");
    obj.shipType = input()[0].toLatin1();
    print("Обслужен ли самолет? (1 - да, 0 - нет): ");
    obj.isDocked = input().toInt();
    print("Введите имя самолета (не более " + QString::number(Ship::maxNameLength) + " символов): ");
    QString shipName = input();
    strncpy(obj.shipName, shipName.toStdString().c_str(), Ship::maxNameLength - 1);
    obj.shipName[Ship::maxNameLength - 1] = '\0'; // Ensure null termination
    print("Введите размер экипажа: ");
    obj.crewSize = input().toInt();

    file << obj.shipId << " " << obj.shipLength << " " << obj.shipType << " " << obj.isDocked << " " << obj.shipName << " " << obj.crewSize << "\n";

    print("Структура записана в файл!\n");
    file.close();
}

void MainWindow::readAndPrintStructsFromFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::in);
    if (!file) {
        QMessageBox::critical(this, "Ошибка!", "Ошибка открытия файла!");
        return;
    }
    QString fileName(filename.c_str());
    print(QString("Структуры из файла " + fileName + ":" + "\n"));

    Ship obj;
    while (file >> obj.shipId >> obj.shipLength >> obj.shipType >> obj.isDocked >> obj.shipName >> obj.crewSize) {
        print(QString("Идентификационный номер самолета: ") + QString::number(obj.shipId) + "\n");
        print(QString("Масса самолета: ") + QString::number(obj.shipLength) + "\n");
        print(QString("Тип самолета: ") + QChar(obj.shipType) + "\n");
        print(QString("Обслужен: ") + (obj.isDocked ? "Да" : "Нет") + "\n");
        print(QString("Имя самолета: ") + obj.shipName + "\n");
        print(QString("Размер экипажа: ") + QString::number(obj.crewSize) + "\n\n");
    }

    file.close();
}

void MainWindow::writeSentenceToFile(const std::string &filename)
{
    std::ofstream file(filename, std::ios::app);
    if (!file) {
        QMessageBox::critical(this, "Ошибка!", "Ошибка открытия файла!");
        return;
    }

    print("Введите предложение: ");
    QString sentence = input();
    file << sentence.toStdString() << "\n";

    print("Предложение записано в файл!\n");
    file.close();
}

void MainWindow::readAndPrintSentencesFromFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::in);
    if (!file) {
        QMessageBox::critical(this, "Ошибка!", "Ошибка открытия файла!");
        return;
    }
    QString fileName(filename.c_str());
    print(QString("Предложения из файла " + fileName + ":" + "\n"));

    std::string sentence;
    while (std::getline(file, sentence)) {
        print(QString::fromStdString(sentence) + "\n");
    }

    file.close();
}

void MainWindow::saveShipToBinaryFile(const std::string &filename)
{
    std::ofstream file(filename, std::ios::binary | std::ios::out);
    if (!file) {
        QMessageBox::critical(this, "Ошибка!", "Ошибка открытия файла!");
        return;
    }

    Ship obj;
    print("Введите индентификационный номер самолета: ");
    obj.shipId = input().toInt();
    print("Введите массу самолета: ");
    obj.shipLength = input().toDouble();
    print("Введите тип самолета: ");
    obj.shipType = input()[0].toLatin1();
    print("Обслужен ли самолет? (1 - да, 0 - нет): ");
    obj.isDocked = input().toInt();
    print("Введите имя самолета (не более " + QString::number(Ship::maxNameLength) + " символов): ");
    QString shipName = input();
    strncpy(obj.shipName, shipName.toStdString().c_str(), Ship::maxNameLength - 1);
    obj.shipName[Ship::maxNameLength - 1] = '\0'; // Ensure null termination
    print("Введите размер экипажа: ");
    obj.crewSize = input().toInt();

    file.write(reinterpret_cast<char*>(&obj), sizeof(Ship));

    print("Структура записана в бинарный файл!\n");
    file.close();
}

void MainWindow::printShipsFromBinaryFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::in);
    if (!file) {
        QMessageBox::critical(this, "Ошибка!", "Ошибка открытия файла!");
        return;
    }
    QString fileName(filename.c_str());
    print(QString("Структуры из бинарного файла " + fileName + ":" + "\n"));

    Ship obj;
    while (file.read(reinterpret_cast<char*>(&obj), sizeof(Ship))) {
        print(QString("Идентификационный номер самолета: ") + QString::number(obj.shipId) + "\n");
        print(QString("Масса самолета: ") + QString::number(obj.shipLength) + "\n");
        print(QString("Тип самолета: ") + QChar(obj.shipType) + "\n");
        print(QString("Обслужен: ") + (obj.isDocked ? "Да" : "Нет") + "\n");
        print(QString("Имя самолета: ") + QString::fromUtf8(obj.shipName) + "\n");
        print(QString("Размер экипажа: ") + QString::number(obj.crewSize) + "\n\n");
    }

    file.close();
}
