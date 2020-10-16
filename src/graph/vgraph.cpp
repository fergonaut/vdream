#include <VGraph>
#include <VThread> // for VThreadMgr

// ----------------------------------------------------------------------------
// VGraphObjectList
// ----------------------------------------------------------------------------
VGraphObjectList::VGraphObjectList(VGraph* graph)
{
  m_graph = graph;
  clear();
}

VGraphObjectList::~VGraphObjectList()
{
  clear();
}

void VGraphObjectList::clear()
{
  int _count = this->count();
  for (int i = 0; i < _count; i++)
  {
    VStateObject* object = this->at(i);
    delete object;
  }
  QList<VStateObject*>::clear();
}

bool VGraphObjectList::addObject(VStateObject* object)
{
  this->push_back(object);
  return true;
}

bool VGraphObjectList::delObject(VStateObject* object)
{
  int index = this->indexOf(object);
  if (index == -1) return false;
  delete object;
  this->removeAt(index);
  return true;
}

VStateObject* VGraphObjectList::findObjectByName(QString name)
{
  int _count = this->count();
  for (int i = 0; i < _count; i++)
  {
    VStateObject* obj = this->at(i);
    if (obj->objectName() == name) return obj;
  }
  LOG_ERROR("can not find object for ('%s')", qPrintable(name));
  return NULL;
}

VStateObject* VGraphObjectList::findObjectByClassName(QString className)
{
  int _count = this->count();
  for (int i = 0; i < _count; i++)
  {
    VStateObject* obj = this->at(i);
    if (obj->className() == className) return obj;
  }
  LOG_ERROR("can not find object for ('%s')", qPrintable(className));
  return NULL;
}

QList<VStateObject*> VGraphObjectList::findObjectsByClassName(QString className)
{
  QList<VStateObject*> res;
  int _count = this->count();
  for (int i = 0; i < _count; i++)
  {
    VStateObject* obj = this->at(i);
    if (obj->className() == className) res.push_back(obj);
  }
  return res;
}

QList<VStateObject*> VGraphObjectList::findObjectsByCategoryName(QString categoryName)
{
  VFactory& factory = VFactory::instance();
  VFactory::VMetaObjectList mobjList = factory.getMetaObjectsByCategoryName(categoryName);

  QList<VStateObject*> res;
  int _count = this->count();
  for (int i = 0; i < _count; i++)
  {
    VStateObject* obj = this->at(i);
    foreach (const QMetaObject* mobj, mobjList)
    {
      if (QString(mobj->className()) == obj->className())
      {
        res.push_back(obj);
      }
    }
  }
  return res;
}

QStringList VGraphObjectList::findObjectNamesByClassName(QString className)
{
  QList<VStateObject*> objList = findObjectsByClassName(className);
  QStringList ret;
  foreach (VStateObject* object, objList) ret.push_back(object->objectName());
  return ret;
}

QStringList VGraphObjectList::findObjectNamesByCategoryName(QString categoryName)
{
  QList<VStateObject*> objList = findObjectsByCategoryName(categoryName);
  QStringList res;
  foreach (VStateObject* object, objList) res.push_back(object->objectName());
  return res;
}

#include <QMetaType>
void VGraphObjectList::load(VRep& rep)
{
  clear();
  foreach (const VRep& childRep, rep.childs())
  {
    QString className = childRep["_class"].toString();
    if (className == "")
    {
      LOG_ERROR("_class is null");
      return;
    }
<<<<<<< HEAD:src/graph/vgraph.cpp
    VStateObject* obj = dynamic_cast<VStateObject*>(VFactory::instance().createObjectByClassName(className));
    if (obj == NULL)
    {
      LOG_ERROR("can not create instance for %s", qPrintable(className));
    }
    obj->owner = this->m_graph;
    obj->load(childXml);
    this->push_back(obj);
=======
    VObject* object = (VObject*)VFactory::instance().createByClassName(className);
    if (object == NULL)
    {
      LOG_ERROR("can not create instance for %s", qPrintable(className));
    }
    object->owner = this->m_graph;
    object->load((VRep&)childRep);
    this->push_back(object);
>>>>>>> issue12:include/graph/vgraph.cpp
  }
}

void VGraphObjectList::save(VRep& rep)
{
  rep.clearChilds();
  int _count = this->count();
  for (int i = 0; i < _count; i++)
  {
<<<<<<< HEAD:src/graph/vgraph.cpp
    VStateObject* obj = this->at(i);
    VXml childXml = xml.addChild("object");
    obj->save(childXml);
=======
    VObject* object = this->at(i);
    VRep childRep;
    object->save(childRep);
    rep.insert(QString("object") + QString::number(i), childRep);
>>>>>>> issue12:include/graph/vgraph.cpp
  }
}

// ----------------------------------------------------------------------------
// VGraphConnect
// ----------------------------------------------------------------------------
VGraphConnect::VGraphConnect()
{
  sender   = "";
  signal   = "";
  receiver = "";
  slot     = "";
}

VGraphConnect::~VGraphConnect()
{
}

bool VGraphConnect::operator == (const VGraphConnect& rhs)
{
  if (sender   != rhs.sender)   return false;
  if (signal   != rhs.signal)   return false;
  if (receiver != rhs.receiver) return false;
  if (slot     != rhs.slot)     return false;
  return true;
}

void VGraphConnect::load(VRep& rep)
{
  sender   = rep.getStr("sender",   sender);
  signal   = rep.getStr("signal",   signal);
  receiver = rep.getStr("receiver", receiver);
  slot     = rep.getStr("slot",     slot);
}

