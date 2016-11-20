#include "paranetwork.h"

paraNetwork::paraNetwork()
{
    ip="";
    gateWay="";
    macAddr="";
    subnetMask="";
    networkModel=0;//连网方式 0－服务器
    remoteIp="";
    tcpPort=0;
    udpPort=0;
    dataChanged=0;//数据发生变化时为1
}

paraNetwork::~paraNetwork()
{

}
