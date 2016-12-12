#include "frmmanagermodule.h"
#include "ui_frmmanagermodule.h"
#include "iconhelper.h"
#include "myhelper.h"
#include "frmselectmoduleversion.h"

extern QString sFilePath;
FrmManagerModule::FrmManagerModule(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmManagerModule)
{
    ui->setupUi(this);
    frmModulePara = new FrmModulePara();
    voiceModel=NULL;
    InitStyle();
    connect(ui->btnNew,SIGNAL(clicked(bool)),this,SLOT(btnAddModule()));
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(btnCancel()));
    connect(ui->btnEdit,SIGNAL(clicked(bool)),this,SLOT(btnEditPara()));
    connect(ui->btnDelete,SIGNAL(clicked(bool)),this,SLOT(btnDeleteModule()));
    connect(ui->btnIn,SIGNAL(clicked(bool)),this,SLOT(slotbtnIn()));
    connect(ui->btnOut,SIGNAL(clicked(bool)),this,SLOT(slotbtnOut()));
    connect(frmModulePara,SIGNAL(sigSaveModulePara(QString,paraData*)),this,SLOT(slotSaveModule(QString,paraData*)));
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

FrmManagerModule::~FrmManagerModule()
{
    delete frmModulePara;
    delete voiceModel;
    delete ui;
}
void FrmManagerModule::InitStyle()
{
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;

    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);

    QStringList header;
    header<<"模版名称"<<"适用版本号";
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setColumnWidth(0,150);
    ui->tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->tableWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);//关键

}

bool FrmManagerModule::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void FrmManagerModule::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void FrmManagerModule::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void FrmManagerModule::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void FrmManagerModule::on_btnMenu_Close_clicked()
{
    this->close();
}

void FrmManagerModule::on_btnMenu_Max_clicked()
{
    if (max) {
        this->setGeometry(location);
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
        ui->btnMenu_Max->setToolTip("最大化");
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
        ui->btnMenu_Max->setToolTip("还原");
    }
    max = !max;
}

void FrmManagerModule::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}
void FrmManagerModule::btnAddModule()
{
    /*
    //新建模板
    //emit sigAddParaModule();
    paraData *para = new paraData();
    FrmModulePara *frm= new FrmModulePara();
    connect(frm,SIGNAL(sigSaveModulePara(QString,paraData*)),this,SLOT(slotSaveModule(QString,paraData*)));

    frm->setVoiceModel(voiceModel);
    frm->SetPara("新建模板",para,0);
    frm->SetTitleText("新建模板参数");
    frm->exec();
    delete para;
    */
    FrmSelectModuleVersion *modelVersion = new FrmSelectModuleVersion();
    connect(modelVersion,SIGNAL(sigSaveModule(QString,paraData*)),this,SLOT(slotSaveModule(QString,paraData*)));
    modelVersion->slotSetVersionInfoList(versionInfoList);
    modelVersion->setVoiceModel(voiceModel);
    modelVersion->setModal(true);//半模态调用对话框
    modelVersion->show();
}
void FrmManagerModule::btnDeleteModule()
{
    emit sigDelParaModule(ui->tableWidget->currentIndex());
}
void FrmManagerModule::btnCancel()
{
    this->close();
}
void FrmManagerModule::btnEditPara()
{
    //emit sigEditParaModule(ui->tableView->currentIndex());
    QModelIndex index = ui->tableWidget->currentIndex();
    QString name = slaveVersionList.at(index.row())->name;
    quint8 level = slaveVersionList.at(index.row())->level;
    paraData * tempPara=NULL;
    switch (level) {
    case 1:

        break;
    case 2:
    {
        for(int i=0;i<moduleList.count();i++)
        {
            if(moduleList.at(i)->name == name)
            {
                tempPara = &moduleList.at(i)->pdat;
                break;
            }
        }
        if(tempPara==NULL)
            return;

        FrmModulePara *frm= new FrmModulePara();
        connect(frm,SIGNAL(sigSaveModulePara(QString,paraData*)),this,SLOT(slotSaveModule(QString,paraData*)));
        frm->SetPara(name,tempPara,1);
        frm->setVoiceModel(voiceModel);
        frm->SetTitleText("修改模板参数");
        frm->exec();
    }
        break;
    default:
        break;
    }

}
void FrmManagerModule::setVoiceModel(QSqlTableModel *model)
{
    voiceModel = model;
}
void FrmManagerModule::slotSetVersionInfoList(QList<VersionInfo*> list)
{
    versionInfoList = list;
}
void FrmManagerModule::slotSaveModule(QString name,paraData *data)
{
    emit sigSaveModulePara(name,data);
}
void FrmManagerModule::slotSetTableModel(QList<SlaveVersion *> versionList, QList<paraModule*> list)
{
    //tableModel = model;
    slaveVersionList = versionList;
    moduleList=list;
    ui->tableWidget->setRowCount(slaveVersionList.length());
    for(int i=0;i<slaveVersionList.length();i++)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(slaveVersionList.at(i)->name));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(slaveVersionList.at(i)->version));
    }
    //ui->tableView->setModel(tableModel);
    //ui->tableView->setColumnWidth(0,317);
    //ui->tableView->horizontalHeader()->setStretchLastSection(true);//关键
    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
