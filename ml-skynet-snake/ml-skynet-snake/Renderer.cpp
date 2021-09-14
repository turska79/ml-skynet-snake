#include "Renderer.hpp"

#pragma warning(push)  
#pragma warning(disable : 26819 26812)
#include <SDL.h>
#pragma warning(pop)

#include <vector>
#include "utils/FontCache.hpp"
#include "utils/Utils.hpp"

FontUtils::FontCache fontCache;

Renderer::Renderer(std::size_t windowWidth, std::size_t windowHeight, std::size_t gridStartOffset, SDL_Color& background) noexcept : windowWidth_(windowWidth), windowHeight_(windowHeight), gridStartOffset_(gridStartOffset), backGround_(background)
{
	WindowPtr window(SDL_CreateWindow("SkyNet Snake", 100, 100, static_cast<int>(windowWidth_), static_cast<int>(windowHeight_), SDL_WINDOW_SHOWN));
	window_ = std::move(window);

	RendererPtr renderer(SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
	renderer_ = std::move(renderer);
}

void Renderer::renderText(const unsigned int x, const unsigned int y, const std::string text, TTF_Font &font, const SDL_Color& color) noexcept
{
	SurfacePtr surface(TTF_RenderText_Solid(&font, text.c_str(), color));
	TexturePtr texture(SDL_CreateTextureFromSurface(renderer_.get(), surface.get()));

	const int text_width = surface->w;
	const int text_height = surface->h;

	SDL_Rect textArea;
	textArea.x = x;
	textArea.y = y;
	textArea.w = text_width;
	textArea.h = text_height;

	SDL_RenderCopy(renderer_.get(), texture.get(), nullptr, &textArea);
}

void Renderer::renderText(const std::string text)
{
	TTF_Font& font{ *fontCache.getFont(utils::commonConstants::fontSize::twenty) };
	const SDL_Color color{ utils::commonConstants::color::black };
	
	SurfacePtr surface(TTF_RenderText_Solid(&font, text.c_str(), color));
	TexturePtr texture(SDL_CreateTextureFromSurface(renderer_.get(), surface.get()));

	renderText(0, textsRendered_ * 20, text, *fontCache.getFont(utils::commonConstants::fontSize::twenty), utils::commonConstants::color::black);

	textsRendered_++;

}

void Renderer::renderBackground() noexcept
{
	SDL_SetRenderDrawColor(renderer_.get(), backGround_.r, backGround_.g, backGround_.b, backGround_.a);
	SDL_RenderClear(renderer_.get());
}

void Renderer::renderCells(const std::list<std::unique_ptr<Cell>>& cells)
{
	constexpr unsigned int cellWidth{ 20 };
	constexpr unsigned int cellHeight{ 20 };
	
	for (const auto& cell : cells) {
		SDL_Rect cellArea;
		cellArea.x = static_cast<int>(cell->x_ * cellWidth + gridStartOffset_);
		cellArea.y = static_cast<int>(cell->y_ * cellHeight);
		cellArea.w = cellWidth;
		cellArea.h = cellHeight;

		if (cell->type_ == Cell::Type::wall) {
			SDL_SetRenderDrawColor(renderer_.get(), 105, 105, 105, 255);
			SDL_RenderFillRect(renderer_.get(), &cellArea);
		} else if (cell->type_ == Cell::Type::head) {
			SDL_SetRenderDrawColor(renderer_.get(), 220, 20, 60, 255);
			SDL_RenderFillRect(renderer_.get(), &cellArea);
			SDL_SetRenderDrawColor(renderer_.get(), 255, 255, 255, 255);
			SDL_RenderDrawRect(renderer_.get(), &cellArea);
		} else if (cell->type_ == Cell::Type::body) {
			SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 0, 255);
			SDL_RenderFillRect(renderer_.get(), &cellArea);
			SDL_SetRenderDrawColor(renderer_.get(), 255, 255, 255, 255);
			SDL_RenderDrawRect(renderer_.get(), &cellArea);
		} else if (cell->type_ == Cell::Type::food) {
			SDL_SetRenderDrawColor(renderer_.get(), 0, 128, 0, 255);
			SDL_RenderFillRect(renderer_.get(), &cellArea);
		}
	}
}

void Renderer::present() noexcept
{
	SDL_RenderPresent(renderer_.get());
	textsRendered_ = 0;
}

void Renderer::clear() noexcept
{
	
	SDL_RenderClear(renderer_.get());
}

void Renderer::DrawDottedLine(int x0, int y0, int x1, int y1)
{
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;

	int count{ 0 };
	std::vector<SDL_Point> points;

	while (1) {
		points.emplace_back(SDL_Point{ x0 * 20 + static_cast<int>(gridStartOffset_) + 10, y0 * 20 + 10 });

		if (x0 == x1 && y0 == y1)
			break;
		
		e2 = 2 * err;
		
		if (e2 > dy) {
			err += dy; 
			x0 += sx;
		}

		if (e2 < dx) { 
			err += dx;
			y0 += sy;
		}

		count = (count + 1) % 20;
	}

	SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 0, 255);

	SDL_Point* pointsToDraw = &points[0];
	SDL_RenderDrawPoints(renderer_.get(), pointsToDraw, points.size());
}