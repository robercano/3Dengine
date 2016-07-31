/**
 * @class	OpenGLAsset3D
 * @brief	OpengGL asset 3D representation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "OpenGLAsset3D.hpp"
#include "Logging.hpp"
#include "OpenGL.h"

using namespace Logging;

bool OpenGLAsset3D::prepare()
{
    uint32_t offset;

    /* Generate a vertex array to reference the attributes */
    __(glGenVertexArrays(1, &_gVAO));
    __(glBindVertexArray(_gVAO));
    {
        /* Generate a buffer model for the vertices positions */
        __(glGenBuffers(1, &_vertexDataVBO));
        __(glBindBuffer(GL_ARRAY_BUFFER, _vertexDataVBO));
        {
            /* Upload the data for this buffer */
            __(glBufferData(GL_ARRAY_BUFFER, getVertexData().size() * sizeof getVertexData()[0], &(getVertexData()[0]), GL_STATIC_DRAW));

            /* First attribute contains the vertex coordinates */
            __(glEnableVertexAttribArray(0));
            __(glVertexAttribPointer(0,         // attribute. No particular reason for 0, but must match the layout in the shader.
                                     3,         // size
                                     GL_FLOAT,  // type
                                     GL_FALSE,  // normalized?
                                     sizeof(Asset3D::VertexData),  // stride
                                     (void *)0                     // array buffer offset
                                     ));

            /* Second attibute contains the normals */
            offset = 12;
            __(glEnableVertexAttribArray(1));
            __(glVertexAttribPointer(1,                                // attribute
                                     3,                                // size
                                     GL_FLOAT,                         // type
                                     GL_FALSE,                         // normalized?
                                     sizeof(Asset3D::VertexData),      // stride
                                     reinterpret_cast<void *>(offset)  // array buffer offset
                                     ));

            /* Third attribute contains the UV coordinates */
            offset = 24;
            __(glEnableVertexAttribArray(2));
            __(glVertexAttribPointer(2,                                // attribute
                                     2,                                // size
                                     GL_FLOAT,                         // type
                                     GL_FALSE,                         // normalized?
                                     sizeof(Asset3D::VertexData),      // stride
                                     reinterpret_cast<void *>(offset)  // array buffer offset
                                     ));
        }

        /* Generate the buffer models for the indices */
        __(glGenBuffers(1, &_indicesBO));
        __(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indicesBO));
        {
            /* Upload the data */
            __(glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexData().size() * sizeof(getIndexData()[0]), &(getIndexData()[0]),
                            GL_STATIC_DRAW));
        }
    }
    __(glBindVertexArray(0));

    /* TODO: Upload the textures */
    const std::vector<Texture> &textures = getTextures();

    _texturesIDs.resize(textures.size());
    __(glGenTextures(textures.size(), &_texturesIDs[0]));

    for (size_t i = 0; i < textures.size(); ++i) {
        /* TODO: Once we use our own format, this should not
         * be needed */
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        __(glBindTexture(GL_TEXTURE_2D, _texturesIDs[i]));
        {
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
            __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
            __(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textures[i]._width, textures[i]._height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                            textures[i]._texture));
        }
    }
    __(glBindTexture(GL_TEXTURE_2D, 0));

    return true;
}

bool OpenGLAsset3D::destroy()
{
    __(glDeleteBuffers(1, &_vertexDataVBO));
    __(glDeleteVertexArrays(1, &_gVAO));
    return true;
}
