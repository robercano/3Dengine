/**
 * @class	Shader
 * @brief	Class to abstract shader functionality
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

class Shader
{
  public:
    static Shader *New(void);
    static void Delete(Shader *shader);

    /**
     * Destructor
     */
    virtual ~Shader() {}
    virtual bool init() = 0;

    /**
     * Loads a new shader code and compiles it. Path contains the subdirectory
     * inside data/shaders/ and the name of the shader files without the extension.
     *
     * i.e.: anti-aliasing/fxaa will load data/shaders/anti-aliasing/fxaa.vert
     *       and data/shaders/anti-aliasing/fxaa/frag
     *
     * @param		path  Path to the vertex/fragment shader pair
     * @param[out]	error If compilation fails error will contain
     *					  a description of the error
     *
     * @return true or false
     */
    virtual bool use(const std::string &path, std::string &error) = 0;

    /**
     * Loads a new vertex shader code and compiles it
     *
     * @param		filename  File name of the vertex shader to be compiled
     * @param[out]	error	  If compilation fails error will contain
     *						  a description of the error
     *
     * @return true or false
     */
    virtual bool loadVertexShader(const std::string &filename, std::string &error) = 0;

    /**
     * Loads a new fragment shader code and compiles it
     *
     * @param		filename  File name of the fragment shader to be compiled
     * @param[out]	error	  If compilation fails error will contain
     *						  a description of the error
     *
     * @return true or false
     */
    virtual bool loadFragmentShader(const std::string &filename, std::string &error) = 0;

    /**
     * Links the program together and prepares the shader to be used
     *
     * @param[out] error	If linking fails error will contain a
     *						description of the error
     *
     * @return true or false
     */
    virtual bool linkProgram(std::string &error) = 0;

    /**
     * Makes the shader active
     */
    virtual bool attach(void) = 0;

    /**
     * Makes the shader inactive
     */
    virtual bool detach(void) = 0;

    /**
     * Gets a list of all the shader uniforms
     *
     * @return A vector with all the available uniform names
     */
    virtual const std::map<std::string, uint32_t> &getUniforms(void) = 0;

    /**
     * Retrieves the ID of a specific uniform
     *
     * @param name  Name of the uniform
     * @param id    Output ID
     *
     */
    virtual const bool getUniformID(const std::string &name, uint32_t *id) = 0;
    virtual const bool getAttributeID(const std::string &name, uint32_t *id) = 0;

    /**
     * Sets the value of a shader uniform as a mat4x4
     *
     * @param name  Name of the shader uniform
     * @param value Value of the uniform to be set
     *
     * @return true if the value was set or false if the
     *         attribute cannot be found
     */
    virtual bool setUniformMat4(const std::string &name, const glm::mat4 value[], uint32_t numItems = 1) = 0;
    virtual bool setUniformMat3(const std::string &name, const glm::mat3 value[], uint32_t numItems = 1) = 0;
    virtual bool setUniformTexture2D(const std::string &name, uint32_t unitID) = 0;
    virtual bool setUniformTexture2DArray(const std::string &name, uint32_t unitIDs[], uint32_t numItems) = 0;
    virtual bool setUniformFloat(const std::string &name, float value) = 0;
    virtual bool setUniformUint(const std::string &name, uint32_t value) = 0;
    virtual bool setUniformBool(const std::string &name, bool value) = 0;
    virtual bool setUniformVec4(const std::string &name, glm::vec4 &value) = 0;
    virtual bool setUniformVec3(const std::string &name, glm::vec3 &value) = 0;
    virtual bool setUniformVec2(const std::string &name, glm::vec2 &value) = 0;

    /**
     * Sets custom parameters only known by the implementer class
     */
    virtual void setCustomParams(void) = 0;
};
