load($$[STARLAB])
load($$[SURFACEMESH])
load($$[CHOLMOD])
load($$[EIGEN])
StarlabTemplate(plugin)

HEADERS += surfacemesh_filter_geoheat.h
SOURCES += surfacemesh_filter_geoheat.cpp

HEADERS += GeoHeatHelper.h
HEADERS += GeoDrawObjects.h
