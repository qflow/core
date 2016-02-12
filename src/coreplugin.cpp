#include "coreplugin.h"
#include "qqml.h"
#include "thread.h"
#include "timestamped.h"
#include "errorinfo.h"
#include "timespan.h"
#include "rational.h"
#include "range.h"
#include "signalslotconnection.h"
#include "matrix.h"
#include "helperfunctions.h"
#include "startable.h"
#include <QtQml/qqmlprivate.h>
#include <QtQml/private/qqmlmetatype_p.h>
#include "smartconnection.h"
#include "canvas.h"
#include "scheduler.h"
#include "future.h"
#include "systeminfo.h"
#include "sortfilterproxymodel.h"

namespace QFlow{

QJsonValue sizeToJson(const QSize &val)
{
    QString str = QString("%1x%2").arg(val.width(), val.height());
    QJsonValue json(str);
    return json;
}


void CorePlugin::registerTypes(const char *uri)
{
    // @uri QFlow.Core
    qmlRegisterType<Thread>(uri, 1, 0, "Thread");
 
    qRegisterMetaType<Timestamp>("Timestamp");
    qRegisterMetaType<ErrorInfo>("ErrorInfo");
    qmlRegisterUncreatableType<ErrorInfo>(uri, 1, 0, "ErrorInfo", "ErrorInfo can not be created");
    qRegisterMetaType<Sample>("Sample");
    int idTimespan = qRegisterMetaType<Timespan>("Timespan");
    QQmlMetaType::registerCustomStringConverter(idTimespan, &Timespan::toVar);
    int idRat = qRegisterMetaType<Rational>("Rational");
    QQmlMetaType::registerCustomStringConverter(idRat, &Rational::toVar);
    int idRange = qRegisterMetaType<IntRange>("Range");
    QQmlMetaType::registerCustomStringConverter(idRange, &IntRange::toVar);
 
    qRegisterMetaType<RealMatrix>("RealMatrix");
    qRegisterMetaType<RealComplexMatrix>("RealComplexMatrix");
    qmlRegisterType<Connection>(uri, 1, 0, "Connection");
    qmlRegisterType<Canvas>(uri, 1, 0, "VirtualCanvas");
    qmlRegisterType<SignalSlotConnection>(uri, 1, 0, "SignalSlotConnection");
    qmlRegisterType<SmartConnection>(uri, 1, 0, "SmartConnection");
    qmlRegisterType<Scheduler>(uri, 1, 0, "Scheduler");
    qmlRegisterType<SortFilterProxyModel>(uri, 1, 0, "SortFilterProxyModel");
    qRegisterMetaType<WaitHandle>("WaitHandle");
    qRegisterMetaType<Future>("Future");

    QMetaType::registerConverter<Timespan, QString>(&Timespan::toString);
    QMetaType::registerConverter<QString, Timespan>(&Timespan::fromString);
    QMetaType::registerConverter<Timespan, QJsonValue>(&Timespan::toJson);
    QMetaType::registerConverter<QJsonValue, Timespan>(&Timespan::fromJson);

    QMetaType::registerConverter<Rational, QString>(&Rational::toString);
    QMetaType::registerConverter<QString, Rational>(&Rational::fromString);
    QMetaType::registerConverter<Rational, QJsonValue>(&Rational::toJson);
    QMetaType::registerConverter<QJsonValue, Rational>(&Rational::fromJson);
    QMetaType::registerConverter<IntRange, QString>(&IntRange::toString);
    QMetaType::registerConverter<QString, IntRange>(&IntRange::fromString);
    QMetaType::registerConverter<RealRange, QString>(&RealRange::toString);
    QMetaType::registerConverter<QString, RealRange>(&RealRange::fromString);

    QMetaType::registerConverter<ErrorInfo, QJsonValue>(&ErrorInfo::toJson);
    QMetaType::registerConverter<QJsonValue, ErrorInfo>(&ErrorInfo::fromJson);
    QMetaType::registerConverter<ErrorInfo, int>(&ErrorInfo::toInt);
    QMetaType::registerConverter<int, ErrorInfo>(&ErrorInfo::fromInt);


    qmlRegisterUncreatableType<Rational>(uri, 1, 0, "Rational", "Rational cannot be created");
    qmlRegisterUncreatableType<Timespan>(uri, 1, 0, "Timespan", "Timespan cannot be created");
    qmlRegisterUncreatableType<Future>(uri, 1, 0, "Future", "Future cannot be created");
    qmlRegisterSingletonType<HelperFunctions>(uri, 1, 0, "Helper", helperfunctions_provider);

    qmlRegisterType<SystemInfo>(uri, 1, 0, "SystemInfo");

}
}
