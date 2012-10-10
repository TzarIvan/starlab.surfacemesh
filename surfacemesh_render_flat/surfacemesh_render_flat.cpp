#include <qgl.h>
#include "surfacemesh_render_flat.h"
#include "surface_mesh/gl_wrappers.h"

void surfacemesh_render_wireframe::init(){
    // qDebug() << "surfacemesh_render_flat::init";
    mesh()->update_face_normals();

#ifdef USE_BUFFERS
    /// Initialize triangle buffer
    triangles.clear();
    Surface_mesh::Face_iterator fit, fend=mesh()->faces_end();
    Surface_mesh::Vertex_around_face_circulator fvit, fvend;
    Surface_mesh::Vertex v0, v1, v2;
    for (fit=mesh()->faces_begin(); fit!=fend; ++fit){
        fvit = fvend = mesh()->vertices(fit);
        v0 = fvit;
        v2 = ++fvit;
        do 
        {
            v1 = v2;
            v2 = fvit;
            triangles.push_back(v0.idx());
            triangles.push_back(v1.idx());
            triangles.push_back(v2.idx());
        } 
        while (++fvit != fvend);
    }
#endif
}

void surfacemesh_render_wireframe::render(){
    Surface_mesh::Vertex_property<Point> points   = mesh()->vertex_property<Point>("v:point");
    Surface_mesh::Face_property<Point>   fnormals = mesh()->face_property<Point>("f:normal");
    Surface_mesh::Face_property<Color>   fcolor   = mesh()->get_face_property<Color>("f:color");
    bool has_face_color                           = mesh()->has_face_property<Color>("f:color");

    // setup vertex arrays
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_FLAT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthRange(0.01, 1.0); ///< @todo what is this for?

    /// Set the color
    glColor4d(mesh()->color.redF(), mesh()->color.greenF(), mesh()->color.blueF(), mesh()->color.alphaF());
    
    /// @todo convert to "foreach"
    Surface_mesh::Face_iterator fit, fend=mesh()->faces_end();
    Surface_mesh::Vertex_around_face_circulator fvit, fvend;
    for (fit=mesh()->faces_begin(); fit!=fend; ++fit){
        // this will be slow, but we don't want to distinguish between
        // triangles, quads, and general polygons
        // immediate mode rendering is slow anyway...
        glBegin(GL_POLYGON);
            if(has_face_color) gl::glColor(fcolor[fit]);
            gl::glNormal(fnormals[fit]);
            fvit = fvend = mesh()->vertices(fit);
            do
            {
                gl::glVertex(points[fvit]);
            }
            while (++fvit != fvend);
        glEnd();
    }
}

Q_EXPORT_PLUGIN(surfacemesh_render_wireframe)
