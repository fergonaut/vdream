#include <VUdpSession>
#include <VDebugNew>

#ifdef __linux__
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif // __linux__

// ----------------------------------------------------------------------------
// VUdpSession
// ----------------------------------------------------------------------------
VUdpSession::VUdpSession(void* owner) : VNetSession(owner)
{
  handle = INVALID_SOCKET;
  memset(&addr, 0, sizeof(addr));
}

VUdpSession::~VUdpSession()
{
  close();
}

bool VUdpSession::doOpen()
{
  VLock lock(stateOpenCs); // gilgil temp 2014.03.14

  if (handle == INVALID_SOCKET)
  {
    SET_ERROR(VError, "handle is zero", VError::HANDLE_IS_ZERO);
    return false;
  }
  return true;
}

bool VUdpSession::doClose()
{
  VLock lock(stateCloseCs); // gilgil temp 2014.03.14

  if (handle == INVALID_SOCKET) return true;

  //
  // shutdown
  //
  int res = ::shutdown(handle, SD_BOTH);
  if (res == SOCKET_ERROR)
  {
#ifdef __linux__
    if (WSAGetLastError() != ENOTCONN)
#endif // __linux__
      SET_ERROR(VSocketError, "error in shutdown", (int)WSAGetLastError());
  }

  //
  // closesocket
  //
#ifdef _WIN32
  res = ::closesocket(handle);
#endif // _WIN32
#ifdef __linux__
  res = ::close(handle);
#endif // __linux__
  if (res == SOCKET_ERROR)
  {
    SET_ERROR(VSocketError, "error in closesocket", (int)WSAGetLastError());
  }

  handle = INVALID_SOCKET;
  return true;
}

// ----- gilgil temp 2009.08.16 -----
void VUdpSession::logAddr(struct sockaddr_in* sockAddr)
{
  LOG_DEBUG("[VDUDPSession.cpp] logAddr sin_family=%d sin_port=%d sin_addr=%d.%d.%d.%d",
    sockAddr->sin_family,
    ntohs(sockAddr->sin_port),
    (unsigned char)((sockAddr->sin_addr.s_addr & 0xFF000000) >> 24),
    (unsigned char)((sockAddr->sin_addr.s_addr & 0x00FF0000) >> 16),
    (unsigned char)((sockAddr->sin_addr.s_addr & 0x0000FF00) >> 8),
    (unsigned char)((sockAddr->sin_addr.s_addr & 0x000000FF) >> 0));
}
// ----------------------------------

int VUdpSession::doRead(char* buf, int size)
{
  VLock lock(stateReadCs); // gilgil temp 2014.03.14

  struct sockaddr_in tempAddr;
  socklen_t fromLen = sizeof(tempAddr);
  memset(&tempAddr, 0, sizeof(tempAddr));
  int res = recvfrom(handle, buf, size, 0, (struct sockaddr*)&tempAddr, &fromLen);
  // logAddr(&tempAddr); // gilgil temp 2012.05.29
  addr = tempAddr;
  if (res == SOCKET_ERROR)
  {
    SET_DEBUG_ERROR(VSocketError, "error in recv", (int)WSAGetLastError());
    return VError::FAIL;
  }
  // sometimes, read length can be 0(zero), and check if return value is not zero.
  if (res == 0)
  {
    SET_DEBUG_ERROR(VNetError, "recv return zero", VNetError::ERROR_IN_RECV);
    return VError::FAIL;
  }
  return res;
}

int VUdpSession::doWrite(char* buf, int size)
{
  // LOG_DEBUG("log test"); // gilgil temp 2012.11.01

  VLock lock(stateWriteCs); // gilgil temp 2014.03.14

  int res;
  int restSize = size;

  while (true)
  {
    if (onceWriteSize!= 0 && restSize > onceWriteSize) restSize = onceWriteSize;
    res = ::sendto(handle, buf, restSize, 0, (struct sockaddr*)&addr, sizeof(addr));;
    if (res == SOCKET_ERROR)
    {
      SET_ERROR(VSocketError, "error in send", (int)WSAGetLastError());
      return VError::FAIL;
    }
    buf += res;
    restSize -= res;
    if (restSize == 0) break;
  }
  return size;
}

Ip VUdpSession::getLocalIP()
{
  socklen_t size;
  struct sockaddr_in sockAddr;

  if (handle == INVALID_SOCKET) return 0;
  size = sizeof(sockAddr);
  if (::getsockname(handle, (struct sockaddr*)&sockAddr, &size) != 0) return 0;
  Ip res = *((Ip*)&(sockAddr.sin_addr));
  res = ntohl(res);
  return res;
}

Ip VUdpSession::getRemoteIP()
{
  socklen_t size;
  struct sockaddr_in sockAddr;

  if (handle == INVALID_SOCKET) return 0;
  size = sizeof(sockAddr);
  if (::getpeername(handle, (struct sockaddr*)&sockAddr, &size) != 0) return 0;
  Ip res = *((Ip*)&(sockAddr.sin_addr));
  res = ntohl(res);
  return res;
}

int VUdpSession::getLocalPort()
{
  socklen_t size;
  struct sockaddr_in sockAddr;

  if (handle == INVALID_SOCKET) return 0;
  size = sizeof(sockAddr);
  if (::getsockname(handle, (struct sockaddr*)&sockAddr, &size) != 0) return 0;
  int res = ntohs(sockAddr.sin_port);
  return res;
}

int VUdpSession::getRemotePort()
{
  socklen_t size;
  struct sockaddr_in sockAddr;

  if (handle == INVALID_SOCKET) return 0;
  size = sizeof(sockAddr);
  if (::getpeername(handle, (struct sockaddr*)&sockAddr, &size) != 0) return 0;
  int res = ntohs(sockAddr.sin_port);
  return res;
}

bool VUdpSession::operator == (const VUdpSession& rhs) const
{
  if (handle != rhs.handle) return false;
  if (memcmp(&addr, &rhs.addr, sizeof(addr)) != 0) return false;
  return true;
}
