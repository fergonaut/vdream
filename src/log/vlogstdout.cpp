#include <VLogStdout>
#include <VDebugNew>

// REGISTER_METACLASS(VLogStdout, VLog) // gilgil temp 2015.02.01

// ----------------------------------------------------------------------------
// VLogStdout
// ----------------------------------------------------------------------------
VLogStdout::VLogStdout(void* owner) : VLog(owner)
{
  autoFlush = true;
}

VLogStdout::~VLogStdout()
{
}

void VLogStdout::write(const char* buf, int len)
{
  Q_UNUSED(len)
  printf("%s\n", buf);
  if (autoFlush) fflush(stdout);
}

VLog* VLogStdout::createByURI(const QString& uri)
{
  if (uri == "stdout")
  {
    return new VLogStdout;
  }
  return NULL;
}

void VLogStdout::load(VRep& rep)
{
  VLog::load(xml);

  autoFlush = rep.getBool("autoFlush", autoFlush);
}

void VLogStdout::save(VRep& rep)
{
  VLog::save(xml);

  rep.setBool("autoFlush", autoFlush);
}
