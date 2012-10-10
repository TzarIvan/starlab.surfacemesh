#include "surfacemesh_filter_geoheat.h"
#include "StarlabDrawArea.h"
#include "GeoDrawObjects.h"

void surfacemesh_filter_geoheat::initParameters(RichParameterSet *pars)
{
    pars->addParam(new RichInt("sourceVertex", 0, "Source vertex"));
    pars->addParam(new RichBool("visualizeDistance", true, "Visualize Distance"));
}

void surfacemesh_filter_geoheat::precompute()
{
    h   = new GeoHeatHelper( mesh() );
    A   = h->A();
    t   = h->t();
    Lc  = h->Lc();

    heat_flow.compute( A + (t * Lc) );
    poisson_solver.compute( Lc );
}

void surfacemesh_filter_geoheat::applyFilter(RichParameterSet *pars)
{
    QSet<Vertex> src;
    if(pars)
        src.insert(Vertex(pars->getInt("sourceVertex")));
    else
    {
        // Source points are tagged on mesh vertices
        BoolVertexProperty src_points = mesh()->get_vertex_property<bool>("v:geo_src_points");
        if(!src_points.is_valid()) return;
        foreach(Vertex v, mesh()->vertices())
            if(src_points[v]) src.insert(v);
    }

    ScalarVertexProperty distance = uniformDistance(src);

    // Visualize distance on vertices
    drawArea()->deleteAllRenderObjects();

    if(pars && pars->getBool("visualizeDistance"))
    {
        PointSoup * ps = new PointSoup;
        Vector3VertexProperty points = h->getVector3VertexProperty(VPOINT);

        foreach(Vertex v, mesh()->vertices())
            ps->addPoint( points[v], qtJetColorMap(1.0 - distance[v]) );
        drawArea()->addRenderObject(ps);
    }
}

ScalarVertexProperty surfacemesh_filter_geoheat::uniformDistance(const QSet<Vertex> & source)
{
    if(!h) precompute();

    // 0) Set source vertices
    u0 = h->u0( source );

    // 1) Compute heat flow for time 't'
    h->set( heat_flow.solve( u0 ), "v:heat" );

    // 2) Evaluate vector field X
    h->gradientFaces();

    // 3) Comptue distance function (solve Poisson equation)
    VectorXd d = h->divergenceVertices();
    h->set( poisson_solver.solve( d ), "v:heat_distance");

    return h->unifromDistance();
}

Q_EXPORT_PLUGIN(surfacemesh_filter_geoheat)
