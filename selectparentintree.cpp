#include "selectparentintree.h"
#include "ui_selectparentintree.h"

SelectParentInTree::SelectParentInTree(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectParentInTree)
{
    ui->setupUi(this);
    qName="";
    tableModel=NULL;
    InitStyle();
    connect(ui->btnNew,SIGNAL(clicked(bool)),this,SLOT(btnAddParentInTree()));
    connect(ui->btnSelect,SIGNAL(clicked(bool)),this,SLOT(btnSelect()));
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(btnCancel()));
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(getSelectItem(QModelIndex)));
}

SelectParentInTree::~SelectParentInTree()
{
    delete ui;
}
void SelectParentInTree::InitStyle()
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

bool SelectParentInTree::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void SelectParentInTree::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void SelectParentInTree::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void SelectParentInTree::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void SelectParentInTree::on_btnMenu_Close_clicked()
{
    this->close();
}

void SelectParentInTree::on_btnMenu_Max_clicked()
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

void SelectParentInTree::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}
void SelectParentInTree::btnAddParentInTree()
{
    emit sigAddNewParentInTree();
}
void SelectParentInTree::btnCancel()
{
    this->close();
}
void SelectParentInTree::btnSelect()
{
    emit sigSelected(qName);
    done(QDialog::Accepted);
    this->close();
}
void SelectParentInTree::getSelectItem(QModelIndex index)
{
    qName =  tableModel->data(index).toString();
}
void SelectParentInTree::setTreeViewModel(QSqlTableModel *model)
{
    qName="";
    //if(tableModel==NULL)
    //{
        tableModel = model;
        ui->tableView->setModel(tableModel);
        ui->tableView->hideColumn(1);

        qName =  tableModel->data(tableModel->index(0,0)).toString();
        ui->tableView->setCurrentIndex(tableModel->index(0,0));
    /*}
    else
    {
        ui->tableView->clearSelection();
        qName =  tableModel->data(tableModel->index(0,0)).toString();
        ui->tableView->setCurrentIndex(tableModel->index(0,0));
    }*/
}
