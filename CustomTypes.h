#ifndef CUSTOMTYPES_H
#define CUSTOMTYPES_H
#include <QString>
#include <QMetaType>

struct FilePathPropertyType
{
protected:
    QString _value;
public:
    QString value()
    {
        return _value;
    }

    FilePathPropertyType()
    {
    }

    FilePathPropertyType(QString val)
    {
        _value = val;
    }
    FilePathPropertyType(const FilePathPropertyType &b)
    {
        this->_value = b._value;
    }
    ~FilePathPropertyType()
    {

    }
    bool isEmpty()
    {
        return _value.isEmpty();
    }

    QString toString() const {
        return _value;
    }
    const char* filename()
    {
        const char* filename = _value.toUtf8().constData();
        return filename;
    }
};

struct NewFilePropertyType : public FilePathPropertyType
{
    NewFilePropertyType()
    {
    }

    NewFilePropertyType(QString val) : FilePathPropertyType(val)
    {
    }
    NewFilePropertyType(const NewFilePropertyType &b) : FilePathPropertyType(b)
    {
    }
    ~NewFilePropertyType()
    {

    }
};

struct ScriptPropertyType
{
protected:
    QString _value;
public:
    QString value()
    {
        return _value;
    }

    ScriptPropertyType()
    {
    }

    ScriptPropertyType(QString val)
    {
        _value = val;
    }
    ScriptPropertyType(const ScriptPropertyType &b)
    {
        this->_value = b._value;
    }
    ~ScriptPropertyType()
    {

    }
    QString toString() const {
        return _value;
      }
};
Q_DECLARE_METATYPE(FilePathPropertyType)
Q_DECLARE_METATYPE(NewFilePropertyType)
Q_DECLARE_METATYPE(ScriptPropertyType)

int filePathTypeId();

int newFileTypeId();

int scriptTypeId();

void registerCustomTypes();

#endif // CUSTOMTYPES_H
