#include "treeviewitem.h"
#include "../DATA/paraalarm.h"
#include <QMovie>
TreeViewItem::TreeViewItem()
{
    parentIndexInTree=0;
    index=0;
    mac="";
    text="";
    tooltip="";
    slave=NULL;
    _movie = new QMovie(":/联网状态图标/image/联网状态图标/连接中.gif");
    connect(_movie, SIGNAL(frameChanged(int)), this, SLOT(dataChanged()));
}
TreeViewItem::~TreeViewItem()
{
    delete _movie;
}
void TreeViewItem::setSlave(Equipment *equipment)
{
    slave = equipment;
    if(slave!=NULL)
    {
        connect(slave,SIGNAL(sigStatusChanged()),this,SLOT(dataChanged()));
        text = slave->GetEquName();
        mac = slave->GetEquMac();
    }

}
QIcon TreeViewItem::getListViewQicon(void)
{

     if(slave)
     {
         //设备的连接状态 0断开 1连接 2正在连接
         switch (slave->equParaData.slaveStatus.connectStatus)
         {
         case 0:
             icon1=QIcon(":/设备状态图标/image/设备状态图标/门-未知.png");
             break;
         case 1:
             //如果在线则显示门状态
             switch(slave->equParaData.slaveStatus.gateStatus)
             {
                 case 0:
                     icon1=QIcon(":/使用状态图标/image/使用状态图标/门-关闭.png");
                 break;
                 case 1:
                     icon1=QIcon(":/使用状态图标/image/使用状态图标/门-打开.png");
                 break;
             }
             break;
         case 2:
             icon1=QIcon(":/设备状态图标/image/设备状态图标/门-未知.png");
             break;
         default:
             break;
         }
     }
     else
     {
         icon1=QIcon(":/image/tree1.png");
     }
     return icon1;
}
QIcon TreeViewItem::getQicon(void)
{
    if(slave)
    {
        equStatus s= slave->equParaData.slaveStatus;
        paraAlarm pa=slave->equParaData.alarmPara;
        //设备的连接状态 0断开 1连接 2正在连接
        switch (s.connectStatus)
        {
        case 0:
            _movie->stop();
            icon=QIcon(":/image/slave_offline.ico");
            break;
        case 1:
            _movie->stop();

            if((s.boLiAlarmStatus && pa.boLiAlarmEnable) |
                    (s.buttonAlarmStatus && pa.btnAlarmEnable)|
                    (s.cutAlarmStatus && pa.cutAlarmEnable)|
                    (s.shuiQinAlarmStatus && pa.shuiQinAlarmEnable)|
                    (s.tempAlarmStatus && pa.tempAlarmEnable)|
                    (s.yanWuAlarmStatus && pa.yanWuAlarmEnable)|
                    (s.zhengDongAlarmStatus && pa.zhengDongAlarmEnable)|
                    (s.doorCiAlarmStatus && pa.doorCiAlarmEnable)|
                    (s.existManAlarmStatus && pa.existManAlarmEnable))
            {
                icon=QIcon(":/联网状态图标/image/联网状态图标/连接状态-报警.png");
            }
            else if(slave->equParaData.slaveStatus.lockStatus)
            {
                icon=QIcon(":/联网状态图标/image/联网状态图标/连接状态-锁闭.png");
            }
            else if(slave->equParaData.slaveStatus.fanghucangStatus)
            {
                icon=QIcon(":/联网状态图标/image/联网状态图标/联网状态-停用.png");
            }
            else
            {
                icon=QIcon(":/image/slave_online.ico");
            }

            break;
        case 2:
            _movie->start();
            currFrame = _movie->currentPixmap();
            icon=QIcon(_movie->currentPixmap());
            break;
        default:
            break;
        }
    }
    else
    {
        _movie->stop();
        icon=QIcon(":/image/tree1.png");
    }
    return icon;
}
void TreeViewItem::dataChanged()
{
    emit sigDataChanged(this->index,this->parentIndexInTree);
}
Equipment* TreeViewItem::GetSlave(void)
{
    return this->slave;
}

