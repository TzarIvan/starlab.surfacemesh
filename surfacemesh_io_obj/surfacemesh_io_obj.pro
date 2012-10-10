CONFIG += starlab 
STARLAB_TEMPLATE = plugin 
STARLAB_DEPENDS += ../surfacemesh

HEADERS += surfacemesh_io_obj.h
SOURCES += surfacemesh_io_obj.cpp

# Windows warnings
win32: DEFINES += _CRT_SECURE_NO_WARNINGS
