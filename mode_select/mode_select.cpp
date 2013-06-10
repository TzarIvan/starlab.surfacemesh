#include "mode_select.h"
Q_EXPORT_PLUGIN(mode_select)

#include <QtOpenGL>
#include <QMouseEvent>

#include "StarlabDrawArea.h"
#include "Octree.h"

class Controller{
    bool _selected;
public:
    Vector3 center;
    
    Controller(Vector3 center):
        center(center){
        _selected = false;
    }
      
    virtual void draw(){
        static GLUquadric* quadric = gluNewQuadric();
        // glColor3f();
        glPushMatrix();
            glTranslated(center[0], center[1], center[2]);
            gluSphere(quadric, 0.2, 10, 10);
            gluCylinder(quadric,.1, .1, .4, 5, 1);
        glPopMatrix();
    }
};

class ControllerManager{
private:
    StarlabDrawArea* drawArea;
    QList<Controller*> _list;
public:
    ControllerManager(StarlabDrawArea* drawArea) :
        drawArea( drawArea ){}
    
    void add(Controller* c){
        _list.push_back(c);
    }

    void draw(){
        foreach(Controller* c, _list)
            c->draw();
    }
    
    void draw_with_names(){
        int i=0;
        foreach(Controller* c, _list){
            glPushName(i++);
                c->draw();                    
            glPopName();
        }
    }
    
    void select(){
        
    }
};

void mode_select::create(){
    manager = new ControllerManager( drawArea() );
    manager->add( new Controller( Vector3(  0,  0,  0) ) );
    manager->add( new Controller( Vector3( .2, .2, .2) ) );
    
#ifdef REENABLE
    mesh = SurfaceMesh::safe_cast( selection() );
    octree = new Octree(mesh);
#endif
}

void mode_select::destroy(){
    delete octree;
}

bool mode_select::mouseReleaseEvent(QMouseEvent* event){
    /// Convert pick to world coordinates
    QPoint pickpoint = event->pos();

    /// Request a selection
    drawArea()->select(pickpoint);
    qDebug() << drawArea()->selectedName();
    
    Controller* controller = manager->select(pickpoint);
    
#ifdef REENABLE
    Vector3 orig, dir;
    drawArea()->convertClickToLine(pickpoint,orig,dir);
    
    /// Perform intersection test   
    int faceIndex = -1; ///< guess
    Vector3 ipoint = octree->closestIntersectionPoint( Ray(orig, dir), &faceIndex );

    /// Debug reasons
    drawArea()->drawPoint(ipoint).size(10).color(Qt::red);
    drawArea()->updateGL();
#endif

    return true;
}

void mode_select::decorate(){
    
}

void mode_select::drawWithNames(){
       
}
