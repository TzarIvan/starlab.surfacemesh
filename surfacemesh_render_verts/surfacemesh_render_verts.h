#pragma once
#include "SurfaceMeshPlugins.h"
class surfacemesh_render_verts : public SurfaceMeshRenderPlugin{
    Q_OBJECT
    Q_INTERFACES(RenderPlugin)
  
public: 
    RenderPlugin* factory(){ return new surfacemesh_render_verts(); }
    QString name() { return "Vertices as Dots"; }
    QIcon icon(){ return QIcon(":/surfacemesh_render_verts.png"); }
    void render();
};
