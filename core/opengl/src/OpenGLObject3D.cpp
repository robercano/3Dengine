/**
 * @class	OpenGLObject3D
 * @brief	OpengGL object 3D representation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "OpenGLObject3D.hpp"
#include "OpenGL.h"

bool OpenGLObject3D::init(const Object3D &object)
{
    uint32_t offset;

	/* Generate a vertex array to reference the attributes */
	GL( glGenVertexArrays(1, &_gVAO) );
	GL( glBindVertexArray(_gVAO) );
    {
        /* Generate a buffer object for the vertices positions */
        GL( glGenBuffers(1, &_vertexDataVBO) );
        GL( glBindBuffer(GL_ARRAY_BUFFER, _vertexDataVBO) );
        {
            /* Upload the data for this buffer */
            GL( glBufferData(GL_ARRAY_BUFFER,
                             object.getVertexData().size() * sizeof object.getVertexData()[0],
                             &(object.getVertexData()[0]),
                             GL_STATIC_DRAW) );

            /* First attribute contains the vertex coordinates */
            GL( glEnableVertexAttribArray(0) );
            GL( glVertexAttribPointer(
                    0,                             // attribute. No particular reason for 0, but must match the layout in the shader.
                    3,                             // size
                    GL_FLOAT,                      // type
                    GL_FALSE,                      // normalized?
                    sizeof(Object3D::VertexData),// stride
                    (void*)0                       // array buffer offset
                    ) );

            /* Second attibute contains the normals */
            offset = 12;
            GL( glEnableVertexAttribArray(1) );
            GL( glVertexAttribPointer(
                    1,                             // attribute
                    3,                             // size
                    GL_FLOAT,                      // type
                    GL_FALSE,                      // normalized?
                    sizeof(Object3D::VertexData),  // stride
                    reinterpret_cast<void*>(offset)                  // array buffer offset
                    ) );

            /* Third attribute contains the UV coordinates */
            offset = 24;
            GL( glEnableVertexAttribArray(2) );
            GL( glVertexAttribPointer(
                    2,                             // attribute
                    2,                             // size
                    GL_FLOAT,                      // type
                    GL_FALSE,                      // normalized?
                    sizeof(Object3D::VertexData),// stride
                    reinterpret_cast<void*>(offset)                  // array buffer offset
                    ) );
        }

        /* Generate the buffer objects for the indices */
        GL( glGenBuffers(1, &_indicesBO) );
        GL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indicesBO) );
        {
            /* Upload the data */
            GL( glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                             object.getIndexData().size() * sizeof(object.getIndexData()[0]),
                             &(object.getIndexData()[0]),
                             GL_STATIC_DRAW) );
        }
    }
    GL( glBindVertexArray(0) );

    /* TODO: Upload the textures */
    const std::vector< Texture > &textures = object.getTextures();

    _texturesIDs.resize(textures.size());
    GL( glGenTextures(textures.size(), &_texturesIDs[0]) );

    for(int i = 0; i < textures.size(); ++i) {
        /* TODO: Once we use our own format, this should not
         * be needed */
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GL( glBindTexture(GL_TEXTURE_2D, _texturesIDs[i]) );
        {
            GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
            GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
            GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
            GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
            GL( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                             textures[i]._width,
                             textures[i]._height,
                             0, GL_RGB, GL_UNSIGNED_BYTE,
                             textures[i]._texture) );
        }

    }
    GL( glBindTexture(GL_TEXTURE_2D, 0) );

    _materials = object.getMaterials();
    _indicesOffsets = object.getIndicesOffsets();
    _indicesCount = object.getIndicesCount();

    return true;
}

bool OpenGLObject3D::destroy()
{
	GL( glDeleteBuffers(1, &_vertexDataVBO) );
	GL( glDeleteVertexArrays(1, &_gVAO) );
    return true;
}
