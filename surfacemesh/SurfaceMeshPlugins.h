/// @todo this file should be split in a number of files inside the folder interfaces/
/// otherwise you will end up having to recompile all the surfacemeshplugins when it changes...

#pragma once
#include "InputOutputPlugin.h"
#include "DecoratePlugin.h"
#include "FilterPlugin.h"
#include "ModePlugin.h"
#include "RenderPlugin.h"
#include "RichParameterSet.h"
#include "SurfaceMeshModel.h"

/// Allow user to use Qt "foreach" constructs
#include "helpers/SurfaceMeshQForEachHelpers.h"

namespace{
    /// Is the given model a SurfaceMeshModel?
    bool isA(Model* model){ return qobject_cast<SurfaceMesh::Model*>(model); }
    /// Safely convert to a surfacemesh
    SurfaceMesh::Model* safeCast(Model* model){
        SurfaceMesh::Model* mesh = qobject_cast<SurfaceMesh::Model*>(model);
        if(!mesh) throw StarlabException("Model is not a SurfaceMeshModel");
        return mesh;
    }
}

class SurfaceMeshInputOutputPlugin : public InputOutputPlugin{
private: 
    void save(Model* model,QString path){ save(safeCast(model),path); }
    bool isApplicable(Model* model){ return isA(model); }    
public:
    virtual void save(SurfaceMesh::Model* model, QString path) = 0;
};

class SurfaceMeshRenderPlugin : public RenderPlugin{
private: 
    bool isApplicable(Model* model){ return isA(model); }
public: 
    SurfaceMesh::Model* mesh(){ return safeCast(model()); }
};

class SurfaceMeshFilterPlugin : public FilterPlugin{
public:
    SurfaceMesh::Model* mesh(){ return safeCast(model()); }    
private:
    bool isApplicable(Model* model) { return isA(model); }
};

class SurfaceMeshModePlugin : public ModePlugin{
public:
    SurfaceMesh::Model* mesh(){ return safeCast(document()->selectedModel()); }
private:
    bool isApplicable() { return isA(document()->selectedModel()); }
};

#if 0
class SurfaceMeshDecoratePlugin : public DecoratePlugin{
public:
    SurfaceMeshModel* mesh(){ return safeCast(document()->selectedModel()); }    
private:
    bool isApplicable(Model* model) { return isA(model); }
};
#endif
