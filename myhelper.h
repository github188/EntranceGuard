#ifndef MYHELPER_H
#define MYHELPER_H

#include <QtCore>
#include <QtGui>
#include <QDesktopWidget>
#include "frmmessagebox.h"
#include "frmoperateparentmsgbox.h"
class myHelper: public QObject
{

public:

    //设置为开机启动
    static void AutoRunWithSystem(bool IsAutoRun, QString AppName, QString AppPath)
    {
        QSettings *reg = new QSettings(
            "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
            QSettings::NativeFormat);

        if (IsAutoRun) {
            reg->setValue(AppName, AppPath);
        } else {
            reg->setValue(AppName, "");
        }
    }

    //设置编码为UTF8
    static void SetUTF8Code()
    {
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForTr(codec);
#endif
    }

    //设置皮肤样式
    static void SetStyle(const QString &styleName)
    {
        QFile file(QString(":/image/%1.css").arg(styleName));
        file.open(QFile::ReadOnly);
        QString qss = QLatin1String(file.readAll());
        qApp->setStyleSheet(qss);
        qApp->setPalette(QPalette(QColor("#F0F0F0")));
    }

    //加载中文字符
    static void SetChinese()
    {
        QTranslator *translator = new QTranslator(qApp);
        translator->load(":/image/qt_zh_CN.qm");
        qApp->installTranslator(translator);
    }

