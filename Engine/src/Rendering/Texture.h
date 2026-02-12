#pragma once

namespace Dawn
{
	class Texture
	{
	public:
		Texture(const class Image* image);
		~Texture();

		void Bind(int slot = 0);

		bool IsValid() const { return mId != 0; }

	private:
		bool Load(const class Image* image);

	private:
		unsigned int mId = 0;

		int mWidth = 0;
		int mHeight = 0;
		int mNumChannels = 0;
	};
}