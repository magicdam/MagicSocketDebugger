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
#include "dialog/createserverdialog.h"

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
        //tcp
        void on_createTcpClient_triggered();
        void on_createTcpServer_triggered();
        void dialog_createTcpServer_confirm(QString ip, quint16 port);

        //ws
        void on_createWsClient_triggered();
        void on_createWsServer_triggered();
        void dialog_createWsServer_confirm(QString ip, quint16 port);

        //删除
        void on_deleteConnection_triggered();

        //点击连接
        void connectionClicked(QTreeWidgetItem* ,int);

        //切换语言
        void on_actionLanguageChinese_triggered();
        void on_actionLanguageEnglish_triggered();

        //反馈
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

        CreateServerDialog *createTcpServerDialog;
        CreateServerDialog *createWsServerDialog;

        void uncheckedAllActionLanguage();

};

#endif // MAINWINDOW_H
