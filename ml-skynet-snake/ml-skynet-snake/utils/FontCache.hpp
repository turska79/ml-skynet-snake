#pragma once

#include <map>
#include <memory>
#include <mutex>

#pragma warning(push)  
#pragma warning(disable : 26819 26812)
#include <SDL_ttf.h>
#pragma warning( pop )

namespace FontUtils {
	using FontPtr = std::unique_ptr<TTF_Font, std::integral_constant<decltype(&TTF_CloseFont), &TTF_CloseFont>>;

	class FontCache
	{
	public:
		FontCache() noexcept {};
		TTF_Font* getFont(unsigned int fontSize);
		void clear() noexcept;
	private:
		std::map<int, FontPtr> cache_;
		std::mutex lock_;
	};
}

