#include "tcpserver.h"
#include <QMessageBox>
#include <QList>

TcpServer::TcpServer(QTreeWidgetItem *qTreeWidgetItemServer, QGridLayout *qGridLayoutParent, QString ip, quint16 port)
{
    this->qTreeWidgetItemServer=qTreeWidgetItemServer;
    this->qGridLayoutParent=qGridLayoutParent;
    this->ip = ip;
    this->port=port;
}

TcpServer::~TcpServer(){
    QHashIterator<long,TcpServerConnection*> iter(serverConnectionList);
    QList<long> qList;
    while(iter.hasNext())
    {
        iter.next();
        qList.push_back(iter.key());
    }

    for(int i=0;i<qList.size();i++){
        TcpServerConnection *item=serverConnectionList.value(qList.value(i));
        serverConnectionList.remove(qList.value(i));
        delete item;
        item=nullptr;
    }
    if(qTreeWidgetItemServer!=nullptr){
        delete qTreeWidgetItemServer;
        qTreeWidgetItemServer=nullptr;
    }
    if(qVBoxLayoutLeft!=nullptr){
        delete qVBoxLayoutLeft;
        qVBoxLayoutLeft=nullptr;
    }
    if(server!=nullptr){
        server->deleteLater();
        server=nullptr;
    }
}

bool TcpServer::start(){
    server = new QTcpServer();
    connect(server,SIGNAL(newConnection()),this,SLOT(server_new_connect()));//监听
    QHostAddress address = QHostAddress(ip);
    if(!server->listen(address, port)) {
         QMessageBox::information(nullptr,tr("错误"),server->errorString());
         return false;
    }
    return true;
}

void TcpServer::server_new_connect() {
    QTcpSocket *qTcpSocket = server->nextPendingConnection();
    QTreeWidgetItem *qTreeWidgetItemConnection=new QTreeWidgetItem();
    qTreeWidgetItemConnection->setText(0,qTcpSocket->peerAddress().toString()+":"+QString::number(qTcpSocket->peerPort()));
//    ServerConnection *serverConnection=new ServerConnection(qTreeWidgetItemConnection,qGridLayoutParent,qTcpSocket,&deleteServerConnectionThread->qMutex,&deleteServerConnectionThread->q,&deleteServerConnectionThread->qSemaphore);
    TcpServerConnection *serverConnection=new TcpServerConnection(qTreeWidgetItemConnection,qGridLayoutParent,qTcpSocket,&serverConnectionList,qTcpSocket->peerAddress().toString(),qTcpSocket->peerPort());
    serverConnection->pingCheckBox->setEnabled(true);
    long key=reinterpret_cast<long>(qTreeWidgetItemConnection);
    serverConnectionList.insert(key,serverConnection);
//    qDebug()<<&serverConnectionList;
    qTreeWidgetItemServer->addChild(qTreeWidgetItemConnection);
}

void TcpServer::deleteServerConnection(long key){
    TcpServerConnection *serverConnection=serverConnectionList.value(key);
    serverConnectionList.remove(key);
    delete serverConnection;
}
