#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QDir>
#include <QDateTime>
#include <QTextCodec>
#include <QInputDialog>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);   

    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)), this,SLOT(connectionClicked(QTreeWidgetItem* ,int)));

    qTreeWidgetItemClient=ui->treeWidget->topLevelItem(0);
    qTreeWidgetItemServer=ui->treeWidget->topLevelItem(1);

//    ui->urlLineEdit->setFocus();
//    QWidget::setTabOrder(ui->urlLineEdit,ui->portLineEdit);
//    QWidget::setTabOrder(ui->portLineEdit,ui->sendEdit);
//    QWidget::setTabOrder(ui->sendEdit,ui->receiveEdit);
//    QWidget::setTabOrder(ui->receiveEdit,ui->pingIntervalEdit);
//    QWidget::setTabOrder(ui->pingIntervalEdit,ui->pingDataEdit);

//    #ifdef Q_OS_WIN
//        qSettings=new QSettings ("config.ini",QSettings::IniFormat);
//    #endif
//    #ifdef Q_OS_MACOS
//        qSettings=new QSettings (QDir().homePath()+"/Library/Preferences/MagicSocketDebugger/config.ini",QSettings::IniFormat);
//    #endif
//    QString url = qSettings->value("url").toString();
//    QString port = qSettings->value("port").toString();
//    QString sendText=qSettings->value("sendText").toString();
//    QString pingInterval=qSettings->value("pingInterval").toString();
//    QString pingData=qSettings->value("pingData").toString();
//    ui->urlLineEdit->setText(url);
//    ui->portLineEdit->setText(port);
//    ui->sendEdit->setText(sendText);
//    ui->sendEdit->setWordWrapMode(QTextOption::WrapAnywhere);
//    ui->receiveEdit->setWordWrapMode(QTextOption::WrapAnywhere);
//    ui->pingIntervalEdit->setText(pingInterval);
//    ui->pingDataEdit->setText(pingData);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete qSettings;
}

void MainWindow::on_createClient_triggered()
{
    hideAllConnectionWidget();
    QTreeWidgetItem *qTreeWidgetItem1=new QTreeWidgetItem();
    qTreeWidgetItem1->setText(0,"新的客户端");
    qTreeWidgetItemClient->insertChild(0,qTreeWidgetItem1);
    ui->treeWidget->setCurrentItem(qTreeWidgetItem1);
    long key=reinterpret_cast<long>(qTreeWidgetItem1);
//    qDebug()<<qTreeWidgetItem1;
    Client *client=new Client(qTreeWidgetItem1,ui->gridLayout_2);
    ui->treeWidget->expandAll();
    clientList.insert(key,client);
    ui->deleteConnection->setEnabled(true);
//    qDebug()<<client;
}

void MainWindow::hideAllConnectionWidget(){
    for (int i=0;i<ui->gridLayout_2->layout()->count();i++)
    {
        QLayoutItem *item = ui->gridLayout_2->layout()->itemAt(i);
        item->widget()->hide();
    }
}

void MainWindow::on_createServer_triggered()
{
    bool ok;
    QString portString = QInputDialog::getText(this, "添加服务端","请输入要监听的端口", QLineEdit::Normal,"",&ok);
    if (!ok || portString.isEmpty())
        return;
    quint16 port=portString.toUShort();
    if(port==0){
        QMessageBox::information(this,"错误","请输入正确的端口号");
    }
    hideAllConnectionWidget();
    QTreeWidgetItem *qTreeWidgetItem1=new QTreeWidgetItem();
    qTreeWidgetItem1->setText(0,"本机:"+QString::number(port));
    Server *server=new Server(qTreeWidgetItem1,ui->gridLayout_2,port);
    if(!server->start()){
        delete qTreeWidgetItem1;
        qTreeWidgetItem1=nullptr;
        return;
    }
    qTreeWidgetItemServer->addChild(qTreeWidgetItem1);
    ui->treeWidget->setCurrentItem(qTreeWidgetItem1);
    long key=reinterpret_cast<long>(qTreeWidgetItem1);
    ui->treeWidget->expandAll();
    serverList.insert(key,server);
    ui->deleteConnection->setEnabled(true);
}

