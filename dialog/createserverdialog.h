#ifndef CREATESERVERDIALOG_H
#define CREATESERVERDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>

class CreateServerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateServerDialog(QWidget *parent = nullptr);
    ~CreateServerDialog();

signals:
    void confirm(QString ip, quint16 port);

public slots:

private:
    QVBoxLayout *mainLayout;

    QHBoxLayout *ipLayout;
    QLabel *ipLabel;
    QComboBox *ipComboBox;

    QHBoxLayout *portLayout;
    QLabel *portLabel;
    QLineEdit *portInput;

    QHBoxLayout *buttonLayout;
    QPushButton *confirmButton;
    QPushButton *cancelButton;

private slots:
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();
};

#endif // CREATESERVERDIALOG_H
