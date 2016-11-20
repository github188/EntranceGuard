#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <qglobal.h>
#include <QApplication>
#include <QByteArray>
#include <QTextCodec>
#include <QDebug>
#include "QsLog/QsLog.h"
#include "QsLog/QsLogDest.h"
#include "QsLog/QsLogLevel.h"
#include <QDateTime>
#include "../DATA/paradata.h"


#define funcTcpCountNum 65  //TCP功能函数个数
#define funcUdpCountNum 1   //UDP功能函数个数
#define KEY (100)

typedef enum
{
    func_tcp_null=0,
    func_tcp_read_slave_para =1,
    func_tcp_read_slave_time =3,
    func_tcp_set_slave_time =4,
    func_tcp_read_slave_status =5,
    func_tcp_set_lineAndAdLine_para =6,
    func_tcp_read_lamp_Para =7,
    func_tcp_set_work_para =8,
    func_tcp_read_work_para =9,
    func_tcp_EnableOrDisable_slave =0x0a,
    func_tcp_openDoor =0x42,
    func_tcp_closeLock =0x44,
    func_tcp_read_msgCount =0x45,
    func_tcp_read_msg =0x46,
    func_tcp_read_led = 0x48,
    func_tcp_set_led = 0x47,
    func_tcp_read_alarmPara = 0x49,//2.8版本读取报警参数
    func_tcp_set_alarmPara = 0x4a,//2.8版本设置报警参数
    func_tcp_read_alarmStatus = 0x4b,//2.8版本读取报警状态
    func_tcp_clear_alarm = 0x4c,//清除报警状态
    /**加钞间新增钥匙协议**/
    func_tcp_set_status = 0x4D,//设置状态
    func_tcp_delete_key = 0x4E,//删除钥匙
    func_tcp_read_key = 0x4F,//读钥匙
    func_tcp_write_mainKey = 0xCE,//写入主钥匙
    /**加钞间新增钥匙协议结束**/
    func_tcp_sd_slave_fangQieWarning =90,//主动上传防切割报警
    func_tcp_sd_slave_buttonWarning =91,//主动上传按钮报警
    func_tcp_sd_slave_temperature =99,//主动上传温度数据
    func_tcp_sd_slave_status =100,//主动上传设备状态
    func_tcp_sd_slave_alarmStatus = 92,//2.8版本报警主动上传
    /****加钞间主动上传****/
    func_tcp_sd_inkey_value =101,//在有人值守状态下，当读到进出门钥匙数据时,加钞间主动上传
    func_tcp_sd_addkey_value =102,//加钞间主动上传设备状态
    /****加钞间主动上传结束****/
    func_tcp_set_hy_volume =0x0f,//设置欢迎语音音量
    func_tcp_set_syz_volume =0x15,//设置使用中语音音量
    func_tcp_set_whz_volume =0x1b,//设置维护中语音音量
    func_tcp_set_timeoutWarn_volume =0x21,//设置超时提醒语音音量
    func_tcp_set_timeoutTiShi_volume =0x27,//设置超时提示语音音量
    func_tcp_set_outDoor_volume =0x2d,//设置出门语音音量大小
    func_tcp_set_openDoor_volume =0x33,//设置门开提示语音音量大小
    func_tcp_set_failLock_volume =0x39,//设置门未锁好语音音量大小
    func_tcp_set_laDoor_volume =0x3f,//设置拉门语音音量大小
    func_tcp_play_sound = 0x41,
    func_tcp_read_hy_volume =0x10,//读取欢迎语音音量
    func_tcp_read_hy_md5 =0x0e,//读取欢迎语音音量
    func_tcp_read_syz_md5 =0x14,//读取欢迎语音音量
    func_tcp_read_whz_md5 =0x1a,//读取欢迎语音音量
    func_tcp_read_timeoutWarn_md5 =0x20,//读取欢迎语音音量
    func_tcp_read_timeoutTiShi_md5 =0x26,//读取欢迎语音音量
    func_tcp_read_outDoor_md5 =0x2c,//读取欢迎语音音量
    func_tcp_read_openDoor_md5 =0x32,//读取欢迎语音音量
    func_tcp_read_failLock_md5 =0x38,//读取欢迎语音音量
    func_tcp_read_laDoor_md5 =0x3e,//读取欢迎语音音量

    func_tcp_set_hy_md5 =0x0d,//读取欢迎语音Md5
    func_tcp_set_syz_md5 =0x13,//读取欢迎语音Md5
    func_tcp_set_whz_md5 =0x19,//读取欢迎语音Md5
    func_tcp_set_timeoutWarn_md5 =0x1f,//读取欢迎语音Md5
    func_tcp_set_timeoutTiShi_md5 =0x25,//读取欢迎语音Md5
    func_tcp_set_outDoor_md5 =0x2b,//读取欢迎语音Md5
    func_tcp_set_openDoor_md5 =0x31,//读取欢迎语音Md5
    func_tcp_set_failLock_md5 =0x37,//读取欢迎语音Md5
    func_tcp_set_laDoor_md5 =0x3d,//读取欢迎语音Md5

    func_tcp_read_hy_sound =0x0c,
    func_tcp_read_syz_sound =0x12,//读取欢迎语音
    func_tcp_read_whz_sound =0x18,//读取欢迎语音
    func_tcp_read_timeoutWarn_sound =0x1e,//读取欢迎语音
    func_tcp_read_timeoutTiShi_sound =0x24,//读取欢迎语音
    func_tcp_read_outDoor_sound =0x2a,//读取欢迎语音
    func_tcp_read_openDoor_sound =0x30,//读取欢迎语音
    func_tcp_read_failLock_sound =0x36,//读取欢迎语音
    func_tcp_read_laDoor_sound =0x3c,//读取欢迎语音

    func_tcp_set_hy_sound =0x0b,//写入欢迎语音
    func_tcp_set_syz_sound =0x11,//写入欢迎语音
    func_tcp_set_whz_sound =0x17,//写入欢迎语音
    func_tcp_set_timeoutWarn_sound =0x1d,//写入欢迎语音
    func_tcp_set_timeoutTiShi_sound =0x23,//写入欢迎语音
    func_tcp_set_outDoor_sound =0x29,//写入欢迎语音
    func_tcp_set_openDoor_sound =0x2f,
    func_tcp_set_failLock_sound =0x35,
    func_tcp_set_laDoor_sound =0x3b

}TcpFuncCode;


