#pragma once
#include "SurfaceMeshPlugins.h"

class surfacemesh_render_wireframe : public SurfaceMeshRenderPlugin{
    Q_OBJECT
    Q_INTERFACES(RenderPlugin)

private:
    ///< index array for buffered OpenGL rendering
    std::vector<unsigned int> triangles; 
    
public: 
    RenderPlugin* factory(){ return new surfacemesh_render_wireframe(); }
    QString name() { return "Flat Shading"; }
    QIcon icon(){ return QIcon(":/surfacemesh_render_flat.png"); }
    void init();
    void render();
};