//导入模板
void FrmManagerModule::slotbtnIn()
{
    int md5size=16;
    QString path = QFileDialog::getOpenFileName(this, tr("选择文件"), ".", tr("Module Files(*.mod)"));
    paraModule * module = new paraModule();
    if(path.length())
    {
        QFile file(path);
        //方式：Append为追加，WriteOnly，ReadOnly
        if (!file.open(QIODevice::ReadOnly))
        {
            myHelper::ShowMessageBoxError("无法读取文件");
            return;
        }

        QDataStream out(&file);

        out >>module->name;

        out >>module->pdat.fangHuCang.openLockTime;
        out >>module->pdat.fangHuCang.OptBussinessTime;
        out >>module->pdat.fangHuCang.timeOutRemind;
        out >>module->pdat.fangHuCang.warnningDelayTime;
        out >>module->pdat.fangHuCang.noManOpenLockTime;
        out >>module->pdat.fangHuCang.fanRunTemperature;
        out >>module->pdat.fangHuCang.signalModel;
        out >>module->pdat.fangHuCang.peopleEquModel;
        out >>module->pdat.fangHuCang.lockModel;
        out >>module->pdat.fangHuCang.kongCangLockorNot;
        out >>module->pdat.fangHuCang.fangQiewarnning;

        out >>module->pdat.zhaoMing.Model;
        out >>module->pdat.zhaoMing.startHour;
        out >>module->pdat.zhaoMing.startMinute;
        out >>module->pdat.zhaoMing.endHour;
        out >>module->pdat.zhaoMing.endMinute;

        out >>module->pdat.ledText[0];
        out >>module->pdat.ledText[1];
        out >>module->pdat.ledText[2];

        out >>module->pdat.soundName[0];
        out >>module->pdat.soundName[1];
        out >>module->pdat.soundName[2];
        out >>module->pdat.soundName[3];
        out >>module->pdat.soundName[4];
        out >>module->pdat.soundName[5];
        out >>module->pdat.soundName[6];
        out >>module->pdat.soundName[7];
        out >>module->pdat.soundName[8];

        out >>module->pdat.soundVolume;
        module->pdat.soundMd5[0].resize(md5size);
        out >>module->pdat.soundMd5[0];
        quint64 datalength;
        out >> datalength;
        QByteArray mp0data;
        mp0data.resize(datalength);
        out >>mp0data;
        //写入文件
        QFile mp0file("./temp/0.mp3");
        //方式：Append为追加，WriteOnly，ReadOnly
        if (!mp0file.open(QIODevice::WriteOnly))
        {
            myHelper::ShowMessageBoxError("无法创建文件");
            return;
        }
        //QDataStream outmp0(&mp0file);
        QLOG_INFO()<<"文件长度0"<<mp0data.size();
        //QLOG_INFO()<<"文件内容"<<mp0data.toHex();
        //outmp0<<mp0data;
        mp0file.write(mp0data.data(),mp0data.length());
        mp0file.flush();
        mp0file.close();
        module->pdat.soundMd5[2].resize(md5size);
        out >>module->pdat.soundMd5[2];
        datalength=0;
        out >> datalength;
        QByteArray mp2data;
        mp2data.resize(datalength);
        out >>mp2data;
        //写入文件
        QFile mp2file("./temp/2.mp3");
        //方式：Append为追加，WriteOnly，ReadOnly
        if (!mp2file.open(QIODevice::WriteOnly))
        {
            myHelper::ShowMessageBoxError("无法创建文件");
            return;
        }
        //QDataStream outmp2(&mp2file);
        //outmp2<<mp2data;
        mp2file.write(mp2data.data(),mp2data.length());
        mp2file.flush();
        mp2file.close();
        module->pdat.soundMd5[3].resize(md5size);
        out >>module->pdat.soundMd5[3];
        datalength=0;
        out >> datalength;
        QByteArray mp3data;
        mp3data.resize(datalength);
        out >>mp3data;
        //写入文件
        QFile mp3file("./temp/3.mp3");
        //方式：Append为追加，WriteOnly，ReadOnly
        if (!mp3file.open(QIODevice::WriteOnly))
        {
            myHelper::ShowMessageBoxError("无法创建文件");
            return;
        }
        //QDataStream outmp3(&mp3file);
        //outmp3<<mp3data;
        mp3file.write(mp3data.data(),mp3data.length());
        mp3file.flush();
        mp3file.close();
        module->pdat.soundMd5[4].resize(md5size);
        out >>module->pdat.soundMd5[4];
        datalength=0;
        out >> datalength;
        QByteArray mp4data;
        mp4data.resize(datalength);
        out >>mp4data;
        //写入文件
        QFile mp4file("./temp/4.mp3");
        //方式：Append为追加，WriteOnly，ReadOnly
        if (!mp4file.open(QIODevice::WriteOnly))
        {
            myHelper::ShowMessageBoxError("无法创建文件");
            return;
        }
        //QDataStream outmp4(&mp4file);
        //outmp4<<mp4data;
        mp4file.write(mp4data.data(),mp4data.length());
        mp4file.flush();
        mp4file.close();
        module->pdat.soundMd5[5].resize(md5size);
        out >>module->pdat.soundMd5[5];
        datalength=0;
        out >> datalength;
        QByteArray mp5data;
        mp5data.resize(datalength);
        out >>mp5data;
        //写入文件
        QFile mp5file("./temp/5.mp3");
        //方式：Append为追加，WriteOnly，ReadOnly
        if (!mp5file.open(QIODevice::WriteOnly))
        {
            myHelper::ShowMessageBoxError("无法创建文件");
            return;
        }
        //QDataStream outmp5(&mp5file);
        //outmp5<<mp5data;
        mp5file.write(mp5data.data(),mp5data.length());
        mp5file.flush();
        mp5file.close();
        module->pdat.soundMd5[6].resize(md5size);
        out >>module->pdat.soundMd5[6];
        datalength=0;
        out >> datalength;
        QByteArray mp6data;
        mp6data.resize(datalength);
        out >>mp6data;
        //写入文件
        QFile mp6file("./temp/6.mp3");
        //方式：Append为追加，WriteOnly，ReadOnly
        if (!mp6file.open(QIODevice::WriteOnly))
        {
            myHelper::ShowMessageBoxError("无法创建文件");
            return;
        }
        //QDataStream outmp6(&mp6file);
        //outmp6<<mp6data;
        mp6file.write(mp6data.data(),mp6data.length());
        mp6file.flush();
        mp6file.close();
        module->pdat.soundMd5[7].resize(md5size);
        out >>module->pdat.soundMd5[7];
        datalength=0;
        out >> datalength;
        QByteArray mp7data;
        mp7data.resize(datalength);
        out >>mp7data;
        //写入文件
        QFile mp7file("./temp/7.mp3");
        //方式：Append为追加，WriteOnly，ReadOnly
        if (!mp7file.open(QIODevice::WriteOnly))
        {
            myHelper::ShowMessageBoxError("无法创建文件");
            return;
        }
        //QDataStream outmp7(&mp7file);
        //outmp7<<mp7data;
        mp7file.write(mp7data.data(),mp7data.length());
        mp7file.flush();
        mp7file.close();
        module->pdat.soundMd5[1].resize(md5size);
        out >>module->pdat.soundMd5[1];
        datalength=0;
        out >> datalength;
        QByteArray mp1data;
        mp1data.resize(datalength);
        out >>mp1data;
        //写入文件
        QFile mp1file("./temp/1.mp3");
        //方式：Append为追加，WriteOnly，ReadOnly
        if (!mp1file.open(QIODevice::WriteOnly))
        {
            myHelper::ShowMessageBoxError("无法创建文件");
            return;
        }
        //QDataStream outmp1(&mp1file);
        //outmp1<<mp1data;
        mp1file.write(mp1data.data(),mp1data.length());
        mp1file.flush();
        mp1file.close();
        module->pdat.soundMd5[8].resize(md5size);
        out >>module->pdat.soundMd5[8];
        datalength=8;
        out >> datalength;
        QByteArray mp8data;
        mp8data.resize(datalength);
        out >>mp8data;
        //写入文件
        QFile mp8file("./temp/8.mp3");
        //方式：Append为追加，WriteOnly，ReadOnly
        if (!mp8file.open(QIODevice::WriteOnly))
        {
            myHelper::ShowMessageBoxError("无法创建文件");
            return;
        }
        //QDataStream outmp8(&mp8file);
        //outmp8<<mp8data;
        mp8file.write(mp8data.data(),mp8data.length());
        mp8file.flush();
        mp8file.close();
        file.close();

        for(int i=0;i<moduleList.count();i++)
        {
            if(moduleList.at(i)->name == module->name)
            {
                frmMessageBox *msg = new frmMessageBox;
                msg->SetMessage("模板名称相同不能导入！", 0);
                msg->exec();
                return;
            }
        }
        emit sigInParaModule(module);
    } else
    {

    }
}
//导出模板
void FrmManagerModule::slotbtnOut()
{
    QByteArray filedata;
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("保存模板"),
            "",
            tr("Module Files (*.mod)"));

    if (!fileName.isNull())
    {
        //写入文件
        QFile file(fileName);
        //方式：Append为追加，WriteOnly，ReadOnly
        if (!file.open(QIODevice::WriteOnly))
        {
            myHelper::ShowMessageBoxError("无法创建文件");
            return;
        }
        QDataStream out(&file);
        QString name = tableModel->data(ui->tableWidget->currentIndex()).toString();
        paraModule * module=NULL;
        for(int i=0;i<moduleList.count();i++)
        {
            if(moduleList.at(i)->name == name)
            {
                module = moduleList.at(i);
                break;
            }
        }
        if(module==NULL)
            return;

        out<<name;
        out<<module->pdat.fangHuCang.openLockTime;
        out<<module->pdat.fangHuCang.OptBussinessTime;
        out<<module->pdat.fangHuCang.timeOutRemind;
        out<<module->pdat.fangHuCang.warnningDelayTime;
        out<<module->pdat.fangHuCang.noManOpenLockTime;
        out<<module->pdat.fangHuCang.fanRunTemperature;
        out<<module->pdat.fangHuCang.signalModel;
        out<<module->pdat.fangHuCang.peopleEquModel;
        out<<module->pdat.fangHuCang.lockModel;
        out<<module->pdat.fangHuCang.kongCangLockorNot;
        out<<module->pdat.fangHuCang.fangQiewarnning;

        out<<module->pdat.zhaoMing.Model;
        out<<module->pdat.zhaoMing.startHour;
        out<<module->pdat.zhaoMing.startMinute;
        out<<module->pdat.zhaoMing.endHour;
        out<<module->pdat.zhaoMing.endMinute;

        out<<module->pdat.ledText[0];
        out<<module->pdat.ledText[1];
        out<<module->pdat.ledText[2];

        out<<module->pdat.soundName[0];
        out<<module->pdat.soundName[1];
        out<<module->pdat.soundName[2];
        out<<module->pdat.soundName[3];
        out<<module->pdat.soundName[4];
        out<<module->pdat.soundName[5];
        out<<module->pdat.soundName[6];
        out<<module->pdat.soundName[7];
        out<<module->pdat.soundName[8];

        out<<module->pdat.soundVolume;
        //module->pdat.soundMd5[0].resize(md5size);
        out<<module->pdat.soundMd5[0];

        quint64 datalength;
        QFile *localFile0 = new QFile(sFilePath+module->pdat.voiceFileName[0]);
        if(!localFile0->open(QFile::ReadOnly))
        {
            qDebug() << "open file error!";
            datalength = 0;
            out << datalength;
        }
        else
        {
            datalength = localFile0->size();
            //文件长度
            out << datalength;
            filedata = localFile0->readAll();
            //文件内容
            out<<filedata;
        }
        QLOG_INFO()<<"文件长度0"<<datalength;
        //QLOG_INFO()<<"文件内容"<<filedata.toHex();
        //module->pdat.soundMd5[2].resize(md5size);
        out<<module->pdat.soundMd5[2];
        QFile *localFile2 = new QFile(sFilePath+module->pdat.voiceFileName[2]);
        if(!localFile2->open(QFile::ReadOnly))
        {
            qDebug() << "open file error!";
            datalength = 0;
            out << datalength;
        }
        else
        {
            datalength = localFile2->size();
            //文件长度
            out << datalength;
            //文件内容
            out<<localFile2->readAll();
        }
        //
        //module->pdat.soundMd5[3].resize(md5size);
        out<<module->pdat.soundMd5[3];
        QFile *localFile3 = new QFile(sFilePath+module->pdat.voiceFileName[3]);
        if(!localFile3->open(QFile::ReadOnly))
        {
            qDebug() << "open file error!";
            datalength = 0;
            out << datalength;
        }
        else
        {
            datalength = localFile3->size();
            //文件长度
            out << datalength;
            //文件内容
            out<<localFile3->readAll();
        }
        //
        //module->pdat.soundMd5[4].resize(md5size);
        out<<module->pdat.soundMd5[4];
        QFile *localFile4 = new QFile(sFilePath+module->pdat.voiceFileName[4]);
        if(!localFile4->open(QFile::ReadOnly))
        {
            qDebug() << "open file error!";
            datalength = 0;
            out << datalength;
        }
        else
        {
            datalength = localFile4->size();
            //文件长度
            out << datalength;
            //文件内容
            out<<localFile4->readAll();
        }
        //
        //module->pdat.soundMd5[5].resize(md5size);
        out<<module->pdat.soundMd5[5];
        QFile *localFile5 = new QFile(sFilePath+module->pdat.voiceFileName[5]);
        if(!localFile5->open(QFile::ReadOnly))
        {
            qDebug() << "open file error!";
            datalength = 0;
            out << datalength;
        }
        else
        {
            quint64 datalength = localFile5->size();
            //文件长度
            out << datalength;
            //文件内容
            out<<localFile5->readAll();
        }
        //
        //module->pdat.soundMd5[6].resize(md5size);
        out<<module->pdat.soundMd5[6];
        QFile *localFile6 = new QFile(sFilePath+module->pdat.voiceFileName[6]);
        if(!localFile6->open(QFile::ReadOnly))
        {
            qDebug() << "open file error!";
            datalength = 0;
            out << datalength;
        }
        else
        {
            datalength = localFile6->size();
            //文件长度
            out << datalength;
            //文件内容
            out<<localFile6->readAll();
        }
        //
        //module->pdat.soundMd5[7].resize(md5size);
        out<<module->pdat.soundMd5[7];
        QFile *localFile7 = new QFile(sFilePath+module->pdat.voiceFileName[7]);
        if(!localFile7->open(QFile::ReadOnly))
        {
            qDebug() << "open file error!";
            datalength = 0;
            out << datalength;
        }
        else
        {
            datalength = localFile7->size();
            //文件长度
            out << datalength;
            //文件内容
            out<<localFile7->readAll();
        }
        //
        //module->pdat.soundMd5[1].resize(md5size);
        out<<module->pdat.soundMd5[1];
        QFile *localFile1 = new QFile(sFilePath+module->pdat.voiceFileName[1]);
        if(!localFile1->open(QFile::ReadOnly))
        {
            qDebug() << "open file error!";
            datalength = 0;
            out << datalength;
        }
        else
        {
            datalength = localFile1->size();
            //文件长度
            out << datalength;
            //文件内容
            out<<localFile1->readAll();
        }
        //
        //module->pdat.soundMd5[8].resize(md5size);
        out<<module->pdat.soundMd5[8];
        QFile *localFile8 = new QFile(sFilePath+module->pdat.voiceFileName[8]);
        if(!localFile8->open(QFile::ReadOnly))
        {
            qDebug() << "open file error!";
            datalength = 0;
            out << datalength;
        }
        else
        {
            datalength = localFile8->size();
            //文件长度
            out << datalength;
            //文件内容
            out<<localFile8->readAll();
        }

        delete localFile0;
        delete localFile1;
        delete localFile2;
        delete localFile3;
        delete localFile4;
        delete localFile5;
        delete localFile6;
        delete localFile7;
        delete localFile8;
        //
        file.flush();
        file.close();

    }
    else
    {
        //点的是取消
    }
}
