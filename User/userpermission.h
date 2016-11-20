#ifndef USERPERMISSION_H
#define USERPERMISSION_H
#include <qglobal.h>

class UserPermission
{
public:
    UserPermission();
    quint8 level;//用户权限等级
    //系统权限
    bool permitEditSystemPar;       //修改系统参数 0-禁止 1-允许
    bool permitEditAreaInfo;        //修改区域信息 0-禁止 1-允许
    bool permitRegFangHuCang;       //注册防护舱 0-禁止 1-允许
    bool permitLogoutFangHuCang;    //注销防护舱 0-禁止 1-允许
    bool permitEditSoundLiabrary;   //编辑语音库 0-禁止 1-允许
    bool permitEditModule;          //编辑参数模板库 0-禁止 1-允许
    //设备权限
    bool permitEditFangHuCangPar;   //编辑防护舱工作参数 0-禁止 1-允许
    bool permitEditZhaoMingPar;     //编辑防护舱照明参数 0-禁止 1-允许
    bool permitEditVoicePar;        //编辑防护舱语音设置 0-禁止 1-允许
    bool permitAdjustTime;          //校准时间 0-禁止 1-允许
    bool permitOpenDoor;            //远程开门 0-禁止 1-允许
    bool permitUseorNotusedFangHuCang;//启用/停用 防护舱 0-禁止 1-允许
    bool permitLockorOpenFangHuCang;//闭锁/解锁 防护舱 0-禁止 1-允许
    bool permitEditLedText;         //编辑LED节目内容

    //加钞间新加权限
    bool permitChangeAdmin;         //更换管理员
};

#endif // USERPERMISSION_H
