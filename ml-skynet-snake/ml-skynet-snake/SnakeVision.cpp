#include "SnakeVision.hpp"
#include "Board.hpp"
//#include "Renderer.hpp"
//#include "utils/Utils.hpp"

const utils::Vec2<int> left{ -1, 0 };
const utils::Vec2<int> leftUp{ -1, -1 };
const utils::Vec2<int> up{ 0, -1 };
const utils::Vec2<int> rightUp{ 1, -1 };
const utils::Vec2<int> right{ 1, 0 };
const utils::Vec2<int> rightDown{ 1, 1 };
const utils::Vec2<int> down{ 0, 1 };
const utils::Vec2<int> downLeft{ -1, 1 };

std::array<float, 24> SnakeVision::lookInAllDirections(Board& board, const utils::Point<std::size_t> fromPosition)
{
	std::array<float, 24> vision;
	constexpr unsigned int numberOfLookParameters{ 3 };
	
	std::array<float, numberOfLookParameters> look = lookInDirection(board, fromPosition, left);
	auto visionArrayIterator = vision.begin();
	std::copy(look.begin(), look.end(), visionArrayIterator);

	look = lookInDirection(board, fromPosition, leftUp);
	std::advance(visionArrayIterator, numberOfLookParameters);
	std::copy(look.begin(), look.end(), visionArrayIterator);

	look = lookInDirection(board, fromPosition, up);
	std::advance(visionArrayIterator, numberOfLookParameters);
	std::copy(look.begin(), look.end(), visionArrayIterator);

	look = lookInDirection(board, fromPosition, rightUp);
	std::advance(visionArrayIterator, numberOfLookParameters);
	std::copy(look.begin(), look.end(), visionArrayIterator);

	look = lookInDirection(board, fromPosition, right);
	std::advance(visionArrayIterator, numberOfLookParameters);
	std::copy(look.begin(), look.end(), visionArrayIterator);

	look = lookInDirection(board, fromPosition, rightDown);
	std::advance(visionArrayIterator, numberOfLookParameters);
	std::copy(look.begin(), look.end(), visionArrayIterator);

	look = lookInDirection(board, fromPosition, down);
	std::advance(visionArrayIterator, numberOfLookParameters);
	std::copy(look.begin(), look.end(), visionArrayIterator);

	look = lookInDirection(board, fromPosition, downLeft);
	std::advance(visionArrayIterator, numberOfLookParameters);
	std::copy(look.begin(), look.end(), visionArrayIterator);

	return vision;
}

std::array<float, 3> SnakeVision::lookInDirection(Board& board, const utils::Point<std::size_t> fromPosition, utils::Vec2<int> direction)
{
	constexpr bool notFound(false);
	constexpr unsigned int food{ 0 };
	constexpr unsigned int body{ 1 };
	constexpr unsigned int found{ 1 };
	constexpr unsigned int distance{ 2 };

	std::array<float, 3> look{ 0 };
	utils::Vec2<int> position(static_cast<int>(fromPosition.x_), static_cast<int>(fromPosition.y_));

	float distanceToWall{ 1 };

	position += direction;
	
	if (position.x < 0) position.x = 0;
	if (position.y < 0) position.y = 0;

	while (!board.isWall(utils::Point<std::size_t>(position.x, position.y))) {
		if (look[food] == notFound && board.isFood(utils::Point<std::size_t>(position.x, position.y))) {
			look[food] = found;
		}
		
		if (look[body] == notFound && board.isSnakeBody(utils::Point<std::size_t>(position.x, position.y))) {
			look[body] = found;
		}
		
		position += (direction);
		distanceToWall += 1;
	}

	look[distance] = 1 / distanceToWall;

	return look;
}

std::list<VisionPoints> SnakeVision::pointsForRendering(Board& board, const utils::Point<std::size_t> fromPosition)
{
	std::list<VisionPoints> points;
	VisionPoints point;

	points.emplace_back(pointsForRenderingDirection(board, fromPosition, left));
	points.emplace_back(pointsForRenderingDirection(board, fromPosition, leftUp));
	points.emplace_back(pointsForRenderingDirection(board, fromPosition, up));
	points.emplace_back(pointsForRenderingDirection(board, fromPosition, rightUp));
	points.emplace_back(pointsForRenderingDirection(board, fromPosition, right));
	points.emplace_back(pointsForRenderingDirection(board, fromPosition, rightDown));
	points.emplace_back(pointsForRenderingDirection(board, fromPosition, down));
	points.emplace_back(pointsForRenderingDirection(board, fromPosition, downLeft));
	points.emplace_back(pointsForRenderingDirection(board, fromPosition, left));

	return points;
}

VisionPoints SnakeVision::pointsForRenderingDirection(Board& board, const utils::Point<std::size_t> fromPosition, const utils::Vec2<int> direction)
{
	utils::Vec2<int> position(static_cast<int>(fromPosition.x_), static_cast<int>(fromPosition.y_));
	
	position += direction;

	while (!board.isWall(utils::Point<std::size_t>(position.x, position.y))) {
		position += (direction);
	}

	return VisionPoints(FromPoint(fromPosition), ToPoint(position.x, position.y));
}
