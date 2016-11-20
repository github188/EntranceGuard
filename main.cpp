#include "frmmain.h"
#include <QApplication>
#include "myhelper.h"
#include "QsLog/QsLog.h"
#include "QsLog/QsLogDest.h"
#include "QsLog/QsLogLevel.h"
#include <stdio.h>
#include <stdlib.h>
#include <QDir>
#include <QTextCodec>
#include <QtSql>
#include "./frmlogin.h"
#include "./AppConfig/myapp.h"
int main(int argc, char *argv[])
{
    using namespace QsLogging;

    QApplication a(argc, argv);
    //初始化日志记录系统
    QsLogging::Logger& logger = QsLogging::Logger::instance();
    logger.setLoggingLevel(QsLogging::InfoLevel);
    const QString sLogPath(QDir(a.applicationDirPath()).filePath("App.log"));
    QsLogging::DestinationPtr fileDestination(QsLogging::DestinationFactory::MakeFileDestination(sLogPath, EnableLogRotation, MaxSizeBytes(1024000), MaxOldLogCount(2)));
    QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);

    myHelper::SetUTF8Code();

//  myHelper::SetStyle("black");//黑色风格
//  myHelper::SetStyle("blue");//蓝色风格
    myHelper::SetStyle("gray");//灰色风格
//  myHelper::SetStyle("navy");//天蓝色风格
    myHelper::SetChinese();

    QLOG_INFO() << "Built with Qt" << QT_VERSION_STR << "running on" << qVersion();
    //a.setApplicationName("AMS");        //设置应用程序名称
    //a.setApplicationVersion("V1.0");    //设置应用程序版本
    //使用插件库 防止图片，文字，按钮等样式显示不正确
    QApplication::addLibraryPath("./plugins");
    //创建共享内存,判断是否已经运行程序
    /*
    QSharedMemory mem("SystemName");
    if(!mem.create(1))
    {
        frmMessageBox *msg = new frmMessageBox();
        msg->SetMessage("程序已运行,软件将自动关闭!", 2);
        msg->exec();
        delete msg;
        return 1;
    }
    */
    myApp::AppPath=QApplication::applicationDirPath()+"/";
    //qDebug()<<myApp::AppPath;
    if (!myHelper::FileIsExist(myApp::AppPath+"table.db"))
    {
        frmMessageBox *msg = new frmMessageBox();
        msg->SetMessage("数据库文件不存在,程序将自动关闭！", 2);
        msg->exec();
        delete msg;
        return 1;
    }

    myApp::ReadConfig();
    FrmLogin w;
    w.show();//显示登录界面


    return a.exec();
}
