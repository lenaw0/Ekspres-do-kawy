#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , coffeeCount(0)
    , waterRefillCount(0)
    , waitingForRefill(false)
{
    ui->setupUi(this);

    espressoButton = new QPushButton("Espresso", this);
    doppioButton = new QPushButton("Doppio", this);
    cappuccinoButton = new QPushButton("Cappuccino", this);
    cafeLatteButton = new QPushButton("Café Latte", this);
    okButton = new QPushButton("OK", this);
    cleanButton = new QPushButton("Czyszczenie", this);
    undoButton = new QPushButton("Cofnij", this);
    statusLabel = new QLabel(this);

    espressoButton->setGeometry(10, 10, 100, 50);
    doppioButton->setGeometry(120, 10, 100, 50);
    cappuccinoButton->setGeometry(230, 10, 100, 50);
    cafeLatteButton->setGeometry(340, 10, 100, 50);
    okButton->setGeometry(10, 70, 100, 50);
    cleanButton->setGeometry(120, 70, 100, 50);
    undoButton->setGeometry(230, 70, 100, 50);
    statusLabel->setGeometry(10, 130, 430, 50);

    okButton->setEnabled(false);
    cleanButton->setEnabled(false);

    connect(espressoButton, &QPushButton::clicked, this, &MainWindow::handleCoffeeSelection);
    connect(doppioButton, &QPushButton::clicked, this, &MainWindow::handleCoffeeSelection);
    connect(cappuccinoButton, &QPushButton::clicked, this, &MainWindow::handleCoffeeSelection);
    connect(cafeLatteButton, &QPushButton::clicked, this, &MainWindow::handleCoffeeSelection);
    connect(okButton, &QPushButton::clicked, this, &MainWindow::handleOkButton);
    connect(cleanButton, &QPushButton::clicked, this, &MainWindow::handleCleanButton);
    connect(undoButton, &QPushButton::clicked, this, &MainWindow::handleUndoButton);
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
        else if (coffeeType == "Cappuccino")
        {
            updateStatus("Włóż 1 kapsułkę kawy i 1 kapsułkę mleka");
        }
        else if ( coffeeType == "Café Latte")
        {
            updateStatus("Włóż 1 kapsułkę kawy i 2 kapsułki mleka");
        }

        okButton->setEnabled(true);
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
        okButton->setEnabled(false);

        if (coffeeCount >= 10)
        {
            updateStatus("Wymagane czyszczenie");
            cleanButton->setEnabled(true);
        }
        else if (coffeeCount % 4 == 0)
        {
            updateStatus("Uzupełnij wodę");
            waitingForRefill = true;
            okButton->setEnabled(true);
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
        cleanButton->setEnabled(false);

        QTimer::singleShot(3000, this, &MainWindow::clearStatus);
    });
}

void MainWindow::handleUndoButton()
{
    if (lastAction.isEmpty()) return;

    coffeeCount = qMax(0, coffeeCount - 1);
    updateStatus("Cofnięto akcję");

    okButton->setEnabled(false);
    cleanButton->setEnabled(coffeeCount >= 10);

    lastAction.clear();

    QTimer::singleShot(3000, this, &MainWindow::clearStatus);
}

void MainWindow::updateStatus(const QString &message)
{
    statusLabel->setText(message);
}

void MainWindow::clearStatus()
{
    statusLabel->clear();
}
