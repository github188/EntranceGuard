#include "paradata.h"

paraData::paraData()
{
    name = "";
    slaveType = 0;//设备类型 0－智能门禁 1－加钞间 ff-bootloader
    version = "";
    iMsgCount = 0;
    iPreMsgCount = 0;
    iGetMsgCount=0;
    slaveDate.setDate(1,1,1);
    slaveTime.setHMS(0,0,0);
    dataChanged=0;//数据发生变化时为1
    flag=0;
    readSoundDone=0;
    readSoundFlag=0;
    soundVolume=0;
    readSoundSize=0;
    soundSize=0;
    twoMainVer=0;
    keyList.clear();
}

paraData::~paraData()
{

}
quint8 paraData::getTwoMainVer()
{
    twoMainVer = this->mainVer*10+this->secondVer;
    return twoMainVer;
}
QString paraData::getSlaveType()
{
    QString retValue;
    switch (slaveType)
    {
    case 0://防护舱
        retValue = "FHC";
        break;
    case 1://加钞间
        retValue = "JCJ";
        break;
    case 0xff:
        retValue = "BootLoader";
        break;
    default:
        break;
    }
    return retValue;
}
void paraData::setFlag()
{
    flag=1;
}
void paraData::resetFlag()
{
    flag=0;
}
quint8 paraData::getFlag()
{
    quint8 retValue;
    retValue = flag;
    return retValue;
}
void paraData::iGetMsgCountAdd(quint32 msgcount)
{
    iGetMsgCount+=msgcount;
}
quint32 paraData::iGetMsgCountCurrentValue()
{
    quint32 retValue;
    retValue = iGetMsgCount;
    return retValue;
}
void paraData::setMsgCount(quint32 msgcount)
{
    iMsgCount = msgcount;
}
quint32 paraData::GetMsgCount()
{
    quint32 retValue;
    retValue = iMsgCount;
    return retValue;
}
void paraData::cleariGetMsgCount()
{
    iGetMsgCount=0;
}
void paraData::setSoundFlag()
{
    readSoundFlag=1;
}
void paraData::resetSoundFlag()
{
    readSoundFlag=0;
}
quint8 paraData::getSoundFlag()
{
    quint8 retValue;
    retValue = readSoundFlag;
    return retValue;
}
QString paraData::getSoundMd5(quint8 id)
{
    return soundMd5[id].toHex();
}
