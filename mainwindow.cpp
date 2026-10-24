#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windowforcommunication.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    networkthread = new QThread(this);
    knxnewwork=new myKNXnetwork();
    knxnewwork->moveToThread(networkthread);
    connect(networkthread, &QThread::started, knxnewwork, &myKNXnetwork::onNetworkInfo);
    // 清理：销毁线程时回收 Worker
    connect(networkthread, &QThread::finished, knxnewwork, &QObject::deleteLater);

    networkthread->start();


    init();

}

MainWindow::~MainWindow()
{
    if (networkthread && knxnewwork) {
        disconnect(networkthread, nullptr, knxnewwork, nullptr);
        disconnect(knxnewwork, nullptr, this, nullptr);
    }

    // 2. 请求子线程退出，并死等
    if (networkthread) {
        networkthread->quit();
        if (!networkthread->wait(3000)) {
            networkthread->terminate(); // 兜底
            networkthread->wait(1000);
        }
    }

    // 3. 子线程已经彻底死了，现在可以安全释放 DLL 对象了
    if (knxnewwork) {
        delete knxnewwork;   // ✅ 安全
        knxnewwork = nullptr;
    }

    // 4. 释放线程对象
    if (networkthread) {
        delete networkthread;
        networkthread = nullptr;
    }

    delete ui;
}
void MainWindow::init(){
    qDebug()<<"init ";

    QWidget * centralWid = ui->centralwidget;

    stackedWidget = new QStackedWidget(this);

    windows = new windowForCommunication(this);


    int userfaceIndex = stackedWidget->addWidget(windows);
    qDebug()<<"userfaceIndex: "<<userfaceIndex;

    stackedWidget->setCurrentIndex(0);
    setCentralWidget(stackedWidget);


    signalConToSlot();

}
void MainWindow::signalConToSlot(){
    connect(windows,&windowForCommunication::sigGetLocalIps,knxnewwork,&myKNXnetwork::onNetworkInfo);
    connect(knxnewwork,&myKNXnetwork::sigLocalIpaddress,windows,&windowForCommunication::onGetLoacalIps);

    connect(windows,&windowForCommunication::sigSearch,knxnewwork,&myKNXnetwork::onsearchrequest);
    connect(knxnewwork,&myKNXnetwork::sigfindedDeviceInfo,windows,&windowForCommunication::onGetDeviceInfo);

    connect(windows,&windowForCommunication::sigDescript,knxnewwork,&myKNXnetwork::ondescriptionrequest);
    connect(knxnewwork,&myKNXnetwork::sigDetailInfo,windows,&windowForCommunication::onGetDescription);

    connect(windows,&windowForCommunication::sigConnect,knxnewwork,&myKNXnetwork::onconnectrequest);
    connect(knxnewwork,&myKNXnetwork::sigconnectInfo,windows,&windowForCommunication::onGetConnectInfo);

    connect(knxnewwork,&myKNXnetwork::sigstate,windows,&windowForCommunication::onGetStatus);

    connect(windows,&windowForCommunication::sigdisConnect,knxnewwork,&myKNXnetwork::ondisconnectrequest);

    connect(windows,&windowForCommunication::sigknxsendvalue,knxnewwork,&myKNXnetwork::tunnelRequest_KNXWrite);
    connect(windows,&windowForCommunication::sigknxreadvalue,knxnewwork,&myKNXnetwork::tunnelRequest_KNXRead);

    connect(knxnewwork,&myKNXnetwork::sigKNXBusMessageReceived,windows,&windowForCommunication::onGetKNXBusRecord);



}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "networkThread:" << networkthread;
    // 假设你的 QThread 指针是 m_thread
    // if (networkthread && networkthread->isRunning()) {
    //     qDebug()<<"it is me";
    //     // 方式一：阻塞等待清理完成（推荐，确保清理干净）
    //     // QMetaObject::invokeMethod(knxnewwork, "m_cleanup", Qt::BlockingQueuedConnection);


    //     // 1. 请求线程退出事件循环
    //     networkthread->quit();
    //     // 2. 请求线程中断当前阻塞的操作（如果你的 run() 里有循环）
    //     networkthread->requestInterruption();
    //     // 3. 阻塞主线程，等待子线程真正运行结束（最多等3秒）
    //     if (!networkthread->wait(3000)) {
    //         // 如果3秒后还没结束，只能强制终止（不推荐，但能兜底）

    //         networkthread->terminate();
    //         networkthread->wait();
    //     }
    // }
    event->accept();
}

