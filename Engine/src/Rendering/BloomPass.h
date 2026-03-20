#pragma once

#include <vector>
#include <glm/vec2.hpp>

namespace Dawn
{
	struct Mip
	{
		glm::vec2 size;
		unsigned int texture;
	};

	class BloomPass
	{
	public:
		BloomPass(unsigned int windowWidth, unsigned int windowHeight, unsigned int mipCount = 6);
		~BloomPass();

	private:
		void Init(unsigned int windowWidth, unsigned int windowHeight, unsigned int mipCount);
		void Shutdown();

	private:
		unsigned int mFBO = 0;
		unsigned int mSourceWidth = 0;
		unsigned int mSourceHeight = 0;

		std::vector<Mip> mMipChain;
	};
}