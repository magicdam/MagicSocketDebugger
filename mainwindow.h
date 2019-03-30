#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QKeyEvent>
#include <QSettings>
#include <QHash>
#include <QTreeWidgetItem>
#include "client.h"
#include "server.h"

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

    private slots:
        void on_createClient_triggered();        
        void on_createServer_triggered();
        void connectionClicked(QTreeWidgetItem* ,int);
        void on_deleteConnection_triggered();

private:
        Ui::MainWindow *ui;        
        QSettings *qSettings;
        QHash<long,Client*> clientList;
        QHash<long,Server*> serverList;
        QTreeWidgetItem *qTreeWidgetItemClient;
        QTreeWidgetItem *qTreeWidgetItemServer;

};

#endif // MAINWINDOW_H
