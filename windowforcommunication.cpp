#include "windowforcommunication.h"
#include "ui_windowforcommunication.h"

windowForCommunication::windowForCommunication(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::windowForCommunication)
{
    ui->setupUi(this);
    init();
    fleshIP();
}

windowForCommunication::~windowForCommunication()
{
    delete ui;
}
void windowForCommunication::init(){
    logWidget = new logwindow(this);
    auto * layout = ui->verticalLayout_8;
    layout->addWidget(logWidget);

    filesink = new fileSink("KNXRecord.log");

    connect(ui->pushButton_7,&QPushButton::clicked,this,&windowForCommunication::fleshIP);
    connect(ui->pushButton,&QPushButton::clicked,this,&windowForCommunication::search);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&windowForCommunication::connectrequest);
    connect(ui->pushButton_4,&QPushButton::clicked,this,&windowForCommunication::disconnect);
    connect(ui->pushButton_5,&QPushButton::clicked,this,&windowForCommunication::KNXValuesend);

    connect(ui->comboBox_3,&QComboBox::currentIndexChanged,ui->comboBox_4,&QComboBox::setCurrentIndex);
    connect(ui->pushButton_6,&QPushButton::clicked,this,&windowForCommunication::KNXValueread);

    connect(ui->pushButton_9,&QPushButton::clicked,this,&windowForCommunication::clearWindow);
    connect(ui->radioButton,&QRadioButton::clicked,this,&windowForCommunication::onFileSink);


    Logger::instance().addSink(new ConsoleSink());
    Logger::instance().addSink(logWidget->sink());


    // 工具栏



    auto *autoScrollCheck = ui->checkBox;
    autoScrollCheck->setChecked(true);

}


void windowForCommunication::onGetLoacalIps(QList <QHostAddress> halist){
    qDebug()<<"windowForCommunication::onGetLoacalIps********************************";
    ui->comboBox->clear();

    for(const auto ha :halist){
        if(!ha.isNull()){
            qDebug()<<"截取的IP："<<ha;
            ui->comboBox->addItem(ha.toString());
            LOG_INFO("Network", QString("local ip: %1").arg(ha.toString()));
        }
    }

}
void windowForCommunication::onGetDescription(QString description){
    qDebug()<<"windowForCommunication::onGetDescription********************************";
    if(description.isEmpty()){
        LOG_ERROR("KNX","description is empty");
    }else{
        LOG_INFO("KNX",description);
    }
    qDebug()<<QString("description is empty,true or false? %1").arg(description.isEmpty())<<description;

    QString localIP = ui->comboBox->currentText();
    if(localIP.isNull()){
        qDebug()<<"local ip is null";
    }
    QString targetIP = ui->comboBox_3->currentText();
    QList <QString> ips ;
    ips.append(localIP);
    ips.append(targetIP);
    emit sigConnect(ips);

}

void windowForCommunication::onGetConnectInfo(QString individual,quint8 channelid){
    qDebug()<<"windowForCommunication::onGetDescription********************************";
    qDebug()<<channelid;
    LOG_INFO("KNX",QString("channalid is %1,individual is %2").arg(channelid).arg(individual));

    ui->lineEdit->setText(QString::number(channelid));
    ui->lineEdit_2->setText(individual);
}
void windowForCommunication::fleshIP(){
    emit sigGetLocalIps();
}
void windowForCommunication::search(){
    QString ip = ui->comboBox->currentText();
    if(!ip.isNull()){
        emit sigSearch(ip);

    }else{
        return;
    }
    LOG_INFO("UI","search button clicked");
}
void windowForCommunication::connectrequest(){
    QString localIP = ui->comboBox->currentText();
    if(localIP.isNull()){
        qDebug()<<"local ip is null";
    }
    QString targetIP = ui->comboBox_3->currentText();
    QList <QString> ips ;
    ips.append(localIP);
    ips.append(targetIP);
    emit sigDescript(ips);
    LOG_INFO("UI","connect button clicked");
}
void windowForCommunication::onGetStatus(Appstate status ){
    QString m_status ;
    switch(status){
    case Appstate::Connected:{
         m_status = "连接成功";
        break;
    }

    case Appstate::Idle :{
        m_status = "空闲";
        break;
    }

    case Appstate::Failure :{
        m_status = "连接失败";
        break;
    }
    case Appstate::Connecting :{
        m_status = "连接中";
        break;
    }
    case Appstate::Descripting :{
        m_status = "描述中";
        break;
    }
    case Appstate::Searching :{
        m_status = "搜索中";
        break;
    }
    default:{
        m_status = "未知";
    }

    LOG_WARN("KNX ConnectState",m_status);
}


    ui->lineEdit_3->setText(m_status);
}
void windowForCommunication::descriptionrequest(){
    QString localIP = ui->comboBox->currentText();
    if(localIP.isNull()){
        qDebug()<<"local ip is null";
    }
    QString targetIP = ui->comboBox_3->currentText();
    QList <QString> ips ;
    ips.append(localIP);
    ips.append(targetIP);
    emit sigConnect(ips);
    LOG_INFO("search request","connect button clicked");
}
void windowForCommunication::disconnect(){

    emit sigdisConnect();
}

