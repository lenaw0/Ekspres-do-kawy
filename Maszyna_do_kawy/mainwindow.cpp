#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , coffeeCount(0)
    , waterRefillCount(0)
    , waitingForRefill(false)
{
    ui->setupUi(this);


    connect(ui->espressoButton, &QPushButton::clicked, this, &MainWindow::handleCoffeeSelection);
    connect(ui->caffeLatteButton, &QPushButton::clicked, this, &MainWindow::handleCoffeeSelection);
    connect(ui->cappuccinoButton, &QPushButton::clicked, this, &MainWindow::handleCoffeeSelection);
    connect(ui->doppioButton, &QPushButton::clicked, this, &MainWindow::handleCoffeeSelection);
    connect(ui->okButton, &QPushButton::clicked, this, &MainWindow::handleOkButton);
    connect(ui->cleanButton, &QPushButton::clicked, this, &MainWindow::handleCleanButton);
    connect(ui->undoButton, &QPushButton::clicked, this, &MainWindow::handleUndoButton);

    ui->okButton->setEnabled(false);
    ui->cleanButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleCoffeeSelection()
{
    if (waitingForRefill) return;

    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QString coffeeType = button->text();
        if (coffeeType == "Espresso")
        {
            updateStatus("Włóż 1 kapsułkę kawy");
        }
        else if(coffeeType == "Doppio")
        {
            updateStatus("Włóż 2 kapsułki kawy");
        }
        else if (coffeeType == "Cafe Latte")
        {
            updateStatus("Włóż 1 kapsułkę kawy i 2 kapsułki mleka");
        }
        else if (coffeeType == "Cappuccino")
        {
            updateStatus("Włóż 1 kapsułkę kawy i 1 kapsułkę mleka");
        }

        ui->okButton->setEnabled(true);
        lastAction = coffeeType;
    }
}

void MainWindow::handleOkButton()
{
    if (waitingForRefill)
    {
        waitingForRefill = false;
        updateStatus("Gotowe");
        QTimer::singleShot(3000, this, &MainWindow::clearStatus);
        return;
    }

    updateStatus("Kawa w przygotowaniu");
    QTimer::singleShot(4000, [this] {
        coffeeCount++;
        ui->okButton->setEnabled(false);

        if (coffeeCount >= 10)
        {
            updateStatus("Wymagane czyszczenie");
            ui->cleanButton->setEnabled(true);
        }
        else if (coffeeCount % 4 == 0)
        {
            updateStatus("Uzupełnij wodę");
            waitingForRefill = true;
            ui->okButton->setEnabled(true);
        }
        else
        {
            updateStatus("Gotowe");
            QTimer::singleShot(3000, this, &MainWindow::clearStatus);
        }
    });
}

void MainWindow::handleCleanButton()
{
    updateStatus("Czyszczenie...");
    QTimer::singleShot(1000, [this] {
        updateStatus("Gotowe");
        coffeeCount = 0;
        ui->cleanButton->setEnabled(false);

        QTimer::singleShot(3000, this, &MainWindow::clearStatus);
    });
}

void MainWindow::handleUndoButton()
{
    if (lastAction.isEmpty()) return;

    coffeeCount = qMax(0, coffeeCount - 1);
    updateStatus("Cofnięto akcję");

    ui->okButton->setEnabled(false);
    ui->cleanButton->setEnabled(coffeeCount >= 10);

    lastAction.clear();

    QTimer::singleShot(3000, this, &MainWindow::clearStatus);
}

void MainWindow::updateStatus(const QString &message)
{
    ui->label->setText(message);
}

void MainWindow::clearStatus()
{
    ui->label->clear();
}
