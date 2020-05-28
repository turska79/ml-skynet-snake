#include "Renderer.hpp"
#include <SDL.h>

Renderer::Renderer(std::size_t windowWidth, std::size_t windowHeight, SDL_Color& background) noexcept : windowWidth_(windowWidth), windowHeight_(windowHeight), backGround_(background)
{
	WindowPtr window(SDL_CreateWindow("SkyNet Snake", 100, 100, static_cast<int>(windowWidth_), static_cast<int>(windowHeight_), SDL_WINDOW_SHOWN));
	window_ = std::move(window);

	RendererPtr renderer(SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED));
	renderer_ = std::move(renderer);
}


void Renderer::renderText(const unsigned int x, const unsigned int y, const std::string& text, TTF_Font &font, const SDL_Color& color) noexcept
{
	int text_width{ 0 };
	int text_height{ 0 };
	
	SurfacePtr surface(TTF_RenderText_Solid(&font, text.c_str(), color));
	TexturePtr texture(SDL_CreateTextureFromSurface(renderer_.get(), surface.get()));

	text_width = surface->w;
	text_height = surface->h;

	SDL_Rect textArea;
	textArea.x = x;
	textArea.y = y;
	textArea.w = text_width;
	textArea.h = text_height;

	SDL_RenderCopy(renderer_.get(), texture.get(), nullptr, &textArea);
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
		cellArea.x = static_cast<int>(cell->x_ * cellWidth);
		cellArea.y = static_cast<int>(cell->y_ * cellHeight);
		cellArea.w = cellWidth;
		cellArea.h = cellHeight;

		if (cell->type_ == Cell::Type::wall) {
			SDL_SetRenderDrawColor(renderer_.get(), 105, 105, 105, 255);
			SDL_RenderFillRect(renderer_.get(), &cellArea);
		} else if (cell->type_ == Cell::Type::head) {
			SDL_SetRenderDrawColor(renderer_.get(), 220, 20, 60, 255);
			SDL_RenderFillRect(renderer_.get(), &cellArea);
		} else if (cell->type_ == Cell::Type::body) {
			SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 0, 255);
			SDL_RenderFillRect(renderer_.get(), &cellArea);
		} else if (cell->type_ == Cell::Type::food) {
			SDL_SetRenderDrawColor(renderer_.get(), 0, 128, 0, 255);
			SDL_RenderFillRect(renderer_.get(), &cellArea);
		}
	}
}

void Renderer::present() noexcept
{
	SDL_RenderPresent(renderer_.get());
}

void Renderer::clear() noexcept
{
	SDL_RenderClear(renderer_.get());
}