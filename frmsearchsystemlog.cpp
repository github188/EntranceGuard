#include "frmsearchsystemlog.h"
#include "ui_frmsearchsystemlog.h"
#include "../odbcexcel.h"
#include <QFileDialog>
frmSearchSystemLog::frmSearchSystemLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmSearchSystemLog)
{
    ui->setupUi(this);
    InitStyle();
    connect(ui->btnSearch,SIGNAL(clicked(bool)),this,SLOT(btnSearch()));
    connect(ui->btnExportExcel,SIGNAL(clicked(bool)),this,SLOT(btnExportExcel()));
    connect(ui->btnDeleteLog,SIGNAL(clicked(bool)),this,SLOT(btnDeleteLog()));
}

frmSearchSystemLog::~frmSearchSystemLog()
{
    delete ui;
}
void frmSearchSystemLog::InitStyle()
{
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;
    //设置窗体关闭时自动释放内存
    this->setAttribute(Qt::WA_DeleteOnClose);
    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);
    //系统日志显示窗口初始化
    ui->tableWidget->setColumnCount(2);
    //ui->tableWidget->horizontalHeader()->hide();//隐藏表头
    //ui->tableWidget->verticalHeader()->hide();//隐藏行号
    QStringList header;
    header <<"时间"<<"信息";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setClickable(false); //设置表头不可点击（默认点击后进行排序）
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);//关键

    ui->tableWidget->setColumnWidth(0, 150);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    //ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

bool frmSearchSystemLog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void frmSearchSystemLog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmSearchSystemLog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void frmSearchSystemLog::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void frmSearchSystemLog::on_btnMenu_Close_clicked()
{
    this->close();
}

void frmSearchSystemLog::on_btnMenu_Max_clicked()
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

void frmSearchSystemLog::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}
void frmSearchSystemLog::setTitle(const QString &title)
{
    ui->lab_Title->setText(title);
}

//设置时间
void frmSearchSystemLog::setDateTime(QDateTime datetime)
{
    ui->endDateTime->setDateTime(datetime.addMonths(1));
    ui->startDateTime->setDateTime(datetime);
}
void frmSearchSystemLog::setDatebaseQuery(QSqlQuery *tempquery )
{
    query = tempquery;
}
void frmSearchSystemLog::btnDeleteLog()
{
    QString startTime = ui->startDateTime->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString endTime = ui->endDateTime->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    //
    QString sql = "delete from TSystemLog where datetime([DateTime])>datetime('"+startTime+"')  and datetime([DateTime])<datetime('"+endTime+"')";
    if(!query->exec(sql))
    {
        QLOG_ERROR() <<query->lastError();
    }
    else
    {
        clearTablewidgetItem();
    }
}
void frmSearchSystemLog::clearTablewidgetItem()
{
    int iLen = ui->tableWidget->rowCount();
    for(int i=0;i<iLen;i++)
    {
        ui->tableWidget->removeRow(0);
    }
}
void frmSearchSystemLog::btnSearch()
{
    clearTablewidgetItem();
    QString startTime = ui->startDateTime->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString endTime = ui->endDateTime->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    //读取分支树父辈
    QString sql = "select * from TSystemLog where  datetime([DateTime])>datetime('"+startTime+"')  and datetime([DateTime])<datetime('"+endTime+"')";
    if(!query->exec(sql))
    {
        QLOG_ERROR() <<query->lastError();
    }
    else
    {
        while(query->next())
        {
            int rowNum = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rowNum);
            ui->tableWidget->setItem(rowNum,0,new QTableWidgetItem(query->value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
            ui->tableWidget->setItem(rowNum,1,new QTableWidgetItem(query->value(1).toString()));
        }
        ui->tableWidget->resizeColumnToContents(0);
        ui->tableWidget->resizeColumnToContents(1);
    }
}
void frmSearchSystemLog::btnExportExcel()
{
    QString filePath = QFileDialog::getSaveFileName(this,
            tr("保存文件"),
            "",
            tr("Excel Files (*.xls)"));

    if (!filePath.isNull())
    {
        //qDebug()<<filePath;
        if(OdbcExcel::saveFromTable(filePath,ui->tableWidget,"注释：无"))
        {
            QMessageBox::information(this,tr("提示"),tr("保存成功"));
        }
        else
        {
            QString msg="保存失败！\n\r"+OdbcExcel::getError();
            QMessageBox::critical(this,tr("错误"),tr(msg.toUtf8()));
        }
    }
    else
    {
        //点的是取消
    }

}
