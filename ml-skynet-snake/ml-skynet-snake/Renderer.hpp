#pragma once

#include <memory>
#include <list>

#pragma warning(push)  
#pragma warning(disable : 26819 26812)
#include <SDL.h>
#include <SDL_ttf.h>
#pragma warning( pop )

#include <string>
#include "Board.hpp"

class Renderer
{
public:
	Renderer(std::size_t windowWidth, std::size_t windowHeight, std::size_t gridStartOffset, SDL_Color& background) noexcept;

	void renderBackground() noexcept;
	void renderCells(const std::list<std::unique_ptr<Cell>>& cells);
	void renderText(const unsigned int x, const unsigned int y, const std::string& text, TTF_Font &font, const SDL_Color& color) noexcept;
	void present() noexcept;
	void clear() noexcept;
	void DrawDottedLine(int x0, int y0, int x1, int y1);
	
	using WindowPtr = std::unique_ptr<SDL_Window, std::integral_constant<decltype(&SDL_DestroyWindow), &SDL_DestroyWindow>>;
	using RendererPtr = std::unique_ptr<SDL_Renderer, std::integral_constant<decltype(&SDL_DestroyRenderer), &SDL_DestroyRenderer>>;
	using SurfacePtr = std::unique_ptr<SDL_Surface, std::integral_constant<decltype(&SDL_FreeSurface), &SDL_FreeSurface>>;
	using TexturePtr = std::unique_ptr<SDL_Texture, std::integral_constant<decltype(&SDL_DestroyTexture), &SDL_DestroyTexture>>;
private:
	WindowPtr window_;
	RendererPtr renderer_;

	std::size_t windowWidth_;
	std::size_t windowHeight_;
	std::size_t gridStartOffset_;
	SDL_Color backGround_;
};

