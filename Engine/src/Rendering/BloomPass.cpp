#include "BloomPass.h"
#include "Utils/Log.h"

#include <glad/glad.h>

namespace Dawn
{
	BloomPass::BloomPass(unsigned int windowWidth, unsigned int windowHeight, unsigned int mipCount)
	{
		Init(windowWidth, windowHeight, mipCount);
	}

	BloomPass::~BloomPass()
	{
		Shutdown();
	}
	
	void BloomPass::Init(unsigned int windowWidth, unsigned int windowHeight, unsigned int mipCount)
	{
		mSourceWidth = windowWidth;
		mSourceHeight = windowHeight;

		// Init FBO
		glGenFramebuffers(1, &mFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

		glm::vec2 mipSize = glm::vec2(mSourceWidth / 2.0f, mSourceHeight / 2.0f);

		// Create Mips
		for (unsigned int i = 0; i < mipCount; i++)
		{
			if (mipSize.x < 2.0f || mipSize.y < 2.0f)
				break;

			Mip mip;
			mip.size = mipSize;

			glGenTextures(1, &mip.texture);
			glBindTexture(GL_TEXTURE_2D, mip.texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F, 
				static_cast<GLsizei>(mip.size.x), static_cast<GLsizei>(mip.size.y), 
				0, GL_RGB, GL_FLOAT, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			mMipChain.push_back(mip);
		}

		// Check FrameBuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mMipChain[0].texture, 0);

		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			LOG_ERROR("BloomPass FBO incomplete: 0x%x", status);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return;
		}

		LOG_INFO("BloomPass initialized: %d mip count", mMipChain.size());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void BloomPass::Shutdown()
	{
		for (Mip& mip : mMipChain)
			if (mip.texture)
				glDeleteTextures(1, &mip.texture);
		mMipChain.clear();

		if (mFBO)
			glDeleteFramebuffers(1, &mFBO);
	}
}