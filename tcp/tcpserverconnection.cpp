#include "tcpserverconnection.h"

TcpServerConnection::TcpServerConnection(QTreeWidgetItem *qTreeWidgetItemConnection,QGridLayout *qGridLayoutParent,QTcpSocket *qTcpSocket,QHash<long,TcpServerConnection*> *serverConnectionList,QString ipAddress,quint16 port):Connection(qGridLayoutParent, true)
{
    this->qTreeWidgetItem=qTreeWidgetItemConnection;
    this->qTcpSocket=qTcpSocket;
    this->serverConnectionList=serverConnectionList;
    this->qTreeWidgetItemConnection=qTreeWidgetItemConnection;
    qVBoxLayoutLeft = new QVBoxLayout(qWidgetLeft);
    qVBoxLayoutLeft->setAlignment(Qt::AlignTop);
    qLabel1= new QLabel();
    qLabel1->setText(tr("客户端地址："));
    qVBoxLayoutLeft->addWidget(qLabel1);
    clientAddressInput=new QLineEdit();
    clientAddressInput->setText(ipAddress);
    clientAddressInput->setEnabled(false);
    qVBoxLayoutLeft->addWidget(clientAddressInput);
    qLabel2 = new QLabel();
    qLabel2->setText(tr("端口："));
    qVBoxLayoutLeft->addWidget(qLabel2);
    clientPortInput=new QLineEdit();
    clientPortInput->setText(QString::number(port));
    clientPortInput->setEnabled(false);
    qVBoxLayoutLeft->addWidget(clientPortInput);
    qLabel=new QLabel();
    qLabel->setText(tr("编码:UTF-8"));
    qVBoxLayoutLeft->addWidget(qLabel);

    connect(qTcpSocket, SIGNAL(readyRead()),this,SLOT(tcp_readyRead()));
    connect(qTcpSocket, SIGNAL(disconnected()), this, SLOT(tcp_disconnect()));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(on_sendButton_clicked()));
    connect(pingCheckBox, SIGNAL(stateChanged(int)), this, SLOT(on_pingCheckBox_stateChanged(int)));
    connect(clearReceiveButton, SIGNAL(clicked()), this, SLOT(on_receiveClearButton_clicked()));
}

TcpServerConnection::~TcpServerConnection(){
//    qDebug()<<"析构开始ServerConnection:"<<this;
    if(qLabel!=nullptr){
        delete qLabel;
        qLabel=nullptr;
    }
    if(qLabel1!=nullptr){
        delete qLabel1;
        qLabel1=nullptr;
    }
    if(qLabel2!=nullptr){
        delete qLabel2;
        qLabel2=nullptr;
    }
    if(clientAddressInput!=nullptr){
        delete clientAddressInput;
        clientAddressInput=nullptr;
    }
    if(clientPortInput!=nullptr){
        delete clientPortInput;
        clientPortInput=nullptr;
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
    if(qTreeWidgetItemConnection!=nullptr){
        delete qTreeWidgetItemConnection;
        qTreeWidgetItemConnection=nullptr;
    }
//    qDebug()<<"析构完成ServerConnection:"<<this;
}

void TcpServerConnection::tcp_disconnect(){
    long key=reinterpret_cast<long>(qTreeWidgetItemConnection);
//    qDebug()<<qTreeWidgetItemConnection;
    serverConnectionList->remove(key);
    delete this;
}

void TcpServerConnection::tcp_readyRead(){
    QTcpSocket* obj = qobject_cast<QTcpSocket*>(sender());
    QString data=obj->readAll();
//    qDebug()<<data;
//    QString msg = QTextCodec::codecForName("GBK")->toUnicode(data);
    receiveEdit_append(data);
}

void TcpServerConnection::on_sendButton_clicked()
{
    tcp_sendData();
}

void TcpServerConnection::receiveEdit_append(QString qString){
    receiveInput->moveCursor(QTextCursor::End);
    receiveInput->insertPlainText(qString);
//    receiveInput->append(qString);
    QTextCursor cursor = receiveInput->textCursor();
    cursor.movePosition(QTextCursor::End);
    receiveInput->setTextCursor(cursor);
}

void TcpServerConnection::tcp_sendData()
{
    QString qString=sendInput->toPlainText();
    QByteArray qByteArray=qString.toUtf8();
    qTcpSocket->write(qByteArray);
//    qTcpSocket->write(QTextCodec::codecForName("GBK")->fromUnicode(qString));
    qTcpSocket->waitForBytesWritten();
//    qSettings->setValue("sendText",qString);
}

void TcpServerConnection::on_pingCheckBox_stateChanged(int state)
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

void TcpServerConnection::ping_interval_time_timeout()
{
    QString qString=pingDataInput->toPlainText();
    qTcpSocket->write(qString.toUtf8());
}

void TcpServerConnection::on_receiveClearButton_clicked()
{
    receiveInput->clear();
}


