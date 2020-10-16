// ----------------------------------------------------------------------------
//
// VDream Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __V_GRAPH_H__
#define __V_GRAPH_H__

#include <VStateObject>
#include <VFactory>

// ----------------------------------------------------------------------------
// VGraphObjectList
// ----------------------------------------------------------------------------
class VGraph;
<<<<<<< HEAD:src/graph/vgraph.h
class VGraphObjectList : public QList<VStateObject*>, public VXmlable
=======
class VGraphObjectList : public QList<VObject*>, public VSerializable
>>>>>>> issue12:include/graph/vgraph.h
{
protected:
  VGraph* m_graph;

public:
  VGraphObjectList(VGraph* graph);
  virtual ~VGraphObjectList();

public:
  void clear();

public:
  bool addObject(VStateObject* object);
  bool delObject(VStateObject* object);

public:
  VStateObject* findObjectByName(QString name);
  VStateObject* findObjectByClassName(QString className);

  QList<VStateObject*> findObjectsByClassName(QString className);
  QList<VStateObject*> findObjectsByCategoryName(QString categoryName);

  QStringList findObjectNamesByClassName(QString className);
  QStringList findObjectNamesByCategoryName(QString categoryName);

public:
  virtual void load(VRep& rep);
  virtual void save(VRep& rep);
};

// ----------------------------------------------------------------------------
// VGraphConnect
// ----------------------------------------------------------------------------
class VGraphConnect : public VSerializable
{
public:
  VGraphConnect();
  virtual ~VGraphConnect();

public:
  QString sender;
  QString signal;
  QString receiver;
  QString slot;

public:
  bool operator == (const VGraphConnect& rhs);

public:
  virtual void load(VRep& rep);
  virtual void save(VRep& rep);
};

// ----------------------------------------------------------------------------
// VGraphConnectList
// ----------------------------------------------------------------------------
class VGraphConnectList : public QList<VGraphConnect>, public VSerializable
{
protected:
  VGraph* m_graph;

public:
  VGraphConnectList(VGraph* graph);
  virtual ~VGraphConnectList();

public:
  void clear();

public:
  bool addConnect(VGraphConnect connect);
  bool delConnect(VGraphConnect connect);

public:
  virtual void load(VRep& rep);
  virtual void save(VRep& rep);
};

// ----------------------------------------------------------------------------
// VGraph
// ----------------------------------------------------------------------------
class VGraph : public VStateObject
{
  Q_OBJECT

public:
  VGraph(void* owner = NULL);
  virtual ~VGraph();

public:
  void clear();

public:
  virtual bool doOpen();
  virtual bool doClose();

public:
  VGraphObjectList  objectList;
  VGraphConnectList connectList;

protected:
<<<<<<< HEAD:src/graph/vgraph.h
  static QStringList methodList(VStateObject* object, QMetaMethod::MethodType methodType);
=======
  static QStringList methodList(VObject* object, QMetaMethod::MethodType methodType);

>>>>>>> issue12:include/graph/vgraph.h
public:
  static QStringList signalList(VStateObject* object);
  static QStringList slotList(VStateObject* object);

public:
  virtual void load(VRep& rep);
  virtual void save(VRep& rep);
};

#endif // __V_GRAPH_H__
