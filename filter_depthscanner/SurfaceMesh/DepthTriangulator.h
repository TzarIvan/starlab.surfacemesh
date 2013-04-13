#include "SurfaceMeshModel.h"
#include "SurfaceMeshHelper.h"
#include "Eigen/Dense"

namespace SurfaceMesh{

class DepthTriangulator : public SurfaceMeshHelper{
    typedef Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> ImageMatrix;
    typedef Eigen::Matrix<Vertex, Eigen::Dynamic, Eigen::Dynamic> VertexMatrix;
    inline bool isnan(double a){ return (a!=a); }

public:
    DepthTriangulator(SurfaceMeshModel* mesh) : SurfaceMeshHelper(mesh){}
    void execute(ImageMatrix X, ImageMatrix Y, ImageMatrix Z){
        VertexMatrix VIDX(X.rows(), X.cols());
        VIDX.setConstant(Vertex()); /// Invalid
        
        /// --------------------///
        ///      Vertices       /// 
        /// --------------------///
        for(int i=0; i<X.rows(); i++)
            for(int j=0; j<X.cols(); j++)
                if( !isnan( X(i,j) ) )
                    VIDX(i,j) = mesh->add_vertex( Vector3(X(i,j),Y(i,j),Z(i,j)) );

        /// --------------------///
        ///    Triangulation    /// 
        /// --------------------///
        for(int i=0; i<X.rows()-1; i++){
            for(int j=0; j<X.cols()-1; j++){
                /// smesh assumes clockwise??
                Vertex v1 = VIDX(i+0,j+0);
                Vertex v2 = VIDX(i+0,j+1);
                Vertex v3 = VIDX(i+1,j+0);
                Vertex v4 = VIDX(i+1,j+1);
                               
                /// if possible, add triangles
                if( v1.is_valid() && v2.is_valid() && v3.is_valid() )
                    mesh->add_triangle(v1, v2, v3);
                if( v3.is_valid() && v2.is_valid() && v4.is_valid() )
                    mesh->add_triangle(v3, v2, v4);
            }   
        }
        
        /// --------------------///
        ///        GARBAGE      /// 
        /// --------------------///
        // remove vertices
        foreach(Vertex v, mesh->vertices())
            if(mesh->valence(v) == 0)
                mesh->remove_vertex(v);
        mesh->garbage_collection();
    }
};

}

#pragma once
#if 0
Vector3VertexProperty vnormals = scan->vertex_normals(getNormals);


    if(getNormals){
        vnormals[v] = fnormal;
        // drawArea()->drawRay(ipoint, fnormal,1,Qt::red,.1);
    }
    
    
    triangulate_depth_map       
    for(int winX=0; winX<w; winX+=step){
        for(int winY=0; winY<h; winY+=step){
            if( !isnan( X(winX,winY) ) ){
                Vertex v = scan->add_vertex( ipoint );     
                
            }
        }
    }
#endif 
