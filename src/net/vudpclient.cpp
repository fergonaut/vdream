#include <VUdpClient>
#include <VDebugNew>

// REGISTER_METACLASS(VUdpClient, VNet) // gilgil temp 2015.02.01

// ----------------------------------------------------------------------------
// VUdpClient
// ----------------------------------------------------------------------------
VUdpClient::VUdpClient(void* owner) : VNetClient(owner)
{
  udpSession = new VUdpSession(this);
}

VUdpClient::~VUdpClient()
{
  close();
  SAFE_DELETE(udpSession);
}

bool VUdpClient::doOpen()
{
  VLock lock(stateOpenCs); // gilgil temp 2014.03.14

  if (port == 0)
  {
    SET_ERROR(VNetError, "port is zero", VNetError::PORT_IS_ZERO);
    return false;
  }

  // ------------------------------------------------------------------------
  // socket
  // ------------------------------------------------------------------------
  udpSession->handle = socket(AF_INET, SOCK_DGRAM, 0);
  if (udpSession->handle == INVALID_SOCKET)
  {
    SET_ERROR(VSocketError, "error in socket", (int)WSAGetLastError());
    return false;
  }

  // ------------------------------------------------------------------------
  // bind
  // ------------------------------------------------------------------------
  udpSession->addr.sin_family = AF_INET;
  udpSession->addr.sin_port = htons(quint16(localPort));
  if (localHost == "")
  {
    udpSession->addr.sin_addr.s_addr = INADDR_ANY;
  } else
  {
    Ip ip = VNet::resolve(localHost);
    if (ip == 0)
    {
      SET_ERROR(VNetError, QString("can not resolve host(%s)").arg(localHost), VNetError::CAN_NOT_RESOLVE_HOST);
      return false;
    }
    udpSession->addr.sin_addr.s_addr = htonl(ip);
  }
  memset(&udpSession->addr.sin_zero, 0, sizeof(udpSession->addr.sin_zero));

  int res = bind(udpSession->handle, (struct sockaddr*)&udpSession->addr, sizeof(udpSession->addr));
  if (res == SOCKET_ERROR)
  {
    SET_ERROR(VSocketError, QString("error in bind(%1:%2)").arg(localHost).arg(localPort), (int)WSAGetLastError());
    return false;
  }

  // ------------------------------------------------------------------------
  // connect(nothing)
  // ------------------------------------------------------------------------
  udpSession->addr.sin_family = AF_INET;
  udpSession->addr.sin_port = htons(quint16(port));
  if (host == "")
  {
    SET_ERROR(VNetError, "host is invalid", VNetError::INVALID_HOST);
    return false;
  }
  Ip ip = VNet::resolve(host);
  if (ip == 0)
  {
    SET_ERROR(VNetError, QString("can not resolve host(%1)").arg(host), VNetError::CAN_NOT_RESOLVE_HOST);
    return false;
  }
  udpSession->addr.sin_addr.s_addr = htonl(ip);
  memset(udpSession->addr.sin_zero, 0, sizeof(udpSession->addr.sin_zero));

  // ------------------------------------------------------------------------
  // ok
  // ------------------------------------------------------------------------
  udpSession->onceWriteSize = this->onceWriteSize;
  if (!udpSession->open()) return false;

  return true;
}

bool VUdpClient::doClose()
{
  VLock lock(stateCloseCs); // gilgil temp 2014.03.14

  udpSession->close();
  return true;
}

int  VUdpClient::doRead(char* buf, int size)
{
  // VLock lock(stateReadCs); // gilgil temp 2014.03.14

  int res = udpSession->read(buf, size);
  if (res == VError::FAIL)
    error = udpSession->error;
  return res;
}

int  VUdpClient::doWrite(char* buf, int size)
{
  // VLock lock(stateWriteCs); // gilgil temp 2014.03.14

  int res = udpSession->write(buf, size);
  if (res == VError::FAIL)
    error = udpSession->error;
  return res;
}

void VUdpClient::load(VRep& rep)
{
  VNetClient::load(xml);
}

void VUdpClient::save(VRep& rep)
{
  VNetClient::save(xml);
}

#ifdef QT_GUI_LIB
void VUdpClient::optionAddWidget(QLayout* layout)
{
  VNetClient::optionAddWidget(layout);
}

void VUdpClient::optionSaveDlg(QDialog* dialog)
{
  VNetClient::optionSaveDlg(dialog);
}
#endif // QT_GUI_LIB
