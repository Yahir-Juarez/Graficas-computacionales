#include "ImageProcess.h"
#include "ImageProcess/Image.h"

void ImageProcess::createFile(const Path path)
{
    g_CodecMan.AddCodec(new BMPCodec());
    Image image;
    image.CreateFromImageFile(path);
    Image imageRotate;
    Image imageRasterRotate;
    imageRotate.rotate(127, image);
    imageRasterRotate.rotate(237, image);

    auto codec = g_CodecMan.GetCodecByFileExt(".bmp");
    codec->Encode(imageRotate, "StarWarsRotate.bmp");
}