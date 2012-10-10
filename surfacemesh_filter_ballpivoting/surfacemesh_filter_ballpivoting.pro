CONFIG += starlab 
STARLAB_TEMPLATE += plugin 
STARLAB_DEPENDS += ../surfacemesh
STARLAB_EXTERNAL += nanoflann # for fast KD-tree

HEADERS += surfacemesh_filter_ballpivoting.h
SOURCES += surfacemesh_filter_ballpivoting.cpp
