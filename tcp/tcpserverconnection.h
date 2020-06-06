#ifndef TCPSERVERCONNECTION_H
#define TCPSERVERCONNECTION_H

#include "connection.h"
#include <QTcpSocket>
#include <QTimer>
#include <functional>
#include <QHash>
#include <QSemaphore>

class TcpServerConnection:public Connection
{
    Q_OBJECT
public:
    TcpServerConnection(QTreeWidgetItem *,QGridLayout *,QTcpSocket *,QHash<long,TcpServerConnection*> *,QString,quint16);
    ~TcpServerConnection();
private:
    QTcpSocket *qTcpSocket=nullptr;
    QTreeWidgetItem *qTreeWidgetItemConnection=nullptr;
    QVBoxLayout* qVBoxLayoutLeft=nullptr;
    QLabel* qLabel=nullptr;
    QLabel* qLabel1=nullptr;
    QLabel* qLabel2=nullptr;
    QLineEdit* clientAddressInput=nullptr;
    QLineEdit* clientPortInput=nullptr;
    QTimer* timer=nullptr;
    QHash<long,TcpServerConnection*> *serverConnectionList;
    void receiveEdit_append(QString qString);
    void tcp_sendData();

private slots:
    void tcp_disconnect();
    void tcp_readyRead();
    void on_sendButton_clicked();
    void ping_interval_time_timeout();
    void on_pingCheckBox_stateChanged(int state);
    void on_receiveClearButton_clicked();
};

#endif // SERVER_H
