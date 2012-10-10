#pragma once
#include "SurfaceMeshHelper.h"
#include <float.h>

static uint qHash( const Vertex &key ){return qHash(key.idx()); }

// Eigne matrix library
#include <Eigen/Core>
#include <Eigen/Sparse>
#include <Eigen/CholmodSupport>
using namespace Eigen;
typedef CholmodSupernodalLLT< SparseMatrix<double> > CholmodSolver;

#define qRanged(min, v, max) ( qMax(min, qMin(v, max)) )
#define EPSILON 1e-12

class GeoHeatHelper : public SurfaceMeshHelper{

public:
    ScalarVertexProperty vfunction;
    ScalarVertexProperty vcot;
    ScalarVertexProperty u;
    ScalarVertexProperty vdiv;
    ScalarEdgeProperty ecot;
    Vector3FaceProperty fgradient;

    GeoHeatHelper(SurfaceMeshModel* mesh) : SurfaceMeshHelper(mesh)
    {
        points  = this->getVector3VertexProperty(VPOINT);
        varea   = this->computeVertexVoronoiArea();
        fnormal = this->computeFaceNormals();
        farea   = this->computeFaceAreas();
        elenght = this->computeEdgeLengths();

        // Heat method properties
        u           = mesh->vertex_property<Scalar> ("v:heat", 0);
        vdiv        = mesh->vertex_property<Scalar> ("v:divergence", 0);
        vcot        = mesh->vertex_property<Scalar> ("v:cotan", 0);
        vfunction   = mesh->vertex_property<Scalar> ("v:function", 0);
        ecot        = mesh->edge_property<Scalar>   ("e:cotan", 0);
        fgradient   = mesh->face_property<Vector3>  ("f:gradient", Vector3(0));
    }

    Scalar t()
    {
        // t = (avg edge length) ^ 2
        double avg = 0.0;
        int count = 0;

        foreach(Edge e, mesh->edges()){
            if(elenght[e] > 0.0){
                avg += elenght[e];
                count++;
            }
        }

        avg /= count;
        return avg * avg;
    }

    VectorXd u0(const QSet<Vertex> & vidx)
    {
        mesh->remove_vertex_property(vfunction);
        vfunction = mesh->vertex_property<Scalar> ("v:function", 0);

        // u0 on source vertices = 1.0 and zero elsewhere
        VectorXd u_0 = VectorXd::Zero(mesh->n_vertices());
        foreach(Vertex v, vidx)
            u_0[v.idx()] = vfunction[v] = 1.0;

        return u_0;
    }

    SparseMatrix<Scalar> A()
    {
        // A is diagonal matrix of vertex areas
        typedef Eigen::Triplet<double> T;
        std::vector< T > A_elements;

        foreach(Vertex v, mesh->vertices())
            A_elements.push_back( T(v.idx(), v.idx(), qMax(varea[v], EPSILON) ) );

        SparseMatrix<Scalar> matA(mesh->n_vertices(), mesh->n_vertices());
        matA.setFromTriplets(A_elements.begin(), A_elements.end());
        return matA;
    }

    SparseMatrix<Scalar> Lc()
    {
        // Efficent sparse matrix construction
        typedef Eigen::Triplet<double> T;
        std::vector< T > L_c;

        // Fill as cotan operator
        foreach(Edge e, mesh->edges()){
            Scalar cot_alpha = 0, cot_beta = 0;

            Vertex vi = mesh->vertex(e, 0);
            Vertex vj = mesh->vertex(e, 1);

            Vertex v_a = mesh->to_vertex(mesh->next_halfedge(mesh->halfedge(e, 0)));
            if(has_halfedge(v_a, vj)) cot_alpha = dot(points[vi]-points[v_a], points[vj]-points[v_a]) / cross(points[vi]-points[v_a], points[vj]-points[v_a]).norm();

            Vertex v_b = mesh->to_vertex(mesh->next_halfedge(mesh->halfedge(e, 1)));
            if(has_halfedge(v_b, vi)) cot_beta  = dot(points[vi]-points[v_b], points[vj]-points[v_b]) / cross(points[vi]-points[v_b], points[vj]-points[v_b]).norm();

            Scalar cots = (0.5 * (cot_alpha + cot_beta));

            if(abs(cots) == 0) continue;

            L_c.push_back(T(vi.idx(), vj.idx(), -cots));
            L_c.push_back(T(vj.idx(), vi.idx(), -cots));
            L_c.push_back(T(vi.idx(), vi.idx(), cots + EPSILON));
            L_c.push_back(T(vj.idx(), vj.idx(), cots + EPSILON));

            // Just for record
            vcot[vi] += cots;
            vcot[vj] += cots;
            ecot[e] = cots;
        }

        // Initialize a sparse matrix
        SparseMatrix<Scalar> Lc_mat(mesh->n_vertices(), mesh->n_vertices());
        Lc_mat.setFromTriplets(L_c.begin(), L_c.end());
        return Lc_mat;
    }

