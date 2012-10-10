CONFIG += starlab 
STARLAB_TEMPLATE += plugin 
STARLAB_DEPENDS += surfacemesh_model skeleton_model
STARLAB_EXTERNAL += eigen-3.1.0-rc1

INCLUDEPATH += . src/umfpack_solver

HEADERS += au_skeleton.h \
    src/VertexRecord.h \
    src/SkeletonExtract.h
SOURCES += au_skeleton.cpp \
    src/SkeletonExtract.cpp \
    src/Skeleton.cpp \
    src/PriorityQueue.cpp
 
