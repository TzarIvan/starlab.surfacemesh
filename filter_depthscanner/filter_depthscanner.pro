load($$[STARLAB])
load($$[SURFACEMESH])
load($$[OCTREE])
load($$[EIGEN])
StarlabTemplate(plugin)

HEADERS += SurfaceMesh/DepthTriangulator.h

HEADERS += filter_depthscan.h
SOURCES += filter_depthscan.cpp
