#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QKeyEvent>
#include <QSettings>
#include <QHash>
#include <QTreeWidgetItem>
#include "tcp/tcpclient.h"
#include "tcp/tcpserver.h"
#include "websocket/wsclient.h"
#include "websocket/wsserver.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        void hideAllConnectionWidget();
        static QSettings *qSettings;
        static int aaa;

    private slots:
        void on_createTcpClient_triggered();
        void on_createTcpServer_triggered();
        void connectionClicked(QTreeWidgetItem* ,int);
        void on_deleteConnection_triggered();
        void on_createWsClient_triggered();
        void on_createWsServer_triggered();
        void on_actionLanguageChinese_triggered();
        void on_actionLanguageEnglish_triggered();

        void on_actionFeedback_triggered();

private:
        Ui::MainWindow *ui;        
        QHash<long,TcpClient*> tcpClientList;
        QHash<long,TcpServer*> tcpServerList;
        QHash<long,WsClient*> wsClientList;
        QHash<long,WsServer*> wsServerList;
        QTreeWidgetItem *qTreeWidgetItemTcpClient;
        QTreeWidgetItem *qTreeWidgetItemTcpServer;
        QTreeWidgetItem *qTreeWidgetItemWsClient;
        QTreeWidgetItem *qTreeWidgetItemWsServer;
        void uncheckedAllActionLanguage();

};

#endif // MAINWINDOW_H
