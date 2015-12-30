QT      += core gui \
           xml \
           network
HEADERS += \
    electronicsitem.h \
    neededTypes.h \
    mainwindow.h \
    connection.h \
    electronicsview.h \
    elinitdialog.h \
    electronicsxmlparser.h \
    elserverconnection.h \
    electronicsitemtoolbutton.h

SOURCES += \
    electronicsitem.cpp \
    main.cpp \
    mainwindow.cpp \
    connection.cpp \
    electronicsview.cpp \
    elinitdialog.cpp \
    electronicsxmlparser.cpp \
    elserverconnection.cpp \
    electronicsitemtoolbutton.cpp \
    auxiliaryFunctions.cpp

RESOURCES += \
    images.qrc
