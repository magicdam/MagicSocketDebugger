#include "websocket/wsclient.h"
#include <QTextCodec>
#include <QMessageBox>
#include <QTimer>
#include <mainwindow.h>
#include <QDebug>

WsClient::WsClient(QTreeWidgetItem *qTreeWidgetItem,QGridLayout *qGridLayoutParent):Connection(qGridLayoutParent, false)
{
    this->qTreeWidgetItem=qTreeWidgetItem;

    qVBoxLayoutLeft = new QVBoxLayout(qWidgetLeft);
    qVBoxLayoutLeft->setAlignment(Qt::AlignTop);
    qLabel1= new QLabel();
    qLabel1->setText("服务器地址：");
    qVBoxLayoutLeft->addWidget(qLabel1);
    serverAddressInput=new QLineEdit();
    serverAddressInput->setPlaceholderText("如127.0.0.1");
    qVBoxLayoutLeft->addWidget(serverAddressInput);
    qLabel2 = new QLabel();
    qLabel2->setText("端口：");
    qVBoxLayoutLeft->addWidget(qLabel2);
    serverPortInput=new QLineEdit();
    qVBoxLayoutLeft->addWidget(serverPortInput);
    connectButton=new QPushButton();
    connectButton->setText("连接");
    qVBoxLayoutLeft->addWidget(connectButton);

    connect(connectButton, SIGNAL(clicked()), this, SLOT(on_connectButton_clicked()));
    connect(serverAddressInput, SIGNAL(returnPressed()), this, SLOT(click_connectButton()));
    connect(serverPortInput, SIGNAL(returnPressed()), this, SLOT(click_connectButton()));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(on_sendButton_clicked()));
    connect(pingCheckBox, SIGNAL(stateChanged(int)), this, SLOT(on_pingCheckBox_stateChanged(int)));
    connect(clearReceiveButton, SIGNAL(clicked()), this, SLOT(on_receiveClearButton_clicked()));

    qWidget->show();
}

WsClient::~WsClient(){
    if(qLabel!=nullptr){
        delete qLabel;
        qLabel=nullptr;
    }
    if(serverAddressInput!=nullptr){
        delete serverAddressInput;
        serverAddressInput=nullptr;
    }
    if(connectButton!=nullptr){
        delete connectButton;
        connectButton=nullptr;
    }
    if(serverPortInput!=nullptr){
        delete serverPortInput;
        serverPortInput=nullptr;
    }
    if(qLabel2!=nullptr){
        delete qLabel2;
        qLabel2=nullptr;
    }
    if(serverAddressInput!=nullptr){
        delete serverAddressInput;
        serverAddressInput=nullptr;
    }
    if(qLabel1!=nullptr){
        delete qLabel1;
        qLabel1=nullptr;
    }
    if(qVBoxLayoutLeft!=nullptr){
        delete qVBoxLayoutLeft;
        qVBoxLayoutLeft=nullptr;
    }
    if(timer!=nullptr){
        delete timer;
        timer=nullptr;
    }
    if(qWebSocket!=nullptr){
        qWebSocket->deleteLater();
        qWebSocket=nullptr;
    }
}

void WsClient::on_connectButton_clicked()
{
    if(connectButton->text()=="连接"){
        connectServer();
    }
    else if(connectButton->text()=="断开连接")
    {
        disconnectConnection();
    }
    else if(connectButton->text()=="正在连接...")
    {
        qWebSocket->abort();
    }
}

void WsClient::connection_connected()
{
//    qSettings->setValue("url",ui->urlLineEdit->text());
//    qSettings->setValue("port",ui->portLineEdit->text());
    connectButton->setText("断开连接");
//    connectButton->setEnabled(true);
    sendButton->setEnabled(true);
    pingCheckBox->setEnabled(true);
    QTextCursor cursor = sendInput->textCursor();
    cursor.movePosition(QTextCursor::End);
    sendInput->setTextCursor(cursor);
}

void WsClient::connection_disconnected()
{

}

void WsClient::click_connectButton()
{
    if(connectButton->text()=="连接"){
        connectServer();
    }
}

void WsClient::connection_textMessageReceived(QString data)
{
    receiveEdit_append(data);
}

