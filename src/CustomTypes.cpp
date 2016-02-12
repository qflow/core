#include "CustomTypes.h"

void registerCustomTypes()
{
    qRegisterMetaType<FilePathPropertyType>("FilePathPropertyType");
    qRegisterMetaType<NewFilePropertyType>("NewFilePropertyType");
    qRegisterMetaType<ScriptPropertyType>("ScriptPropertyType");
}

int filePathTypeId()
{
    return qMetaTypeId<FilePathPropertyType>();
}

int newFileTypeId()
{
    return qMetaTypeId<NewFilePropertyType>();
}

int scriptTypeId()
{
    return qMetaTypeId<ScriptPropertyType>();
}
