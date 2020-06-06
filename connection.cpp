#include "connection.h"

Connection::~Connection(){
    if(receiveInput!=nullptr){
        delete receiveInput;
        receiveInput=nullptr;
    }
    if(qLabel5!=nullptr){
        delete qLabel5;
        qLabel5=nullptr;
    }
    if(qWidget3!=nullptr){
        delete qWidget3;
        qWidget3=nullptr;
    }
    if(qLabel4!=nullptr){
        delete qLabel4;
        qLabel4=nullptr;
    }
    if(pingDataInput!=nullptr){
        delete pingDataInput;
        pingDataInput=nullptr;
    }
    if(pingIntervalInput!=nullptr){
        delete pingIntervalInput;
        pingIntervalInput=nullptr;
    }
    if(pingCheckBox!=nullptr){
        delete pingCheckBox;
        pingCheckBox=nullptr;
    }
    if(qWidget2!=nullptr){
        delete qWidget2;
        qWidget2=nullptr;
    }
    if(sendButton!=nullptr){
        delete sendButton;
        sendButton=nullptr;
    }
    if(qLabel3!=nullptr){
        delete qLabel3;
        qLabel3=nullptr;
    }
    if(qWidget1!=nullptr){
        delete qWidget1;
        qWidget1=nullptr;
    }
    if(qGridLayoutRight!=nullptr){
        delete qGridLayoutRight;
        qGridLayoutRight=nullptr;
    }
    if(qWidgetRight!=nullptr){
        delete qWidgetRight;
        qWidgetRight=nullptr;
    }
    if(qWidgetLeft!=nullptr){
        delete qWidgetLeft;
        qWidgetLeft=nullptr;
    }
    if(qGridLayout!=nullptr){
        delete qGridLayout;
        qGridLayout=nullptr;
    }
    if(qWidget!=nullptr){
        delete qWidget;
        qWidget=nullptr;
    }
}

Connection::Connection(QGridLayout *qGridLayoutParent, bool sendDataBtnEnable):QObject()
{
    qWidget=new QWidget();
    qWidget->hide();
    qGridLayoutParent->addWidget(qWidget);
    qGridLayout=new QGridLayout(qWidget);
    qGridLayout->setMargin(0);
    qGridLayout->setSpacing(0);

    qWidgetLeft=new QWidget();
    qWidgetLeft->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    qWidgetLeft->setMinimumSize(200,500);
    qWidgetLeft->setMaximumSize(200,16777215);
//    qWidgetLeft->setStyleSheet("background-color: yellow");
    qGridLayout->addWidget(qWidgetLeft, 0, 0, 1, 1);

    qWidgetRight=new QWidget();
    qWidgetRight->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
//    qWidgetRight->setStyleSheet("background-color: blue");
    qGridLayout->addWidget(qWidgetRight, 0, 1, 1, 1);

    qGridLayoutRight=new QGridLayout(qWidgetRight);
    qGridLayoutRight->setMargin(2);
    qWidget1=new QWidget();
//    qWidget1->setStyleSheet("background-color: white");
    qWidget1->setMinimumSize(0,40);
    qWidget1->setMaximumSize(16777215,40);
    qGridLayoutRight  -> addWidget(qWidget1,0,0,1,1);
    qLabel3=new QLabel(qWidget1);
    qLabel3  -> setGeometry(0,2,50,36);
    qLabel3  -> setText("发送");
    sendButton=new QPushButton(qWidget1);
    sendButton  -> setGeometry(60,2,70,36);
    sendButton  -> setText("发送");
    if(!sendDataBtnEnable) {
        sendButton->setEnabled(false);
    }

    sendInput=new QTextEdit();
    sendInput->setMinimumSize(0,100);
    sendInput->setMaximumSize(16777215,200);
    sendInput->setWordWrapMode(QTextOption::WrapAnywhere);
    qGridLayoutRight->addWidget(sendInput,1,0,1,1);

    qWidget2=new QWidget();
//    qWidget2->setStyleSheet("background-color: white");
    qWidget2->setMinimumSize(0,40);
    qWidget2->setMaximumSize(16777215,40);
    qGridLayoutRight->addWidget(qWidget2,2,0,1,1);
    pingCheckBox=new QCheckBox(qWidget2);
    pingCheckBox->setEnabled(false);
    pingCheckBox->setGeometry(0,2,110,36);
    pingCheckBox->setText("心跳包");
    pingIntervalInput=new QLineEdit(qWidget2);
    pingIntervalInput->setGeometry(120,9,50,22);
    qLabel4=new QLabel(qWidget2);
    qLabel4->setGeometry(175,2,26,36);
    qLabel4->setText("秒");

    pingDataInput=new QTextEdit();
    pingDataInput->setMinimumSize(0,50);
    pingDataInput->setMaximumSize(16777215,50);
    qGridLayoutRight->addWidget(pingDataInput,3,0,1,1);

    qWidget3=new QWidget();
//    qWidget3->setStyleSheet("background-color: white");
    qWidget3->setMinimumSize(0,40);
    qWidget3->setMaximumSize(16777215,40);
    qGridLayoutRight->addWidget(qWidget3,4,0,1,1);
    qLabel5=new QLabel(qWidget3);
    qLabel5->setGeometry(0,2,50,36);
    qLabel5->setText("接收");
    clearReceiveButton=new QPushButton(qWidget3);
    clearReceiveButton->setGeometry(60,2,70,36);
    clearReceiveButton->setText("清空");

    receiveInput=new QTextEdit();
    receiveInput->setWordWrapMode(QTextOption::WrapAnywhere);
    qGridLayoutRight->addWidget(receiveInput,5,0,1,1);        

}
