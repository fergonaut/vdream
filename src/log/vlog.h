// ----------------------------------------------------------------------------
//
// VDream Component Suite version 9.01
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __V_LOG_H__
#define __V_LOG_H__

<<<<<<< HEAD:src/log/vlog.h
// #include <VBase> // gilgil temp 2015.02.01
// #include <VMetaClass> // gilgil temp 2015.02.01
#include <VObject>
=======
#include <VBase>
#include <VMetaClass>
#include <VSerializable>
>>>>>>> issue12:include/log/vlog.h

// ----------------------------------------------------------------------------
// VShowDateTime
// ----------------------------------------------------------------------------
class VShowDateTime
{
public:
  enum _VShowDateTime
  {
    None,
    Time,
    DateTime
  };

protected:
  _VShowDateTime value;

public:
  VShowDateTime()                           {                      } // default ctor
  VShowDateTime(const _VShowDateTime value) { this->value = value; } // conversion ctor
  operator _VShowDateTime() const           { return value;        } // cast operator

public:
  VShowDateTime(const QString s);
  QString str() const;
};

<<<<<<< HEAD:src/log/vlog.h
class VLog : public VObject
=======
class VLog : public VSerializable
>>>>>>> issue12:include/log/vlog.h
{
  Q_OBJECT

  friend class VLogList;
  friend class VLogFactory;

public:
  enum {
    LEVEL_NONE = 0,
    LEVEL_DEBUG,
    LEVEL_INFO,
    LEVEL_WARN,
    LEVEL_ERROR,
    LEVEL_FATAL
  };

public:
  static const int DEFAULT_LOG_BUF_SIZE = 65536;

public:
  static const char* __extractFileName__(const char* fileName);
  static const char* __extractFuncName__(const char* funcName);

  //
  // Option
  //
public:
  int           level;
  VShowDateTime showDateTime;
  bool          showThreadID;

  //
  // Constructor and destructor
  //
public:
  VLog(void* owner = NULL);
  virtual ~VLog();

  //
  // Used internally
  //
protected:
  int makeBuf(const char* buf, int len, const char* fmt, va_list args); // dateTime, threadID, args

  //
  // VLog function
  //
public:
  virtual void debug(const char* fmt, ...) __attribute__((format(gnu_printf, 2, 3)));
  virtual void info (const char* fmt, ...) __attribute__((format(gnu_printf, 2, 3)));
  virtual void warn (const char* fmt, ...) __attribute__((format(gnu_printf, 2, 3)));
  virtual void error(const char* fmt, ...) __attribute__((format(gnu_printf, 2, 3)));
  virtual void fatal(const char* fmt, ...) __attribute__((format(gnu_printf, 2, 3)));
  virtual void trace(const char* fmt, ...) __attribute__((format(gnu_printf, 2, 3)));

protected:
  virtual void doDebug(const char *fmt, va_list args);
  virtual void doInfo (const char *fmt, va_list args);
  virtual void doWarn (const char *fmt, va_list args);
  virtual void doError(const char *fmt, va_list args);
  virtual void doFatal(const char *fmt, va_list args);
  virtual void doTrace(const char *fmt, va_list args);

  //
  // Virtual function
  //
protected:
  virtual bool open();
  virtual bool close();

protected:
  virtual void write(const char* buf, int len) = 0; // gilgil temp 2012.11.16
  // virtual void write(const char* buf, int len){ Q_UNUSED(buf); Q_UNUSED(len); }

public:
  virtual VLog* createByURI(const QString& uri);

  //
  // XML
  //
public:
  virtual void load(VRep& rep);
  virtual void save(VRep& rep);

  //
  // Global
  //
protected:
  static VLog* g_log;
public:
  static VLog* getLog();             //{                                                         return g_log;  } // gilgil temp 2012.11.01
  static VLog* setLog(VLog* log);    //{ VLog* oldLog = g_log; g_log = log;                      return oldLog; } // gilgil temp 2012.11.01
  static VLog* changeLog(VLog* log); //{ VLog* oldLog = g_log; g_log = log; SAFE_DELETE(oldLog); return oldLog; } // gilgil temp 2012.11.01

private:
  friend class VLogInstance;
};

// ----------------------------------------------------------------------------
// Global Function
// ----------------------------------------------------------------------------
inline VLog* getLog()             { return VLog::getLog();       }
inline VLog* setLog(VLog* log)    { return VLog::setLog(log);    }
inline VLog* changeLog(VLog* log) { return VLog::changeLog(log); }

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------
#define __FILENAME__ VLog::__extractFileName__(__FILE__)
#define __FUNCNAME__ VLog::__extractFuncName__(__FUNCTION__)

#ifndef LOG_DEBUG
#define LOG_DEBUG(fmt, ...) { VLog* log = getLog(); if (log != NULL && log->level <= VLog::LEVEL_DEBUG) log->debug("[%s:%d] %s " fmt, __FILENAME__, __LINE__, __FUNCNAME__, ##__VA_ARGS__); }
#define LOG_INFO(fmt, ...)  { VLog* log = getLog(); if (log != NULL && log->level <= VLog::LEVEL_INFO ) log->info ("[%s:%d] %s " fmt, __FILENAME__, __LINE__, __FUNCNAME__, ##__VA_ARGS__); }
#define LOG_WARN(fmt, ...)  { VLog* log = getLog(); if (log != NULL && log->level <= VLog::LEVEL_WARN ) log->warn ("[%s:%d] %s " fmt, __FILENAME__, __LINE__, __FUNCNAME__, ##__VA_ARGS__); }
#define LOG_ERROR(fmt, ...) { VLog* log = getLog(); if (log != NULL && log->level <= VLog::LEVEL_ERROR) log->error("[%s:%d] %s " fmt, __FILENAME__, __LINE__, __FUNCNAME__, ##__VA_ARGS__); }
#define LOG_FATAL(fmt, ...) { VLog* log = getLog(); if (log != NULL && log->level <= VLog::LEVEL_FATAL) log->fatal("[%s:%d] %s " fmt, __FILENAME__, __LINE__, __FUNCNAME__, ##__VA_ARGS__); }
#define LOG_TRACE(fmt, ...) { VLog* log = getLog(); if (log != NULL)                                    log->trace("[%s:%d] %s " fmt, __FILENAME__, __LINE__, __FUNCNAME__, ##__VA_ARGS__); }
#endif // LOG_DEBUG

#define LOG_DEBUG_MEMORY    LOG_DEBUG("%p %d %p", this, sizeof(*this), (char*)this + sizeof(*this))

#ifdef _DEBUG
#define LOG_ASSERT(expr) if (!(expr)) LOG_FATAL("LOG_ASSERT FAILED(%s)", (#expr))
#else // _DEBUG
#define LOG_ASSERT(expr)
#endif // _DEBUG

#endif // __V_LOG_H__
