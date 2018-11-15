#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QTcpSocket>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectButton_clicked();
    void tcp_connected();
    void tcp_disconnected();
    void click_connectButton();
    void tcp_connect();
    void tcp_disconnect();
    void tcp_readyRead();
    void on_sendButton_clicked();
    void keyPressEvent(QKeyEvent  *event);
    void on_receiveClearButton_clicked();

private:
    Ui::MainWindow *ui;
    QSettings *qSettings;
    QTcpSocket* qTcpSocket;
    void receiveEdit_append(QString qString);
    void tcp_sendData();

};

#endif // MAINWINDOW_H
