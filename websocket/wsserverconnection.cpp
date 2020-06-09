#include "websocket/wsserverconnection.h"

WsServerConnection::WsServerConnection(QTreeWidgetItem *qTreeWidgetItemConnection,QGridLayout *qGridLayoutParent,QWebSocket *qWebSocket,QHash<long,WsServerConnection*> *serverConnectionList,QString ipAddress,quint16 port):Connection(qGridLayoutParent, true)
{
    this->qTreeWidgetItem=qTreeWidgetItemConnection;
    this->qWebSocket=qWebSocket;
    this->serverConnectionList=serverConnectionList;
    this->qTreeWidgetItemConnection=qTreeWidgetItemConnection;
    qVBoxLayoutLeft = new QVBoxLayout(qWidgetLeft);
    qVBoxLayoutLeft->setAlignment(Qt::AlignTop);
    qLabel1= new QLabel();
    qLabel1->setText("客户端地址：");
    qVBoxLayoutLeft->addWidget(qLabel1);
    clientAddressInput=new QLineEdit();
    clientAddressInput->setText(ipAddress);
    clientAddressInput->setEnabled(false);
    qVBoxLayoutLeft->addWidget(clientAddressInput);
    qLabel2 = new QLabel();
    qLabel2->setText("端口：");
    qVBoxLayoutLeft->addWidget(qLabel2);
    clientPortInput=new QLineEdit();
    clientPortInput->setText(QString::number(port));
    clientPortInput->setEnabled(false);
    qVBoxLayoutLeft->addWidget(clientPortInput);

    connect(qWebSocket, SIGNAL(textMessageReceived(QString)),this,SLOT(connection_textMessageReceived(QString)));
    connect(qWebSocket, SIGNAL(disconnected()), this, SLOT(connection_disconnect()));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(on_sendButton_clicked()));
    connect(pingCheckBox, SIGNAL(stateChanged(int)), this, SLOT(on_pingCheckBox_stateChanged(int)));
    connect(clearReceiveButton, SIGNAL(clicked()), this, SLOT(on_receiveClearButton_clicked()));
}

WsServerConnection::~WsServerConnection(){
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
    if(qWebSocket!=nullptr){
        qWebSocket->deleteLater();
        qWebSocket=nullptr;
    }
    if(qTreeWidgetItemConnection!=nullptr){
        delete qTreeWidgetItemConnection;
        qTreeWidgetItemConnection=nullptr;
    }
//    qDebug()<<"析构完成ServerConnection:"<<this;
}

void WsServerConnection::connection_disconnect(){
    long key=reinterpret_cast<long>(qTreeWidgetItemConnection);
//    qDebug()<<qTreeWidgetItemConnection;
    serverConnectionList->remove(key);
    delete this;
}

void WsServerConnection::connection_textMessageReceived(QString data){
//    qDebug()<<data;
//    QString msg = QTextCodec::codecForName("GBK")->toUnicode(data);
    receiveEdit_append(data);
}

void WsServerConnection::on_sendButton_clicked()
{
    tcp_sendData();
}

void WsServerConnection::receiveEdit_append(QString qString){
    if (!receiveInput->document()->isEmpty()) {
        receiveInput->moveCursor(QTextCursor::End);
        receiveInput->insertPlainText("\n");
    }
    receiveInput->insertHtml("<font color=\"#0000FF\">" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") + "</font> ");
    receiveInput->insertPlainText("\n");
    receiveInput->insertPlainText(qString);
//    receiveInput->append(qString);
    QTextCursor cursor = receiveInput->textCursor();
    cursor.movePosition(QTextCursor::End);
    receiveInput->setTextCursor(cursor);
}

void WsServerConnection::tcp_sendData()
{
    QString qString=sendInput->toPlainText();
//    QByteArray qByteArray=qString.toUtf8();
    qWebSocket->sendTextMessage(qString);
//    qTcpSocket->write(QTextCodec::codecForName("GBK")->fromUnicode(qString));
    qWebSocket->flush();
//    qSettings->setValue("sendText",qString);
}

void WsServerConnection::on_pingCheckBox_stateChanged(int state)
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

void WsServerConnection::ping_interval_time_timeout()
{
    QString qString=pingDataInput->toPlainText();
    qWebSocket->sendTextMessage(qString);
}

void WsServerConnection::on_receiveClearButton_clicked()
{
    receiveInput->clear();
}


