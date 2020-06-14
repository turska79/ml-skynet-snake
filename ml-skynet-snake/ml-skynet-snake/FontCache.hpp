#pragma once
#include <map>
#include <memory>
#include <SDL_ttf.h>

using FontPtr = std::unique_ptr<TTF_Font, std::integral_constant<decltype(&TTF_CloseFont), &TTF_CloseFont>>;

class FontCache
{
public:
	FontCache() noexcept {};
	TTF_Font* getFont(unsigned int fontSize);
	void clear() noexcept;
private:
	std::map<int, FontPtr> cache_;
};

