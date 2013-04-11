#include "Plugin.h"
Q_EXPORT_PLUGIN(Plugin)

#include <QtOpenGL>
#include "SurfaceMeshPlugins.h"
#include "Eigen/Dense"
#include "StarlabDrawArea.h"

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
    bool double_side;
    Vector3VertexProperty points;
    Vector3VertexProperty normals;
    
    void initParameters(){
        parameters()->addParam( new RichBool("lights_on", true, "Enable illumination") );
        parameters()->addParam( new RichBool("double_side", true, "Enable double side illumination") );
        parameters()->addParam( new RichFloat("gl_point_size", 3, "GLPoint size") );
        parameters()->addParam( new RichBool("use_splats", false, "Use disk-splats") );
        parameters()->addParam( new RichFloat("splat_size", 0.015f, "Disk splats size") );
    }
    
    void init(){
        points = mesh()->vertex_coordinates();
        normals = mesh()->vertex_normals();
        gl_point_size = parameters()->getFloat("gl_point_size");
        use_splats = parameters()->getBool("use_splats");
        lights_on = parameters()->getBool("lights_on");
        double_side = parameters()->getBool("double_side");
        splat_size = parameters()->getFloat("splat_size");
    }
    
    void render(){
        qglviewer::Vec cp = plugin()->drawArea()->camera()->position();
        Vector3 camera_position( cp.x, cp.y, cp.z );
        
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
            glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, double_side);
            if(use_splats){
                foreach(Vertex v, mesh()->vertices()){
                    Eigen::Vector3d n = normals[v];
                    glPushMatrix();
                        double theta = acos( zaxis.dot(n) ) * 180 / PI;
                        glTranslatef( points[v].x(), points[v].y(), points[v].z() );
                        if(theta > 1e-8 || theta < -1e-8) {
                            Eigen::Vector3d crossv = (zaxis.cross(n)).normalized();
                            glRotatef( theta, crossv[0], crossv[1], crossv[2] );
                        }
                        gluDisk( q, 0, splat_size/2.0, 20, 1 );
                    glPopMatrix();            
                }
            } else {
                glBegin(GL_POINTS);
                    Vector3 normal;
                    foreach(Vertex v, mesh()->vertices()){
                        normal = normals[v];
                        if(double_side && dot(points[v]-camera_position,normals[v])>0)
                            normal = -normal;
                        glNormal3dv(normal);
                        glVertex3dv(points[v].data());
                    } 
                glEnd();
            }
        }
    }
};

Renderer* Plugin::instance(){ return new PointCloudRenderer(); }

