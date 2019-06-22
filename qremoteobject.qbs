import qbs
import qbs.File
Product{
    name:"qremoteobject"
    id:qremoteObject
    type:"staticlibrary"
    property string libName:
        qremoteObject.type == "dynamiclibrary"?
            (qbs.targetOS.contains("windows") ? "":"lib") + name + (qbs.targetOS.contains("windows") ? ".dll":".so"):
            (qbs.targetOS.contains("windows") ? "":"lib") + name + (qbs.targetOS.contains("windows") ? ".lib":".a")

    Depends{name:'cpp'}
    cpp.cxxLanguageVersion: "c++11"
    Depends{name:  "Qt"; submodules:["core","core-private"]}

    Export{
        Depends {name: "cpp"}
        cpp.cxxLanguageVersion: "c++11"
//        Depends{name:  "Qt"; submodules:["core","core-private"]}
        cpp.includePaths:[qbs.installRoot +installPathExt +  "/include/qtremoteobject"];
        cpp.libraryPaths:[qbs.installRoot +installPathExt +  "/lib" ];
        cpp.dynamicLibraries:[qbs.installRoot + installPathExt + "/lib/" + qremoteObject.libName ]
    }
    cpp.defines:['QREMOTEOBJECT_LIBRARY']

    Group {
        qbs.install: true
        fileTagsFilter: qremoteObject.type
        qbs.installDir: "/lib"
    }
    Group  {
        name: "headers"
        files: [
            "src/qobject_serial.h",
            "src/qremoteobject.h",
            "src/qremoteservice.h",
        ]
        qbs.installDir: "/include/qtremoteobject/"
        qbs.install: true
    }
    Group  {
        name: "private"
        files: [
            "src/private/qremoteobject_p.h",
            "src/private/qremoteservice_p.h",
            "src/private/qremoteobject_p.cpp",
        ]
        qbs.installDir:"/include/qtremoteobject/private"
        qbs.install: true
    }
    Group{
        name:"source"
        files:[
            "src/qobject_serial.cpp",
            "src/qremoteobject.cpp",
            "src/qremoteservice.cpp",
        ]
        qbs.install:false
    }

}
