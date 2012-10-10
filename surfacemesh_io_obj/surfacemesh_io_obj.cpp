#include "surfacemesh_io_obj.h"
#include "SurfaceMeshHelper.h"

Model* surfacemesh_io_obj::open(QString path){
    SurfaceMeshModel* model = new SurfaceMeshModel(path);
    /// Load the mesh
    model->read(qPrintable(path));
    return model;
}

void surfacemesh_io_obj::save(SurfaceMeshModel* mesh,QString path){
    FILE* fid = fopen( qPrintable(path), "w" );
    if( fid == NULL ) throw StarlabException("the file cannot be opened");
    
    SurfaceMeshHelper helper(mesh);
    Vector3VertexProperty p = helper.getVector3VertexProperty(VPOINT);
    // bool has_vnormals = mesh->has_vertex_property<Vector3>(VNORMAL);
    if(true /*!has_vnormals*/){
        foreach(Vertex v, mesh->vertices())
            fprintf( fid, "v %.10f %.10f %.10f\n", p[v].x(), p[v].y(), p[v].z() );        
        foreach(Face f, mesh->faces()){
            // int nV = mesh->valence(f);
            fprintf(fid, "f");
            Surface_mesh::Vertex_around_face_circulator fvit=mesh->vertices(f), fvend=fvit;
            do{ fprintf(fid, " %d", ((Surface_mesh::Vertex)fvit).idx()+1);
            } while (++fvit != fvend);
            fprintf(fid, "\n");
        }        
    }
    
    qWarning() << "[WARNING] surfacemesh_io_obj::save() NEVER save normals";
    fclose(fid);   

#ifdef TODO_SAVE_WITH_NORMALS    
    if(has_vnormals){
        Vector3VertexProperty vnormals = helper.getVector3VertexProperty(VNORMAL);
        foreach(Vertex v, mesh->vertices())
            ; // fprintf( fid, "v %f %f %f\n", M.vertices(i,1), M.vertices(i,2), M.vertices(i,3) );        
        foreach(Vertex v, mesh->vertices())
            ; // fprintf( fid, "vn %f %f %f\n", VN(i,1), VN(i,2), VN(i,3) );
        foreach(Face   f, mesh->faces())
            ; // fprintf( fid, "f %.0f//%.0f %.0f//%.0f %.0f//%.0f\n",
    }
#endif
}

Q_EXPORT_PLUGIN(surfacemesh_io_obj)

