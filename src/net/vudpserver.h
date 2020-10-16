// ----------------------------------------------------------------------------
//
// VDream Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __V_UDP_SERVER_H__
#define __V_UDP_SERVER_H__

#include <set>
#include <VNetServer>
#include <VUdpSession>

// ----------------------------------------------------------------------------
// VSockAddrList
// ----------------------------------------------------------------------------
struct sockaddr_in_Compare
{
  bool operator() (const struct sockaddr_in& lhs, const struct sockaddr_in& rhs) const
  {
    return memcmp(&lhs, &rhs, sizeof(struct sockaddr_in)) < 0;
  }
};

class VSockAddrList : public std::set<struct sockaddr_in, sockaddr_in_Compare>, public VLockable
{
public:
  VSockAddrList::iterator findBySockAddr(struct sockaddr_in sockAddr);
  VSockAddrList::iterator findByInAddr(struct in_addr inAddr);
};

// ----------------------------------------------------------------------------
// VUdpServer
// ----------------------------------------------------------------------------
class VUdpServer : public VNetServer, protected VStateLockable
{
public:
  VUdpServer(void* owner = NULL);
  virtual ~VUdpServer();

protected:
  virtual bool doOpen();
  virtual bool doClose();
  virtual int  doRead(char* buf, int size);
  virtual int  doWrite(char* buf, int size);

public:
  VUdpSession*  udpSession; // read only
  VSockAddrList sockAddrList;

public:
  virtual void load(VRep& rep);
  virtual void save(VRep& rep);

#ifdef QT_GUI_LIB
private: // for VOptionable
  QLineEdit* lePort;
  QLineEdit* leLocalHost;
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __V_UDP_SERVER_H__
