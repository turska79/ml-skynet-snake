#include "FontCache.hpp"

TTF_Font* FontUtils::FontCache::getFont(unsigned int fontSize)
{
	const std::lock_guard<std::mutex> guard(lock_);
	if (cache_.find(fontSize) == cache_.end()) {
		FontPtr font(TTF_OpenFont("C:\\WINDOWS\\Fonts\\Calibrib.ttf", fontSize));
		cache_[fontSize] = std::move(font);
	}

	return cache_[fontSize].get();
}

void FontUtils::FontCache::clear() noexcept
{
	const std::lock_guard<std::mutex> guard(lock_);
	cache_.clear();
}
