#include "server.h"
#include <QMessageBox>
#include <QList>

Server::Server(QTreeWidgetItem *qTreeWidgetItemServer,QGridLayout *qGridLayoutParent,quint16 port)
{
    this->qTreeWidgetItemServer=qTreeWidgetItemServer;
    this->qGridLayoutParent=qGridLayoutParent;
    this->port=port;
}

Server::~Server(){
    QHashIterator<long,ServerConnection*> iter(serverConnectionList);
    QList<long> qList;
    while(iter.hasNext())
    {
        iter.next();
        qList.push_back(iter.key());
    }

    for(int i=0;i<qList.size();i++){
        ServerConnection *item=serverConnectionList.value(qList.value(i));
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

bool Server::start(){
    server = new QTcpServer();
    connect(server,SIGNAL(newConnection()),this,SLOT(server_new_connect()));//监听
    if(!server->listen(QHostAddress::AnyIPv4, port)) {
         QMessageBox::information(nullptr,"错误",server->errorString());
         return false;
    }
    return true;
}

void Server::server_new_connect() {
    QTcpSocket *qTcpSocket = server->nextPendingConnection();
    QTreeWidgetItem *qTreeWidgetItemConnection=new QTreeWidgetItem();
    qTreeWidgetItemConnection->setText(0,qTcpSocket->peerAddress().toString()+":"+QString::number(qTcpSocket->peerPort()));
//    ServerConnection *serverConnection=new ServerConnection(qTreeWidgetItemConnection,qGridLayoutParent,qTcpSocket,&deleteServerConnectionThread->qMutex,&deleteServerConnectionThread->q,&deleteServerConnectionThread->qSemaphore);
    ServerConnection *serverConnection=new ServerConnection(qTreeWidgetItemConnection,qGridLayoutParent,qTcpSocket,&serverConnectionList,qTcpSocket->peerAddress().toString(),qTcpSocket->peerPort());
    serverConnection->pingCheckBox->setEnabled(true);
    long key=reinterpret_cast<long>(qTreeWidgetItemConnection);
    serverConnectionList.insert(key,serverConnection);
//    qDebug()<<&serverConnectionList;
    qTreeWidgetItemServer->addChild(qTreeWidgetItemConnection);
}

void Server::deleteServerConnection(long key){
    ServerConnection *serverConnection=serverConnectionList.value(key);
    serverConnectionList.remove(key);
    delete serverConnection;
}
