# qremoteobject
Remote object library that utilizes the built-in signal/slot structure of Qt

# The WHAT
Helps to generate applications that stay synchronized over wire (anything that can send QByteArrays)
![Jagwire Example](/docs/jagwire-example.gif)

# The HOW
The QRemoteObject library provides methods, slots and signals that are inserted by the meta object compiler at build time.
![QRemoteObject HOW](/docs/QRemoteObject(How).png)

# The WHY
The QRemoteObject library looks to provide a method of defining remote objects (similar to QtRemoteObjects) but does not require a separate definition file (REPC .rep). The library utilizes QByteArray signals and slots, making it very easy to plug directly into applications using other QIODevice subclasses.
![QRemoteObject HOW](/docs/QRemoteObject(Why).png)

# Must have a valid meta object compiler (moc)  modified for QRemoteObjects
   - Not a hard dependency for building library or examples, but the examples will not work as intended
   - see [RMOC latest](https://github.com/Tpimp/rmoc/tree/5.12)

# To Build

 - Pull the repository
 - open either qremoteobject.qbs or one of the example's top level qbs file in Qt Creator.
 - build and (run if example) as usual
