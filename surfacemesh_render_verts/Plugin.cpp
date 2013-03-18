#include <QtOpenGL>
#include "Plugin.h"
#include "SurfaceMeshPlugins.h"
#include "Eigen/Dense"

using namespace SurfaceMesh;

// GLU was removed from Qt in version 4.8 
#ifdef Q_OS_MAC
# include <OpenGL/glu.h>
#else
# include <GL/glu.h>
#endif

class PointCloudRenderer : public SurfaceMeshRenderer{
    double gl_point_size;
    bool use_splats;
    double splat_size;
    bool lights_on;
    Vector3VertexProperty points;
    Vector3VertexProperty normals;
    
    void initParameters(){
        parameters()->addParam( new RichBool("lights_on", true, "Enable illumination") );
        parameters()->addParam( new RichFloat("gl_point_size", 3, "GLPoint size") );
        parameters()->addParam( new RichBool("use_splats", false, "Use disk-splats") );
        parameters()->addParam( new RichFloat("splat_size", .015, "Disk splats size") );
    }
    
    void init(){
        points = mesh()->vertex_coordinates();
        normals = mesh()->vertex_normals();
        gl_point_size = parameters()->getFloat("gl_point_size");
        use_splats = parameters()->getBool("use_splats");
        lights_on = parameters()->getBool("lights_on");
        splat_size = parameters()->getFloat("splat_size");
    }
    
    void render(){
        GLUquadricObj *q = gluNewQuadric();
//		gluQuadricNormals (q,GLU_TRUE);
//		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, surfelcolor);
//		if( drawbackpoint == 0 )
//			glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, diskback);
//		else if( drawbackpoint == 1 )
//			glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, surfelcolor);
        
        /// Constants
        Eigen::Vector3d zaxis(0.0,0.0,1.0);
        const double PI = std::acos(-1.0);
        
        glPointSize(gl_point_size);
        
        /// If you don't have normals, just show points
        if(!lights_on || !normals){
            glDisable(GL_LIGHTING);
                glBegin(GL_POINTS);
                    foreach(Vertex v, mesh()->vertices())
                        glVertex3dv(points[v].data());
                glEnd();
            glEnable(GL_LIGHTING);
        } 
        
        /// If you do, you can either shade points, or use oriented splats
        else {
            glEnable(GL_LIGHTING);           
            if(use_splats){
                foreach(Vertex v, mesh()->vertices()){
                    Eigen::Vector3d n = normals[v];
                    glPushMatrix();
                        double theta = acos( zaxis.dot(n) ) * 180 / PI;
                        Eigen::Vector3d crossv = zaxis.cross(n);
                        glTranslatef( points[v].x(), points[v].y(), points[v].z() );
                        glRotatef( theta, crossv[0], crossv[1], crossv[2] );
                        gluDisk( q, 0, splat_size/2.0, 20, 1 );
                    glPopMatrix();            
                }
            } else {
                glBegin(GL_POINTS);
                    foreach(Vertex v, mesh()->vertices()){
                        glNormal3dv(normals[v].data());
                        glVertex3dv(points[v].data());
                    } 
                glEnd();
            }
        }
    }
};

Renderer* Plugin::instance(){ return new PointCloudRenderer(); }

Q_EXPORT_PLUGIN(Plugin)
