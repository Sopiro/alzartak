#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>

#include "alzartak/image.h"
#include "alzartak/parallel_for.h"

namespace alzartak
{

Image1 ReadImage1(const std::filesystem::path& filename, int32 channel, bool non_color, Image1::Type multiplier)
{
    stbi_ldr_to_hdr_gamma(non_color ? 1.0f : 2.2f);

    int32 width, height;
    int32 components_per_pixel;
    float* data = stbi_loadf(filename.string().c_str(), &width, &height, &components_per_pixel, STBI_rgb_alpha);

    if (!data)
    {
        return {};
    }

    if (channel < 0 || channel >= components_per_pixel)
    {
        return {};
    }

    Image1 image(width, height);

    if (channel < 3)
    {
        if (width * height > 64 * 1024)
        {
            ParallelFor(0, width * height, [&](int32 i) {
                image[i] = Float(std::fmax(0, multiplier * data[STBI_rgb_alpha * i + channel]));
            });
        }
        else
        {
            for (int32 i = 0; i < width * height; ++i)
            {
                image[i] = Float(std::fmax(0, multiplier * data[STBI_rgb_alpha * i + channel]));
            }
        }
    }
    else if (components_per_pixel == STBI_rgb_alpha)
    {
        for (int32 i = 0; i < width * height; ++i)
        {
            image[i] = Float(std::fmax(0, multiplier * data[STBI_rgb_alpha * i + channel]));
        }
    }

    stbi_image_free(data);
    return image;
}

Image3 ReadImage3(const std::filesystem::path& filename, bool non_color, Image3::Type multiplier)
{
    stbi_ldr_to_hdr_gamma(non_color ? 1.0f : 2.2f);

    int32 width, height;
    int32 components_per_pixel;
    float* data = stbi_loadf(filename.string().c_str(), &width, &height, &components_per_pixel, STBI_rgb);

    if (!data)
    {
        return {};
    }

    Image3 image(width, height);

    if (width * height > 64 * 1024)
    {
        ParallelFor(0, width * height, [&](int32 i) {
            image[i][0] = Float(std::fmax(0, multiplier[0] * data[STBI_rgb * i + 0]));
            image[i][1] = Float(std::fmax(0, multiplier[1] * data[STBI_rgb * i + 1]));
            image[i][2] = Float(std::fmax(0, multiplier[2] * data[STBI_rgb * i + 2]));
        });
    }
    else
    {
        for (int32 i = 0; i < width * height; ++i)
        {
            image[i][0] = Float(std::fmax(0, multiplier[0] * data[STBI_rgb * i + 0]));
            image[i][1] = Float(std::fmax(0, multiplier[1] * data[STBI_rgb * i + 1]));
            image[i][2] = Float(std::fmax(0, multiplier[2] * data[STBI_rgb * i + 2]));
        }
    }

    stbi_image_free(data);
    return image;
}

Image4 ReadImage4(const std::filesystem::path& filename, bool non_color, Image4::Type multiplier)
{
    stbi_ldr_to_hdr_gamma(non_color ? 1.0f : 2.2f);

    int32 width, height;
    int32 components_per_pixel;
    float* data = stbi_loadf(filename.string().c_str(), &width, &height, &components_per_pixel, STBI_rgb_alpha);

    if (!data)
    {
        return {};
    }

    Image4 image(width, height);

    if (width * height > 64 * 1024)
    {
        ParallelFor(0, width * height, [&](int32 i) {
            image[i][0] = Float(std::fmax(0, multiplier[0] * data[STBI_rgb_alpha * i + 0]));
            image[i][1] = Float(std::fmax(0, multiplier[1] * data[STBI_rgb_alpha * i + 1]));
            image[i][2] = Float(std::fmax(0, multiplier[2] * data[STBI_rgb_alpha * i + 2]));

            if (components_per_pixel == STBI_rgb_alpha)
            {
                image[i][3] = Float(std::fmax(0, multiplier[3] * data[STBI_rgb_alpha * i + 3]));
            }
        });
    }
    else
    {
        for (int32 i = 0; i < width * height; ++i)
        {
            image[i][0] = Float(std::fmax(0, multiplier[0] * data[STBI_rgb_alpha * i + 0]));
            image[i][1] = Float(std::fmax(0, multiplier[1] * data[STBI_rgb_alpha * i + 1]));
            image[i][2] = Float(std::fmax(0, multiplier[2] * data[STBI_rgb_alpha * i + 2]));

            if (components_per_pixel == STBI_rgb_alpha)
            {
                image[i][3] = Float(std::fmax(0, multiplier[3] * data[STBI_rgb_alpha * i + 3]));
            }
        }
    }

    stbi_image_free(data);
    return image;
}

void WriteImage(const Image3& image, const std::filesystem::path& filename, ToneMappingCallback* callback)
{
    std::string extension = filename.extension().string();
    if (extension == ".hdr")
    {
        stbi_write_hdr(filename.string().c_str(), image.width, image.height, 3, &image[0].x);
    }
    else if (extension == ".jpg" || extension == ".png")
    {
        std::vector<uint8> pixels(image.width * image.height * 3);

        if (image.width * image.height > 64 * 1024)
        {
            ParallelFor(0, image.width * image.height, [&](int32 i) {
                Vec3 mapped = callback({ image[i].x, image[i].y, image[i].z });

                pixels[i * 3 + 0] = uint8(std::min(std::clamp(mapped[0], 0.0f, 1.0f) * 256.0, 255.0));
                pixels[i * 3 + 1] = uint8(std::min(std::clamp(mapped[1], 0.0f, 1.0f) * 256.0, 255.0));
                pixels[i * 3 + 2] = uint8(std::min(std::clamp(mapped[2], 0.0f, 1.0f) * 256.0, 255.0));
            });
        }
        else
        {
            for (int32 i = 0; i < image.width * image.height; ++i)
            {
                Vec3 mapped = callback({ image[i].x, image[i].y, image[i].z });

                pixels[i * 3 + 0] = uint8(std::min(std::clamp(mapped[0], 0.0f, 1.0f) * 256.0, 255.0));
                pixels[i * 3 + 1] = uint8(std::min(std::clamp(mapped[1], 0.0f, 1.0f) * 256.0, 255.0));
                pixels[i * 3 + 2] = uint8(std::min(std::clamp(mapped[2], 0.0f, 1.0f) * 256.0, 255.0));
            }
        }

        if (extension == ".jpg")
        {
            stbi_write_jpg(filename.string().c_str(), image.width, image.height, 3, &pixels[0], 100);
        }
        else
        {
            stbi_write_png(filename.string().c_str(), image.width, image.height, 3, &pixels[0], image.width * 3);
        }
    }
    else
    {
        std::cout << "Faild to write image, extention not supported: " << extension << std::endl;
        std::cout << "Supported extensions: .jpg .png .hdr" << std::endl;
    }
}

void WriteImage(const Image1& image, const std::filesystem::path& filename, ToneMappingCallback* callback)
{
    std::string extension = filename.extension().string();
    if (extension == ".hdr")
    {
        stbi_write_hdr(filename.string().c_str(), image.width, image.height, 1, &image[0]);
    }
    else if (extension == ".jpg" || extension == ".png")
    {
        std::vector<uint8> pixels(image.width * image.height * 3);

        if (image.width * image.height > 64 * 1024)
        {
            ParallelFor(0, image.width * image.height, [&](int32 i) {
                Vec3 mapped = callback({ image[i], image[i], image[i] });

                pixels[i * 3 + 0] = uint8(std::min(std::clamp(mapped[0], 0.0f, 1.0f) * 256.0, 255.0));
                pixels[i * 3 + 1] = uint8(std::min(std::clamp(mapped[1], 0.0f, 1.0f) * 256.0, 255.0));
                pixels[i * 3 + 2] = uint8(std::min(std::clamp(mapped[2], 0.0f, 1.0f) * 256.0, 255.0));
            });
        }
        else
        {
            for (int32 i = 0; i < image.width * image.height; ++i)
            {
                Vec3 mapped = callback({ image[i], image[i], image[i] });

                pixels[i * 3 + 0] = uint8(std::min(std::clamp(mapped[0], 0.0f, 1.0f) * 256.0, 255.0));
                pixels[i * 3 + 1] = uint8(std::min(std::clamp(mapped[1], 0.0f, 1.0f) * 256.0, 255.0));
                pixels[i * 3 + 2] = uint8(std::min(std::clamp(mapped[2], 0.0f, 1.0f) * 256.0, 255.0));
            }
        }

        if (extension == ".jpg")
        {
            stbi_write_jpg(filename.string().c_str(), image.width, image.height, 3, &pixels[0], 100);
        }
        else
        {
            stbi_write_png(filename.string().c_str(), image.width, image.height, 3, &pixels[0], image.width * 3);
        }
    }
    else
    {
        std::cout << "Faild to write image, extention not supported: " << extension << std::endl;
        std::cout << "Supported extensions: .jpg .png .hdr" << std::endl;
    }
}

} // namespace alzartak
