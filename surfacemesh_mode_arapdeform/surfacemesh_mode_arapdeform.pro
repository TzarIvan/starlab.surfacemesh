CONFIG += starlab 
STARLAB_TEMPLATE += plugin 
STARLAB_DEPENDS += ../surfacemesh
STARLAB_EXTERNAL += cholmod eigen

HEADERS += surfacemesh_mode_arapdeform.h ARAPDeformer.h ARAPDeformerHandle.h \
    arap_globals.h \
    arap_widget.h
SOURCES += surfacemesh_mode_arapdeform.cpp ARAPDeformer.cpp \
    arap_widget.cpp
RESOURCES += surfacemesh_mode_arapdeform.qrc

FORMS += \
    arap_widget.ui
