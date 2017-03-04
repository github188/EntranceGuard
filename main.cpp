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
    //使用插件库 防止图片，文字，按钮等样式显示不正确
    QApplication::addLibraryPath("./plugins");
    //初始化日志记录系统
    QsLogging::Logger& logger = QsLogging::Logger::instance();
    logger.setLoggingLevel(QsLogging::InfoLevel);
    const QString sLogPath(QDir(a.applicationDirPath()).filePath("App.log"));
    QsLogging::DestinationPtr fileDestination(QsLogging::DestinationFactory::MakeFileDestination(sLogPath, EnableLogRotation, MaxSizeBytes(1024000), MaxOldLogCount(2)));
    QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);

    myHelper::SetUTF8Code();

    myHelper::SetStyle("gray");//灰色风格
    QLOG_INFO() << "Built with Qt" << QT_VERSION_STR << "running on" << qVersion();

    //创建共享内存,判断是否已经运行程序
    myApp::AppPath=QApplication::applicationDirPath()+"/";
    //qDebug()<<myApp::AppPath;
    //QString path = myApp::AppPath+"table.db";
    if (!myHelper::FileIsExist(myApp::AppPath+"table.db"))
    {
        frmMessageBox *msg = new frmMessageBox();
        msg->SetMessage("数据库文件不存在,程序将自动关闭！", 2);
        return msg->exec();
    }

    myApp::ReadConfig();
    FrmLogin w;
    w.show();//显示登录界面


    return a.exec();
}