void windowForCommunication::onGetDeviceInfo(QList <DeviceInfo> infos){

    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
    for(const auto info :infos){
        ui->comboBox_3->addItem(info.address.toString());
        ui->comboBox_4->addItem(QString::number(info.port));
        LOG_INFO("KNX",info.address.toString()+" : "+QString::number(info.port));
    }
    ui->comboBox_3->addItem("224.0.23.12");
    ui->comboBox_4->addItem("3671");

}

void windowForCommunication::KNXsend(QString address,QString value){
    emit sigknxsendvalue(address,value);
}
void windowForCommunication::KNXValuesend(){
    QString address;
    QString value;

    if(ui->lineEdit_5->text().isEmpty()){
        address = "2/0/0";
    }else{address = ui->lineEdit_5->text();}
    if(ui->lineEdit_6->text().isEmpty()){
        value = "00";
    }else{value = ui->lineEdit_6->text();}
    KNXsend(address,value);
    qDebug()<<"value send clicked";
    LOG_INFO("KNX",QString("send value : address %1 , value %2").arg(address).arg(value));
}

void windowForCommunication::KNXValueread(){
    QString address;

    if(ui->lineEdit_5->text().isEmpty()){
        address = "2/0/0";
    }else{address = ui->lineEdit_5->text();}

    emit sigknxreadvalue(address);
    qDebug()<<"value read clicked";
    LOG_INFO("UI",QString("read GroupAddress value : address %1").arg(address));
}
void windowForCommunication::onGetKNXBusRecord(KnxBusRecord record){
    // LOG_INFO("KNX",QString("时间：%1 通道ID：%2 序列号：%3 MessageCode:%8 源地址：%4 目的地址：%5 操作：%6 数据长度:%9 数据：%7")
    //                     .arg(record.timestamp.toString("yyyy-MM-dd HH:mm:ss.zzz")).arg(record.channelID).arg(record.sequenceNumber)
    //                     .arg(record.sourceAddress).arg(record.destAddress).arg(record.dataType).arg(record.dataHex).arg(record.messageCode).arg( dptToString(record.datalength))
    //                                 );

    LOG_INFO("KNX",QString("通道ID：%1 序列号：%2 MessageCode:%7 源地址：%3 目的地址：%4 操作：%5 数据长度:%8 数据：%6")
                        .arg(record.channelID).arg(record.sequenceNumber)
                        .arg(record.sourceAddress).arg(record.destAddress).arg(record.dataType).arg(record.dataHex).arg(record.messageCode).arg( dptToString(record.datalength))
             );
}

void windowForCommunication::clearWindow(){
    logWidget->clearWindow();
}
void windowForCommunication::autoSave(){
    Logger::instance().addSink(filesink);
}
void windowForCommunication::disSave(){
    Logger::instance().removeSink(filesink);
}
void windowForCommunication::onFileSink(){
    if(ui->radioButton->isChecked()){
        autoSave();
    }else{
        disSave();
    }
}
