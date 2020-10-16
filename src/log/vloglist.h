// ----------------------------------------------------------------------------
//
// VDream Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __V_LOG_LIST_H__
#define __V_LOG_LIST_H__

#include <list>
#include <VLog>

// ----------------------------------------------------------------------------
// VLogList
// ----------------------------------------------------------------------------
class VLogList : public VLog
{
  Q_OBJECT

public:
  Q_INVOKABLE VLogList(void* owner = NULL);
  virtual ~VLogList();
  void clear();

public:
  QList<VLog*> items;

public:
  virtual void debug(const char* fmt, ...);
  virtual void info (const char* fmt, ...);
  virtual void warn (const char* fmt, ...);
  virtual void error(const char* fmt, ...);
  virtual void fatal(const char* fmt, ...);
  virtual void trace(const char* fmt, ...);

protected:
  virtual bool open();
  virtual bool close();

protected:
  virtual void write(const char* buf, int len);

public:
  virtual VLog*  createByURI(const QString& uri);

  //
  // PTree
  //
public:
  virtual void load(VRep& rep);
  virtual void save(VRep& rep);
};

#endif // __V_LOG_LIST_H__
