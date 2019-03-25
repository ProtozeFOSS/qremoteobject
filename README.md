# qremoteobject
Remote object library that utilizes the built-in signal/slot structure of Qt

# The WHAT
Helps to generate applications that stay synchronized over wire (anything that can send/receive QByteArrays)
![Jagwire Example](/docs/jagwire-example.gif)

In this example, each keyfob is a separate process. A QWebsocket server and websockets are used for convenience. 

# The HOW
The QRemoteObject library provides methods, slots and signals that are inserted by the meta object compiler at build time.
![QRemoteObject HOW](/docs/QRemoteObject(How).png)

# The WHY
The QRemoteObject library looks to provide a method of defining remote objects (similar to QtRemoteObjects) but does not require a separate definition file [REPC .rep](https://doc.qt.io/qt-5.11/qtremoteobjects-repc.html). The library utilizes QByteArray signals and slots, making it very easy to plug directly into applications using other QIODevice subclasses.
![QRemoteObject HOW](/docs/QRemoteObject(Why).png)

# Must have a valid meta object compiler (moc)  modified for QRemoteObjects
   - Read more on [QMetaMethod 'tags'](https://doc.qt.io/qt-5/qmetamethod.html#tag)
   - Not a hard dependency for building library or examples, but the examples will not work as intended
   - see [RMOC latest](https://github.com/Tpimp/rmoc/tree/5.12)

# To Build
 - Edit your qobjectdef.h (or make a copy of the original and edit that one) as seen here [RemoteInvokeMetaMethod](https://github.com/ProtozeFOSS/qremoteobject/wiki/Missing-reference-to-RemoteInvokeMetaMethod)
 - Pull the repository
 - open either qremoteobject.qbs or one of the example's top level qbs file in Qt Creator.
 - build and (run if example) as usual