void MainWindow::connectionClicked(QTreeWidgetItem *qTreeWidgetItem ,int column){
    bool deleteButtonEnable=false;
    if(qTreeWidgetItem->parent()!=nullptr){
        if(qTreeWidgetItem->parent()==qTreeWidgetItemClient){
            deleteButtonEnable=true;
    //        qDebug()<<"客户端";
            hideAllConnectionWidget();
            long key=reinterpret_cast<long>(qTreeWidgetItem);
            Client *client = clientList.value(key);
            client->qWidget->show();
        }
        else if(qTreeWidgetItem->parent()==qTreeWidgetItemServer){
            deleteButtonEnable=true;
        }
        else if(qTreeWidgetItem->parent()->parent()==qTreeWidgetItemServer){
            deleteButtonEnable=true;
            hideAllConnectionWidget();
            long serverKey=reinterpret_cast<long>(qTreeWidgetItem->parent());
            long serverConnectionKey=reinterpret_cast<long>(qTreeWidgetItem);
            Server *server = serverList.value(serverKey);
            server->serverConnectionList.value(serverConnectionKey)->qWidget->show();
        }
    }
    if(deleteButtonEnable){
        ui->deleteConnection->setEnabled(true);
    }
    else{
        ui->deleteConnection->setEnabled(false);
    }
}

void MainWindow::on_deleteConnection_triggered()
{
    QTreeWidgetItem *qTreeWidgetItem=ui->treeWidget->currentItem();
    QTreeWidgetItem *parent=qTreeWidgetItem->parent();
    if(qTreeWidgetItem->parent()==qTreeWidgetItemClient){
        long key=reinterpret_cast<long>(qTreeWidgetItem);
        Client *client = clientList.value(key);
        clientList.remove(key);
        if(client!=nullptr){
            delete client;
            client=nullptr;
        }
        int p=parent->indexOfChild(qTreeWidgetItem);
        if(parent->childCount()>0 && p!=parent->childCount()-1){
            parent->removeChild(qTreeWidgetItem);
            ui->treeWidget->setCurrentItem(parent->child(p));
            long key=reinterpret_cast<long>(parent->child(p));
            Client *client = clientList.value(key);
            client->qWidget->show();
        }
        else{
            ui->deleteConnection->setEnabled(false);
            parent->removeChild(qTreeWidgetItem);
            ui->treeWidget->setCurrentItem(qTreeWidgetItem->parent());
        }
    }
    else if(qTreeWidgetItem->parent()==qTreeWidgetItemServer){
        long key=reinterpret_cast<long>(qTreeWidgetItem);
        Server *server = serverList.value(key);
        serverList.remove(key);
        if(server!=nullptr){
            delete server;
            server=nullptr;
        }
        int p=parent->indexOfChild(qTreeWidgetItem);
        if(parent->childCount()>0 && p!=parent->childCount()-1){
            parent->removeChild(qTreeWidgetItem);
            ui->treeWidget->setCurrentItem(parent->child(p));
        }
        else{
            ui->deleteConnection->setEnabled(false);
            parent->removeChild(qTreeWidgetItem);
            ui->treeWidget->setCurrentItem(qTreeWidgetItem->parent());
        }
    }
    else if(qTreeWidgetItem->parent()->parent()==qTreeWidgetItemServer){
        long parentKey=reinterpret_cast<long>(qTreeWidgetItem->parent());
        long childKey=reinterpret_cast<long>(qTreeWidgetItem);
        int p=parent->indexOfChild(qTreeWidgetItem);
        Server *server = serverList.value(parentKey);
        server->deleteServerConnection(childKey);
        if(parent->childCount()>0){
            ui->treeWidget->setCurrentItem(parent->child(p));
        }
        else{
            ui->deleteConnection->setEnabled(false);
            ui->treeWidget->setCurrentItem(parent);
        }
    }

//    QGridLayout *qGridLayout=reinterpret_cast<QGridLayout *>(test);
//    qDebug()<<qGridLayout->layout()->count();

}
