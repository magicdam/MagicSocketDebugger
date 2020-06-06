#ifndef CONNECTION_H
#define CONNECTION_H

#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QObject>
#include <QTreeWidgetItem>
#include <memory>

class Connection:public QObject
{
    Q_OBJECT
    public:
        Connection(QGridLayout *, bool sendDataBtnEnable);
        ~Connection();
        QWidget* qWidget=nullptr;        
        QCheckBox* pingCheckBox=nullptr;
        QTreeWidgetItem *qTreeWidgetItem=nullptr;
    protected:       
        QWidget* qWidgetLeft=nullptr;
        QPushButton* sendButton=nullptr;        
        QTextEdit* sendInput=nullptr;
        QTextEdit*  receiveInput=nullptr;
        QLineEdit*  pingIntervalInput=nullptr;
        QTextEdit*  pingDataInput=nullptr;
        QPushButton*  clearReceiveButton=nullptr;

    private slots:

    private:
        QGridLayout* qGridLayout=nullptr;
        QWidget*  qWidgetRight=nullptr;
        QGridLayout*  qGridLayoutRight=nullptr;
        QWidget*  qWidget1=nullptr;
        QLabel*  qLabel3=nullptr;
        QWidget*  qWidget2=nullptr;
        QLabel*  qLabel4=nullptr;
        QWidget*  qWidget3=nullptr;
        QLabel*  qLabel5=nullptr;
        QLabel*  qLabel=nullptr;
};

#endif // CONNECTION_H
