#include "myapp.h"
#include "../myhelper.h"
#include <QSettings>

//程序运行过程中需要的全局变量
QString myApp::AppPath="";
/*
QString myApp::CurrentImage="main.jpg";//当前图片名称
int myApp::DeskWidth=1024;
int myApp::DeskHeigth=768;
QString myApp::KeyData="";

//存储当前登录用户信息,方便用户密码修改和用户退出操作
QString myApp::CurrentUserName="admin";
QString myApp::CurrentUserPwd="admin";
QString myApp::CurrentUserType="管理员";
*/
//存储全局配置信息,程序启动时加载,关闭时保存
QString myApp::SoftTitle="智能门禁管理系统";
QString myApp::SoftVersion="1.0.0.0";
QString myApp::CompanyName="济南奥盾电子科技有限公司";
QString myApp::ContactName="张三";

QString myApp::WebStation = "www.aoddz.com";//网址
QString myApp::telephone = "0531-85606870";//电话
QString myApp::cellphone = "15550021288";//手机
QString myApp::LastLoginer="admin";
QString myApp::MainPageImage="image/ATM.jpg";
QString myApp::LogoImage="image/logo.ico";
QString myApp:: UdpPort = "3611";
QString myApp:: TcpPort = "13710";

/*
bool myApp::AutoRun=false;
bool myApp::AutoLogin=false;
bool myApp::AutoBank=false;
*/
//配置文件名
QString fileName="AppConfig.ini";

void myApp::ReadConfig()
{

    //如果配置文件不存在,则以初始值继续运行
    //没有这个判断的话,配置赋值都为空
    if (!myHelper::FileIsExist(myApp::AppPath+fileName))
    {
        //对应中文转成正确的编码
        //myApp::CurrentUserType=myApp::CurrentUserType.toLatin1();
        myApp::SoftTitle=myApp::SoftTitle.toLatin1();
        myApp::MainPageImage=myApp::MainPageImage.toLatin1();
        myApp::LogoImage=myApp::LogoImage.toLatin1();
        myApp::CompanyName=myApp::CompanyName.toLatin1();
        myApp::WebStation = myApp::WebStation.toLatin1();//网址
        myApp::telephone = myApp::telephone.toLatin1();//电话
        myApp::cellphone = myApp::cellphone.toLatin1();//手机
        //myApp::ContactName=myApp::ContactName.toLatin1();
        //myApp::MsgTemplet=myApp::MsgTemplet.toLatin1();
        myApp::UdpPort = myApp::UdpPort.toLatin1();
        myApp::TcpPort = myApp::TcpPort.toLatin1();
        return;
    }

    QSettings *set=new QSettings(myApp::AppPath+fileName,QSettings::IniFormat);

    set->beginGroup("AppConfig");

    myApp::SoftTitle=set->value("SoftTitle").toString();
    myApp::MainPageImage =set->value("MainPageImage").toString();
    myApp::LogoImage =set->value("LogoImage").toString();
    myApp::CompanyName = set->value("CompanyName").toString();
    myApp::UdpPort = set->value("UdpPort").toString();
    myApp::TcpPort = set->value("TcpPort").toString();
    myApp::WebStation = set->value("WebStation").toString();//网址
    myApp::telephone = set->value("telephone").toString();//电话
    myApp::cellphone = set->value("cellphone").toString();//手机
    set->endGroup();
    delete set;
}

void myApp::WriteConfig()
{    
    QSettings *set=new QSettings(myApp::AppPath+fileName,QSettings::IniFormat);

    set->beginGroup("AppConfig");

    set->setValue("SoftTitle",myApp::SoftTitle);
    set->setValue("CompanyName",myApp::CompanyName);
    set->setValue("MainPageImage",myApp::MainPageImage);
    set->setValue("LogoImage",myApp::LogoImage);
    set->setValue("UdpPort",myApp::UdpPort);
    set->setValue("TcpPort",myApp::TcpPort);
    set->setValue("WebStation",myApp::WebStation);
    set->setValue("telephone",myApp::telephone);
    set->setValue("cellphone",myApp::cellphone);
    set->endGroup();
    delete set;
}

