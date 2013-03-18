#include <QtOpenGL>
#include "Plugin.h"
#include "SurfaceMeshPlugins.h"

using namespace SurfaceMesh;

class PointCloudRenderer : public SurfaceMeshRenderer{
    double splat_size;
    bool splat_disks;
    bool lights_on;
    Vector3VertexProperty points;
    Vector3VertexProperty normals;
    
    void initParameters(){
        parameters()->addParam( new RichFloat("splat_size", 3, "Splat size") );
        parameters()->addParam( new RichBool("splat_disks", false, "Splat with disks") );
        parameters()->addParam( new RichBool("lights_on", true, "Enable illumination") );
    }
    
    void init(){
        points = mesh()->vertex_coordinates();
        normals = mesh()->vertex_normals();
        splat_size = parameters()->getFloat("splat_size");
        splat_disks = parameters()->getBool("splat_disks");
        lights_on = parameters()->getBool("lights_on");
    }
    
    void render(){
        glPointSize(splat_size);
        
        if(!lights_on || !normals){
            glDisable(GL_LIGHTING);
                glBegin(GL_POINTS);
                    foreach(Vertex v, mesh()->vertices())
                        glVertex3dv(points[v].data());
                glEnd();
            glEnable(GL_LIGHTING);
        } else {
            glEnable(GL_LIGHTING);
            glBegin(GL_POINTS);
                foreach(Vertex v, mesh()->vertices()){
                    glNormal3dv(normals[v].data());
                    glVertex3dv(points[v].data());
                } 
            glEnd();            
        }
    }
};

Renderer* Plugin::instance(){ return new PointCloudRenderer(); }

Q_EXPORT_PLUGIN(Plugin)
