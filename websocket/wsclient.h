#ifndef WSCLIENT_H
#define WSCLIENT_H

#include <QWebSocket>
#include "connection.h"

class WsClient:public Connection
{
    Q_OBJECT
public:
    WsClient(QTreeWidgetItem *,QGridLayout *);
    ~WsClient();
    QLineEdit* serverAddressInput=nullptr;
    QLineEdit* serverPortInput=nullptr;
private:
    QPushButton* connectButton=nullptr;
    QWebSocket *qWebSocket=nullptr;
    QTimer* timer=nullptr;
    QVBoxLayout* qVBoxLayoutLeft=nullptr;
    QLabel* qLabel1=nullptr;
    QLabel* qLabel2=nullptr;
    QLabel* qLabel=nullptr;
    void receiveEdit_append(QString qString);
    void connection_sendData();
    void connectServer();
    void disconnectConnection();
private slots:
    void on_connectButton_clicked();
    void connection_connected();
    void connection_disconnected();
    void click_connectButton();
    void connection_textMessageReceived(QString);
    void tcp_stateChanged(QAbstractSocket::SocketState);
    void on_sendButton_clicked();
    void on_receiveClearButton_clicked();
    void on_pingCheckBox_stateChanged(int state);
    void ping_interval_time_timeout();

};

#endif