    //判断是否是IP地址
    static bool IsIP(QString IP)
    {
        QRegExp RegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
        return RegExp.exactMatch(IP);
    }
    //显示新建区域对话框
    static void ShowNewParentMsgBoxInfo(QString info)
    {
        FrmOperateParentMsgBox *msg = new FrmOperateParentMsgBox;
        msg->setModal(true);
        msg->SetMessage(info, 0);

        msg->show();
    }
    //显示修改区域对话框
    static void ShowEditParentMsgBoxInfo(QString info)
    {
        FrmOperateParentMsgBox *msg = new FrmOperateParentMsgBox;
        msg->setModal(true);
        msg->SetMessage(info, 1);
        msg->show();
    }
    //显示信息框,仅确定按钮
    static void ShowMessageBoxInfo(QString info)
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(info, 0);
        msg->setModal(true);
        //msg->exec();
        msg->show();
    }

    //显示错误框,仅确定按钮
    static void ShowMessageBoxError(QString info)
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(info, 2);
        msg->setModal(true);
        //msg->setWindowFlags(msg->windowFlags() | Qt::WindowStaysOnTopHint);
        //msg->exec();
        msg->show();
    }

    //显示询问框,确定和取消按钮
    static int ShowMessageBoxQuesion(QString info)
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(info, 1);
        msg->setWindowFlags(msg->windowFlags() | Qt::WindowStaysOnTopHint);
        return msg->exec();
    }

    //延时
    static void Sleep(int sec)
    {
        QTime dieTime = QTime::currentTime().addMSecs(sec);
        while ( QTime::currentTime() < dieTime ) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }

    //窗体居中显示
    static void FormInCenter(QWidget *frm)
    {
        int frmX = frm->width();
        int frmY = frm->height();
        QDesktopWidget w;
        int deskWidth = w.width();
        int deskHeight = w.height();
        QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
        frm->move(movePoint);
    }
    //汉字转化为GBK编码
    static QByteArray codecToGBK(QString code)
    {
        //汉字转GBK编码
        QByteArray tmpArray;
        QTextCodec *codec=QTextCodec::codecForName("GBK");

        tmpArray = codec->fromUnicode(code);
        return tmpArray;
    }
    //GBK编码转化为汉字
    static QString decodecToGBK(QByteArray tmpArray)
    {
        QString name;
        QTextCodec *codec=QTextCodec::codecForName("GBK");
        //GBK编码转汉字
        name = codec->toUnicode(tmpArray);

        return name;
    }
    static QByteArray GetFileMd5(QString FilePath)
    {
        //获取文件的md5
        QByteArray md5;
        QFile localFile(FilePath);

         if (!localFile.open(QFile::ReadOnly))
         {
             qDebug() << "file open error.";
             return md5;
         }

         QCryptographicHash ch(QCryptographicHash::Md5);

         quint64 totalBytes = 0;
         quint64 bytesWritten = 0;
         quint64 bytesToWrite = 0;
         quint64 loadSize = 1024 * 4;
         QByteArray buf;

         totalBytes = localFile.size();
         bytesToWrite = totalBytes;

         while (1)
         {
             if(bytesToWrite > 0)
             {
                 buf = localFile.read(qMin(bytesToWrite, loadSize));
                 ch.addData(buf);
                 bytesWritten += buf.length();
                 bytesToWrite -= buf.length();
                 buf.resize(0);
             }
             else
             {
                 break;
               }

             if(bytesWritten == totalBytes)
             {
                 break;
             }
         }

         localFile.close();
         md5 = ch.result();

         return md5;


    }
    /*************************************************
    Function: formatString()
    Description: 将十六进制字串每字节中间加空格分隔
    Calls:
    Called By:hexStringtoByteArray()
    Input: org->待处理的字串
           n->间隔数默认为2
           ch->分隔标志，在此取空格
    Output: NULL
    Return: void
    Others: NULL
    *************************************************/
    static void formatString(QString &org, int n=2, const QChar &ch=QChar(' '))
    {
        int size= org.size();
        int space= qRound(size*1.0/n+0.5)-1;
        if(space<=0)
            return;
        for(int i=0,pos=n;i<space;++i,pos+=(n+1))
        {
            org.insert(pos,ch);
        }
    }
    /*************************************************
    Function: hexStringtoByteArray()
    Description: 十六进制字串转化为十六进制编码
    Calls: formatString()
    Called By:
    Input: hex->待转化的十六进制字串
    Output: NULL
    Return: QByteArray
    Others: NULL
    *************************************************/
    static QByteArray hexStringtoByteArray(QString hex)
    {
        QByteArray ret;
        hex=hex.trimmed();
        formatString(hex,2,' ');
        QStringList sl=hex.split(" ");
        foreach(QString s,sl)
        {
            if(!s.isEmpty())
                ret.append((char)s.toInt(0,16)&0xFF);
        }
        return ret;
    }
    //显示标准输入框
    static QString ShowInputBox(QWidget *frm,QString info)
    {
        bool ok;
        return QInputDialog::getText(frm,"提示",info,QLineEdit::Password,"",&ok);
    }

    //16进制字符串转字节数组
    static QByteArray HexStrToByteArray(QString str)
    {
        QByteArray senddata;
        int hexdata,lowhexdata;
        int hexdatalen = 0;
        int len = str.length();
        senddata.resize(len/2);
        char lstr,hstr;
        for(int i=0; i<len; ) {
            hstr=str[i].toLatin1();
            if(hstr == ' ') {
                i++;
                continue;
            }
            i++;
            if(i >= len)
                break;
            lstr = str[i].toLatin1();
            hexdata = ConvertHexChar(hstr);
            lowhexdata = ConvertHexChar(lstr);
            if((hexdata == 16) || (lowhexdata == 16))
                break;
            else
                hexdata = hexdata*16+lowhexdata;
            i++;
            senddata[hexdatalen] = (char)hexdata;
            hexdatalen++;
        }
        senddata.resize(hexdatalen);
        return senddata;
    }

    static char ConvertHexChar(char ch)
    {
        if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
        else return (-1);
    }

    //字节数组转16进制字符串
    static QString ByteArrayToHexStr(QByteArray data)
    {
        QString temp="";
        QString hex=data.toHex();
        for (int i=0;i<hex.length();i=i+2){
            temp+=hex.mid(i,2)+" ";
        }
        return temp.trimmed().toUpper();
    }

    //16进制字符串转10进制
    static int StrHexToDecimal(QString strHex)
    {
        bool ok;
        return strHex.toInt(&ok,16);
    }

    //10进制字符串转10进制
    static int StrDecimalToDecimal(QString strDecimal)
    {
        bool ok;
        return strDecimal.toInt(&ok,10);
    }

    //2进制字符串转10进制
    static int StrBinToDecimal(QString strBin)
    {
        bool ok;
        return strBin.toInt(&ok,2);
    }

    //16进制字符串转2进制字符串
    static QString StrHexToStrBin(QString strHex)
    {
        uchar decimal=StrHexToDecimal(strHex);
        QString bin=QString::number(decimal,2);
        uchar len=bin.length();
        if (len<8){
            for (int i=0;i<8-len;i++){
                bin="0"+bin;
            }
        }
        return bin;
    }

    //10进制转2进制字符串一个字节
    static QString DecimalToStrBin1(int decimal)
    {
        QString bin=QString::number(decimal,2);
        uchar len=bin.length();
        if (len<=8){
            for (int i=0;i<8-len;i++){
                bin="0"+bin;
            }
        }
        return bin;
    }

    //10进制转2进制字符串两个字节
    static QString DecimalToStrBin2(int decimal)
    {
        QString bin=QString::number(decimal,2);
        uchar len=bin.length();
        if (len<=16){
            for (int i=0;i<16-len;i++){
                bin="0"+bin;
            }
        }
        return bin;
    }

    //10进制转16进制字符串,补零.
    static QString DecimalToStrHex(int decimal)
    {
        QString temp=QString::number(decimal,16);
        if (temp.length()==1){
            temp="0"+temp;
        }
        return temp;
    }

    //设置系统日期时间
    static void SetSystemDateTime(int year,int month,int day,int hour,int min,int sec)
    {
        QProcess p(0);

        p.start("cmd");
        p.waitForStarted();
        p.write(QString("date %1-%2-%3\n").arg(year).arg(month).arg(day).toLatin1());
        p.closeWriteChannel();
        p.waitForFinished(1000);
        p.close();

        p.start("cmd");
        p.waitForStarted();
        p.write(QString("time %1:%2:%3.00\n").arg(hour).arg(min).arg(sec).toLatin1());
        p.closeWriteChannel();
        p.waitForFinished(1000);
        p.close();
    }

    //窗体居中,并且只有关闭按钮,不能调整大小
    static void FormOnlyCloseInCenter(QWidget *frm,int deskWidth,int deskHeigth)
    {
        int frmX=frm->width();
        int frmY=frm->height();
        QPoint movePoint(deskWidth/2-frmX/2,deskHeigth/2-frmY/2);
        frm->move(movePoint);                          //窗体居中
        frm->setFixedSize(frmX,frmY);                  //窗体不能调整大小
        frm->setWindowFlags(frm->windowFlags() & Qt::WindowCloseButtonHint);//窗体只有关闭按钮
        frm->setAttribute(Qt::WA_DeleteOnClose);       //关闭时自动释放资源
    }

    //窗体居中显示
    static void FormInCenter(QWidget *frm,int deskWidth,int deskHeigth)
    {
        int frmX=frm->width();
        int frmY=frm->height();
        QPoint movePoint(deskWidth/2-frmX/2,deskHeigth/2-frmY/2);
        frm->move(movePoint);
    }

    //窗体不能改变大小
    static void FormNotResize(QWidget *frm)
    {
        frm->setFixedSize(frm->width(),frm->height());
    }

    //获取选择的文件
    static QString GetFileName(QString filter)
    {
        return QFileDialog::getOpenFileName(0,"选择文件",QCoreApplication::applicationDirPath(),filter);
    }

    //获取选择的文件集合
    static QStringList GetFileNames(QString filter)
    {
        return QFileDialog::getOpenFileNames(0,"选择文件",QCoreApplication::applicationDirPath(),filter);
    }

    //获取选择的目录
    static QString GetFolderName()
    {
        return QFileDialog::getExistingDirectory();;
    }

    //获取文件名,含拓展名
    static QString GetFileNameWithExtension(QString strFilePath)
    {
        QFileInfo fileInfo(strFilePath);
        return fileInfo.fileName();
    }

    //获取选择文件夹中的文件
    static QStringList GetFolderFileNames(QStringList filter)
    {
        QStringList fileList;
        QString strFolder = QFileDialog::getExistingDirectory();
        if (!strFolder.length()==0){
            QDir myFolder(strFolder);
            if (myFolder.exists()){
                fileList= myFolder.entryList(filter);
            }
        }
        return fileList;
    }

    //文件夹是否存在
    static bool FolderIsExist(QString strFolder)
    {
        QDir tempFolder(strFolder);
        return tempFolder.exists();
    }

    //文件是否存在
    static bool FileIsExist(QString strFile)
    {
        QFile tempFile(strFile);
        return tempFile.exists();
    }

    //复制文件
    static bool CopyFile(QString sourceFile, QString targetFile)
    {
        if (FileIsExist(targetFile)){
            int ret=QMessageBox::information(0,"提示","文件已经存在,是否替换?",QMessageBox::Yes | QMessageBox::No);
            if (ret!=QMessageBox::Yes){
                return false;
            }
        }
        return QFile::copy(sourceFile,targetFile);
    }

    //异或加密算法
    static QString getXorEncryptDecrypt(QString str, char key)
    {
        QByteArray data=str.toLatin1();
        int size=data.size();
        for(int i=0; i<size; i++){
            data[i] = data[i] ^ key;
        }
        return QLatin1String(data);
    }
};

#endif // MYHELPER_H
