#include "filter_depthscan.h"
Q_EXPORT_PLUGIN(filter_depthscan)

#include <QtOpenGL>
#include <qgl.h>
#include "SurfaceMeshModel.h"
#include "StarlabDrawArea.h"
#include "SurfaceMeshNormalsHelper.h"
#include "Octree.h"

using namespace SurfaceMesh;

void filter_depthscan::initParameters(RichParameterSet* pars){
    pars->addParam( new RichInt("density", 5, "A ray every K-pixels", "How many rays to shoot? (clamped [1,inf])") );
    pars->addParam( new RichFloat("maxangle", 80, "Grazing TH < ", "Discard when above certain grazing angle (degrees)") );
    pars->addParam( new RichBool("getnormal", true, "Sample Normals", "Store normals by reading them from the mesh") );
    // pars->addParam( new RichFloat("saveviewdir", 80, "Sample Viewpoint", "Store view directions") );
}

void filter_depthscan::applyFilter(RichParameterSet* pars){
    typedef qglviewer::Vec QGLVec;
    /// Window size
    int w = drawArea()->width();
    int h = drawArea()->height();
   
    /// Create query octree
    SurfaceMeshModel* selection = SurfaceMesh::safe_cast( model() );
    Octree octree( selection );

    if(selection->n_faces()==0)
        throw StarlabException("Cannot scan a point cloud, need n_faces()>0");
    
    /// Need face normals for angle rejection
    if(!selection->has_face_normals())
        SurfaceMesh::NormalsHelper(selection).compute_face_normals();
    Vector3FaceProperty fnormals = selection->face_normals();
    
    /// Create a model to store scans & add it
    SurfaceMeshModel* scan = new SurfaceMeshModel("","Scan");
    document()->addModel(scan);   
    drawArea()->setRenderer(scan,"Vertices as Dots");
    scan->color = Qt::red;

    /// assert( step>1 )
    int step = qMax( pars->getInt("density"),1 );
    
    /// angle: [0,90]
    double angle = qBound(0.0, (double) pars->getFloat("maxangle"), 90.0);
    double mincosangle = cos( angle * M_PI / 180 );
    
    /// Fetch normals?
    bool getNormals = pars->getBool("getnormal");
    Vector3VertexProperty vnormals = scan->vertex_normals(getNormals);
    
    /// Perform scan
    for(int winX=0; winX<w; winX+=step){
        for(int winY=0; winY<h; winY+=step){
            QGLVec _orig, _dir;
            drawArea()->camera()->convertClickToLine( QPoint(winX, winY), _orig, _dir );
            Vector3 orig(_orig[0],_orig[1],_orig[2]);
            Vector3 dir(_dir[0],_dir[1],_dir[2]);
            int isectHit = -1;
            Vec3d ipoint = octree.closestIntersectionPoint( Ray(orig, dir), &isectHit );
            
            /// ------------- THESE ARE RENDERED ---------------
            // drawArea()->drawRay(orig, dir, 1, Qt::red, 3);
            /// ------------- THESE ARE RENDERED ---------------
            
            if(isectHit>=0){
                Vector3 fnormal = fnormals[ Face(isectHit) ];
                double cosangle = dot( fnormal, -dir );
                if(cosangle>mincosangle){
                    Vertex v = scan->add_vertex( ipoint );     
                    qDebug() << v.idx();
                    if(getNormals){
                        vnormals[v] = fnormal;
                        qDebug() << vnormals[v];
                        // drawArea()->drawRay(ipoint, fnormal,1,Qt::red,.1);
                    }
                }
            }
        }
    }
    
    /// Inform user
    showMessage("Scanned #P=%d points", scan->n_vertices());
}




/// Depth map based scan.. even by setting the depth map to 32 bits precision, 
/// we get shitty results..
#if 0
void filter_depthscan::applyFilter(RichParameterSet* /*pars*/){
    glEnable(GL_DEPTH_TEST);

    /// Window size
    int w = drawArea()->width();
    int h = drawArea()->height();

    /// Create a model to store scans & add it
    SurfaceMeshModel* model = new SurfaceMeshModel();
    document()->addModel(model);   
    
#if 0
    /// Query buffer precision
    QGLFormat format = drawArea()->format();
    format.setDepthBufferSize(32);
    drawArea()->setFormat(format);
    GLint bits;
    glGetIntegerv(GL_DEPTH_BITS, &bits);
    qDebug() << "depth buffer precision" << bits;
#endif 
    
#if 0
    drawArea()->updateGL();
#else
    /// Render essentials
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
        glMultMatrixd( document()->transform.data() );
        foreach(StarlabModel* model, document()->models())
            if(model->isVisible && model->renderer()!=NULL ){ 
                drawArea()->qglColor(model->color);
                model->renderer()->render();
            }
    glPopMatrix();
#endif
    
/// QGLViewer version will take ****ages**** to give you something
#if 0
    for(int winX=0; winX<w; winX++){
        for(int winY=0; winY<h; winY++){
            qDebug() << winX << winY;
            bool found=false;
            qglviewer::Vec p = drawArea()->camera()->pointUnderPixel(QPoint(winX, winY),found);
            if(found) model->add_vertex( Vector3(p[0], p[1], p[2]) );
        }
    }

/// Pure OpenGL version
#else
    /// Fetch the depth buffer (BOTTLENECK!!!!)
    /// http://stackoverflow.com/questions/516778/get-depth-buffer-from-qglpixelbuffer
    std::vector<GLfloat> winZv(w*h);
    GLfloat* winZ = winZv.data();
    glReadPixels(0, 0, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, winZ);
    
    /// Fetch view matrices 
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
    
    /// Position of point in 3D space
    GLdouble posX, posY, posZ;
       
    /// Unproject depth buffer
    for(int winX=0; winX<w; winX++){
        for(int winY=0; winY<h; winY++){
            float depth = winZ[w*winY + winX];
            if(depth<1){
                gluUnProject(winX, winY, depth, modelview, projection, viewport, &posX, &posY, &posZ);
                model->add_vertex( Vector3(posX, posY, posZ) );
            }
        }
    }
   
    /// Don't affect visualization
    drawArea()->updateGL();    
#endif
}
#endif
