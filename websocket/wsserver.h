#ifndef WSSERVER_H
#define WSSERVER_H

#include "websocket/wsserverconnection.h"
#include <QVBoxLayout>
#include <QTreeWidgetItem>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QHash>
#include <QMainWindow>

class WsServer:public QObject
{
    Q_OBJECT
public:
    WsServer(QTreeWidgetItem *, QGridLayout *, QString ip, quint16);
    ~WsServer();
    QTreeWidgetItem *qTreeWidgetItemServer=nullptr;
    QHash<long,WsServerConnection*> serverConnectionList;
    bool start();
    void deleteServerConnection(long);
private:
    QWebSocketServer *server = nullptr;
    QVBoxLayout* qVBoxLayoutLeft=nullptr;
    QGridLayout *qGridLayoutParent=nullptr;
    QString ip;
    quint16 port;    
private slots:
    void connection_connected();
};

#endif // SERVER_H
