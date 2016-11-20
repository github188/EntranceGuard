#ifndef SLAVEMSG_H
#define SLAVEMSG_H

#include <qglobal.h>
#include <QByteArray>
#include <QTextCodec>
#include <QDebug>
#include "../QsLog/QsLog.h"
#include "../QsLog/QsLogDest.h"
#include "../QsLog/QsLogLevel.h"

class SlaveMsg
{

public:
    SlaveMsg();
    ~SlaveMsg();
public:
   quint32 time;
   quint8 msgType;
   quint8 fushu1;
   quint8 fushu2;
   uchar cardNum[8];
   QString logItem;
public:
   QString getCardNum(void);
   QString getTime(void);
   QString getLogItem(void);
   QString getTypeName(void);
   QString getFuShu_1_name(void);
   QString getFuShu_2_name(void);
   QString getFuShu_1_1(void);
   QString getFuShu_1_2(void);
   QString getFuShu_1_3(void);
   QString getFuShu_1_4(void);
   QString getFuShu_1_5(void);
   QString getFuShu_1_200(void);
   QString getFuShu_1_201(void);
   QString getFuShu_2_1(void);
   QString getFuShu_2_2(void);
   QString getFuShu_2_3(void);
   QString getFuShu_2_4(void);
   QString getFuShu_2_5(void);
   QString getFuShu_2_200(void);
   QString getFuShu_2_201(void);

};

#endif // SLAVEMSG_H
