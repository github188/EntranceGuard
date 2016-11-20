#ifndef USERINFO_H
#define USERINFO_H

#include <QObject>

class UserInfo
{
public:
    UserInfo();
    QString userName;   //用户名
    QString userPwd;    //密码的md5校验
    quint8 level;       //用户等级 0级最高 1－管理员 2－操作员
                        //操作员只能操作不能修改参数 管理员可以修改参数
};

#endif // USERINFO_H