/*
 * 数据解密
 *
 * @buf 需要解密的数据
 * @startid 开始字节
 * @length 需要解密的长度
 *
 * */
static void getRealValue(char *buf,quint8 startid,quint8 length)
{
    for(int i=startid;i<startid+length;i++)
    {
        buf[i]=buf[i]^0x59;
    }
}
/*
 * CRC校验
 *
 * @vals 需要校验的数据
 * @num  需要校验数据的长度
 *
 * */
static uchar getCRC(uchar *vals, unsigned int num)
{
    uchar cnt, j;
    uchar crc;
    uchar * val;

    val = vals;
    crc = 0;
    for(cnt=0; cnt<num; cnt++)
    {
        crc = crc ^ val[cnt];
        for(j=0; j<8; j++)
        {
            if(crc & 0x01)
            {
                crc >>= 1;
                crc ^= 0x8c;
            }
            else
                crc >>= 1;
        }
    }
    return crc;
}
/******************************************************
*函数名称:CheckSum32
*输   入:buf 要校验的数据
        len 校验数据的长
*输   出:校验和
*功   能:校验和-32
*******************************************************/
static quint32 CheckSum32(uchar *buf,quint32 len)
{
    quint32    i=0;
    quint32 Sum=0;

    for (i=0;i<len;i++)
    {
        Sum+=*buf++;
    }
    return Sum;
}
//TCP功能函数处理
typedef quint8 (*pfunc_tcp) (char *buf,paraData *pdat);

//UDP功能函数处理
typedef quint8 (*pfunc_udp) (QByteArray buf,paraData *pdat);

//TCP功能码和处理函数结构体
typedef struct
{
    TcpFuncCode     ucFunctionCode;//功能码
    pfunc_tcp pxHandler;//TCP处理函数指针
} tcpFunctionHandler;

//UDP功能码和处理函数结构体
typedef struct
{
    uchar     ucFunctionCode;//功能码
    pfunc_udp pxHandler;//UDP处理函数指针
} udpFunctionHandler;

