#ifndef INDOORKEYMSG_H
#define INDOORKEYMSG_H

#include <qglobal.h>
#include <QByteArray>
#include <QTextCodec>
class IndoorKeyMsg
{
public:
    IndoorKeyMsg();
public:
   quint32 time;//时间，4个字节
   quint8 msgType;//记录类型，1个字节
   quint8 msgEvent;//记录事件，1个字节
   quint8 fushu1;//附属信息1，1个字节
   quint8 fushu2;//附属信息2，1个字节
   quint8 fushu3;//附属信息3，1个字节
   uchar cardNum[8];//卡号8个字节
   quint8 check;//校验，1个字节
public:
   QString getCardNum(void);
   QString getTime(void);
   QString getEventName(void);
   QString getTypeName(void);
   quint8 getCardPosition(void);//刷卡位置
   quint8 getCardType(void);//刷卡类型
   QString getFuShu_2(void);//附属信息2

   //获取刷卡位置
   QString getCardPositionStr(void);
};

#endif // INDOORKEYMSG_H
