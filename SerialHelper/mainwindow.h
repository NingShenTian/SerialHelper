#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QSerialPort *port;
    Ui::MainWindow *ui;
    bool b;
    void showformat(const QByteArray &arr);
    void sendformat(const QString &strdata);
    inline QSerialPort::DataBits getbit(int bit)        //
    {
        switch (bit)
        {
        case 5:
            return QSerialPort::Data5;
        case 6:
            return QSerialPort::Data6;
        case 7:
            return QSerialPort::Data7;
        default:
            return QSerialPort::Data8;
        }
    }
private slots:
    void readread();
    void on_pushButtonopen_clicked();
    void on_checkBoxtextformat_stateChanged(int arg1);
    void on_pushButtonsend_clicked();
    void on_pushButtonclear_clicked();
    void on_pushButtonhelp_clicked();
    void on_checkBoxsendformat_stateChanged(int arg1);
    void on_pushButtoncleasend_clicked();
    void on_radioButtonsendauto_clicked();
    void on_radioButtonreadauto_clicked();
    void on_radioButtonsendformat_clicked();
    void on_radioButtonreadformat_clicked();
    void on_comboBoxserial_currentIndexChanged(int index);
};

#endif // MAINWINDOW_H