//处理返回数据包功能函数
quint8 Func_tcp_1(char *buf,paraData *pdat);
quint8 FunctTcp1(char *buf,paraData *pdat);
quint8 Func_tcp_2(char *buf,paraData *pdat);
quint8 Func_tcp_3(char *buf,paraData *pdat);
quint8 Func_tcp_4(char *buf,paraData *pdat);
quint8 Func_tcp_5(char *buf,paraData *pdat);
quint8 Func_tcp_6(char *buf,paraData *pdat);
quint8 Func_tcp_7(char *buf,paraData *pdat);
quint8 Func_tcp_8(char *buf,paraData *pdat);
quint8 Func_tcp_9(char *buf,paraData *pdat);
quint8 Func_tcp_10(char *buf,paraData *pdat);
quint8 Func_tcp_15(char *buf,paraData *pdat);
quint8 Func_tcp_16(char *buf,paraData *pdat);
quint8 Func_tcp_65(char *buf,paraData *pdat);
quint8 Func_tcp_66(char *buf,paraData *pdat);
quint8 Func_tcp_68(char *buf,paraData *pdat);
quint8 Func_tcp_69(char *buf,paraData *pdat);
quint8 Func_tcp_71(char *buf,paraData *pdat);
quint8 Func_tcp_70(char *buf,paraData *pdat);
quint8 Func_tcp_72(char *buf,paraData *pdat);
quint8 Func_tcp_73(char *buf,paraData *pdat);
quint8 Func_tcp_74(char *buf,paraData *pdat);
quint8 Func_tcp_75(char *buf,paraData *pdat);
quint8 Func_tcp_76(char *buf,paraData *pdat);
quint8 Func_tcp_77(char *buf,paraData *pdat);
quint8 Func_tcp_78(char *buf,paraData *pdat);
quint8 Func_tcp_79(char *buf,paraData *pdat);
quint8 Func_tcp_90(char *buf,paraData *pdat);
quint8 Func_tcp_91(char *buf,paraData *pdat);
quint8 Func_tcp_92(char *buf,paraData *pdat);
quint8 Func_tcp_99(char *buf,paraData *pdat);
quint8 Func_tcp_100(char *buf,paraData *pdat);
quint8 Func_tcp_101(char *buf,paraData *pdat);
quint8 Func_tcp_102(char *buf,paraData *pdat);
quint8 Func_tcp_read_sound(char *buf, paraData *pdat);
quint8 Func_tcp_set_sound(char *buf, paraData *pdat);
quint8 Func_tcp_read_sound_md5(char *buf, paraData *pdat);
quint8 Func_tcp_set_sound_md5(char *buf, paraData *pdat);
quint8 Func_tcp_read_sound_volume(char *buf, paraData *pdat);
quint8 Func_tcp_set_sound_volume(char *buf, paraData *pdat);
//TCP数据包解析函数数指针数组
static tcpFunctionHandler funcTcpArray[]={
    {func_tcp_read_slave_para,Func_tcp_1},
    //{0x02,Func_tcp_2},
    {func_tcp_read_slave_time,Func_tcp_3},
    {func_tcp_set_slave_time,Func_tcp_4},
    {func_tcp_read_slave_status,Func_tcp_5},
    {func_tcp_set_lineAndAdLine_para,Func_tcp_6},
    {func_tcp_read_lamp_Para,Func_tcp_7},
    {func_tcp_set_work_para,Func_tcp_8},
    {func_tcp_read_work_para,Func_tcp_9},
    {func_tcp_EnableOrDisable_slave,Func_tcp_10},
    {func_tcp_read_msgCount,Func_tcp_69},
    {func_tcp_read_msg,Func_tcp_70},
    {func_tcp_openDoor,Func_tcp_66},
    {func_tcp_closeLock,Func_tcp_68},
    {func_tcp_set_led,Func_tcp_71},
    {func_tcp_read_led,Func_tcp_72},
    {func_tcp_read_alarmPara,Func_tcp_73},
    {func_tcp_set_alarmPara,Func_tcp_74},
    {func_tcp_read_alarmStatus,Func_tcp_75},
    {func_tcp_clear_alarm,Func_tcp_76},
    {func_tcp_set_status,Func_tcp_77},
    {func_tcp_delete_key,Func_tcp_78},
    {func_tcp_read_key,Func_tcp_79},
    {func_tcp_sd_slave_fangQieWarning,Func_tcp_90},
    {func_tcp_sd_slave_buttonWarning,Func_tcp_91},
    {func_tcp_sd_slave_alarmStatus,Func_tcp_92},
    {func_tcp_sd_slave_temperature,Func_tcp_99},
    {func_tcp_sd_slave_status,Func_tcp_100},
    {func_tcp_sd_inkey_value,Func_tcp_101},
    {func_tcp_sd_addkey_value,Func_tcp_102},
    {func_tcp_set_hy_volume,Func_tcp_set_sound_volume},
    {func_tcp_set_syz_volume,Func_tcp_set_sound_volume},               //设置使用中语音音量
    {func_tcp_set_whz_volume,Func_tcp_set_sound_volume},               //设置维护中语音音量
    {func_tcp_set_timeoutWarn_volume,Func_tcp_set_sound_volume},       //设置超时提醒语音音量
    {func_tcp_set_timeoutTiShi_volume,Func_tcp_set_sound_volume},      //设置超时提示语音音量
    {func_tcp_set_outDoor_volume,Func_tcp_set_sound_volume},           //设置出门语音音量大小
    {func_tcp_set_openDoor_volume,Func_tcp_set_sound_volume},          //设置门开提示语音音量大小
    {func_tcp_set_failLock_volume,Func_tcp_set_sound_volume},          //设置门未锁好语音音量大小
    {func_tcp_set_laDoor_volume,Func_tcp_set_sound_volume},            //设置拉门语音音量大小
    {func_tcp_read_hy_volume,Func_tcp_read_sound_volume},
    {func_tcp_read_hy_md5,Func_tcp_read_sound_md5},
    {func_tcp_read_syz_md5,Func_tcp_read_sound_md5},//
    {func_tcp_read_whz_md5,Func_tcp_read_sound_md5},//
    {func_tcp_read_timeoutWarn_md5,Func_tcp_read_sound_md5},//
    {func_tcp_read_timeoutTiShi_md5,Func_tcp_read_sound_md5},//
    {func_tcp_read_outDoor_md5,Func_tcp_read_sound_md5},//
    {func_tcp_read_openDoor_md5,Func_tcp_read_sound_md5},//
    {func_tcp_read_failLock_md5,Func_tcp_read_sound_md5},//
    {func_tcp_read_laDoor_md5,Func_tcp_read_sound_md5},//
    {func_tcp_set_hy_sound,Func_tcp_set_sound},//
    {func_tcp_set_syz_sound,Func_tcp_set_sound},//
    {func_tcp_set_whz_sound,Func_tcp_set_sound},//
    {func_tcp_set_timeoutWarn_sound,Func_tcp_set_sound},//
    {func_tcp_set_timeoutTiShi_sound,Func_tcp_set_sound},//
    {func_tcp_set_openDoor_sound,Func_tcp_set_sound},//
    {func_tcp_set_failLock_sound,Func_tcp_set_sound},//
    {func_tcp_set_outDoor_sound,Func_tcp_set_sound},//
    {func_tcp_set_laDoor_sound,Func_tcp_set_sound},//
    {func_tcp_read_hy_sound,Func_tcp_read_sound},
    {func_tcp_read_laDoor_sound,Func_tcp_read_sound},
    {func_tcp_read_syz_sound,Func_tcp_read_sound},
    {func_tcp_read_whz_sound,Func_tcp_read_sound},
    {func_tcp_read_timeoutWarn_sound,Func_tcp_read_sound},
    {func_tcp_read_timeoutTiShi_sound,Func_tcp_read_sound},
    {func_tcp_read_outDoor_sound,Func_tcp_read_sound},
    {func_tcp_read_openDoor_sound,Func_tcp_read_sound},
    {func_tcp_play_sound,Func_tcp_65},
    {func_tcp_read_failLock_sound,Func_tcp_read_sound}
};


//UDP数据包解析函数数
quint8 Func_Udp_01(char *buf, paraData*pdat);

//UDP数据包解析函数数指针数组
/*
static udpFunctionHandler funcUdpArray[]={
    {0x01,Func_Udp_01}
};
*/
//发送Tcp命令
quint8 createTcpRequest(quint8 func, uchar *sendData, quint16 length, QByteArray *datagram);
quint8 createTcpSetVoiceRequest(quint8 func, uchar *sendData, quint16 length, QByteArray *datagram);

#endif // DATASTRUCT_H
