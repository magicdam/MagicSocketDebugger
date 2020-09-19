#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "tcp/tcpserverconnection.h"
#include <QVBoxLayout>
#include <QTreeWidgetItem>
#include <QTcpServer>
#include <QHash>
#include <QMainWindow>

class TcpServer:public QObject
{
    Q_OBJECT
public:
    TcpServer(QTreeWidgetItem *, QGridLayout *, QString, quint16);
    ~TcpServer();
    QTreeWidgetItem *qTreeWidgetItemServer=nullptr;
    QHash<long,TcpServerConnection*> serverConnectionList;
    bool start();
    void deleteServerConnection(long);
private:
    QTcpServer *server = nullptr;
    QVBoxLayout* qVBoxLayoutLeft=nullptr;
    QGridLayout *qGridLayoutParent=nullptr;
    QString ip;
    quint16 port;    
private slots:
    void server_new_connect();
};

#endif // SERVER_H
