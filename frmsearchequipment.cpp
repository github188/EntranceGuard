#include "frmsearchequipment.h"
#include "ui_frmsearchequipment.h"

frmSearchEquipment::frmSearchEquipment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmSearchEquipment)
{
    ui->setupUi(this);
    InitStyle();
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(pushButton_clicked()));
    connect(ui->btnRegister,SIGNAL(clicked(bool)),this,SLOT(RegisterSlaves()));
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(btnCancel()));
    connect(ui->btnSelectAll,SIGNAL(clicked(bool)),this,SLOT(btnSelectAll()));
    connect(ui->btnSelectChange,SIGNAL(clicked(bool)),this,SLOT(btnSelectChange()));
    connect(ui->btnSelectNull,SIGNAL(clicked(bool)),this,SLOT(btnSelectNull()));
    //connect(this,SIGNAL(sigShowSlaveList(QList<paraData*>*)),this,SLOT(showSlaveList(QList<paraData*>*)));
}

frmSearchEquipment::~frmSearchEquipment()
{
    delete ui;
}
void frmSearchEquipment::InitStyle()
{
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;
    //设置窗体关闭时自动释放内存
    //this->setAttribute(Qt::WA_DeleteOnClose);
    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);

    ui->tableWidget->setColumnCount(7);
    //ui->tableWidget->setRowCount(5);
    QStringList header;
    header<<"选择"<<"名称"<<"设备型号"<<"版本"<<"序列号"<<"IP地址"<<"联网模式";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->resizeColumnsToContents();
    //ui->tableWidget->horizontalHeader()->setStretchLastSection(true);//自
}

bool frmSearchEquipment::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void frmSearchEquipment::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmSearchEquipment::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void frmSearchEquipment::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void frmSearchEquipment::on_btnMenu_Close_clicked()
{
    this->close();
}

void frmSearchEquipment::on_btnMenu_Max_clicked()
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

void frmSearchEquipment::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void frmSearchEquipment::pushButton_clicked()
{
    emit SearchEqument();
}

void frmSearchEquipment::DisplayResult(QList<TreeViewItem*> *registeredList, QList<paraData*> *searchedAll)
{
    searchedList = searchedAll;

    if(ui->checkBox->checkState() == Qt::Checked)
    {
        for(int i=0;i<searchedList->count();i++)
        {
            for(int j=0;j<registeredList->count();j++)
            {
                if(registeredList->at(j)->mac == searchedAll->at(i)->net.macAddr)
                {
                    searchedAll->removeAt(i);
                    i--;
                    break;
                }
            }
        }
    }

    //emit sigShowSlaveList(searchedList);
    showSlaveList(searchedList);

}
void frmSearchEquipment::showSlaveList(QList<paraData*> *searchedList)
{
    //删除所有行
    int count = ui->tableWidget->rowCount();
    for(int i=0;i<count;i++)
    {
        ui->tableWidget->removeRow(0);
    }
    //重新设置行数
    ui->tableWidget->setRowCount(searchedList->count());

    for(int i=0;i<searchedList->count();i++)
    {
        paraData* pdata = searchedList->at(i);
        //插入复选框并居中显示
        QCheckBox *ckb = new QCheckBox(ui->tableWidget);
        QHBoxLayout *hLayout = new QHBoxLayout(ui->tableWidget);
        QWidget *widget = new QWidget(ui->tableWidget);
        ckb->setChecked(true);//默认为选中状态
        hLayout->addWidget(ckb);
        hLayout->setMargin(0);                          // 必须添加, 否则CheckBox不能正常显示
        hLayout->setAlignment(ckb, Qt::AlignCenter);
        widget->setLayout(hLayout);

        ui->tableWidget->setCellWidget(i,0,widget);
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(pdata->name));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(pdata->getSlaveType()));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(pdata->version));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(pdata->net.macAddr));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(pdata->net.ip));
        ui->tableWidget->setItem(i,6,new QTableWidgetItem(pdata->net.networkModel?"客户端模式":"服务器模式"));
    }

    ui->tableWidget->resizeColumnsToContents();
}
void frmSearchEquipment::RegisterSlaves()
{

    QList<paraData*> resultList;
    resultList.clear();

    int rowcount = ui->tableWidget->rowCount();
    for(int i=0;i<rowcount;i++)
    {
        //获取复选框指针
        QWidget *widget = (QWidget *)ui->tableWidget->cellWidget(i,0);
        QCheckBox *ckb = (QCheckBox *)widget->children().at(1);
        if(ckb->isChecked())
        {
            resultList.append(searchedList->at(i));
        }

    }

    emit registerSlaves(resultList);
    this->close();
}
void frmSearchEquipment::btnCancel()
{
    this->close();
}
void frmSearchEquipment::btnSelectAll()
{
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        //获取复选框指针
        QWidget *widget = (QWidget *)ui->tableWidget->cellWidget(i,0);
        QCheckBox *ckb = (QCheckBox *)widget->children().at(1);
        if(!ckb->isChecked())
        {
            ckb->setChecked(true);
        }
    }
}
void frmSearchEquipment::btnSelectNull()
{
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        //获取复选框指针
        QWidget *widget = (QWidget *)ui->tableWidget->cellWidget(i,0);
        QCheckBox *ckb = (QCheckBox *)widget->children().at(1);
        if(ckb->isChecked())
        {
            ckb->setChecked(false);
        }
    }
}
void frmSearchEquipment::btnSelectChange()
{

    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        //获取复选框指针
        QWidget *widget = (QWidget *)ui->tableWidget->cellWidget(i,0);
        QCheckBox *ckb = (QCheckBox *)widget->children().at(1);
        if(ckb->isChecked())
        {
            ckb->setChecked(false);
        }
        else
        {
            ckb->setChecked(true);
        }
    }
}
