#ifndef ERRORINFO_H
#define ERRORINFO_H

#include "coreplugin_global.h"
#include <QString>
#include <QMetaType>
#include <QObject>
#include <QDebug>
#include <QJsonObject>

namespace QFlow{

class COREPLUGIN_EXPORT ErrorInfo
{
    Q_GADGET
    Q_PROPERTY(QString errorText READ errorText WRITE setErrorText)
    Q_PROPERTY(int errorCode READ errorCode WRITE setErrorCode)
private:
    QString _errorText;
    int _errorCode;
public:
    ErrorInfo();
    ErrorInfo(QString errText, int errCode);
    ErrorInfo(const ErrorInfo& b);
    ~ErrorInfo();

    QString errorText() const;
    int errorCode();
    void setErrorText(QString str);
    void setErrorCode(int code);
    void appendErrorText(QString str);
    operator int();

    operator bool();
    QJsonValue toJson() const;
    static ErrorInfo fromJson(const QJsonValue& val);

    int toInt() const;
    static ErrorInfo fromInt(int val);
};
}

Q_DECLARE_METATYPE(QFlow::ErrorInfo)

QDebug operator<<(QDebug dbg, const QFlow::ErrorInfo &info);
#endif // ERRORINFO_H
