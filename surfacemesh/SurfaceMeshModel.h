#pragma once
#include "dynamic_surfacemesh_global.h"

#include <QDebug>
#include <QString>
#include "Model.h"
#include "StarlabException.h"
#include "surface_mesh/Surface_mesh.h"

namespace SurfaceMesh{

/// @{ Default types 
    typedef Surface_mesh::Scalar    Scalar;              ///< Scalar type
    typedef Surface_mesh::Vector3   Vector3;             ///< 3D Vector type
    typedef Surface_mesh::Point     Point;               ///< Point type
    typedef Surface_mesh::Normal    Normal;              ///< Point type
    typedef Surface_mesh::Color     Color;               ///< Point type
    typedef int                     Integer;             ///< int
    typedef unsigned int            Counter;             ///< To count stuff
    typedef unsigned int            Size;                ///< @obsolete To index stuff (i.e. matlab pointer) 
/// @} 

/// @{ Default Surface_mesh::property names
    const std::string VPOINT = "v:point";
    const std::string FNORMAL = "f:normal";
    const std::string VNORMAL = "v:normal";
    const std::string VAREA = "v:area";
    const std::string ELENGTH = "e:length";
    const std::string VQUALITY = "v:quality";
    const std::string FAREA = "f:area";
    const std::string FBARYCENTER="f:barycenter";
/// @}

/// @{ Forward declaration of helpers (reduces clutter of this class header)
    class SurfaceMeshForEachVertexHelper;
    class SurfaceMeshForEachEdgeHelper;
    class SurfaceMeshForEachOneRingEdgesHelper;
    class SurfaceMeshForEachFaceHelper;
    class SurfaceMeshForEachHalfedgeHelper;
/// @}
    
/// @{ Import basic Surface_mesh names 
    typedef Surface_mesh::Edge                      Edge;
    typedef Surface_mesh::Halfedge                  Halfedge;
    typedef Surface_mesh::Vertex                    Vertex;
    typedef Surface_mesh::Face                      Face;
/// @}
    
/// @{ Default Vertex properties
    typedef Surface_mesh::Vertex_property<Scalar>   ScalarVertexProperty;
    typedef Surface_mesh::Vertex_property<Integer>  IntegerVertexProperty;
    typedef Surface_mesh::Vertex_property<Vector3>  Vector3VertexProperty;
    typedef Surface_mesh::Vertex_property<bool>     BoolVertexProperty;
/// @} 
    
/// @{ Default Face properties
    typedef Surface_mesh::Face_property<Scalar>     ScalarFaceProperty;
    typedef Surface_mesh::Face_property<Vector3>    Vector3FaceProperty;
    typedef Surface_mesh::Face_property<bool>       BoolFaceProperty;
/// @}
    
/// @{ Default Edge properties
    typedef Surface_mesh::Edge_property<bool>       BoolEdgeProperty;    
    typedef Surface_mesh::Edge_property<Scalar>     ScalarEdgeProperty;    
/// @}    
    

/// @{ Default Halfedge properties
    typedef Surface_mesh::Halfedge_property<Scalar> ScalarHalfedgeProperty;
/// @}
    
class DYNAMIC_SURFACEMESH_EXPORT SurfaceMeshModel : public Starlab::Model, public Surface_mesh{
    Q_OBJECT
    Q_INTERFACES(Starlab::Model)
    
    /// @{ Basic Model Implementation
    public:
        SurfaceMeshModel(QString path=QString(), QString name=QString());
        void updateBoundingBox();
        virtual void decorateLayersWidgedItem(QTreeWidgetItem* parent);
    /// @}

    /// @{ Qt foreach helpers
    ///    Example: foreach(Vertex v, m->validVertices()){ ... }
    public:
        using Surface_mesh::halfedges; /// Allows to use homonim method from Surface_mesh
        using Surface_mesh::vertices;  /// Allows to use homonim method from Surface_mesh
        using Surface_mesh::faces;     /// Allows to use homonim method from Surface_mesh
        SurfaceMeshForEachHalfedgeHelper halfedges();
        SurfaceMeshForEachVertexHelper vertices(); 
        SurfaceMeshForEachEdgeHelper edges();
        SurfaceMeshForEachFaceHelper faces();
        SurfaceMeshForEachOneRingEdgesHelper onering_hedges(Vertex v);
    /// @}

    /// @{ Access to default properties
        Vector3VertexProperty vertex_coordinates(){ return get_vertex_property<Vector3>(VPOINT); }
        Vector3VertexProperty vertex_normals(){ return get_vertex_property<Vector3>(VNORMAL); }
    /// @}
};

/// Allows you to refer to SurfaceMeshModel as "SurfaceMesh::Model"
typedef SurfaceMeshModel Model;    

} // namespace

/// Allow user to use Qt "foreach" constructs
#include "helpers/SurfaceMeshQForEachHelpers.h"
