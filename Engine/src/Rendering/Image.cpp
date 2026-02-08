#include "Image.h"
#include "Utils/Log.h"

#include <stb/stb_image.h>

namespace Dawn
{
	Image::Image(const char* filename, int desiredChannels)
	{
		mData = stbi_load(filename, &mWidth, &mHeight, &mNumChannels, desiredChannels);

		if (mData == nullptr)
			LOG_ERROR("Failed to load image: %s", filename);
	}

	Image::~Image()
	{
		if (mData)
			stbi_image_free(mData);
	}
}