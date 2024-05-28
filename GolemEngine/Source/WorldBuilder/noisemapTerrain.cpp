#include "WorldBuilder/noisemapTerrain.h"

#include "Resource/resourceManager.h"
#include "Image/stb_image.h"


NoisemapTerrain::NoisemapTerrain(std::string _name, Transform* _transform)
    : Terrain(_name, _transform)
{}

void NoisemapTerrain::Init(const char* _noisemapPath)
{
    m_noisemapPath = _noisemapPath;
    
    // Load noisemap
    stbi_set_flip_vertically_on_load(false);
    unsigned char* noisemap = stbi_load(_noisemapPath, &xResolution, &zResolution, &m_nChannel, STBI_grey);
    
    std::vector<int> noisemapValues;

    // Make a grid of vertices using the noisemap's dimensions
    for (int i = 0; i < xResolution-1; i++)
    {
        for (int j = 0; j < zResolution-1; j++)
        {
            unsigned int noisemapValue0 = (noisemap + (i + xResolution * j))[0];
            unsigned int noisemapValue1 = (noisemap + ((i+1) + xResolution * j))[0];
            unsigned int noisemapValue2 = (noisemap + (i + xResolution * (j+1)))[0];
            unsigned int noisemapValue3 = (noisemap + ((i+1) + xResolution * (j+1)))[0];
           // Set the vertex position
            VertexGpu vertex0;
            VertexGpu vertex1;
            VertexGpu vertex2;
            VertexGpu vertex3;

            vertex0.position.x = (i / (float)xResolution) - 0.5f;    // - 0.5f to center to center origin
            vertex0.position.z = (j / (float)zResolution) - 0.5f;    // - 0.5f to center to center origin
            vertex0.position.y = noisemapValue0 / 255.0f;
            // Set the vertex0 texture postion
            vertex0.textureCoords.x = (j / (float)zResolution);
            vertex0.textureCoords.y = 1.0f - (i / (float)xResolution);

            vertex1.position.x = ((i+1) / (float)xResolution) - 0.5f;    // - 0.5f to center to center origin
            vertex1.position.z = (j / (float)zResolution) - 0.5f;    // - 0.5f to center to center origin
            vertex1.position.y = noisemapValue1 / 255.0f;
            // Set the vertex1 texture postion
            vertex1.textureCoords.x = (j / (float)zResolution);
            vertex1.textureCoords.y = 1.0f - ((i+1) / (float)xResolution);

            vertex2.position.x = (i / (float)xResolution) - 0.5f;    // - 0.5f to center to center origin
            vertex2.position.z = ((j+1) / (float)zResolution) - 0.5f;    // - 0.5f to center to center origin
            vertex2.position.y = noisemapValue2 / 255.0f;
            // Set the vertex2 texture postion
            vertex2.textureCoords.x = ((j+1) / (float)zResolution);
            vertex2.textureCoords.y = 1.0f - (i / (float)xResolution);

            vertex3.position.x = ((i+1) / (float)xResolution) - 0.5f;    // - 0.5f to center to center origin
            vertex3.position.z = ((j+1) / (float)zResolution) - 0.5f;    // - 0.5f to center to center origin
            vertex3.position.y = noisemapValue3 / 255.0f;
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

std::string NoisemapTerrain::GetNoisemapPath()
{
    return m_noisemapPath;
}

void NoisemapTerrain::SetNoisemapPath(std::string _noisemapPath)
{
    m_noisemapPath = _noisemapPath;
}
