#ifndef WSSERVERCONNECTION_H
#define WSSERVERCONNECTION_H

#include <connection.h>
#include <QWebSocket>
#include <QTimer>
#include <functional>
#include <QHash>
#include <QSemaphore>

class WsServerConnection:public Connection
{
    Q_OBJECT
public:
    WsServerConnection(QTreeWidgetItem *,QGridLayout *,QWebSocket *,QHash<long,WsServerConnection*> *,QString,quint16);
    ~WsServerConnection();
private:
    QWebSocket *qWebSocket=nullptr;
    QTreeWidgetItem *qTreeWidgetItemConnection=nullptr;
    QVBoxLayout* qVBoxLayoutLeft=nullptr;
    QLabel* qLabel=nullptr;
    QLabel* qLabel1=nullptr;
    QLabel* qLabel2=nullptr;
    QLineEdit* clientAddressInput=nullptr;
    QLineEdit* clientPortInput=nullptr;
    QTimer* timer=nullptr;
    QHash<long,WsServerConnection*> *serverConnectionList;
    void receiveEdit_append(QString qString);
    void tcp_sendData();

private slots:
    void connection_disconnect();
    void connection_textMessageReceived(QString);
    void on_sendButton_clicked();
    void ping_interval_time_timeout();
    void on_pingCheckBox_stateChanged(int state);
    void on_receiveClearButton_clicked();
};

#endif // SERVER_H
