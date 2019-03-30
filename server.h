#ifndef SERVER_H
#define SERVER_H

#include "serverconnection.h"
#include <QVBoxLayout>
#include <QTreeWidgetItem>
#include <QTcpServer>
#include <QHash>
#include <QMainWindow>

class Server:public QObject
{
    Q_OBJECT
public:
    Server(QTreeWidgetItem *,QGridLayout *,quint16);
    ~Server();
    QTreeWidgetItem *qTreeWidgetItemServer=nullptr;
    QHash<long,ServerConnection*> serverConnectionList;
    bool start();
    void deleteServerConnection(long);
private:
    QTcpServer *server = nullptr;
    QVBoxLayout* qVBoxLayoutLeft=nullptr;
    QGridLayout *qGridLayoutParent=nullptr;
    quint16 port;    
private slots:
    void server_new_connect();
};

#endif // SERVER_H
