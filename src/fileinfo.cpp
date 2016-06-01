#include "fileinfo.h"
#include <QDir>

namespace QFlow{
FileInfo::FileInfo(QFileInfo info) : _fileInfo(info)
{

}
FileInfo::FileInfo(QString file) : _fileInfo(QFileInfo(file))
{

}

FileInfo::FileInfo()
{

}

FileInfo::~FileInfo()
{

}

QString FileInfo::absoluteFilePath() const
{
    return _fileInfo.absoluteFilePath();
}
QString FileInfo::fileName() const
{
    return _fileInfo.fileName();
}
bool FileInfo::isDir() const
{
    return _fileInfo.isDir();
}
QString FileInfo::absolutePath() const
{
    return _fileInfo.absolutePath();
}
QString FileInfo::relativeFilePath(QString dirStr) const
{
    QDir dir(dirStr);
    return dir.relativeFilePath(_fileInfo.absoluteFilePath());
}
QString FileInfo::baseName() const
{
    return _fileInfo.baseName();
}
bool FileInfo::exists() const
{
    return _fileInfo.exists();
}
bool FileInfo::operator==(const FileInfo &other) const
{
    return _fileInfo == other._fileInfo;
}
}
