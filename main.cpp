#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();

    int ret = a.exec();   // 事件循环结束，准备退出

    // ✅ 关键：在 QApplication 销毁前，先让所有 Sink 停止工作
    Logger::instance().stopALL();

    return ret;
}
