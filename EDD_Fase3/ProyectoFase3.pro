QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arbolavlfunciones.cpp \
    arbolbclientes.cpp \
    arbolbinariopeliculas.cpp \
    grafosedes.cpp \
    funcion.cpp \
    listabeneficios.cpp \
    listapromociones.cpp \
    matrizdisperza.cpp \
    tablahashreservas.cpp \
    login.cpp \
    vistaadmin.cpp \
    vistacliente.cpp \
    Pelicula.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    arbolavlfunciones.h \
    arbolbclientes.h \
    arbolbinariopeliculas.h \
    cliente.h \
    datoscine.h \
    funcion.h \
    listabeneficios.h \
    listapromociones.h \
    matrizdisperza.h \
    reserva.h \
    tablahashreservas.h \
    login.h \
    vistaadmin.h \
    vistacliente.h \
    Pelicula.h \
    json.hpp \
    grafosedes.h \
    mainwindow.h

FORMS += \
    login.ui \
    vistaadmin.ui \
    vistacliente.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target