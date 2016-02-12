/*
 * LibraryLoader.cpp
 *
 *  Created on: Nov 14, 2010
 *      Author: michal
 */

#include "LibraryLoader.h"
#include <QSettings>
#include <QPluginLoader>
#include <QDir>
#include "CustomTypes.h"
#include <QCoreApplication>
#include <QJsonObject>

LibraryLoader::LibraryLoader() {
	// TODO Auto-generated constructor stub

}

LibraryInfo::LibraryInfo() : Name(QString::null), Modules(NULL)
{

}

ErrorInfo LibraryLoader::loadLibraries(QList<LibraryInfo*>* libraries)
{
    ErrorInfo err;
    registerCustomTypes();
    QCoreApplication::setOrganizationName("Michal");
    QCoreApplication::setOrganizationDomain("michal.com");
    QCoreApplication::setApplicationName("Dataflow");
    QSettings settings;
    if(!settings.contains("PluginPaths"))
    {
        settings.setValue("PluginPaths",PLUGINPATHS);
    }
    QString pathsStr = settings.value("PluginPaths").toString();

    QStringList paths = pathsStr.split(';', QString::SkipEmptyParts);
    for (int i = 0; i < paths.size(); ++i)
    {
        QString path = paths.at(i);
        QDir pluginsDir(path);
        QStringList filters;
#ifdef Q_OS_UNIX
        filters << "*.so";
#endif
#ifdef Q_OS_WIN
        filters << "*.dll";
#endif
        QStringList fileNames = pluginsDir.entryList(filters);
        foreach (QString fileName, fileNames)
        {
            QPluginLoader loader;
            loader.setLoadHints(QLibrary::ResolveAllSymbolsHint | QLibrary::ExportExternalSymbolsHint);
            QString absPath = pluginsDir.absoluteFilePath(fileName);
            loader.setFileName(absPath);
            loader.load();
            QObject *plugin = loader.instance();
            if(loader.isLoaded())
            {
                PluginInterface* pluginInt = qobject_cast<PluginInterface*>(plugin);
                LibraryInfo* info = new LibraryInfo();
                QJsonObject metadata = loader.metaData().value("MetaData").toObject();
                info->Description = metadata["description"].toString();
                info->Name = pluginInt->getLibrayName();
                info->Modules = pluginInt->getModules();
                info->Plugin = pluginInt;
                QString ver = metadata["version"].toString();
                libraries->push_back(info);
            }
            else
            {
                QString errStr = loader.errorString();
                err.appendErrorText("Failed to load plugin " + absPath +
                                    " : " + errStr + "\n");
                err.setErrorCode(-1);
            }
        }
    }
    return err;
}
