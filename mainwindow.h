#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//默认定时器时间 30S
#define TIMER_TIMEOUT   (30 *1000)

enum SENDCMDTYPE{
    SendCmdType_Login=1,        //登陆命令
    SendCmdType_HearBeat,       //心跳
    SendCmdType_UPloadSta,      //上传状态
};




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QTcpSocket *m_tcpSocket;

    void RecvStrParserPack(QString str);
    void RecvStrParserType(QString str);

    //设备主动发送信息
    void SocketSendPack_Login();
    void SocketSendPack_HeartBeat();

    void SocketSendPack_UploadSta();



    //设备回复状态信息
    void SocketSendPack_UploadSta_ACK(int Mi);

    //设备回复执行ACK信息 - 通用应答
    void SocketSendPack_Exec_ACK(int Mi);

    //设备回复 -获取参数信息
    void SocketSendPack_GetParam_ACK(int Mi);

    //设备回复 - 获取版本号
    void SocketSendPack_GetVer_ACK(int Mi);

    //设备回复 - 获取网络信息
    void SocketSendPack_GetNet_ACK(int Mi);

    //设备回复 - 开始升级OTA
    void SocketSendPack_GetOTA_ACK(int Mi);

    //设备回复 - 召测告警信息
    void SocketSendPack_GetAlarm_ACK(int Mi);

    //设备回复 - 查询策略
    void SocketSendPack_GetSche_ACK(int Mi);



    void onConnected();
    void onDisConnected();
    void onReadyRead();
    void onStateChanged(QAbstractSocket::SocketState);

    char* getTextFromStr(QString string);


    //发送的Mi号
    int SendMiSerial;

    //等待接收命令的类型
    int WaitAckCmdType;



    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
     void HearBeatTimoutFunc();  //超时处理函数

private slots:
    void on_pushButton_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;

    QTimer *m_pTimer;
};
#endif // MAINWINDOW_H
