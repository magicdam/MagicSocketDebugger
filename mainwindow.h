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
        void on_pingCheckBox_stateChanged(int state);
        void on_ping_interval_time_timeout();

    private:
        Ui::MainWindow *ui;
        QSettings *qSettings;
        QTcpSocket* qTcpSocket;
        QTimer *timer;
        void receiveEdit_append(QString qString);
        void tcp_sendData();
};

#endif // MAINWINDOW_H
