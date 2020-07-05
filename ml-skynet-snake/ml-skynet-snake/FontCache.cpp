#include "FontCache.hpp"

TTF_Font* FontCache::getFont(unsigned int fontSize)
{
	const std::lock_guard<std::mutex> lock(lock);
	if (cache_.find(fontSize) == cache_.end()) {
		FontPtr font(TTF_OpenFont("C:\\WINDOWS\\Fonts\\Calibrib.ttf", fontSize));
		cache_[fontSize] = std::move(font);
	}

	return cache_[fontSize].get();
}

void FontCache::clear() noexcept
{
	const std::lock_guard<std::mutex> lock(lock);
	cache_.clear();
}
