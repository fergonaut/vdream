CONFIG += VDREAM_LIB_BUILD
include (vdream.pri)

TARGET   = $${VDREAM_LIB_NAME}
TEMPLATE = lib
CONFIG  += staticlib
DESTDIR  = $${PWD}/lib

SOURCES += \
  src/base/vapp.cpp \
  src/base/vbase.cpp \
  src/base/vdebugnew.cpp \
  src/base/vdependency.cpp \
  src/base/verror.cpp \
  src/base/vfactory.cpp \
  src/base/vfile.cpp \
  src/base/vinstance.cpp \
  src/base/vmemoryleak.cpp \
  src/base/vobject.cpp \
  src/base/vobjectwidget.cpp \
  src/base/vperformance.cpp \
  src/base/vprimitive.cpp \
  src/base/vrwobject.cpp \
  src/base/vstate.cpp \
  src/base/vstateobject.cpp \
  src/base/vsystem.cpp \
  src/base/vthread.cpp \
  src/base/vtick.cpp \
  src/base/vxml.cpp \
  src/base/vxmldoc.cpp \
  src/etc/vdatachange.cpp \
  src/etc/vdatafind.cpp \
  src/etc/vint.cpp \
  src/etc/vlistwidget.cpp \
  src/etc/vmyobject.cpp \
  src/graph/vgraph.cpp \
  src/graph/vgraphlink.cpp \
  src/graph/vgraphnode.cpp \
  src/graph/vgraphstream.cpp \
  src/log/vlog.cpp \
  src/log/vlogdbwin32.cpp \
  src/log/vlogfactory.cpp \
  src/log/vlogfile.cpp \
  src/log/vloghttp.cpp \
  src/log/vloglist.cpp \
  src/log/vlognull.cpp \
  src/log/vlogstderr.cpp \
  src/log/vlogstdout.cpp \
  src/log/vlogudp.cpp \
  src/net/http/vhttpchunkbody.cpp \
  src/net/http/vhttpheader.cpp \
  src/net/http/vhttprequest.cpp \
  src/net/http/vhttpresponse.cpp \
  src/net/http/vwebproxy.cpp \
  src/net/http/vwebproxywidget.cpp \
  src/net/vnet.cpp \
  src/net/vnetclient.cpp \
  src/net/vnetfactory.cpp \
  src/net/vnetserver.cpp \
  src/net/vsslbase.cpp \
  src/net/vsslclient.cpp \
  src/net/vsslserver.cpp \
  src/net/vsslsession.cpp \
  src/net/vtcpclient.cpp \
  src/net/vtcpserver.cpp \
  src/net/vtcpsession.cpp \
  src/net/vudpclient.cpp \
  src/net/vudpserver.cpp \
  src/net/vudpsession.cpp

HEADERS += \
  src/base/vapp.h \
  src/base/vbase.h \
  src/base/vdebugnew.h \
  src/base/vdebugnewcancel.h \
  src/base/vdependency.h \
  src/base/verror.h \
  src/base/vfactory.h \
  src/base/vfile.h \
  src/base/vinstance.h \
  src/base/vlinux.h \
  src/base/vmemoryleak.h \
  src/base/vobject.h \
  src/base/vobjectwidget.h \
  src/base/vperformance.h \
  src/base/vprimitive.h \
  src/base/vrwobject.h \
  src/base/vstate.h \
  src/base/vstateobject.h \
  src/base/vsystem.h \
  src/base/vthread.h \
  src/base/vtick.h \
  src/base/vwindows.h \
  src/base/vxml.h \
  src/base/vxmldoc.h \
  src/etc/vdatachange.h \
  src/etc/vdatafind.h \
  src/etc/vint.h \
  src/etc/vlistwidget.h \
  src/etc/vmyobject.h \
  src/graph/vgraph.h \
  src/graph/vgraphlink.h \
  src/graph/vgraphnode.h \
  src/graph/vgraphstream.h \
  src/log/vlog.h \
  src/log/vlogdbwin32.h \
  src/log/vlogfactory.h \
  src/log/vlogfile.h \
  src/log/vloghttp.h \
  src/log/vloglist.h \
  src/log/vlognull.h \
  src/log/vlogstderr.h \
  src/log/vlogstdout.h \
  src/log/vlogudp.h \
  src/net/http/vhttpbase.h \
  src/net/http/vhttpchunkbody.h \
  src/net/http/vhttpheader.h \
  src/net/http/vhttprequest.h \
  src/net/http/vhttpresponse.h \
  src/net/http/vwebproxy.h \
  src/net/http/vwebproxywidget.h \
  src/net/vnet.h \
  src/net/vnetclient.h \
  src/net/vnetfactory.h \
  src/net/vnetserver.h \
  src/net/vnetsession.h \
  src/net/vsslbase.h \
  src/net/vsslclient.h \
  src/net/vsslserver.h \
  src/net/vsslsession.h \
  src/net/vtcpclient.h \
  src/net/vtcpserver.h \
  src/net/vtcpsession.h \
  src/net/vudpclient.h \
  src/net/vudpserver.h \
  src/net/vudpsession.h

contains(QT, gui) {
FORMS += \
  src/etc/vlistwidget.ui \
  src/net/http/vwebproxywidget.ui
}