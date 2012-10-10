CONFIG += starlab
STARLAB_TEMPLATE += plugin
STARLAB_DEPENDS += ../surfacemesh
STARLAB_EXTERNAL += cholmod eigen

HEADERS += surfacemesh_filter_geoheat.h
SOURCES += surfacemesh_filter_geoheat.cpp

HEADERS += GeoHeatHelper.h
HEADERS += GeoDrawObjects.h
