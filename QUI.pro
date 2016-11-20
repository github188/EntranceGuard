#-------------------------------------------------
#
# Project created by QtCreator 2014-08-15T14:34:50
#
#-------------------------------------------------

QT       += core gui network sql xml
QT        += phonon
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QUI
TEMPLATE = app


SOURCES += main.cpp\
        frmmain.cpp \
    iconhelper.cpp \
    frmmessagebox.cpp\
    QsLog/QsLogDestFunctor.cpp \
    QsLog/QsLogDestFile.cpp \
    QsLog/QsLogDestConsole.cpp \
    QsLog/QsLogDest.cpp \
    QsLog/QsLog.cpp \
    TCPIP/mytcpserver.cpp \
    Equipment/equipment.cpp \
    frmaddequ.cpp \
    frmsearchequipment.cpp \
    datastruct.cpp \
    CycleBuffer/CCycleBuffer.cpp \
    DATA/equstatus.cpp \
    DATA/parazhaoming.cpp \
    DATA/parafanghucang.cpp \
    DATA/paranetwork.cpp \
    DATA/paradata.cpp \
    TreeViewClass/itemdelegate.cpp \
    TreeViewClass/treeitem.cpp \
    TreeViewClass/treemodel.cpp \
    DATA/treeviewitem.cpp \
    selectparentintree.cpp \
    frmoperateparentmsgbox.cpp \
    frmoperateparentintree.cpp \
    ListViewClass/listmodel.cpp \
    frmaddslave.cpp \
    frmeditparamater.cpp \
    frmeditzhaomingpara.cpp \
    DATA/slavemsg.cpp \
    frmreadslavemsg.cpp \
    frmmanagervoice.cpp \
    frmsavesound.cpp \
    frmreadsoundprocess.cpp \
    frmselectsound.cpp \
    frmaddsound.cpp \
    frmmanagermodule.cpp \
    frmusemodule.cpp \
    frmmodulepara.cpp \
    DATA/paramodule.cpp \
    frmalarmbox.cpp \
    frmeditalarmpara.cpp \
    DATA/paraalarm.cpp \
    frmlogin.cpp \
    AppConfig/myapp.cpp \
    frmsoftwareinfo.cpp \
    frmedituserpwd.cpp \
    frmguestpermissionconfig.cpp \
    DATA/voiceinfobank.cpp \
    User/userpermission.cpp \
    User/userinfo.cpp \
    DATA/systemlogitem.cpp \
    DATA/fanghucanglogitem.cpp \
    frmsearchslavelog.cpp \
    frmsearchsystemlog.cpp \
    odbcexcel.cpp \
    frmeditcjcworkpara.cpp \
    frmslaveuserlist.cpp \
    DATA/indoorkeymsg.cpp
HEADERS  += frmmain.h \
    iconhelper.h \
    frmmessagebox.h \
    myhelper.h\
    QsLog/QsLogLevel.h \
    QsLog/QsLogDisableForThisFile.h \
    QsLog/QsLogDestFunctor.h \
    QsLog/QsLogDestFile.h \
    QsLog/QsLogDestConsole.h \
    QsLog/QsLogDest.h \
    QsLog/QsLog.h \
    TCPIP/mytcpserver.h \
    datastruct.h \
    Equipment/equipment.h \
    frmaddequ.h \
    frmsearchequipment.h \
    CycleBuffer/CCycleBuffer.h \
    DATA/equstatus.h \
    DATA/parazhaoming.h \
    DATA/parafanghucang.h \
    DATA/paranetwork.h \
    DATA/paradata.h \
    TreeViewClass/itemdelegate.h \
    TreeViewClass/treeitem.h \
    TreeViewClass/treemodel.h \
    DATA/treeviewitem.h \
    selectparentintree.h \
    frmoperateparentmsgbox.h \
    frmoperateparentintree.h \
    ListViewClass/listmodel.h \
    frmaddslave.h \
    frmeditparamater.h \
    frmeditzhaomingpara.h \
    DATA/slavemsg.h \
    frmreadslavemsg.h \
    frmmanagervoice.h \
    frmsavesound.h \
    frmreadsoundprocess.h \
    frmselectsound.h \
    frmaddsound.h \
    frmmanagermodule.h \
    frmusemodule.h \
    frmmodulepara.h \
    DATA/paramodule.h \
    frmalarmbox.h \
    frmeditalarmpara.h \
    DATA/paraalarm.h \
    frmlogin.h \
    DATA/userinfo.h \
    AppConfig/myapp.h \
    frmsoftwareinfo.h \
    frmedituserpwd.h \
    frmguestpermissionconfig.h \
    DATA/voiceinfobank.h \
    User/userpermission.h \
    User/userinfo.h \
    DATA/systemlogitem.h \
    DATA/fanghucanglogitem.h \
    frmsearchslavelog.h \
    frmsearchsystemlog.h \
    odbcexcel.h \
    frmeditcjcworkpara.h \
    frmslaveuserlist.h \
    DATA/indoorkeymsg.h
FORMS    += frmmain.ui \
    frmmessagebox.ui \
    frmaddequ.ui \
    frmsearchequipment.ui \
    selectparentintree.ui \
    frmoperateparentmsgbox.ui \
    frmoperateparentintree.ui \
    frmaddslave.ui \
    frmeditparamater.ui \
    frmeditzhaomingpara.ui \
    frmreadslavemsg.ui \
    frmmanagervoice.ui \
    frmsavesound.ui \
    frmreadsoundprocess.ui \
    frmselectsound.ui \
    frmaddsound.ui \
    frmmanagermodule.ui \
    frmusemodule.ui \
    frmmodulepara.ui \
    frmalarmbox.ui \
    frmeditalarmpara.ui \
    frmlogin.ui \
    frmsoftwareinfo.ui \
    frmedituserpwd.ui \
    frmguestpermissionconfig.ui \
    frmsearchslavelog.ui \
    frmsearchsystemlog.ui \
    frmeditcjcworkpara.ui \
    frmslaveuserlist.ui

MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
DESTDIR=bin
INCLUDEPATH += $$PWD/DATA

win32:RC_FILE=main.rc

RESOURCES += \
    rc.qrc
DEFINES += QS_LOG_LINE_NUMBERS # automatically writes the file and line for each log message
# DEFINES += QS_LOG_DISABLE         # logging code is replaced with a no-op
# DEFINES += QS_LOG_SEPARATE_THREAD # messages are queued and written from a separate thread
