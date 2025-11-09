#include "alzartak/texture.h"
#include "stb_image.h"
#include "wak/image.h"

namespace alzartak
{

Texture::Texture(const std::string& filepath)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (!data)
    {
        throw std::runtime_error("Failed to load texture: " + filepath);
    }

    GLenum format;
    switch (channels)
    {
    case 1:
        format = GL_RED;
        break;
    case 2:
        format = GL_RG;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        stbi_image_free(data);
        throw std::runtime_error("Unsupported channel count");
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void Texture::Bind(uint32 unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace alzartak