#ifndef VOICEINFOBANK_H
#define VOICEINFOBANK_H
#include <qglobal.h>
#include <QApplication>
#include <QByteArray>
#include <QTextCodec>
#include <QDebug>
#include <QDateTime>

class VoiceInfoBank
{
public:
    VoiceInfoBank();
    ~VoiceInfoBank();

    QString md5;        //声音文件的md5
    QString fileName;   //声音文件的存储文件名
    QString name;       //声音文件的显示名称
    quint8 generation;
signals:

public slots:
};

#endif // VOICEINFOBANK_H
