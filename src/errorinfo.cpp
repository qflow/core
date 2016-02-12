#include "errorinfo.h"

namespace QFlow{

ErrorInfo::ErrorInfo() : _errorText(QString()), _errorCode(0)
{

}
ErrorInfo::ErrorInfo(QString errText, int errCode) : _errorText(errText), _errorCode(errCode)
{

}

ErrorInfo::ErrorInfo(const ErrorInfo& b)
{
    _errorText = b._errorText;
    _errorCode = b._errorCode;
}
ErrorInfo::~ErrorInfo()
{

}

QString ErrorInfo::errorText() const
{
    return _errorText;
}
int ErrorInfo::errorCode()
{
    return _errorCode;
}
void ErrorInfo::setErrorText(QString str)
{
    _errorText = str;
}
void ErrorInfo::setErrorCode(int code)
{
    _errorCode = code;
}
void ErrorInfo::appendErrorText(QString str)
{
    _errorText += str;
}
ErrorInfo::operator int()
{
    return _errorCode;
}

ErrorInfo::operator bool()
{
    if(_errorCode < 0) return true;
    return false;
}
QJsonValue ErrorInfo::toJson() const
{
    QJsonObject obj;
    obj.insert("errorCode", _errorCode);
    obj.insert("errorText", _errorText);
    return obj;
}
ErrorInfo ErrorInfo::fromJson(const QJsonValue& val)
{
    ErrorInfo info;
    QJsonObject obj = val.toObject();
    info.setErrorCode(obj["errorCode"].toInt());
    info.setErrorText(obj["errorText"].toString());
    return info;
}

int ErrorInfo::toInt() const
{
    return _errorCode;
}
ErrorInfo ErrorInfo::fromInt(int val)
{
    return ErrorInfo(QString(), val);
}
}
