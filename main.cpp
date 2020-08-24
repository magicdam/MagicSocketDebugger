#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDir>

QSettings *MainWindow::qSettings = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    #ifdef Q_OS_WIN
        MainWindow::qSettings=new QSettings ("config.ini",QSettings::IniFormat);
    #elif defined Q_OS_MACOS
        MainWindow::qSettings=new QSettings (QDir().homePath()+"/Library/Preferences/MagicSocketDebugger/config.ini",QSettings::IniFormat);
    #endif

    QTranslator translator;
    QVariant language = MainWindow::qSettings->value("language");
    if (language.isNull()) {
        QLocale ql;
        QString languageName = nullptr;
        switch (ql.language())
        {
            case QLocale::Chinese:
                languageName = "chs";
                break;
            case QLocale::English:
                languageName = "en";
                break;
            default:
                languageName = "en";
                break;
        }
        translator.load(":/i18n/" + languageName +".qm");
        MainWindow::qSettings->setValue("language", languageName);
    } else {
        translator.load(":/i18n/" + language.toString() + ".qm");
    }
    qApp->installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