void WsClient::tcp_stateChanged(QAbstractSocket::SocketState state)
{
//    qDebug() << state;
    if (state == QAbstractSocket::UnconnectedState) {
//        qDebug() << qTcpSocket->error();
//        qDebug() << qTcpSocket->errorString();

        disconnect(qWebSocket,SIGNAL(connected()),this,SLOT(connection_connected()));
        disconnect(qWebSocket,SIGNAL(disconnected()),this,SLOT(connection_disconnected()));
        disconnect(qWebSocket,SIGNAL(textMessageReceived(QString)),this,SLOT(connection_textMessageReceived(QString)));
        disconnect(qWebSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(tcp_stateChanged(QAbstractSocket::SocketState)));
        qWebSocket->deleteLater();
        qWebSocket=nullptr;

        serverAddressInput->setEnabled(true);
        serverPortInput->setEnabled(true);
        connectButton->setText("连接");
        sendButton->setEnabled(false);
        pingCheckBox->setEnabled(false);
        pingCheckBox->setChecked(false);
    }
}

void WsClient::connectServer()
{
    QString url=serverAddressInput->text();
    QString portString=serverPortInput->text();

    qTreeWidgetItem->setText(0,url+":"+portString);

    connectButton->setText("正在连接...");

    qWebSocket = new QWebSocket();

    connect(qWebSocket,SIGNAL(connected()),this,SLOT(connection_connected()));
    connect(qWebSocket,SIGNAL(disconnected()),this,SLOT(connection_disconnected()));
    connect(qWebSocket,SIGNAL(textMessageReceived(QString)),this,SLOT(connection_textMessageReceived(QString)));
    connect(qWebSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(tcp_stateChanged(QAbstractSocket::SocketState)));

    serverAddressInput->setEnabled(false);
    serverPortInput->setEnabled(false);

    QString path = QString("ws://%1:%2").arg(url).arg(portString);
    QUrl qUrl = QUrl(path);
    qWebSocket->open(qUrl);
}

void WsClient::disconnectConnection()
{
    qWebSocket->close();
}

void WsClient::on_sendButton_clicked()
{
    connection_sendData();
}

void WsClient::receiveEdit_append(QString qString){
    receiveInput->moveCursor(QTextCursor::End);
    receiveInput->insertPlainText(qString);
//    receiveInput->append(qString);
    QTextCursor cursor = receiveInput->textCursor();
    cursor.movePosition(QTextCursor::End);
    receiveInput->setTextCursor(cursor);
}

void WsClient::connection_sendData()
{
    QString qString=sendInput->toPlainText();
    QByteArray qByteArray=qString.toUtf8();
    qWebSocket->sendTextMessage(qByteArray);
//    qTcpSocket->write(QTextCodec::codecForName("GBK")->fromUnicode(qString));
    qWebSocket->flush();
//    qSettings->setValue("sendText",qString);
}

void WsClient::on_receiveClearButton_clicked()
{
    receiveInput->clear();
}

void WsClient::on_pingCheckBox_stateChanged(int state)
{
    if(state==Qt::Checked)
    {
        pingIntervalInput->setEnabled(false);
        pingDataInput->setEnabled(false);
        int pingInterval=pingIntervalInput->text().toInt();
        if(pingInterval>0)
        {
            pingInterval*=1000;
//            qSettings->setValue("pingInterval",ui->pingIntervalEdit->text());
//            qSettings->setValue("pingData",ui->pingDataEdit->toPlainText());
            QString qString=pingDataInput->toPlainText();
            qWebSocket->sendTextMessage(qString);
            timer = new QTimer(this);
            connect(timer, SIGNAL(timeout()), this, SLOT(ping_interval_time_timeout()));
            timer->start(pingInterval);
        }
    }
    else if(state==Qt::Unchecked)
    {
        pingIntervalInput->setEnabled(true);
        pingDataInput->setEnabled(true);
        if(timer!=nullptr)
        {
            timer->stop();
            delete timer;
            timer=nullptr;
        }
    }

}

void WsClient::ping_interval_time_timeout()
{
    QString qString=pingDataInput->toPlainText();
    qWebSocket->sendTextMessage(qString);
}
