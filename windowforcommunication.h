#ifndef WINDOWFORCOMMUNICATION_H
#define WINDOWFORCOMMUNICATION_H

#include "basicdatatype.h"
#include <QWidget>
#include <QHostAddress>
#include <QTime>
#include <QDateTime>
#include <QCheckBox>
#include <QPlainTextEdit>
#include "LOGWINDOW.h"
#include "LOGRECORD.h"
#include <MYLOGGER.h>
#include "CONSOLESINK.h"
#include "FILESINK.h"
#include "WINDOWSINK.h"

namespace Ui {
class windowForCommunication;
}

enum class MessageLevel;

class windowForCommunication : public QWidget
{
    Q_OBJECT


public:
    explicit windowForCommunication(QWidget *parent = nullptr);
    ~windowForCommunication();

    void init();
    void fleshIP();
    void search();
    void descriptionrequest();
    void connectrequest();
    void disconnect();
    void KNXsend(QString address,QString value);
    void KNXask();
    void KNXValuesend();
    void KNXValueread();

    void clearWindow();
    void autoSave();
    void disSave();

public slots:
    void onGetLoacalIps(QList <QHostAddress> halist);
    void onGetDeviceInfo(QList <DeviceInfo>);
    void onGetDescription(QString description);
    void onGetConnectInfo(QString individual,quint8 channelid);
    void onGetStatus(Appstate);

    void onGetKNXBusRecord(KnxBusRecord);
    void onFileSink();
private:

    Ui::windowForCommunication *ui;
    logwindow * logWidget;
    fileSink * filesink;
signals:
    void sigGetLocalIps();
    void sigSearch(QString ip);
    void sigDescript(QList <QString> ip);
    void sigConnect(QList <QString> ip);
    void sigdisConnect();
    void sigknxsendvalue(QString address,QString value);
    void sigknxreadvalue(QString address);



};

#endif // WINDOWFORCOMMUNICATION_H
