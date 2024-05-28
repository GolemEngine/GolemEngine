#include "WorldBuilder/defaultTerrain.h"

#include "Resource/resourceManager.h"


DefaultTerrain::DefaultTerrain(std::string _name, Transform* _transform)
    : Terrain(_name, _transform)
{}

void DefaultTerrain::Init(int _xResolution, int _zResolution)
{
    xResolution = _xResolution;
    zResolution = _zResolution;
    
    // Make a grid of vertices using the terrain's dimensions
    for (int i = 0; i < xResolution; i++)
    {
        for (int j = 0; j < zResolution; j++)
        {
            // Set the vertex position
            VertexGpu vertex0;
            VertexGpu vertex1;
            VertexGpu vertex2;
            VertexGpu vertex3;

            vertex0.position.x = (i / (float)xResolution) - 0.5f;    // - 0.5f to center to center origin
            vertex0.position.z = (j / (float)zResolution) - 0.5f;    // - 0.5f to center to center origin
            vertex0.position.y = 0.0f;
            // Set the vertex0 texture postion
            vertex0.textureCoords.x = (j / (float)zResolution);
            vertex0.textureCoords.y = 1.0f - (i / (float)xResolution);

            vertex1.position.x = ((i+1) / (float)xResolution) - 0.5f;    // - 0.5f to center to center origin
            vertex1.position.z = (j / (float)zResolution) - 0.5f;    // - 0.5f to center to center origin
            vertex1.position.y = 0.0f;
            // Set the vertex1 texture postion
            vertex1.textureCoords.x = (j / (float)zResolution);
            vertex1.textureCoords.y = 1.0f - ((i+1) / (float)xResolution);

            vertex2.position.x = (i / (float)xResolution) - 0.5f;    // - 0.5f to center to center origin
            vertex2.position.z = ((j+1) / (float)zResolution) - 0.5f;    // - 0.5f to center to center origin
            vertex2.position.y = 0.0f;
            // Set the vertex2 texture postion
            vertex2.textureCoords.x = ((j+1) / (float)zResolution);
            vertex2.textureCoords.y = 1.0f - (i / (float)xResolution);

            vertex3.position.x = ((i+1) / (float)xResolution) - 0.5f;    // - 0.5f to center to center origin
            vertex3.position.z = ((j+1) / (float)zResolution) - 0.5f;    // - 0.5f to center to center origin
            vertex3.position.y = 0.0f;
            // Set the vertex3 texture postion
            vertex3.textureCoords.x = ((j+1) / (float)zResolution);
            vertex3.textureCoords.y = 1.0f - ((i+1) / (float)xResolution);
            
            m_vertices.push_back(vertex0);
            m_vertices.push_back(vertex1);
            m_vertices.push_back(vertex2);
            m_vertices.push_back(vertex1);
            m_vertices.push_back(vertex2);
            m_vertices.push_back(vertex3);
        }
    }

    SetupMesh();
}
