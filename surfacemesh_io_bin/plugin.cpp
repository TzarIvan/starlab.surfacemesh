#include "plugin.h"
#include <fstream>
using namespace SurfaceMesh;
struct Pointf { float x; float y; float z;};
Model* surfacemesh_io_obj::open(QString path) {
    /// Load the mesh (copy&paste from Surface_mesh)
    SurfaceMeshModel* mesh = new SurfaceMeshModel(path);
    Vector3VertexProperty normals = mesh->vertex_normals(true);
    std::ifstream stream(path.toStdString().c_str(), std::ios_base::binary);
    int w, h;
    std::vector<Pointf> v(w*h);
    std::vector<Pointf> n(w*h);
    {
        stream.read(reinterpret_cast<char *>(&w), sizeof(int));
        stream.read(reinterpret_cast<char *>(&h), sizeof(int));
        stream.read(reinterpret_cast<char *>(v.data()), sizeof(float)*w*h*3);
    }
    {
        stream.read(reinterpret_cast<char *>(&w), sizeof(int));
        stream.read(reinterpret_cast<char *>(&h), sizeof(int));
        stream.read(reinterpret_cast<char *>(n.data()), sizeof(float)*w*h*3);
    }
    for(unsigned int i=0; i<w*h; i++) {
        const Pointf& f = v[i];
        if(f.x!=0.0f || f.y!=0.0f || f.z!=0.0f) {
            Surface_mesh::Vertex vertex = mesh->add_vertex(SurfaceMesh::Point(v[i].x, v[i].y, v[i].z));
            normals[vertex] = Surface_mesh::Vector3(n[i].x, n[i].y, n[i].z);
        }
    }
    return mesh;
}

void surfacemesh_io_obj::save(SurfaceMeshModel* mesh,QString path) {
    throw StarlabException("Not supported...");
}

Q_EXPORT_PLUGIN(surfacemesh_io_obj)
