#include <qgl.h>
#include "surfacemesh_render_verts.h"
#include "SurfaceMeshHelper.h"
using namespace SurfaceMesh;

void surfacemesh_render_verts::render(){
    Vector3VertexProperty points = mesh()->vertex_coordinates();
    glDisable(GL_LIGHTING);
        glPointSize(3.0);
        glBegin(GL_POINTS);
            foreach(Vertex v, mesh()->vertices())
                glVertex3dv(points[v].data());
        glEnd();
    glEnable(GL_LIGHTING);
}

Q_EXPORT_PLUGIN(surfacemesh_render_verts)
