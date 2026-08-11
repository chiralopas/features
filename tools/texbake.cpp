/* texbake: encode the image with libsquish to S3TC (DXT1, no alpha)
 * writes a header followed by the raw compressed blocks to .tex
 * usage: 'texbake <input.jpg> <output.tex>' */

#include <cstdio>
#include <cstdint>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <squish.h>
#include "tex_header.h"


int main(int argc, char** argv)
{
    if (argc != 3) return 2;

    const char* input = argv[1];
    const char* output = argv[2];

    int width, height;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* source = stbi_load(input, &width, &height, nullptr, 4);

    printf("compressing %s ...\n", input);
    int flags = squish::kDxt1 | squish::kColourRangeFit;
    std::vector<uint8_t> blocks(squish::GetStorageRequirements(width, height, flags));
    squish::CompressImage(source, width, height, blocks.data(), flags);

    tex_header hdr = {
        width, height, blocks.size()
    };

    FILE* file = fopen(output, "wb");
    fwrite(&hdr, sizeof(hdr), 1, file);
    fwrite(blocks.data(), blocks.size(), 1, file);
    fclose(file);
    stbi_image_free(source);

    printf("done: %s\n", output);
    return 0;
}