#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->radioButtonsendformat->setEnabled(false);
    ui->comboBoxsendformat->setEnabled(false);
    ui->radioButtonreadauto->setEnabled(false);
    ui->radioButtonreadformat->setEnabled(false);
    ui->comboBoxreadformat->setEnabled(false);
    ui->radioButtonsendauto->setEnabled(false);
    ui->spinBox->setProperty("noinput",true);
    ui->textBrowser->setProperty("noinput",true);
    b=false;
    port=new QSerialPort;
    connect(port,SIGNAL(readyRead()),this,SLOT(readread()));
    QList<QSerialPortInfo> strlist=QSerialPortInfo::availablePorts();
    QList<QSerialPortInfo>::const_iterator iter;
    for(iter=strlist.constBegin();iter!=strlist.constEnd();++iter)
    {
        ui->comboBoxserial->addItem((*iter).portName());
    }
    QList<QByteArray>list=QTextCodec::availableCodecs();
    for(int i=0;i<list.count();++i)
    {
        ui->comboBoxsendformat->addItem(list.at(i));
        ui->comboBoxreadformat->addItem(list.at(i));
    }
    ui->labelstatu->setStyleSheet("border-image: url(:/new/img/red.png);");
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::readread()
{
    QByteArray arr= port->readAll();
    if(ui->checkBoxtextformat->isChecked())
    {
        showformat(arr);
    }
    else
    {
        QString str;
        for(int i=0;i<arr.length();i++)
        {
            str+=QString("%1").arg((uchar)arr.at(i),2,16,QLatin1Char('0')).toUpper()+" ";
        }
        ui->textBrowser->append(str);
    }
}
void MainWindow::showformat(const QByteArray &arr)
{
    if(ui->radioButtonsendauto->isChecked())
    {
        QTextCodec *cod= QTextCodec::codecForName(ui->comboBoxsendformat->currentText().toLatin1());
        QString str=cod->toUnicode(arr);
        ui->textBrowser->append(str);
    }
    else
    {
        QString str(arr);
        ui->textBrowser->append(str);
    }
}
void MainWindow::on_pushButtonsend_clicked()
{
    if(ui->checkBoxsendautoclear->isChecked())
    {
        ui->lineEdit->clear();
    }
    if(!port->isOpen())
    {
        return;
    }
    if(ui->checkBoxsendformat->isChecked())
    {
        sendformat(ui->lineEdit->text());
    }
    else
    {
        QString str=ui->lineEdit->text();
        QStringList strlist=str.trimmed().split(" ");
        QByteArray arr;
        for(int i=0;i<strlist.count();++i)
        {
            bool bl=false;
            QString ch=strlist.at(i);
            uchar byte=(uchar)ch.toInt(&bl,16);
            if(!bl)
            {
                return;
            }
            arr.append(byte);
        }
        port->write(arr);
    }

}
void MainWindow::sendformat(const QString &strdata)
{
    if(ui->radioButtonreadauto->isChecked())
    {
        QTextCodec *cod= QTextCodec::codecForName(ui->comboBoxreadformat->currentText().toLatin1());
        QByteArray arr=cod->fromUnicode(strdata);
        port->write(arr);
    }
    else
    {
        QString str=ui->lineEdit->text();
        QByteArray arr(str.toLatin1());
        port->write(arr);
    }
}
void MainWindow::on_pushButtonopen_clicked()
{
    if(b)
    {
        port->close();
        ui->pushButtonopen->setText("打开");
        ui->labelstatu->setStyleSheet("border-image: url(:/new/img/red.png);");
        b=false;
        ui->labelstate->setText("串口已关闭！");
    }
    else
    {
        port->setPortName("/dev/"+ui->comboBoxserial->currentText());
        port->setBaudRate(ui->comboBoxbute->currentText().toInt());
        port->setDataBits(getbit(ui->spinBox->value()));
        if(port->open(QIODevice::ReadWrite))
        {
            ui->pushButtonopen->setText("关闭");
            ui->labelstatu->setStyleSheet("border-image: url(:/new/img/lv.png);");
            b=true;
            ui->labelstate->setText("串口已打开！");
        }
        else
        {
            b=false;
            ui->labelstate->setText("打开失败！");
            ui->labelstatu->setStyleSheet("border-image: url(:/new/img/red.png);");
        }
    }
}
void MainWindow::on_checkBoxtextformat_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    if(ui->checkBoxtextformat->isChecked())
    {
        ui->radioButtonreadformat->setEnabled(true);
        ui->radioButtonreadauto->setEnabled(true);
    }
    else
    {
        ui->radioButtonreadformat->setEnabled(false);
        ui->radioButtonreadauto->setEnabled(false);
    }
}
void MainWindow::on_checkBoxsendformat_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    if(ui->checkBoxsendformat->isChecked())
    {
        ui->radioButtonsendformat->setEnabled(true);
        ui->radioButtonsendauto->setEnabled(true);
    }
    else
    {
        ui->radioButtonsendformat->setEnabled(false);
        ui->radioButtonsendauto->setEnabled(false);
    }
}
void MainWindow::on_pushButtonclear_clicked()
{
    ui->textBrowser->clear();
}
void MainWindow::on_pushButtoncleasend_clicked()
{
    ui->lineEdit->clear();
}
void MainWindow::on_pushButtonhelp_clicked()
{
    QString strtext="帮助信息：\n"
                    "1.默认情况下接收和发送均是16进制，用英文空格' '将数据隔开发送数据格式：\n"
                    "01 02 03 04 05 06 07 08 09 0A 0B 0C 0E 0D 0F 10 11 12\n"
                    "或者：\n 1 2 3 4 5 6 7 8 9 A B C D E F 10 11 12\n"
                    "同样，收到的数据会按照16进制以空格分隔的方式显示;\n"
                    "2.当选择文本格式发送或着接收时，可以选择编码后发送或显示，具体编码格式根据下拉框选择;\n"
                    "3.当发送汉字的时候需要选择UTF-8编码或者GBK编码,默认采用系统默认;"
                    "4.嵌入式环境下，点击输入框会有软件盘用于输入，关闭软件盘需要点击空白处;";
    ui->textBrowser->append(strtext);
}
void MainWindow::on_radioButtonsendauto_clicked()
{
    if(ui->radioButtonsendauto->isChecked())
    {
        ui->comboBoxsendformat->setEnabled(true);
    }
    else
    {
        ui->comboBoxsendformat->setEnabled(false);
    }
}
void MainWindow::on_radioButtonreadauto_clicked()
{
    if(ui->radioButtonreadauto->isChecked())
    {
        ui->comboBoxreadformat->setEnabled(true);
    }
    else
    {
        ui->comboBoxreadformat->setEnabled(false);
    }
}
void MainWindow::on_radioButtonsendformat_clicked()
{
    if(ui->radioButtonsendauto->isChecked())
    {
        ui->comboBoxsendformat->setEnabled(true);
    }
    else
    {
        ui->comboBoxsendformat->setEnabled(false);
    }
}
void MainWindow::on_radioButtonreadformat_clicked()
{
    if(ui->radioButtonreadauto->isChecked())
    {
        ui->comboBoxreadformat->setEnabled(true);
    }
    else
    {
        ui->comboBoxreadformat->setEnabled(false);
    }
}
void MainWindow::on_comboBoxserial_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if(this->port->isOpen())
    {
        this->port->close();
        b=false;
        ui->labelstatu->setStyleSheet("border-image: url(:/new/img/red.png);");
    }
    if(ui->pushButtonopen->text()=="关闭")
    {
        ui->pushButtonopen->setText("打开");
    }
    ui->labelstate->setText("选择串口："+ui->comboBoxserial->currentText());
}
