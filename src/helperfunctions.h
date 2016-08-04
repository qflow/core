#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include "rational.h"
#include "timespan.h"
#include "range.h"
#include "errorinfo.h"
#include "fileinfo.h"
#include <QObject>
#include <QQmlEngine>

namespace QFlow{

class Timespan;

class COREPLUGIN_EXPORT HelperFunctions : public QObject
{
    Q_OBJECT
public:
    HelperFunctions();
    ~HelperFunctions();
    Q_INVOKABLE bool isError(ErrorInfo info);
    Q_INVOKABLE QString errorText(ErrorInfo info);
    Q_INVOKABLE Timespan timespan(QString value);
    Q_INVOKABLE Rational rational(int p, int q);
    Q_INVOKABLE Rational rational(QString str);
    Q_INVOKABLE IntRange intRange(int low, int high);
    Q_INVOKABLE QString localHostName();
    Q_INVOKABLE bool copy(const QString & fileName, const QString & newName);
    Q_INVOKABLE bool mkdir(const QString & dirName) const;
    Q_INVOKABLE QVariantList getFileList(QString dir, QString pattern);
    Q_INVOKABLE QVariantList getFileListRecursive(QString dir, QString pattern);
    Q_INVOKABLE FileInfo getFileInfo(QString str);
    Q_INVOKABLE QString currentDir();
    Q_INVOKABLE Timespan newTimespan(QString value);
    Q_INVOKABLE void sleepSecs(int value);
    Q_INVOKABLE void sleepMSecs(int value);
    Q_INVOKABLE QByteArray getenv(QString varname);
    Q_INVOKABLE void startExternalProgram(QString cmd);
signals:
};

QObject COREPLUGIN_EXPORT *helperfunctions_provider(QQmlEngine *engine, QJSEngine *scriptEngine);

namespace Math
{
double COREPLUGIN_EXPORT scale(double value, double min, double max, double lower, double upper);
double COREPLUGIN_EXPORT scale(double value, RealRange from, RealRange to);
}

int COREPLUGIN_EXPORT toFourCC(const QString str);
QString COREPLUGIN_EXPORT getStringFromUnsignedChar( unsigned char *str );
QString COREPLUGIN_EXPORT expand_environment_variables( QString s );
}

#endif // HELPERFUNCTIONS_H
