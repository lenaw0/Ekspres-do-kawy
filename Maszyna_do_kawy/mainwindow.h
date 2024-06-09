#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleCoffeeSelection();
    void handleOkButton();
    void handleCleanButton();
    void handleUndoButton();
    void clearStatus();

private:
    Ui::MainWindow *ui;
    int coffeeCount;
    int waterRefillCount;
    QString lastAction;
    bool waitingForRefill;

    void updateStatus(const QString &message);

};

#endif // MAINWINDOW_H