void VGraphConnect::save(VRep& rep)
{
  rep.setStr("sender",   sender);
  rep.setStr("signal",   signal);
  rep.setStr("receiver", receiver);
  rep.setStr("slot",     slot);
}

// ----------------------------------------------------------------------------
// VGraphConnectList
// ----------------------------------------------------------------------------
VGraphConnectList::VGraphConnectList(VGraph* graph): m_graph(graph)
{
  clear();
}

VGraphConnectList::~VGraphConnectList()
{
  clear();
}

void VGraphConnectList::clear()
{
  QList<VGraphConnect>::clear();
}

bool VGraphConnectList::addConnect(const VGraphConnect connect)
{
  int i = this->indexOf(connect); // gilgil temp 2012.07.27
  if (i != -1)
  {
    LOG_ERROR("connect(%s %s %s %s) already exists",
      qPrintable(connect.sender),
      qPrintable(connect.signal),
      qPrintable(connect.receiver),
      qPrintable(connect.slot));
    return false;
  }

  VObject*    sender   = m_graph->objectList.findObjectByName(connect.sender);
  QMetaMethod signal   = VObject::findMethod(sender, connect.signal);

  VObject*    receiver = m_graph->objectList.findObjectByName(connect.receiver);
  QMetaMethod slot     = VObject::findMethod(receiver, connect.slot);

  if (sender == NULL)   return false;
  if (receiver == NULL) return false;

  bool res = VObject::connect(sender, signal, receiver, slot, Qt::DirectConnection);
  if (!res)
  {
    LOG_ERROR("VObject::connect(%s %s %s %s) return false",
      qPrintable(sender->objectName()),   qPrintable(connect.signal),
      qPrintable(receiver->objectName()), qPrintable(connect.slot));
    return false;
  }

  this->push_back(connect);
  return true;
}

bool VGraphConnectList::delConnect(VGraphConnect connect)
{
  int i = this->indexOf(connect); // gilgil temp 2012.07.27
  if (i == -1)
  {
    LOG_ERROR("can not find connect(%s %s %s %s)",
      qPrintable(connect.sender),
      qPrintable(connect.signal),
      qPrintable(connect.receiver),
      qPrintable(connect.slot));
    return false;
  }

  VStateObject* sender   = m_graph->objectList.findObjectByName(connect.sender);
  QMetaMethod   signal   = VObject::findMethod(sender, connect.signal);

  VStateObject* receiver = m_graph->objectList.findObjectByName(connect.receiver);
  QMetaMethod   slot     = VObject::findMethod(receiver, connect.slot);

  if (sender == NULL)   return false;
  if (receiver == NULL) return false;

  bool res = VObject::disconnect(sender, signal, receiver, slot);
  if (!res) return false;

  this->removeAt(i);
  return true;
}

void VGraphConnectList::load(VRep& rep)
{
  clear();
  xml_foreach (childXml, rep.childs())
  {
    VGraphConnect connect;
    connect.load(childXml);
    addConnect(connect);
  }
}

void VGraphConnectList::save(VRep& rep)
{
  rep.clearChild();
  int _count = this->count();
  for (int i = 0; i < _count; i++)
  {
    VGraphConnect connect = at(i);
    connect.save(rep.addChild("connect"));
  }
}

// ----------------------------------------------------------------------------
// VGraph
// ----------------------------------------------------------------------------
VGraph::VGraph(void* owner) : VStateObject(owner), objectList(this), connectList(this)
{
  clear();
}

VGraph::~VGraph()
{
  close();
  clear();
}

void VGraph::clear()
{
  objectList.clear();
  connectList.clear();
}

bool VGraph::doOpen()
{
  VThreadMgr& mgr = VThreadMgr::instance();
  // mgr.clear(true); // gilgil temp 2012.08.17
  mgr.suspend();

  int res = true;
  int _count = objectList.count();
  for (int i = 0; i < _count; i++)
  {
    VStateObject* object = objectList.at(i);
    if (!object->open())
    {
      error = object->error;
      QString msg = QString(error.msg) + "(" + object->objectName() + ")";
      error.msg = msg;
      res = false;
      break;
    }
    if (!VObject::connect(object, SIGNAL(closed()), this, SLOT(close())))
    {
      LOG_ERROR("connect %s closed > this close() return false", qPrintable(object->objectName()));
    }
  }
  mgr.resume();
  return res;
}

bool VGraph::doClose()
{
  int _count = objectList.count();
  for (int i = 0; i < _count; i++)
  {
    VStateObject* object = objectList.at(i);
    object->close();
  }
  return true;
}

QStringList VGraph::methodList(VStateObject* object, QMetaMethod::MethodType methodType)
{
  QStringList res;

  int _count = object->metaObject()->methodCount();
  for (int i = 0; i < _count; i++)
  {
    QMetaMethod _method = object->metaObject()->method(i);
    if (_method.access() == QMetaMethod::Public && _method.methodType() == methodType)
    {
      QString methodName = _method.methodSignature();
      res.push_back(methodName);
     }
  }
  return res;
}

QStringList VGraph::signalList(VStateObject *object)
{
  return methodList(object, QMetaMethod::Signal);
}

QStringList VGraph::slotList(VStateObject *object)
{
  return methodList(object, QMetaMethod::Slot);
}

void VGraph::load(VRep& rep)
{
  objectList.load(rep.gotoChild("objectList"));
  connectList.load(rep.gotoChild("connectList"));
}

void VGraph::save(VRep& rep)
{
  objectList.save(rep.gotoChild("objectList"));
  connectList.save(rep.gotoChild("connectList"));
}
