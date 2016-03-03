import qbs

QmlPlugin{
    files: [
        "coreplugin.h",
        "coreplugin.cpp",
        "binarybuffer.cpp",
        "binarybuffer.h",
        "canvas.cpp",
        "canvas.h",
        "notimplementedexception.cpp",
        "notimplementedexception.h",
        "radixtreenode.cpp",
        "radixtreenode.h",
        "random.cpp",
        "random.h",
        "signalobserver.cpp",
        "signalobserver.h",
        "sortfilterproxymodel.cpp",
        "sortfilterproxymodel.h",
        "systeminfo.cpp",
        "systeminfo.h",
        "treeitem.cpp",
        "treeitem.h",
        "blockingqueue.h",
        "blockingqueue.cpp",
        "future.h",
        "future.cpp",
        "jsonhelper.cpp",
        "helperfunctions.h",
        "jsonhelper.h",
        "functor.h",
        "helperfunctions.cpp",
        "timespan.cpp",
        "timespan.h",
        "errorinfo.h",
        "errorinfo.cpp",
        "rational.h",
        "rational.cpp",

    ]
    name: "core"
    Group
    {
        name: "headers"
        files:[
            "Sync.h",
            "thread.h",
            "Dynamic.h",
            "peer.h",
            "CustomTypes.h",
            "timeutils.h",
            "initiable.h",
            "blocking.h",
            "waveform.h",
            "timestamped.h",
            "circullarbuffer.h",
            "connection.h",
            "acceptsconnection.h",
            "gaussmodel.h",
            "startable.h",
            "matrix.h",
            "matrix_p.h",
            "signalslotconnection.h",
            "complex.h",
            "range.h",
            "coreplugin_global.h",
            "smartconnection.h",
            "connectionattached.h",
            "scheduler.h",
            "symbols.h",
        ]
    }
    Group
    {
        name: "sources"
        files:[
            "peer.cpp",
            "CustomTypes.cpp",
            "initiable.cpp",
            "connection.cpp",
            "acceptsconnection.cpp",
            "timestamped.cpp",
            "gaussmodel.cpp",
            "waveform.cpp",
            "matrix.cpp",
            "signalslotconnection.cpp",
            "complex.cpp",
            "range.cpp",
            "thread.cpp",
            "Dynamic.cpp",
            "Sync.cpp",
            "smartconnection.cpp",
            "connectionattached.cpp",
            "scheduler.cpp",
        ]
    }
    pluginNamespace: "QFlow.Core"
    pluginRootPath: "bin/plugins"
    Group
    {
        name: "platform"
        qbs.install: true
        qbs.installDir: "bin/platforms"
        files:
        {
            var files = [];
            if(qbs.targetOS.contains("windows"))
                files.push(Qt.core.pluginPath + "/platforms/qwindows" + Qt.core.libInfix + cpp.dynamicLibrarySuffix);
            if(qbs.targetOS.contains("linux"))
            {
                files.push(Qt.core.pluginPath + "/platforms/libqxcb" + cpp.dynamicLibrarySuffix);
                files.push(Qt.core.pluginPath + "/platforms/libqminimal" + cpp.dynamicLibrarySuffix);
            }
            return files;
        }

    }
    Group
    {
        name: "libraries"
        qbs.install: true
        qbs.installDir: "bin"
        files:
        {
            var files = [];
            if(qbs.targetOS == "windows")
            {
                files = [Qt.core.binPath + "/icu*.dll",
                         Qt.core.binPath + "/Qt5Core" + infix + cpp.dynamicLibrarySuffix,
                         Qt.qml.binPath + "/Qt5" + Qt.qml.qtModuleName + infix + cpp.dynamicLibrarySuffix
                        ];
                if(qbs.architecture == "x86") files.push(cpp.toolchainInstallPath + "/../redist/x86/Microsoft.VC140.CRT/msvcp140.dll");
                if(qbs.architecture == "x86_64") files.push(cpp.toolchainInstallPath + "/../../redist/x64/Microsoft.VC140.CRT/msvcp140.dll");
            }
            if(qbs.targetOS.contains("linux"))
            {
                files = [Qt.core.libPath + "/libicui*",
                         Qt.core.libPath + "/libicuuc*",
                         Qt.core.libPath + "/libicudata*",
                         Qt.core.libPath + "/libQt5Core" + cpp.dynamicLibrarySuffix + "*",
                         Qt.qml.libPath + "/libQt5" + Qt.qml.qtModuleName + Qt.core.libInfix + cpp.dynamicLibrarySuffix + "*"
                         ];
            }
            return files;
        }
        fileTags: "libraries"
    }
    /*Group
    {
        name: "QtQuick"
        qbs.installDir: "bin/plugins"
        qbs.install: true
        files: [Qt.core.binPath + "/../qml/QtQuick"]
        //qbs.installSourceBase: "/../qml/QtQuick"
    }
    Group {
        name: "QtQuick.2"
        files: [Qt.core.binPath + "/../qml/QtQuick.2/*"]
        qbs.install: true
        qbs.installDir: "bin/plugins/QtQuick.2"
        //qbs.installSourceBase: "QtQuick.2"
    }*/
    cpp.dynamicLibraries:
    {
        var libs = [];
        if(qbs.targetOS.contains("windows"))
        {
            libs.push("Pdh");
        }

        return libs;
    }

    Depends{name: "Qt"; submodules: ["qml-private"]}
}
