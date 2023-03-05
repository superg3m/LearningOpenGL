#include <Headers/texture_class.h>

void Texture_Class::addTexture(const std::string texturePath)
{
    this->currently_loaded_textures.push_back(std::make_tuple(texturePath, loadTexture(texturePath)));
}

unsigned int Texture_Class::loadTexture(const std::string texturePath)
{
    unsigned int textureID;
    unsigned char* data;
    glActiveTexture(GL_TEXTURE0 + this->currently_loaded_textures.size());
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int width, height, nrChannels;
    
    for (auto tuple : this->currently_loaded_textures)
    {
        std::string texturePathLocal;
        unsigned int textureID;
        std::tie(texturePathLocal, textureID) = tuple;
        if (texturePath == texturePathLocal)
        {
            return textureID;
        }
    }

    data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

    GLenum format = (nrChannels == 1) ? GL_RED : (nrChannels == 3) ? GL_RGB : (nrChannels == 4) ? GL_RGBA : NULL;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return textureID;
}