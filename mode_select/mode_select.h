#pragma once
#include "StarlabMainWindow.h"
#include "SurfaceMeshPlugins.h"

/// FW
class Octree;
class ControllerManager;

class mode_select : public ModePlugin{
    Q_OBJECT
    Q_INTERFACES(ModePlugin)

    QIcon icon(){ return QIcon(":/icons/select_points.png"); }
    bool isApplicable(){ return true; }

private:
    SurfaceMeshModel* mesh;
    Octree* octree;
    
    ControllerManager* manager;
    
public:
    /// Functions part of the EditPlugin system
    void create();
    void destroy();
    void decorate();
    void drawWithNames();
    
    bool mouseReleaseEvent(QMouseEvent*);


//    bool mousePressEvent(QMouseEvent*);
//    bool mouseMoveEvent(QMouseEvent*);
};
