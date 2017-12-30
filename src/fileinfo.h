#ifndef FILEINFO_H
#define FILEINFO_H

#include <QObject>
#include <QFileInfo>

namespace QFlow
{
class FileInfo
{
    Q_GADGET
public:
    FileInfo(QFileInfo info);
    FileInfo(QString file);
    FileInfo();
    ~FileInfo();
    Q_INVOKABLE QString absoluteFilePath() const;
    Q_INVOKABLE QString fileName() const;
    Q_INVOKABLE bool isDir() const;
    Q_INVOKABLE QString absolutePath() const;
    Q_INVOKABLE QString baseName() const;
    Q_INVOKABLE QString relativeFilePath(QString dir) const;
    Q_INVOKABLE bool exists() const;
    bool operator==(const FileInfo &other) const;
private:
    QFileInfo _fileInfo;
};
}
Q_DECLARE_METATYPE(QFlow::FileInfo)
#endif
