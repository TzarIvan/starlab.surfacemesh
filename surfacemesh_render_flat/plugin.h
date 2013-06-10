#pragma once
#include "SurfaceMeshPlugins.h"

class surfacemesh_render_wireframe : public SurfaceMeshRenderPlugin{
    Q_OBJECT
    Q_INTERFACES(RenderPlugin)

    QString name() { return "Flat Shading"; }
    QIcon icon(){ return QIcon(":/icons/flat_shading.png"); }
    Renderer* instance();
};
