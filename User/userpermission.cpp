#include "userpermission.h"

UserPermission::UserPermission()
{
    level = 2;//用户权限等级
    //系统权限
    permitEditSystemPar = false;       //修改系统参数 0-禁止 1-允许
    permitEditAreaInfo = false;        //修改区域信息 0-禁止 1-允许
    permitRegFangHuCang = false;       //注册防护舱 0-禁止 1-允许
    permitLogoutFangHuCang = false;    //注销防护舱 0-禁止 1-允许
    permitEditSoundLiabrary = false;   //编辑语音库 0-禁止 1-允许
    permitEditModule = false;          //编辑参数模板库 0-禁止 1-允许
    //设备权限
    permitEditFangHuCangPar = false;   //编辑防护舱工作参数 0-禁止 1-允许
    permitEditZhaoMingPar = false;     //编辑防护舱照明参数 0-禁止 1-允许
    permitEditVoicePar = false;        //编辑防护舱语音设置 0-禁止 1-允许
    permitAdjustTime = false;          //校准时间 0-禁止 1-允许
    permitOpenDoor = false;            //远程开门 0-禁止 1-允许
    permitUseorNotusedFangHuCang = false;//启用/停用 防护舱 0-禁止 1-允许
    permitLockorOpenFangHuCang = false;//闭锁/解锁 防护舱 0-禁止 1-允许
    permitEditLedText = false;         //编辑LED节目内容

    permitChangeAdmin=false;//更换管理员
}

