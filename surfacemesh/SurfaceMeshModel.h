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
    class SurfaceMeshForEachVertexOnFaceHelper;
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
    
class DYNAMIC_SURFACEMESH_EXPORT SurfaceMeshModel : public StarlabModel, public Surface_mesh{
    Q_OBJECT
    Q_INTERFACES(Starlab::Model)
    
    /// @{ Basic Model Implementation
    public:
        SurfaceMeshModel(QString path=QString(), QString name=QString());
        void updateBoundingBox();
        void decorateLayersWidgedItem(QTreeWidgetItem* parent);
    /// @}

    /// @{ Qt foreach helpers
    ///    Example: foreach(Vertex v, m->validVertices()){ ... }
    public:
        using Surface_mesh::halfedges; /// Allows to use homonim method from Surface_mesh
        using Surface_mesh::faces;     /// Allows to use homonim method from Surface_mesh
        SurfaceMeshForEachHalfedgeHelper halfedges();
        SurfaceMeshForEachEdgeHelper edges();
        SurfaceMeshForEachFaceHelper faces();
        SurfaceMeshForEachOneRingEdgesHelper onering_hedges(Vertex v);
        
        /// @brief Allows to use homonim method from Surface_mesh
        using Surface_mesh::vertices;
        /// @brief Returns iterator visiting every (valid) mesh vertex
        SurfaceMeshForEachVertexHelper vertices(); 
        /// @brief Returns iterator visiting every mesh vertex on a given face
        /// @note Transparently overrides Surface_mesh's method but supports Qt::foreach
        SurfaceMeshForEachVertexOnFaceHelper vertices(Face f); 
    /// @}

    /// @{ Query existence of basic properties
        bool has_vertex_normals(){ return has_vertex_property<Vector3>(VNORMAL); }
        bool has_face_normals(){ return has_face_property<Vector3>(FNORMAL); }
    /// @}
        
    /// @{ Access to default properties
        Vector3VertexProperty vertex_coordinates(bool create_if_missing=false);
        Vector3VertexProperty vertex_normals(bool create_if_missing=false);
    /// @}
        
    /// @{ forced garbage collection!!
        void garbage_collection(){ garbage_ = true; Surface_mesh::garbage_collection(); }
    /// @}

    /// @{ Extra exposed functionality
        void remove_vertex(Vertex v);
    /// @}
};

/// Allows you to refer to SurfaceMeshModel as "SurfaceMesh::Model"
typedef SurfaceMeshModel Model;    

/// @{ Casting & Type-checking
    /// @brief is the given starlab model a SurfaceMeshModel?
    bool is_a(StarlabModel* model);
    /// @brief safely cast to SurfaceMesh throwing exception on fail
    SurfaceMeshModel* safe_cast(Starlab::Model* model);
    /// @obsolete use safe_cast
    SurfaceMeshModel* safeCast(Starlab::Model* model);
    /// @obsolete use is_a
    bool isA(StarlabModel* model);    
/// @}

} // namespace

/// Allow the use of Qt "foreach" constructs
#include "helpers/SurfaceMeshQForEachHelpers.h"

/// Append namespace to name
//typedef SurfaceMesh::Model SurfaceMeshModel; // this causes ambiguous case for compiler
