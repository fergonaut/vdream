#include <VFile>
#include <VDebugNew>

// ----------------------------------------------------------------------------
// VFile
// ----------------------------------------------------------------------------
VFile::VFile(void* owner) : VRwObject(owner)
{
  fileName = "";
  mode     = QIODevice::NotOpen;
}

VFile::VFile(const QString& fileName) : VRwObject(NULL)
{
  this->fileName = fileName;
  mode           = QIODevice::NotOpen;
}

VFile::VFile(const QString& fileName, QIODevice::OpenMode mode)
{
  this->fileName = fileName;
  this->mode     = mode;
}

VFile::~VFile()
{
  close();
}

bool VFile::doOpen()
{
  if (fileName == "")
  {
    SET_ERROR(VFileError, "file name is not specified", VFileError::FILENAME_NOT_SPECIFIED);
    return false;
  }
  primitive.setFileName(fileName);
  if (!primitive.open(mode))
  {
    if (mode | QIODevice::ReadOnly)
    {
      if (!QFile::exists(fileName))
      {
        SET_ERROR(VFileError, QString("file not exist(%1)").arg(fileName), VFileError::FILE_NOT_EXIST);
        return false;
      }
    }
	SET_ERROR(VFileError, primitive.errorString(), primitive.error());
    return false;
  }
  return true;
}

bool VFile::doClose()
{
  primitive.close();
  return true;
}

int VFile::doRead(char* buf,  int size)
{
  int res = (int)primitive.read(buf, (qint64)size);
  if (res == -1)
  {
    SET_ERROR(VFileError, QString("error in read file(%1)").arg(fileName), VFileError::ERROR_IN_READ_FILE);
    return VError::FAIL;
  }
  if (res == 0)
  {
	if (primitive.atEnd())
    {
      SET_DEBUG_ERROR(VFileError, QString("end of file(%1)").arg(fileName), VFileError::END_OF_FILE);
    } else
    {
	  SET_ERROR(VFileError, QString("read return zero(%1) %2").arg(fileName).arg(primitive.errorString()), primitive.error());
    }
    return VError::FAIL;
  }
  return res;
}

int VFile::doWrite(char* buf, int size)
{
  int res = (int)primitive.write(buf, (qint64)size);
  if (res != size)
  {
	SET_ERROR(VFileError, QString("error in write file(%1) %2").arg(fileName).arg(primitive.errorString()), primitive.error());
    return VError::FAIL;
  }
  return res;
}

bool VFile::createFolder(QString folder)
{
#ifdef _WIN32
  while (true)
  {
    int pos = folder.indexOf("/");
    if (pos == -1) break;
    folder = folder.replace(pos, 1, "\\");
  }
#endif // _WIN32
  if (folder != "")
  {
    // ----- gilgil temp 2012.05.26 -----
    if (folder.right(1) != QDir::separator())
      folder += QDir::separator();
    // ----------------------------------
    QDir dir(folder);
    if (!dir.exists())
    {
      bool res;
      if (dir.isAbsolute())
        res = dir.mkdir(folder);
      else
      {
        QString dirName = QDir::currentPath() + QDir::separator() + folder;
        res = dir.mkdir(dirName);
      }
      if (!res)
      {
        LOG_ERROR("can not create folder(%s)", qPrintable(folder));
      }
    }
  }
  return true;
}

void VFile::load(VRep& rep)
{
  VRwObject::load(xml);

  fileName = rep.getStr("fileName", fileName);
  mode = (QIODevice::OpenMode)rep.getInt("mode", (int)mode);
}

void VFile::save(VRep& rep)
{
  VRwObject::save(xml);

  rep.setStr("fileName", fileName);
  rep.setInt("mode", (int)mode);
}

#ifdef GTEST
#include <VDebugNewCancel>
#include <gtest/gtest.h>

const static QString FILE_NAME = "abcd.txt";

TEST( File, createTest )
{
  VFile file(FILE_NAME, QIODevice::WriteOnly | QIODevice::Text);
  EXPECT_TRUE( file.open() );
  int writeLen = file.write("abcdefghijklmnopqrstuvwxyz");
  EXPECT_TRUE( writeLen == 26 );
  file.close();
}

TEST( File, readTest )
{
  VFile file(FILE_NAME, QIODevice::ReadOnly | QIODevice::Text);
  EXPECT_TRUE( file.open() );
  QFile& qfile = file;
  EXPECT_EQ( qfile.size(), 26 );

  char expect = 'a';
  int  readCnt = 0;
  for (int i = 0; i < 26; i++)
  {
    char real;
    int readLen = file.read(&real, 1);
    if (readLen == VError::FAIL) break;
    EXPECT_EQ( expect, real );
    expect++;
    readCnt++;
  }
  EXPECT_EQ( readCnt, 26 );
  file.close();
}

TEST( File, appendTest )
{
  VFile file(FILE_NAME, QIODevice::Append | QIODevice::Text);
  EXPECT_TRUE( file.open() );
  int writeLen = file.write("ABCDEFGHIJKLMNOPQRSTUVEXYZ");
  EXPECT_TRUE( writeLen == 26 );
  file.close();

  file.mode = QIODevice::ReadOnly | QIODevice::Text;
  EXPECT_TRUE( file.open() );
  QFile& qfile = file;
  EXPECT_EQ( qfile.size(), 26 * 2 );
  file.close();
}

#endif // GTEST
