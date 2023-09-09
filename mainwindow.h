#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTcpSocket>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QTcpSocket *m_tcpSocket;

    void RecvStrParserPack(QString str);
    void RecvStrParserType(QString str);


    void SocketSendPack_Login();
    void SocketSendPack_Status();


    void onConnected();
    void onDisConnected();
    void onReadyRead();
    void onStateChanged(QAbstractSocket::SocketState);

    char* getTextFromStr(QString string);

    int SendMiSerial;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
