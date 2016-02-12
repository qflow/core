#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <QQmlExtensionPlugin>

namespace QFlow{

class CorePlugin: public QQmlExtensionPlugin
{
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "CorePlugin" FILE "core_metadata.json")
public:
        void registerTypes(const char *uri);
};
}
#endif // COREPLUGIN_H
