#include "SnakeVision.hpp"
#include "Board.hpp"
#include "Simulation.hpp"
#include "Renderer.hpp"

std::array<float, 24> SnakeVision::lookInAllDirections(const Board& board, const Point<std::size_t>& fromPosition, const Simulation& simulation, Renderer& renderer)
{
	const Vec2<int> left{ -1, 0 };
	const Vec2<int> leftUp{ -1, -1 };
	const Vec2<int> up{ 0, -1 };
	const Vec2<int> rightUp{ 1, -1 };
	const Vec2<int> right{ 1, 0 };
	const Vec2<int> rightDown{ 1, 1 };
	const Vec2<int> down{ 0, 1 };
	const Vec2<int> downLeft{ -1, 1 };

	constexpr unsigned int numberOfLookParameters{ 3 };
	std::array<float, 24> vision;

	std::array<float, numberOfLookParameters> look = lookInDirection(board, fromPosition, simulation, renderer, left);
	auto visionArrayIterator = vision.begin();
	std::copy(look.begin(), look.end(), visionArrayIterator);

	look = lookInDirection(board, fromPosition, simulation, renderer, leftUp);
	std::advance(visionArrayIterator, numberOfLookParameters);
	std::copy(look.begin(), look.end(), visionArrayIterator);

	look = lookInDirection(board, fromPosition, simulation, renderer, up);
	std::advance(visionArrayIterator, numberOfLookParameters);
	std::copy(look.begin(), look.end(), visionArrayIterator);

	look = lookInDirection(board, fromPosition, simulation, renderer, rightUp);
	std::advance(visionArrayIterator, numberOfLookParameters);
	std::copy(look.begin(), look.end(), visionArrayIterator);

	look = lookInDirection(board, fromPosition, simulation, renderer, right);
	std::advance(visionArrayIterator, numberOfLookParameters);
	std::copy(look.begin(), look.end(), visionArrayIterator);

	look = lookInDirection(board, fromPosition, simulation, renderer, rightDown);
	std::advance(visionArrayIterator, numberOfLookParameters);
	std::copy(look.begin(), look.end(), visionArrayIterator);

	look = lookInDirection(board, fromPosition, simulation, renderer, down);
	std::advance(visionArrayIterator, numberOfLookParameters);
	std::copy(look.begin(), look.end(), visionArrayIterator);

	look = lookInDirection(board, fromPosition, simulation, renderer, downLeft);
	std::advance(visionArrayIterator, numberOfLookParameters);
	std::copy(look.begin(), look.end(), visionArrayIterator);

	return vision;
}

std::array<float, 3> SnakeVision::lookInDirection(const Board& board, const Point<std::size_t>& fromPosition, const Simulation& simulation, Renderer& renderer, Vec2<int> direction)
{
	constexpr bool notFound(false);
	constexpr unsigned int food{ 0 };
	constexpr unsigned int body{ 1 };
	constexpr unsigned int found{ 1 };
	constexpr unsigned int distance{ 2 };

	std::array<float, 3> look{ 0 };
	Vec2<int> position(static_cast<int>(fromPosition.x_), static_cast<int>(fromPosition.y_));

	float distanceToWall{ 0 };

	position += direction;
	distanceToWall += 1;

	//while (!simulation.checkForCollisionWithWall(board, Point<std::size_t>(position.x, position.y))) {
		//if (look[food] == notFound && simulation.checkForFood(board, Point<std::size_t>(position.x, position.y))) {
		//	look[food] = found;
		//}
		
		//if (look[body] == notFound && simulation.checkForCollisionWithSnakeBody(board, Point<std::size_t>(position.x, position.y))) {
		//	look[body] = found;
		//}
		
	//	position += (direction);
	//	distanceToWall += 1;
	//}

	look[distance] = 1 / distanceToWall;

	renderer.DrawDottedLine(fromPosition.x_, fromPosition.y_, position.x, position.y);
	return look;
}
