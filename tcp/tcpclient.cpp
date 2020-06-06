#include "tcpclient.h"
#include <QTextCodec>
#include <QMessageBox>
#include <QTimer>
#include <mainwindow.h>
#include <QDebug>

TcpClient::TcpClient(QTreeWidgetItem *qTreeWidgetItem,QGridLayout *qGridLayoutParent):Connection(qGridLayoutParent, false)
{
    this->qTreeWidgetItem=qTreeWidgetItem;

    qVBoxLayoutLeft = new QVBoxLayout(qWidgetLeft);
    qVBoxLayoutLeft->setAlignment(Qt::AlignTop);
    qLabel1= new QLabel();
    qLabel1->setText("服务器地址：");
    qVBoxLayoutLeft->addWidget(qLabel1);
    serverAddressInput=new QLineEdit();
    qVBoxLayoutLeft->addWidget(serverAddressInput);
    qLabel2 = new QLabel();
    qLabel2->setText("端口：");
    qVBoxLayoutLeft->addWidget(qLabel2);
    serverPortInput=new QLineEdit();
    qVBoxLayoutLeft->addWidget(serverPortInput);
    connectButton=new QPushButton();
    connectButton->setText("连接");
    qVBoxLayoutLeft->addWidget(connectButton);
    qLabel=new QLabel();
    qLabel->setText("编码:UTF-8");
    qVBoxLayoutLeft->addWidget(qLabel);

    connect(connectButton, SIGNAL(clicked()), this, SLOT(on_connectButton_clicked()));
    connect(serverAddressInput, SIGNAL(returnPressed()), this, SLOT(click_connectButton()));
    connect(serverPortInput, SIGNAL(returnPressed()), this, SLOT(click_connectButton()));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(on_sendButton_clicked()));
    connect(pingCheckBox, SIGNAL(stateChanged(int)), this, SLOT(on_pingCheckBox_stateChanged(int)));
    connect(clearReceiveButton, SIGNAL(clicked()), this, SLOT(on_receiveClearButton_clicked()));

    qWidget->show();
}

TcpClient::~TcpClient(){
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
    if(qTcpSocket!=nullptr){
        qTcpSocket->deleteLater();
        qTcpSocket=nullptr;
    }
}

void TcpClient::on_connectButton_clicked()
{
    if(connectButton->text()=="连接"){
        tcp_connect();
    }
    else if(connectButton->text()=="断开连接")
    {
        tcp_disconnect();
    }
    else if(connectButton->text()=="正在连接...")
    {
        qTcpSocket->abort();
    }
}

void TcpClient::tcp_connected()
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

void TcpClient::tcp_disconnected()
{

}

void TcpClient::click_connectButton()
{
    if(connectButton->text()=="连接"){
        tcp_connect();
    }
}

void TcpClient::tcp_readyRead()
{
    QTcpSocket* obj = qobject_cast<QTcpSocket*>(sender());
    QString data=obj->readAll();
//    QString msg = QTextCodec::codecForName("GBK")->toUnicode(data);
    receiveEdit_append(data);
}

void TcpClient::tcp_stateChanged(QAbstractSocket::SocketState state)
{
//    qDebug() << state;
    if (state == QAbstractSocket::UnconnectedState) {
//        qDebug() << qTcpSocket->error();
//        qDebug() << qTcpSocket->errorString();

        disconnect(qTcpSocket,SIGNAL(connected()),this,SLOT(tcp_connected()));
        disconnect(qTcpSocket,SIGNAL(disconnected()),this,SLOT(tcp_disconnected()));
        disconnect(qTcpSocket,SIGNAL(readyRead()),this,SLOT(tcp_readyRead()));
        disconnect(qTcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(tcp_stateChanged(QAbstractSocket::SocketState)));
        qTcpSocket->deleteLater();
        qTcpSocket=nullptr;

        serverAddressInput->setEnabled(true);
        serverPortInput->setEnabled(true);
        connectButton->setText("连接");
        sendButton->setEnabled(false);
        pingCheckBox->setEnabled(false);
        pingCheckBox->setChecked(false);
    }
}

void TcpClient::tcp_connect()
{    
    QString url=serverAddressInput->text();
    QString portString=serverPortInput->text();
    quint16 port=quint16(portString.toUInt());

    qTreeWidgetItem->setText(0,url+":"+portString);

    connectButton->setText("正在连接...");

    qTcpSocket = new QTcpSocket();

    connect(qTcpSocket,SIGNAL(connected()),this,SLOT(tcp_connected()));
    connect(qTcpSocket,SIGNAL(disconnected()),this,SLOT(tcp_disconnected()));
    connect(qTcpSocket,SIGNAL(readyRead()),this,SLOT(tcp_readyRead()));
    connect(qTcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(tcp_stateChanged(QAbstractSocket::SocketState)));

    serverAddressInput->setEnabled(false);
    serverPortInput->setEnabled(false);

    qTcpSocket->connectToHost(url, port);
}

void TcpClient::tcp_disconnect()
{
    qTcpSocket->disconnectFromHost();
}

void TcpClient::on_sendButton_clicked()
{
    tcp_sendData();
}

void TcpClient::receiveEdit_append(QString qString){
    receiveInput->moveCursor(QTextCursor::End);
    receiveInput->insertPlainText(qString);
//    receiveInput->append(qString);
    QTextCursor cursor = receiveInput->textCursor();
    cursor.movePosition(QTextCursor::End);
    receiveInput->setTextCursor(cursor);
}

void TcpClient::tcp_sendData()
{
    QString qString=sendInput->toPlainText();
    QByteArray qByteArray=qString.toUtf8();
    qTcpSocket->write(qByteArray);
//    qTcpSocket->write(QTextCodec::codecForName("GBK")->fromUnicode(qString));
    qTcpSocket->waitForBytesWritten();
//    qSettings->setValue("sendText",qString);
}

void TcpClient::on_receiveClearButton_clicked()
{
    receiveInput->clear();
}

void TcpClient::on_pingCheckBox_stateChanged(int state)
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
            qTcpSocket->write(qString.toUtf8());
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

void TcpClient::ping_interval_time_timeout()
{
    QString qString=pingDataInput->toPlainText();
    qTcpSocket->write(qString.toUtf8());
}
