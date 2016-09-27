#include "mainwindow.h"
#include <QApplication>
#include "frminput.h"
#include "frmnum.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
    qApp->setFont(QFont("Microsoft YaHei", 12));
#ifndef __arm__
    QString path = qApp->applicationDirPath();
    QFile file(path + "/py.db");
    if (!file.exists() || file.size()==0) {
        QStringList list = path.split("/");
        QString fileName, newName;
        for (int i = 0; i < list.count() - 2; i++) {
            fileName = fileName + list.at(i) + "/";
        }
        fileName += "InputTool/py.db";
        newName = path + "/py.db";
        file.remove();
        QFile::copy(fileName, newName);
    }

#endif

    MainWindow w;
#ifdef __arm__
    w.showMaximized();
    frmInput::Instance()->Init("control", "silvery", 12, 12);
    //frmNum::Instance()->Init("black", 10);
#else
    w.show();
#endif
    return a.exec();
}
