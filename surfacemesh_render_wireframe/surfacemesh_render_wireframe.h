#pragma once
#include "SurfaceMeshPlugins.h"

class surfacemesh_render_wireframe : public SurfaceMeshRenderPlugin{
    Q_OBJECT
    Q_INTERFACES(RenderPlugin)

#ifdef TODO_WIREFRAME_VBUFFER
private:
    /// index array for buffered OpenGL rendering
    std::vector<unsigned int>  edges;
#endif 
    
public: 
    RenderPlugin* factory(){ return new surfacemesh_render_wireframe(); }
    QString name() { return "Wireframe"; }
    QIcon icon(){ return QIcon(":/surfacemesh_render_wireframe.png"); }
    void init();
    void render();
};
