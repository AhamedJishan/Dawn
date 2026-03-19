#pragma once

namespace Dawn
{
	class HDRFramebuffer
	{
	public:
		HDRFramebuffer();
		~HDRFramebuffer();

		void BindFrameBuffer();
		void BindTexture();

	private:
		void Init(int width, int height);

	private:
		unsigned int mId = 0;
		unsigned int mColorTexture = 0;
		unsigned int mDepthRBO = 0;
	};
}