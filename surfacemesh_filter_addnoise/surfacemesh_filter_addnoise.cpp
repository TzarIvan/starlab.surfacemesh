#include "surfacemesh_filter_addnoise.h"
#include "StarlabDrawArea.h"

using namespace SurfaceMesh;

QString printBounding(QBox3D box){
    QString retval;
    QTextStream sout(&retval);
    QVector3D c = box.center();
    QVector3D s   = box.size();
    sout << "Center[" << c.x() << " " << c.y() << " " << c.z() << "]" 
         << "  Size[" << s.x() << " " << s.y() << " " << s.z() << "]";
    return retval;
}

void surfacemesh_filter_addnoise::initParameters(RichParameterSet *pars){
    QStringList modes;
    modes << "Normal Noise";
    modes << "Environment Noise";

    pars->addParam( new RichFloat("noiseperc", .01,"Noise amount [0..1]", "Movement of vertices w.r.t. bounding box diagonal") );
    pars->addParam( new RichStringSet("mode", modes, "Noise Profile", "Which type of noise we add to the system"));
}

void surfacemesh_filter_addnoise::applyFilter(RichParameterSet* pars){
    Scalar bboxdiag = mesh()->bbox().size().length();   
    Scalar noise = bboxdiag*pars->getFloat("noiseperc");

    if( pars->getString("mode") == "Normal Noise" ){
        Vector3VertexProperty points = mesh()->vertex_property<Vector3>(VPOINT);
        foreach(Vertex v, mesh()->vertices()){
            /// @todo drand48 is for UNIX only... 
            /// qRand() does something differently :(
            points[v].x() += ( (drand48()-.5) * noise );
            points[v].y() += ( (drand48()-.5) * noise );
            points[v].z() += ( (drand48()-.5) * noise );
        }
    }

    /// Generate random points within bbox
    if( pars->getString("mode") == "Environment Noise" ){
        int n=100;
        for(int i=0; i<n; i++){
            double x = drand48()*mesh()->bbox().size().x() + mesh()->bbox().minimum().x();
            double y = drand48()*mesh()->bbox().size().y() + mesh()->bbox().minimum().y();
            double z = drand48()*mesh()->bbox().size().z() + mesh()->bbox().minimum().z();
            mesh()->add_vertex( Point(x,y,z) );
        }
    }
    
    /// And update it after, so we can reset the viewpoint
    mesh()->updateBoundingBox();
    if(drawArea()) drawArea()->resetViewport();
    
    qDebug() << "New bounding box: " << printBounding(mesh()->bbox());
}

Q_EXPORT_PLUGIN(surfacemesh_filter_addnoise)


