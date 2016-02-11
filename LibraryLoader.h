/*
 * LibraryLoader.h
 *
 *  Created on: Nov 14, 2010
 *      Author: michal
 */

#ifndef LIBRARYLOADER_H_
#define LIBRARYLOADER_H_

#define PLUGINPATHS "./plugins;../plugins"


#include <qobject.h>
#include <QString>
#include <QList>
#include "PluginInterface.h"
#include <QResource>
#include "errorinfo.h"

class LibraryInfo
{
public:
    LibraryInfo();
    QString Name;
    QList<const QMetaObject*>* Modules;
    PluginInterface* Plugin;
    QString Description;
};

class LibraryLoader
{
public:
    LibraryLoader();
    ErrorInfo loadLibraries(QList<LibraryInfo*>* libraries);
};

#endif /* LIBRARYLOADER_H_ */