    void gradientFaces(bool isNormalizeNegateGradient = true)
    {
        // Compute gradient on faces
        foreach(Face f, mesh->faces()){
            Vector3 vsum(0);

            Surface_mesh::Halfedge_around_face_circulator h(mesh, f), hend = h;
            do{
                Vector3 ei = points[mesh->from_vertex(h)] - points[mesh->from_vertex(mesh->prev_halfedge(h))];
                Vertex i = mesh->to_vertex(h);
                vsum += u[i] * cross(fnormal[f], ei);
            }while (++h != hend);

            fgradient[f] = ( 1.0 / (2.0 * farea[f]) ) * vsum;

            if(isNormalizeNegateGradient)
                fgradient[f] = (-fgradient[f]).normalized();
        }
    }

    VectorXd divergenceVertices()
    {
        VectorXd d(mesh->n_vertices());

        foreach(Vertex i, mesh->vertices()){
            double sum_j = 0.0;

            Surface_mesh::Halfedge_around_vertex_circulator j(mesh, i), hend = j;
            do {
                if(!mesh->is_valid(mesh->face(j))) continue; // todo: handle this?

                // Face gradient
                Vector3 Xj = fgradient[ mesh->face(j) ];

                // Vertex position
                Vector3 pi = points[mesh->from_vertex(j)];
                Vector3 p1 = points[mesh->to_vertex(j)];
                Vector3 p2 = points[mesh->to_vertex(mesh->next_halfedge(j))];

                // Incident edges
                Vector3 e1 = p1 - pi;
                Vector3 e2 = p2 - pi;

                // Angles
                double theta1 = acos( dot((p1-p2).normalized(), (pi-p2).normalized()) );
                double theta2 = acos( dot((p2-p1).normalized(), (pi-p1).normalized()) );
                double cot1 = 1.0 / tan(theta1);
                double cot2 = 1.0 / tan(theta2);

                sum_j += (cot1 * dot(e1, Xj)) + (cot2 * dot(e2, Xj));

            } while(++j != hend);

            vdiv[i] = 0.5 * sum_j;

            d(i.idx()) = vdiv[i];
        }

        return d;
    }

    VectorXd toEigenVector(const ScalarVertexProperty & vproperty){
        VectorXd V( mesh->n_vertices() );
        foreach(Vertex i, mesh->vertices()) V(i.idx()) = vproperty[i];
        return V;
    }

    void set(const VectorXd & V, const string property){
        ScalarVertexProperty vprop = mesh->vertex_property<Scalar>(property);
        foreach(Vertex v, mesh->vertices())
            vprop[v] = V[v.idx()];
    }

    bool has_halfedge(Vertex start, Vertex end){
        Halfedge h = mesh->halfedge(start);
        const Halfedge hh = h;
        if (h.is_valid()){
            do{
                if (mesh->to_vertex(h) == end) return true;
                h = mesh->cw_rotated_halfedge(h);
            } while (h != hh);
        }
        return false;
    }

    ScalarVertexProperty unifromDistance( const string property = "v:uniformDistance" ){
        ScalarVertexProperty vprop = mesh->vertex_property<Scalar>(property);
        ScalarVertexProperty dist = mesh->vertex_property<Scalar>("v:heat_distance");

        double minDist = DBL_MAX, maxDist = -DBL_MAX;
        foreach(Vertex v, mesh->vertices()){
            minDist = qMin(dist[v], minDist);
            maxDist = qMax(dist[v], maxDist);
        }
        double range = maxDist - minDist;

        foreach(Vertex v, mesh->vertices())
            vprop[v] = 1 - ((dist[v] - minDist) / range);

        return vprop;
    }

    void cleanUp(bool isAll = false)
    {
        ScalarVertexProperty heat  = getScalarVertexProperty("v:heat_distance");
        ScalarVertexProperty udist = getScalarVertexProperty("v:uniformDistance");
                
        ecot        = mesh->edge_property<Scalar>   ("e:cotan", 0);
        fgradient   = mesh->face_property<Vector3>  ("f:gradient", Vector3(0));

        mesh->remove_vertex_property(u);
        mesh->remove_vertex_property(vdiv);
        mesh->remove_vertex_property(vcot);
        mesh->remove_vertex_property(vfunction);
        mesh->remove_edge_property(ecot);
        mesh->remove_face_property(fgradient);
        mesh->remove_vertex_property(heat);
        
        if(isAll) mesh->remove_vertex_property(udist);
    }
};
