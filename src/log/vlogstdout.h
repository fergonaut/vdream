// ----------------------------------------------------------------------------
//
// VDream Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __V_LOG_STDOUT_H__
#define __V_LOG_STDOUT_H__

#include <VLog>

// ----------------------------------------------------------------------------
// VLogStdout
// ----------------------------------------------------------------------------
class VLogStdout : public VLog
{
  Q_OBJECT

public:
  Q_INVOKABLE VLogStdout(void* owner = NULL);
  virtual ~VLogStdout();

public:
  bool autoFlush;

protected:
  virtual void write(const char* buf, int len);

public:
  virtual VLog* createByURI(const QString& uri);

public:
  virtual void load(VRep& rep);
  virtual void save(VRep& rep);
};

#endif // __V_LOG_STDOUT_H__
