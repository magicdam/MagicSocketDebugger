#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QString msg="程序启动";
//    qDebug() << msg.toStdString().data();

    ui->urlLineEdit->setFocus();
    QWidget::setTabOrder(ui->urlLineEdit,ui->portLineEdit);
    QWidget::setTabOrder(ui->portLineEdit,ui->sendEdit);
    QWidget::setTabOrder(ui->sendEdit,ui->receiveEdit);

    connect(ui->urlLineEdit, SIGNAL(returnPressed()), this, SLOT(click_connectButton()));
    connect(ui->portLineEdit, SIGNAL(returnPressed()), this, SLOT(click_connectButton()));

    qSettings = new QSettings ("config.ini",QSettings::IniFormat);
    QString url = qSettings->value("url").toString();
    QString port = qSettings->value("port").toString();
    QString sendText=qSettings->value("sendText").toString();
    ui->urlLineEdit->setText(url);
    ui->portLineEdit->setText(port);
    ui->sendEdit->setText(sendText);
//    qDebug()<<ui->receiveEdit->wordWrapMode();
    ui->receiveEdit->setWordWrapMode(QTextOption::WrapAnywhere);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete qSettings;
}

void MainWindow::on_connectButton_clicked()
{
    if(ui->connectButton->text()=="连接"){
        tcp_connect();
    }
    else if(ui->connectButton->text()=="断开连接")
    {
        tcp_disconnect();
    }
}

void MainWindow::tcp_connected()
{
    qSettings->setValue("url",ui->urlLineEdit->text());
    qSettings->setValue("port",ui->portLineEdit->text());
    ui->connectButton->setText("断开连接");
    ui->connectButton->setEnabled(true);
    ui->sendButton->setEnabled(true);
}

void MainWindow::tcp_disconnected()
{
    ui->connectButton->setText("连接");
    ui->sendButton->setEnabled(false);
    qTcpSocket->deleteLater();
}

void MainWindow::click_connectButton()
{
    if(ui->connectButton->text()=="连接"){
        tcp_connect();
    }
}

void MainWindow::tcp_readyRead()
{
    QTcpSocket* obj = qobject_cast<QTcpSocket*>(sender());
    QString msg = obj->readAll();
    receiveEdit_append(msg);
}

void MainWindow::tcp_connect()
{
    QString url=ui->urlLineEdit->text();
    QString portString=ui->portLineEdit->text();
    quint16 port=quint16(portString.toUInt());

    ui->connectButton->setEnabled(false);
    ui->connectButton->setText("正在连接...");

    qTcpSocket = new QTcpSocket();
    connect(qTcpSocket,SIGNAL(connected()),this,SLOT(tcp_connected()));
    connect(qTcpSocket,SIGNAL(disconnected()),this,SLOT(tcp_disconnected()));
    connect(qTcpSocket,SIGNAL(readyRead()),this,SLOT(tcp_readyRead()));
    qTcpSocket->connectToHost(url, port);
    if(!qTcpSocket->waitForConnected(1000)){
        QMessageBox::information(this, "提示", "连接失败或者超时",tr("确定"));
        ui->connectButton->setText("连接");
        ui->connectButton->setEnabled(true);
    }
}

void MainWindow::tcp_disconnect()
{
    qTcpSocket->disconnectFromHost();
}

void MainWindow::on_sendButton_clicked()
{
    tcp_sendData();
}

void MainWindow::receiveEdit_append(QString qString){
    ui->receiveEdit->append(qString);
    QTextCursor cursor = ui->receiveEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->receiveEdit->setTextCursor(cursor);
}

void MainWindow::keyPressEvent(QKeyEvent  *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        if(ui->sendEdit->hasFocus() && ui->sendButton->isEnabled())
        {
            tcp_sendData();
        }
    }
}

void MainWindow::tcp_sendData()
{
    QString qString=ui->sendEdit->toPlainText();
    qTcpSocket->write(qString.toUtf8());
    qTcpSocket->waitForBytesWritten();
    qSettings->setValue("sendText",qString);
}

void MainWindow::on_receiveClearButton_clicked()
{
    ui->receiveEdit->clear();
}
