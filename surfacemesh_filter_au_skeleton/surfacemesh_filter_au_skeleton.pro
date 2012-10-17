load($$[STARLAB])
load($$[EIGEN])
load($$[SURFACEMESH])
StarlabTemplate(plugin)

INCLUDEPATH += . src/umfpack_solver

HEADERS += au_skeleton.h \
    src/VertexRecord.h \
    src/SkeletonExtract.h
SOURCES += au_skeleton.cpp \
    src/SkeletonExtract.cpp \
    src/Skeleton.cpp \
    src/PriorityQueue.cpp
 
