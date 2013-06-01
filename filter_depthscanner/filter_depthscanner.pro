include($$[STARLAB])
include($$[OCTREE])
include($$[SURFACEMESH])
StarlabTemplate(plugin)

message($$INCLUDEPATH)

HEADERS += SurfaceMesh/DepthTriangulator.h

HEADERS += filter_depthscan.h
SOURCES += filter_depthscan.cpp
