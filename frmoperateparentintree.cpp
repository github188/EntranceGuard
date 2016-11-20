#include "frmoperateparentintree.h"
#include "ui_frmoperateparentintree.h"

frmOperateParentInTree::frmOperateParentInTree(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmOperateParentInTree)
{
    ui->setupUi(this);
    changed = false;
    tableModel=NULL;
    InitStyle();
    ui->btnEdit->setVisible(false);
    connect(ui->btnDelete,SIGNAL(clicked(bool)),this,SLOT(btnDelete()));
    connect(ui->btnAddNew,SIGNAL(clicked(bool)),this,SLOT(btnAddNewParentInTree()));
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(btnCancel()));
    connect(ui->btnSave,SIGNAL(clicked(bool)),this,SLOT(btnSave()));
    connect(ui->btnEdit,SIGNAL(clicked(bool)),this,SLOT(btnEdit()));
}

frmOperateParentInTree::~frmOperateParentInTree()
{
    delete ui;
}
void frmOperateParentInTree::InitStyle()
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

}

bool frmOperateParentInTree::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void frmOperateParentInTree::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmOperateParentInTree::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void frmOperateParentInTree::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void frmOperateParentInTree::on_btnMenu_Close_clicked()
{
    tableModel->revert();
    this->close();
}

void frmOperateParentInTree::on_btnMenu_Max_clicked()
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

void frmOperateParentInTree::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}
void frmOperateParentInTree::btnAddNewParentInTree()
{
    int rowNum = tableModel->rowCount();
    tableModel->insertRow(rowNum);
    tableModel->setData(tableModel->index(rowNum, 0),"测试开发区域");
    tableModel->setData(tableModel->index(rowNum, 1), rowNum);

    ui->tableView->setCurrentIndex(tableModel->index(tableModel->rowCount() - 1, 0));
}
void frmOperateParentInTree::btnCancel()
{
    tableModel->revert();
    this->close();
}
void frmOperateParentInTree::btnDelete()
{
    int curRow = ui->tableView->currentIndex().row();
    tableModel->removeRow(curRow);
    int ok = QMessageBox::warning(this,tr("删除当前行!"),tr("你确定"
                                                              "删除当前行吗？"),
                            QMessageBox::Yes,QMessageBox::No);
       if(ok == QMessageBox::No)
       {
          tableModel->revertAll(); //如果不删除，则撤销
       }
       else
       {
           tableModel->submitAll(); //否则提交，在数据库中删除该行
       }

}
void frmOperateParentInTree::btnSave()
{
    tableModel->submitAll();
    emit sigParentInTreeChanged(1);
    this->close();
}
void frmOperateParentInTree::btnEdit()
{

}
void frmOperateParentInTree::setTreeViewModel(QSqlTableModel *model)
{
    if(tableModel==NULL)
    {
        tableModel = model;
        ui->tableView->setModel(tableModel);
        ui->tableView->hideColumn(1);
    }
}
