// ----------------------------------------------------------------------------
//
// VDream Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __V_OBJECT_H__
#define __V_OBJECT_H__

#include <QObject>

#include <VError>
#include <VSerializable>
#include <VLog>

// ----------------------------------------------------------------------------
// VObjectConnection
// ----------------------------------------------------------------------------
class VObjectConnection
{
public:
  VObjectConnection(QByteArray  signal, QObject* receiver, QByteArray  slot);
  VObjectConnection(const char* signal, QObject* receiver, const char* slot);

public:
  QByteArray signal;
  QObject*   receiver;
  QByteArray slot;

public:
  bool operator == (const VObjectConnection& r) const;
};

// ----------------------------------------------------------------------------
// VObjectConnections
// ----------------------------------------------------------------------------
class VObjectConnections : public QList<VObjectConnection>
{
};

// ----------------------------------------------------------------------------
// VObject
// ----------------------------------------------------------------------------
class VObject :
  public QObject,
  public VSerializable
{
  Q_OBJECT

public:
  //
  // constructor and destructor
  //
public:
  void* owner;

public:
  VObject(void* owner = NULL);
  VObject(const VObject& rhs) : QObject(NULL) { Q_UNUSED(rhs) }
  virtual ~VObject();

public:

  VObjectConnections connections;
  static bool connect(QObject* sender, const char* signal, QObject* receiver, const char* slot, Qt::ConnectionType type = Qt::AutoConnection);
  static bool connect(QObject *sender, const QMetaMethod &signal, QObject *receiver, const QMetaMethod &slot, Qt::ConnectionType type = Qt::AutoConnection);
  static bool disconnect(QObject* sender, const char* signal, QObject* receiver, const char* slot);
  static bool disconnect(QObject *sender, const QMetaMethod &signal, QObject *receiver, const QMetaMethod &slot);
  static QMetaMethod findMethod(QObject* object, QString methodName);

protected:
  VState m_state;

public:
  int     tag; // used for debugging
  VState  state()     { return m_state;                   }
  QString className() { return this->metaObject()->className(); }
  bool    active()    { return m_state == VState::Opened; }

  //
  // error
  //
public:
  VError error;

  //
  // open and close
  //
public slots:
  virtual bool open();
  virtual bool close();

public:
  virtual bool close(bool wait, VTimeout timeout = VBase::TIMEOUT);
  virtual bool wait(VTimeout timeout = VBase::TIMEOUT);

protected:
  virtual bool doOpen();
  virtual bool doClose();

signals:
  void opened();
  void closed();

  //
  // load and save
  //
public:
  virtual void load(VRep& rep);
  virtual void save(VRep& rep);

#ifdef QT_GUI_LIB
public:
  virtual QWidget* createWidget(QWidget* parent);
  virtual void createTreeWidgetItems(VTreeWidgetItem* parent);

public slots:
  void objectNameEditingFinished();
  void textEditingFinished();
  void boolStateChanged(int state);
  void enumCurrentIndexChanged(int index);
  void pbAddClicked();
  void pbDelClicked();
#endif // QT_GUI_LIB
};
Q_DECLARE_METATYPE(VObject*)

class VObjectList : public QList<VObject*>
{
public:
  virtual VObject* createObject() { return new VObject; }
  virtual ~VObjectList() {}
};
Q_DECLARE_METATYPE(VObjectList*)

template <class T>
class _VObjectList : public VObjectList
{
public:
  virtual VObject* createObject() { return new T; }
};

#endif // __V_OBJECT_H__
