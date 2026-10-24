#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "windowforcommunication.h"
#include <QCloseEvent>
#include "QThread"
#include "QStackedWidget"
#include "myknxnetwork.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void init();

    void signalConToSlot();

protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void sigLocalIP();

private:
    Ui::MainWindow *ui;

    myKNXnetwork * knxnewwork;

    windowForCommunication * windows;
    QThread * networkthread;
    QStackedWidget * stackedWidget;



};
#endif // MAINWINDOW_H
