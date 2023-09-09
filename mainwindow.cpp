#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->textEdit->setText("192.168.31.128");  //默认IP地址
    ui->textEdit_2->setText("8080");  //默认端口

    m_tcpSocket = new QTcpSocket(this);
    connect(m_tcpSocket,&QTcpSocket::connected,this,&MainWindow::onConnected);
    connect(m_tcpSocket,&QTcpSocket::disconnected,this,&MainWindow::onDisConnected);
    connect(m_tcpSocket,&QTcpSocket::stateChanged,this,&MainWindow::onStateChanged);
    connect(m_tcpSocket,&QTcpSocket::readyRead,this,&MainWindow::onReadyRead);
}


void MainWindow::onConnected()
{
    ui->textBrowser->append("**已连接到服务器");

    //获取TextEdit内所有内容
    //    ui->plainTextEdit->appendPlainText("**已连接到服务器");
    //    ui->plainTextEdit->appendPlainText("**peer address: "+
    //    m_tcpSocket->peerAddress().toString());
    //    ui->plainTextEdit->appendPlainText("**peer port: "+
    //    QString::number(m_tcpSocket->peerPort()));
}


void MainWindow::onDisConnected()
{
    //    ui->plainTextEdit->appendPlainText("**已断开服务器");
    //    ui->btnConnect->setEnabled(true);
    //    ui->btnDisConnect->setEnabled(false);
     ui->textBrowser->append("**已断开服务器");
}


void MainWindow::onStateChanged(QAbstractSocket::SocketState)
{

}



void MainWindow::onReadyRead()
{
    //    while(m_tcpSocket->canReadLine())
    //    {
    //        ui->textBrowser->append("[in] "+ m_tcpSocket->readLine());
    //    }
    //显示接收的数据
    //ui->textBrowser->append("[Recv:] "+ m_tcpSocket->readLine());

    QString tSocketRecvStr = m_tcpSocket->readLine();
    ui->textBrowser->append("[接收:] "+ tSocketRecvStr);
    qDebug() << "接收 " << tSocketRecvStr << endl;

#if 1
    if(tSocketRecvStr.startsWith("`"))
    {
         int nIndex = tSocketRecvStr.indexOf("'"); // 到下一次出现
         QString str_target1 = tSocketRecvStr.mid(1,nIndex-1);
         ui->textBrowser->append("[分割第1包:] "+ str_target1);
         RecvStrParserPack(str_target1);

         //qDebug() << "len1 " << str_target1.length() << endl;
         //qDebug() << "nIndex " << nIndex << endl;

         if(tSocketRecvStr.length() > nIndex +2)
         {
             ui->textBrowser->append("[还有数据:] ");
             tSocketRecvStr = tSocketRecvStr.remove(0, nIndex + 1);

             int nIndexStart = tSocketRecvStr.indexOf("`"); // 到下一次出现
             int nIndexEnd = tSocketRecvStr.indexOf("'"); // 到下一次出现

             QString str_target2 = tSocketRecvStr.mid(nIndexStart+1, nIndexEnd-1); // 把分号也去掉
             if(str_target2 != NULL)  //还有数据
             {
                 ui->textBrowser->append("[分割第2包:] " + str_target2);
                 RecvStrParserPack(str_target2);
             } else {   //没有数据了
                 ui->textBrowser->append("[分割结束:] ");
             }
         } else {
            ui->textBrowser->append("[分割结束:] ");
         }
    }
 #endif

    //分割对应数据
    //ui->textBrowser->append(ShowInfo);
}


MainWindow::~MainWindow()
{
    delete ui;
}

//连接服务器按钮按下
void MainWindow::on_pushButton_clicked()
{
    QString addr = ui->textEdit->toPlainText();
    qDebug() << "addr " << addr << endl;   //方式1

    //QString addr = ui->textEdit->text();
    qint16 port = ui->textEdit_2->toPlainText().toInt();
    qDebug() << "port " << port << endl;
    QString ShowInfo = "开始连接服务器:" + addr + "/" + ui->textEdit_2->toPlainText();

    ui->textBrowser->append(ShowInfo);

    m_tcpSocket->connectToHost(addr,port);
}


//断开连接按钮按下
void MainWindow::on_pushButton_6_clicked()
{
    m_tcpSocket->disconnectFromHost();
}

//报警上报
void MainWindow::on_pushButton_4_clicked()
{
    // 获取当前时间戳
    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    int timeStr = timeDate .toTime_t();   				// 将当前时间转为时间戳
    int ProtolTim = timeStr - 946684800;            //将真正的协议时间减去 30年的时间 到2020年
    QString ShowInfo = "转换之后的时间戳: " + QString::number(ProtolTim);

    ui->textBrowser->append(ShowInfo);

    //ui->statusBar->append(QString::number(timeStr));
}

//手动发送数据按钮按下
void MainWindow::on_pushButton_7_clicked()
{
    //获取发送的数据
    QString str = ui->textEdit_10->toPlainText();
    //转换成QByteArray
    QByteArray arr;
    arr.append(str);

    //发送
    m_tcpSocket->write(arr);

    QString ShowInfo = "发送数据 " + str;
    ui->textBrowser->append(ShowInfo);

    qDebug() << "发送数据 " << str << endl;
}


//登录按钮按下
void MainWindow::on_pushButton_2_clicked()
{
     QString str = "`{\"T\":\"ECX/990620230829/R_LogIn\",\"ts\":746032715,\"mi\":1,\"ver\":\"5.16\",\"pver\":\"1.1\",\"imei\":\"862821068198130\",\"imsi\":\"460086978403500\",\"iccid\":\"898604E9092320843500\"}C6D8'";
     QByteArray arr;
     arr.append(str);
     m_tcpSocket->write(arr);
     QString ShowInfo = "发送数据 " + str;
     ui->textBrowser->append(ShowInfo);
     qDebug() << "发送数据 " << str << endl;
}

//状态上报
void MainWindow::on_pushButton_3_clicked()
{
    SocketSendPack_Login();
}
