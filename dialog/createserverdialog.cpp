#include "createserverdialog.h"
#include <QNetworkInterface>
#include <QMessageBox>

CreateServerDialog::CreateServerDialog(QWidget *parent) : QDialog(parent)
{
    mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(9,9,9,5);

    //本机地址
    ipLayout = new QHBoxLayout;
    mainLayout->addLayout(ipLayout);

    ipLabel= new QLabel();
    ipLabel->setText(tr("监听地址："));
    ipLayout->addWidget(ipLabel);

    ipComboBox = new QComboBox(this);
    ipComboBox->setEditable(true);

    QStringList qStringList;
    qStringList.append("0.0.0.0");
    QString ipAddr;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            ipAddr = address.toString();
            qStringList.append(ipAddr);
        }
    }
    ipComboBox->addItems(qStringList);
    ipLayout->addWidget(ipComboBox);

    //本机端口
    portLayout = new QHBoxLayout;
    mainLayout->addLayout(portLayout);

    portLabel= new QLabel();
    portLabel->setText(tr("监听端口："));
    portLayout->addWidget(portLabel);

    portInput=new QLineEdit();
    portLayout->addWidget(portInput);

    //按钮
    buttonLayout = new QHBoxLayout;

    confirmButton = new QPushButton();
    confirmButton->setText(tr("确认"));
    buttonLayout->addWidget(confirmButton);

    cancelButton = new QPushButton();
    cancelButton->setText(tr("取消"));
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    this->setLayout(mainLayout);

    connect(confirmButton, SIGNAL(clicked()), this, SLOT(on_confirmButton_clicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));

}

CreateServerDialog::~CreateServerDialog() {
    if(ipLabel!=nullptr){
        delete ipLabel;
        ipLabel=nullptr;
    }
    if(ipComboBox!=nullptr){
        delete ipComboBox;
        ipComboBox=nullptr;
    }
    if(ipLayout!=nullptr){
        delete ipLayout;
        ipLayout=nullptr;
    }

    if(portLabel!=nullptr){
        delete portLabel;
        portLabel=nullptr;
    }
    if(portInput!=nullptr){
        delete portInput;
        portInput=nullptr;
    }
    if(portLayout!=nullptr){
        delete portLayout;
        portLayout=nullptr;
    }

    if(cancelButton!=nullptr){
        delete cancelButton;
        cancelButton=nullptr;
    }
    if(buttonLayout!=nullptr){
        delete buttonLayout;
        buttonLayout=nullptr;
    }
    if(buttonLayout!=nullptr){
        delete buttonLayout;
        buttonLayout=nullptr;
    }

    if(mainLayout!=nullptr){
        delete mainLayout;
        mainLayout=nullptr;
    }
}

void CreateServerDialog::on_confirmButton_clicked() {
    QHostAddress test;
    QString ip = ipComboBox->lineEdit()->text();
    if (!test.setAddress(ip))
    {
        QMessageBox::information(this,tr("提示"),tr("请输入正确的ip地址"));
        return;
    }
    QString portString = portInput->text();
    bool ok;
    quint16 port = portString.toUShort(&ok);
    if (!ok) {
        QMessageBox::information(this,tr("提示"),tr("请输入正确的端口号"));
        return;
    }
    emit confirm(ip, port);
}

void CreateServerDialog::on_cancelButton_clicked() {
    this->close();
}
