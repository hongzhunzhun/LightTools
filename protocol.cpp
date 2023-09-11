#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <string.h>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>



static const uint8_t aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

static const uint8_t aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};

uint16_t CRC16_IBM_Calc(const void* buf, uint32_t len, uint16_t init_val)
{
    uint8_t ucCRCHi = init_val >> 8;
    uint8_t ucCRCLo = init_val & 0xFF;
    uint8_t iIndex;
    uint8_t* p = (uint8_t*)buf;
    while(len > 0)
    {
        iIndex = ucCRCLo ^ *( p++ );
        ucCRCLo = ( uint8_t )( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
        len--;
    }
    return ( uint16_t )( ucCRCHi << 8 | ucCRCLo );
}



char* MainWindow::getTextFromStr(QString string)
{
    QString str = string;
    char* text = NULL;
    QByteArray ba = string.toLatin1();
    text = ba.data();
    return text; // 这里text直接用没问题，最好不要return，上述strup的使用也不是很安全
}

#if 0
// 构建 JSON 文档
QJsonDocument document;
document.setObject(json);
QByteArray byteArray = document.toJson(QJsonDocument::Compact);
QString strJson(byteArray);
qDebug() << strJson;
#endif



//发送数据:设备登陆
void MainWindow::SocketSendPack_Login()
{
    QString str = "`{\"T\":\"ECX/990620230829/R_LogIn\",\"ts\":746032715,\"mi\":1,\"ver\":\"5.16\",\"pver\":\"1.1\",\"imei\":\"862821068198130\",\"imsi\":\"460086978403500\",\"iccid\":\"898604E9092320843500\"}C6D8'";


#if 0
    QString str = "`{\"T\":\"ECX/990620230829/R_LogIn\",\"ts\":746032715,\"mi\":1,\"ver\":\"5.16\",\"pver\":\"1.1\",\"imei\":\"862821068198130\",\"imsi\":\"460086978403500\",\"iccid\":\"898604E9092320843500\"}C6D8'";
    QByteArray arr;
    arr.append(str);
    m_tcpSocket->write(arr);

    QString ShowInfo = "发送数据 " + str;
    ui->textBrowser->append(ShowInfo);
    qDebug() << "发送数据 " << str << endl;
#endif
    //QString SnType = "ECX" + "/" + "990620230829" + "/" + "R_LogIn";
    QString SnType = "ECX/990620230829/R_LogIn";

    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    int timeStr = timeDate .toTime_t();   				// 将当前时间转为时间戳
    int ProtolTim = timeStr - 946684800;            //将真正的协议时间减去 30年的时间 到2020年
    QString sTimeStamp = QString::number(ProtolTim);

    SendMiSerial = 1;

#if 0
    QJsonObject json;
    json.insert("T", SnType);
    json.insert("ts", ProtolTim);
    json.insert("mi", SendMiSerial);
    json.insert("ver", "5.16");
    json.insert("pver", "1.1");
    json.insert("imei", "862821068198130");
    json.insert("imsi", "460086978403500");
    json.insert("iccid", "898604E9092320843500");
    json.insert("iccid", "5.16");


    // 构建 JSON 文档
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
#endif
    QString SendStr = "{\"T\":\"ECX/990620230829/R_LogIn\",\"ts\":" + sTimeStamp + ",\"mi\":1,\"ver\":\"5.16\",\"pver\":\"1.1\",\"imei\":\"862821068198130\",\"imsi\":\"460086978403500\",\"iccid\":\"898604E9092320843500\"}";

    //qDebug() << strJson;
    std::string str222 = SendStr.toStdString();
    const char* tBuffer = str222.c_str();
    int sLen = SendStr.length();

    //2.解析对应命令类型
    int init_val = 65535;
    uint16_t CRC_NUM = CRC16_IBM_Calc(tBuffer, sLen, init_val);
    qDebug() << "CRC_NUM " << CRC_NUM << endl;
    QString valueMaxHex = QString().sprintf("%04X", CRC_NUM);
    qDebug() << "valueMaxHex = " << valueMaxHex;

    QString SendStrOut = "`" + SendStr + valueMaxHex + "'";

    QByteArray arr;
    arr.append(SendStrOut);
    m_tcpSocket->write(arr);



    QString ShowInfo = "发送数据 " + SendStrOut;
    ui->textBrowser->append(ShowInfo);
    //qDebug() << "发送数据 " << SendStrOut << endl;
    qDebug() << "最终输出[" << SendStrOut << "]" << endl;
}



//发送数据:心跳包
void MainWindow::SocketSendPack_HeartBeat()
{
    //`{"T":"ECX/990620230829/HB","ts":746032780,"rsrp":-28}7190'
    // {\"T\":\"ECX/990620230829/HB\",\"ts\":746032780,\"rsrp\":-28}

    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    int timeStr = timeDate .toTime_t();   				// 将当前时间转为时间戳
    int ProtolTim = timeStr - 946684800;            //将真正的协议时间减去 30年的时间 到2020年
    QString sTimeStamp = QString::number(ProtolTim);

    //SendMiSerial ++;
    QString SendStr = "{\"T\":\"ECX/990620230829/HB\",\"ts\":" + sTimeStamp +",\"rsrp\":-28}";

    std::string str222 = SendStr.toStdString();
    const char* tBuffer = str222.c_str();
    int sLen = SendStr.length();

    //2.解析对应命令类型
    int init_val = 65535;
    uint16_t CRC_NUM = CRC16_IBM_Calc(tBuffer, sLen, init_val);
    qDebug() << "CRC_NUM " << CRC_NUM << endl;
    QString valueMaxHex = QString().sprintf("%04X", CRC_NUM);
    qDebug() << "valueMaxHex = " << valueMaxHex;

    QString SendStrOut = "`" + SendStr + valueMaxHex + "'";

    QByteArray arr;
    arr.append(SendStrOut);
    m_tcpSocket->write(arr);

    QString ShowInfo = "发送数据 " + SendStrOut;
    ui->textBrowser->append(ShowInfo);
    //qDebug() << "发送数据 " << SendStrOut << endl;
    qDebug() << "最终输出[" << SendStrOut << "]" << endl;
}



//发送数据:状态同步
void MainWindow::SocketSendPack_UploadSta()
{
    //{"T":"ECX/990620230829/R_Stat","ts":746032709,"mi":2,"ver":"5.16","rsrp":-88,"snr":5,"warn":0,"auto1":1,"val1":100,"warn1":0,"u1":2338,"i1":45,"p1":0,"e1":0,"auto2":1,"val2":100,"warn2":0,"u2":2338,"i2":45,"p2":0,"e2":0}
    //{\"T\":\"ECX/990620230829/R_Stat\",\"ts\":746032709,\"mi\":2,\"ver\":\"5.16\",\"rsrp\":-88,\"snr\":5,\"warn\":0,\"auto1\":1,\"val1\":100,\"warn1\":0,\"u1\":2338,\"i1\":45,\"p1\":0,\"e1\":0,\"auto2\":1,\"val2\":100,\"warn2\":0,\"u2\":2338,\"i2\":45,\"p2\":0,\"e2\":0}

    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    int timeStr = timeDate .toTime_t();   				// 将当前时间转为时间戳
    int ProtolTim = timeStr - 946684800;                // 将真正的协议时间减去 30年的时间 到2020年
    QString sTimeStamp = QString::number(ProtolTim);

    SendMiSerial ++;
    QString SendStr = "{\"T\":\"ECX/990620230829/R_Stat\",\"ts\":" + sTimeStamp + ",\"mi\":" + QString::number(SendMiSerial) + ",\"ver\":\"5.16\",\"rsrp\":-88,\"snr\":5,\"warn\":0,\"auto1\":1,\"val1\":100,\"warn1\":0,\"u1\":2338,\"i1\":45,\"p1\":0,\"e1\":0,\"auto2\":1,\"val2\":100,\"warn2\":0,\"u2\":2338,\"i2\":45,\"p2\":0,\"e2\":0}";

    std::string str222 = SendStr.toStdString();
    const char* tBuffer = str222.c_str();
    int sLen = SendStr.length();


    //2.解析对应命令类型
    int init_val = 65535;
    uint16_t CRC_NUM = CRC16_IBM_Calc(tBuffer, sLen, init_val);
    qDebug() << "CRC_NUM " << CRC_NUM << endl;
    QString valueMaxHex = QString().sprintf("%04X", CRC_NUM);
    qDebug() << "valueMaxHex = " << valueMaxHex;
    QString SendStrOut = "`" + SendStr + valueMaxHex + "'";


    QByteArray arr;
    arr.append(SendStrOut);
    m_tcpSocket->write(arr);


    QString ShowInfo = "发送数据 " + SendStrOut;
    ui->textBrowser->append(ShowInfo);
    //qDebug() << "发送数据 " << SendStrOut << endl;
    qDebug() << "最终输出[" << SendStrOut << "]" << endl;
}




//设备回复上传状态信息
void MainWindow::SocketSendPack_UploadSta_ACK(int Mi)
{
    //{"T":"ECX/990620230829/R_Stat","ts":746032709,"mi":2,"ver":"5.16","rsrp":-88,"snr":5,"warn":0,"auto1":1,"val1":100,"warn1":0,"u1":2338,"i1":45,"p1":0,"e1":0,"auto2":1,"val2":100,"warn2":0,"u2":2338,"i2":45,"p2":0,"e2":0}
    //{\"T\":\"ECX/990620230829/R_Stat\",\"ts\":746032709,\"mi\":2,\"ver\":\"5.16\",\"rsrp\":-88,\"snr\":5,\"warn\":0,\"auto1\":1,\"val1\":100,\"warn1\":0,\"u1\":2338,\"i1\":45,\"p1\":0,\"e1\":0,\"auto2\":1,\"val2\":100,\"warn2\":0,\"u2\":2338,\"i2\":45,\"p2\":0,\"e2\":0}

    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    int timeStr = timeDate .toTime_t();   				// 将当前时间转为时间戳
    int ProtolTim = timeStr - 946684800;                // 将真正的协议时间减去 30年的时间 到2020年
    QString sTimeStamp = QString::number(ProtolTim);

    QString SendStr = "{\"T\":\"ECX/990620230829/B_Stat\",\"ts\":" + sTimeStamp + ",\"mi\":" + QString::number(Mi) + ",\"ver\":\"5.16\",\"rsrp\":-88,\"snr\":5,\"warn\":0,\"auto1\":1,\"val1\":100,\"warn1\":0,\"u1\":2338,\"i1\":45,\"p1\":0,\"e1\":0,\"auto2\":1,\"val2\":100,\"warn2\":0,\"u2\":2338,\"i2\":45,\"p2\":0,\"e2\":0}";

    std::string str222 = SendStr.toStdString();
    const char* tBuffer = str222.c_str();
    int sLen = SendStr.length();


    //2.解析对应命令类型
    int init_val = 65535;
    uint16_t CRC_NUM = CRC16_IBM_Calc(tBuffer, sLen, init_val);
    qDebug() << "CRC_NUM " << CRC_NUM << endl;
    QString valueMaxHex = QString().sprintf("%04X", CRC_NUM);
    qDebug() << "valueMaxHex = " << valueMaxHex;
    QString SendStrOut = "`" + SendStr + valueMaxHex + "'";


    QByteArray arr;
    arr.append(SendStrOut);
    m_tcpSocket->write(arr);


    QString ShowInfo = "发送数据 " + SendStrOut;
    ui->textBrowser->append(ShowInfo);
    //qDebug() << "发送数据 " << SendStrOut << endl;
    qDebug() << "最终输出[" << SendStrOut << "]" << endl;
}



//设备回复执行指令ACK
void MainWindow::SocketSendPack_Exec_ACK(int Mi)
{
    //{"T":"ECX/990620230829/A","ts":746032688,"mi":5,"errid":0}
    //{\"T\":\"ECX/990620230829/A\",\"ts\":746032688,\"mi\":5,\"errid\":0}

    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    int timeStr = timeDate .toTime_t();   				// 将当前时间转为时间戳
    int ProtolTim = timeStr - 946684800;                // 将真正的协议时间减去 30年的时间 到2020年
    QString sTimeStamp = QString::number(ProtolTim);

    QString SendStr = "{\"T\":\"ECX/990620230829/A\",\"ts\":" + sTimeStamp + ",\"mi\":"+ QString::number(Mi) + ",\"errid\":0}";

    std::string str222 = SendStr.toStdString();
    const char* tBuffer = str222.c_str();
    int sLen = SendStr.length();


    //2.解析对应命令类型
    int init_val = 65535;
    uint16_t CRC_NUM = CRC16_IBM_Calc(tBuffer, sLen, init_val);
    qDebug() << "CRC_NUM " << CRC_NUM << endl;
    QString valueMaxHex = QString().sprintf("%04X", CRC_NUM);
    qDebug() << "valueMaxHex = " << valueMaxHex;
    QString SendStrOut = "`" + SendStr + valueMaxHex + "'";


    QByteArray arr;
    arr.append(SendStrOut);
    m_tcpSocket->write(arr);


    QString ShowInfo = "发送数据 " + SendStrOut;
    ui->textBrowser->append(ShowInfo);
    //qDebug() << "发送数据 " << SendStrOut << endl;
    qDebug() << "最终输出[" << SendStrOut << "]" << endl;
}



//获取参数信息 ACK
void MainWindow::SocketSendPack_GetParam_ACK(int Mi)
{
    //{\"T\": \"ECX/21FF011F2D3E/B_Ecx\",     \"ts\": 17173635,     \"mi\": 17173635,     \"Rpt_min_U\": 3,     \"Rpt_min_I\": 100,     \"Rpt_min_P\": 20,     \"Rpt_interval\": 3000,     \"ch_en\": 3,     \"warn_msk\": 255,     \"overcurrent_protect_en\": 0,     \"overcurrent_dly\": 5000,     \"overcurrent_val\": 5000 }
    //{\"T\":\"ECX/21FF011F2D3E/B_Ecx\",\"ts\":17173635,\"mi\":17173635,\"Rpt_min_U\":3,\"Rpt_min_I\":100,\"Rpt_min_P\":20,\"Rpt_interval\":3000,\"ch_en\":3,\"warn_msk\":255,\"overcurrent_protect_en\":0,\"overcurrent_dly\":5000,\"overcurrent_val\":5000}

    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    int timeStr = timeDate .toTime_t();   				// 将当前时间转为时间戳
    int ProtolTim = timeStr - 946684800;                // 将真正的协议时间减去 30年的时间 到2020年
    QString sTimeStamp = QString::number(ProtolTim);

    QString SendStr = "{\"T\":\"ECX/990620230829/B_Ecx\",\"ts\":" + sTimeStamp + ",\"mi\":"+ QString::number(Mi) + ",     \"Rpt_min_U\": 3,     \"Rpt_min_I\": 100,     \"Rpt_min_P\": 20,     \"Rpt_interval\": 3000,     \"ch_en\": 3,     \"warn_msk\": 255,     \"overcurrent_protect_en\": 0,     \"overcurrent_dly\": 5000,     \"overcurrent_val\": 5000 }";

    std::string str222 = SendStr.toStdString();
    const char* tBuffer = str222.c_str();
    int sLen = SendStr.length();


    //2.解析对应命令类型
    int init_val = 65535;
    uint16_t CRC_NUM = CRC16_IBM_Calc(tBuffer, sLen, init_val);
    qDebug() << "CRC_NUM " << CRC_NUM << endl;
    QString valueMaxHex = QString().sprintf("%04X", CRC_NUM);
    qDebug() << "valueMaxHex = " << valueMaxHex;
    QString SendStrOut = "`" + SendStr + valueMaxHex + "'";


    QByteArray arr;
    arr.append(SendStrOut);
    m_tcpSocket->write(arr);


    QString ShowInfo = "发送数据 " + SendStrOut;
    ui->textBrowser->append(ShowInfo);
    //qDebug() << "发送数据 " << SendStrOut << endl;
    qDebug() << "最终输出[" << SendStrOut << "]" << endl;
}



//获取版本号 ACK
void MainWindow::SocketSendPack_GetVer_ACK(int Mi)
{
    //{\"T\":\"ECX/990620230829/B_Ver\",\"ts\":747157623,\"mi\":44,\"ver\":\"5.16\",\"pver\":\"4.6\"}

    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    int timeStr = timeDate .toTime_t();   				// 将当前时间转为时间戳
    int ProtolTim = timeStr - 946684800;                // 将真正的协议时间减去 30年的时间 到2020年
    QString sTimeStamp = QString::number(ProtolTim);

    QString SendStr = "{\"T\":\"ECX/990620230829/B_Ver\",\"ts\":" + sTimeStamp + ",\"mi\":"+ QString::number(Mi) + ",\"ver\":\"5.16\",\"pver\":\"4.6\"}";

    std::string str222 = SendStr.toStdString();
    const char* tBuffer = str222.c_str();
    int sLen = SendStr.length();


    //2.解析对应命令类型
    int init_val = 65535;
    uint16_t CRC_NUM = CRC16_IBM_Calc(tBuffer, sLen, init_val);
    qDebug() << "CRC_NUM " << CRC_NUM << endl;
    QString valueMaxHex = QString().sprintf("%04X", CRC_NUM);
    qDebug() << "valueMaxHex = " << valueMaxHex;
    QString SendStrOut = "`" + SendStr + valueMaxHex + "'";


    QByteArray arr;
    arr.append(SendStrOut);
    m_tcpSocket->write(arr);


    QString ShowInfo = "发送数据 " + SendStrOut;
    ui->textBrowser->append(ShowInfo);
    //qDebug() << "发送数据 " << SendStrOut << endl;
    qDebug() << "最终输出[" << SendStrOut << "]" << endl;
}

//获取网络信息 ACK
void MainWindow::SocketSendPack_GetNet_ACK(int Mi)
{
    //{\"T\":\"ECX/990620230829/B_Net\",\"ts\":484,\"mi\":33,\"domain\":\"10.220.106.45\",\"port\":86,\"hb_interval\":300}

    //{"T":"ECX/21FF011F2D3E/B_Net",\"ts\":17173636,\"mi\":17173636,"domain":"192.168.60.54","port":0,"apn":"UNWAP","usr":"HALO","pwd":"123456"}

    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    int timeStr = timeDate .toTime_t();   				// 将当前时间转为时间戳
    int ProtolTim = timeStr - 946684800;                // 将真正的协议时间减去 30年的时间 到2020年
    QString sTimeStamp = QString::number(ProtolTim);

    QString SendStr = "{\"T\":\"ECX/990620230829/B_Net\",\"ts\":" + sTimeStamp + ",\"mi\":"+ QString::number(Mi) + ",\"domain\":\"10.220.106.45\",\"port\":86,\"hb_interval\":300}";

    std::string str222 = SendStr.toStdString();
    const char* tBuffer = str222.c_str();
    int sLen = SendStr.length();


    //2.解析对应命令类型
    int init_val = 65535;
    uint16_t CRC_NUM = CRC16_IBM_Calc(tBuffer, sLen, init_val);
    qDebug() << "CRC_NUM " << CRC_NUM << endl;
    QString valueMaxHex = QString().sprintf("%04X", CRC_NUM);
    qDebug() << "valueMaxHex = " << valueMaxHex;
    QString SendStrOut = "`" + SendStr + valueMaxHex + "'";


    QByteArray arr;
    arr.append(SendStrOut);
    m_tcpSocket->write(arr);


    QString ShowInfo = "发送数据 " + SendStrOut;
    ui->textBrowser->append(ShowInfo);
    //qDebug() << "发送数据 " << SendStrOut << endl;
    qDebug() << "最终输出[" << SendStrOut << "]" << endl;
}

//开始升级OTA ACK
void MainWindow::SocketSendPack_GetOTA_ACK(int Mi)
{
    //{\"T\":\"ECX/990620230829/A_Update\",\"ts\":742031082,\"mi\":5,\"errid\":0}

    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    int timeStr = timeDate .toTime_t();   				// 将当前时间转为时间戳
    int ProtolTim = timeStr - 946684800;                // 将真正的协议时间减去 30年的时间 到2020年
    QString sTimeStamp = QString::number(ProtolTim);

    QString SendStr = "{\"T\":\"ECX/990620230829/A_Update\",\"ts\":" + sTimeStamp + ",\"mi\":"+ QString::number(Mi) + ",\"errid\":0}";

    std::string str222 = SendStr.toStdString();
    const char* tBuffer = str222.c_str();
    int sLen = SendStr.length();


    //2.解析对应命令类型
    int init_val = 65535;
    uint16_t CRC_NUM = CRC16_IBM_Calc(tBuffer, sLen, init_val);
    qDebug() << "CRC_NUM " << CRC_NUM << endl;
    QString valueMaxHex = QString().sprintf("%04X", CRC_NUM);
    qDebug() << "valueMaxHex = " << valueMaxHex;
    QString SendStrOut = "`" + SendStr + valueMaxHex + "'";


    QByteArray arr;
    arr.append(SendStrOut);
    m_tcpSocket->write(arr);


    QString ShowInfo = "发送数据 " + SendStrOut;
    ui->textBrowser->append(ShowInfo);
    //qDebug() << "发送数据 " << SendStrOut << endl;
    qDebug() << "最终输出[" << SendStrOut << "]" << endl;
}



//召测告警信息 ACK
void MainWindow::SocketSendPack_GetAlarm_ACK(int Mi)
{
    //{\"T\":\"ECX/21FF011F2D3E]/R_Warn\",\"ts\":17173636,\"mi\":17173636,\"warn1\":\"0#1#0\",\"warn2\":\"0#1#0\"}

    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    int timeStr = timeDate .toTime_t();   				// 将当前时间转为时间戳
    int ProtolTim = timeStr - 946684800;                // 将真正的协议时间减去 30年的时间 到2020年
    QString sTimeStamp = QString::number(ProtolTim);

    QString SendStr = "{\"T\":\"ECX/990620230829/R_Warn\",\"ts\":" + sTimeStamp + ",\"mi\":"+ QString::number(Mi) + ",\"warn1\":\"0#1#0\",\"warn2\":\"0#1#0\"}";

    std::string str222 = SendStr.toStdString();
    const char* tBuffer = str222.c_str();
    int sLen = SendStr.length();


    //2.解析对应命令类型
    int init_val = 65535;
    uint16_t CRC_NUM = CRC16_IBM_Calc(tBuffer, sLen, init_val);
    qDebug() << "CRC_NUM " << CRC_NUM << endl;
    QString valueMaxHex = QString().sprintf("%04X", CRC_NUM);
    qDebug() << "valueMaxHex = " << valueMaxHex;
    QString SendStrOut = "`" + SendStr + valueMaxHex + "'";


    QByteArray arr;
    arr.append(SendStrOut);
    m_tcpSocket->write(arr);


    QString ShowInfo = "发送数据 " + SendStrOut;
    ui->textBrowser->append(ShowInfo);
    //qDebug() << "发送数据 " << SendStrOut << endl;
    qDebug() << "最终输出[" << SendStrOut << "]" << endl;
}


//查询策略 ACK
void MainWindow::SocketSendPack_GetSche_ACK(int Mi)
{
    //{\"T\":\"ECX/21FF011F2D3E/B_YearSche\",\"ts\":17173636,\"mi\":17173636,\"lt1\":\"00203202201e04300017201e18205a\",\"lt2\":\"00203202201e04300017201e18205a\"}

    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    int timeStr = timeDate .toTime_t();   				// 将当前时间转为时间戳
    int ProtolTim = timeStr - 946684800;                // 将真正的协议时间减去 30年的时间 到2020年
    QString sTimeStamp = QString::number(ProtolTim);

    QString SendStr = "{\"T\":\"ECX/990620230829/B_YearSche\",\"ts\":" + sTimeStamp + ",\"mi\":"+ QString::number(Mi) + ",\"lt1\":\"00203202201e04300017201e18205a\",\"lt2\":\"00203202201e04300017201e18205a\"}";

    std::string str222 = SendStr.toStdString();
    const char* tBuffer = str222.c_str();
    int sLen = SendStr.length();


    //2.解析对应命令类型
    int init_val = 65535;
    uint16_t CRC_NUM = CRC16_IBM_Calc(tBuffer, sLen, init_val);
    qDebug() << "CRC_NUM " << CRC_NUM << endl;
    QString valueMaxHex = QString().sprintf("%04X", CRC_NUM);
    qDebug() << "valueMaxHex = " << valueMaxHex;
    QString SendStrOut = "`" + SendStr + valueMaxHex + "'";


    QByteArray arr;
    arr.append(SendStrOut);
    m_tcpSocket->write(arr);


    QString ShowInfo = "发送数据 " + SendStrOut;
    ui->textBrowser->append(ShowInfo);
    //qDebug() << "发送数据 " << SendStrOut << endl;
    qDebug() << "最终输出[" << SendStrOut << "]" << endl;
}

//SendCmdType_Login=1,        //登陆命令
//SendCmdType_HearBeat,       //心跳
//SendCmdType_UPloadSta,      //上传状态
//QString MainWindow:: GetCurrentWaitAckType()
//{
//    QString CmdStr;
//    switch (WaitAckCmdType) {
//    case SendCmdType_Login:
//        CmdStr = "登陆命令";
//        break;
//    case SendCmdType_Login:
//        CmdStr = "心跳";
//        break;
//    case SendCmdType_Login:
//        CmdStr = "上传状态";
//        break;
//    default:
//        break;
//    }
//}


//接收数据，解析数据类型
void MainWindow::RecvStrParserType(QString str)
{
  QJsonDocument doucment = QJsonDocument::fromJson(str.toLocal8Bit().data());
  if(doucment.isNull())
  {
    qDebug()<< "解析JSON失败"<< str.toLocal8Bit().data();
  }

    //QJsonParseError jsonError;
    //QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);  // 转化为 JSON 文档

    if (!doucment.isNull()) {  // 解析未发生错误
        qDebug()<< "解析JSON成功";
        if (doucment.isObject()) { // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象

            if (object.contains("T")) {  // 包含指定的 key
                QJsonValue value = object.value("T");  // 获取指定 key 对应的 value
                if (value.isString()) {  // 判断 value 是否为字符串
                    QString strCmdType = value.toString();  // 将 value 转化为字符串
                    QString ShowInfo;
                    qDebug() << "T : " << strCmdType;

                    if(strCmdType.contains("A"))
                    {
                        ShowInfo += "收到:";
                        //ShowInfo += GetCurrentWaitAckType();
                        //QString CmdStr;

                        switch (WaitAckCmdType) {
                            case SendCmdType_Login:
                                ShowInfo += "[登陆命令]";
                                m_pTimer->start(TIMER_TIMEOUT);

                                qDebug() << "启动心跳定时器 ";
                                break;
                            case SendCmdType_HearBeat:
                                ShowInfo += "[心跳]";

                                break;
                            case SendCmdType_UPloadSta:
                                ShowInfo += "[上传状态]";

                                break;
                            default:
                                break;
                        }
                        ShowInfo += ":回复ACK";
                        ui->textBrowser->append(ShowInfo);
                    }

                    if(strCmdType.contains("G_Stat"))               //获取设备状态
                    {
                        ShowInfo += "收到数据类型:获取设备状态";
                        ui->textBrowser->append(ShowInfo);

                        if (object.contains("mi")) {
                            QJsonValue value = object.value("mi");
                            if (value.isDouble()) {
                                int MiValue = value.toVariant().toInt();
                                qDebug() << "mi : " << MiValue;

                                //回复状态信息
                                SocketSendPack_UploadSta_ACK(MiValue);
                            }
                        }

                    }


                    if(strCmdType.contains("E_Ctrl"))                   //回路控制
                    {
                        //ui->textBrowser->append("收到数据类型:控制指令");
                        ShowInfo += "收到数据类型:控制指令 ";
                        if (object.contains("lt1")) {
                            QJsonValue value = object.value("lt1");
                            if (value.isDouble()) {
                                int nFrom1 = value.toVariant().toInt();
                                qDebug() << "lt1 : " << nFrom1;
                                if(nFrom1 == 100)
                                {
                                    ShowInfo += "打开回路1灯光 100";
                                } else if(nFrom1 == 100) {
                                    ShowInfo += "关闭回路1灯光";
                                }
                            }
                        }

                        if (object.contains("lt2")) {
                            QJsonValue value = object.value("lt2");
                            if (value.isDouble()) {
                                int nFrom2 = value.toVariant().toInt();
                                qDebug() << "lt2 : " << nFrom2;
                                if(nFrom2 == 100)
                                {
                                    ShowInfo += "打开回路2灯光 100";
                                } else if(nFrom2 == 100) {
                                    ShowInfo += "关闭回路2灯光";
                                }
                            }
                        }
                        ui->textBrowser->append(ShowInfo);
                        if (object.contains("mi")) {
                            QJsonValue value = object.value("mi");
                            if (value.isDouble()) {
                                int MiValue = value.toVariant().toInt();
                                qDebug() << "mi : " << MiValue;

                                //回复执行OK 通用应答 ACK
                                SocketSendPack_Exec_ACK(MiValue);
                            }
                        }

                    }  //end

                    if(strCmdType.contains("S_Time") || strCmdType.contains("S_YearSche") )               //平台下发时间设置 + 设置开关灯时间
                    {
                        ShowInfo += "收到数据类型:控制指令 ";
                        ShowInfo += "[平台下发时间设置]";
                        ui->textBrowser->append(ShowInfo);

                        if (object.contains("mi")) {
                            QJsonValue value = object.value("mi");
                            if (value.isDouble()) {
                                int MiValue = value.toVariant().toInt();
                                qDebug() << "mi : " << MiValue;

                                //回复执行OK 通用应答 ACK
                                SocketSendPack_Exec_ACK(MiValue);
                            }
                        }
                    }

                    if(strCmdType.contains("S_Net"))               // 平台设置通讯参数
                    {
                        ShowInfo += "收到数据类型:控制指令 ";
                        ShowInfo += "[平台设置通讯参数]";
                        ui->textBrowser->append(ShowInfo);

                        if (object.contains("mi")) {
                            QJsonValue value = object.value("mi");
                            if (value.isDouble()) {
                                int MiValue = value.toVariant().toInt();
                                qDebug() << "mi : " << MiValue;

                                //回复执行OK 通用应答 ACK
                                SocketSendPack_Exec_ACK(MiValue);
                            }
                        }
                    }

                    if(strCmdType.contains("S_Ecx"))               // 平台下发设置参数
                    {
                        ShowInfo += "收到数据类型:控制指令 ";
                        ShowInfo += "[平台下发设置参数]";
                        ui->textBrowser->append(ShowInfo);

                        if (object.contains("mi")) {
                            QJsonValue value = object.value("mi");
                            if (value.isDouble()) {
                                int MiValue = value.toVariant().toInt();
                                qDebug() << "mi : " << MiValue;

                                //回复执行OK 通用应答 ACK
                                SocketSendPack_Exec_ACK(MiValue);
                            }
                        }
                    }



                    if(strCmdType.contains("E_Auto"))               //设置手自动状态
                    {
                        ShowInfo += "收到数据类型:控制指令 ";
                        ShowInfo += "[设置手自动状态]";
                        ui->textBrowser->append(ShowInfo);

                        if (object.contains("mi")) {
                            QJsonValue value = object.value("mi");
                            if (value.isDouble()) {
                                int MiValue = value.toVariant().toInt();
                                qDebug() << "mi : " << MiValue;

                                //回复执行OK 通用应答 ACK
                                SocketSendPack_Exec_ACK(MiValue);
                            }
                        }
                    }

                    if(strCmdType.contains("E_Reboot"))               //设置设备重启
                    {
                        ShowInfo += "收到数据类型:控制指令 ";
                        ShowInfo += "[设置设备重启]";
                        ui->textBrowser->append(ShowInfo);

                        if (object.contains("mi")) {
                            QJsonValue value = object.value("mi");
                            if (value.isDouble()) {
                                int MiValue = value.toVariant().toInt();
                                qDebug() << "mi : " << MiValue;

                                //回复执行OK 通用应答 ACK
                                SocketSendPack_Exec_ACK(MiValue);
                            }
                        }
                    }

                    if(strCmdType.contains("G_Ecx"))                //查询设备参数
                    {
                        ShowInfo += "收到数据类型:控制指令 ";
                        ShowInfo += "[查询设备参数]";
                        ui->textBrowser->append(ShowInfo);

                        if (object.contains("mi")) {
                            QJsonValue value = object.value("mi");
                            if (value.isDouble()) {
                                int MiValue = value.toVariant().toInt();
                                qDebug() << "mi : " << MiValue;

                                //回复执行OK 获取参数信息 ACK
                                SocketSendPack_GetParam_ACK(MiValue);
                            }
                        }
                    }

                    if(strCmdType.contains("G_Ver"))                //平台下发获取版本号
                    {
                        ShowInfo += "收到数据类型:控制指令 ";
                        ShowInfo += "[平台下发获取版本号]";
                        ui->textBrowser->append(ShowInfo);

                        if (object.contains("mi")) {
                            QJsonValue value = object.value("mi");
                            if (value.isDouble()) {
                                int MiValue = value.toVariant().toInt();
                                qDebug() << "mi : " << MiValue;

                                //回复执行OK 获取版本号 ACK
                                SocketSendPack_GetVer_ACK(MiValue);
                            }
                        }
                    }

                    if(strCmdType.contains("G_YearSche"))                //查询设备策略
                    {
                        ShowInfo += "收到数据类型:控制指令 ";
                        ShowInfo += "[查询设备策略]";
                        ui->textBrowser->append(ShowInfo);

                        if (object.contains("mi")) {
                            QJsonValue value = object.value("mi");
                            if (value.isDouble()) {
                                int MiValue = value.toVariant().toInt();
                                qDebug() << "mi : " << MiValue;

                                //回复执行OK 查询设备策略 ACK
                                SocketSendPack_GetSche_ACK(MiValue);
                            }
                        }
                    }

                    if(strCmdType.contains("G_Alarm"))                //召测告警信息
                    {
                        ShowInfo += "收到数据类型:控制指令 ";
                        ShowInfo += "[召测告警信息]";
                        ui->textBrowser->append(ShowInfo);

                        if (object.contains("mi")) {
                            QJsonValue value = object.value("mi");
                            if (value.isDouble()) {
                                int MiValue = value.toVariant().toInt();
                                qDebug() << "mi : " << MiValue;

                                //回复执行OK 召测告警信息 ACK
                                SocketSendPack_GetAlarm_ACK(MiValue);
                            }
                        }
                    }


                    if(strCmdType.contains("E_Update") || strCmdType.contains("E_Wri") || strCmdType.contains("E_Chk") )             //平台下发OTA升级报文
                    {
                        ShowInfo += "收到数据类型:控制指令 ";
                        ShowInfo += "[平台下发OTA升级报文]";
                        ui->textBrowser->append(ShowInfo);

                        if (object.contains("mi")) {
                            QJsonValue value = object.value("mi");
                            if (value.isDouble()) {
                                int MiValue = value.toVariant().toInt();
                                qDebug() << "mi : " << MiValue;

                                //回复执行OK 开始升级OTA ACK
                                SocketSendPack_GetOTA_ACK(MiValue);
                            }
                        }
                    }

                    if(strCmdType.contains("G_Net"))             //平台下发获取网络信息
                    {
                        ShowInfo += "收到数据类型:控制指令 ";
                        ShowInfo += "[平台下发获取网络信息]";
                        ui->textBrowser->append(ShowInfo);

                        if (object.contains("mi")) {
                            QJsonValue value = object.value("mi");
                            if (value.isDouble()) {
                                int MiValue = value.toVariant().toInt();
                                qDebug() << "mi : " << MiValue;

                                //回复执行OK 获取网络信息 ACK
                                SocketSendPack_GetNet_ACK(MiValue);
                            }
                        }
                    }
                }
            }



            if (object.contains("ts")) {
                QJsonValue value = object.value("ts");
                if (value.isDouble()) {
                    int nFrom = value.toVariant().toInt();
                    qDebug() << "ts : " << nFrom;
                }
            }


        }
    }
}





//接收数据包处理函数,解析数据包
void MainWindow::RecvStrParserPack(QString str)
{
    //1.校验CRC-16
    int sLen = str.length();
    int CrcEndLen =  str.length() - 4;
    QString CmdJson = str.mid(0, CrcEndLen);
    QString CmdCRC = str.mid(CrcEndLen, sLen); // 把分号也去掉

    QString ShowInfo = "JSon: [" + CmdJson + "]  CmdCRC: [" + CmdCRC + "]";
    ui->textBrowser->append(ShowInfo);
    //char * tBuffer = getTextFromStr(CmdJson);
    QString testCMD  = "111";

    std::string str222 = CmdJson.toStdString();
    const char* tBuffer = str222.c_str();
    sLen = CmdJson.length();


    //2.解析对应命令类型
    int init_val = 65535;
    //int init_val = 0;
    uint16_t CRC_NUM = CRC16_IBM_Calc(tBuffer, sLen, init_val);
    qDebug() << "CRC_NUM " << CRC_NUM << endl;
    //int myValue = 0x1234;
    QString valueInHex= QString("%1").arg(CRC_NUM , 0, 16);
    qDebug() << "valueInHex = " << valueInHex;

    QString valueMaxHex = QString().sprintf("%04X", CRC_NUM);
    qDebug() << "valueMaxHex = " << valueMaxHex;
    //qDebug()<<"格式化16进制大写输出"<<QString().sprintf("%04X",CRC_NUM);

    if(valueMaxHex == CmdCRC)
    {
        ShowInfo = "CRC校验成功" + valueMaxHex;
    } else {
        ShowInfo = "CRC校验失败" + valueMaxHex;
    }

    ui->textBrowser->append(ShowInfo);
    //3.判断接收数据类型和数据值

    RecvStrParserType(CmdJson);
}
















