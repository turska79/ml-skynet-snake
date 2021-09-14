#pragma once

#pragma warning(push)  
#pragma warning(disable : 26819 26495 4244 26451 6011 26439 26812 4458 4267 4801 4081)
#include <ensmallen.hpp>
#pragma warning( pop )

#include <array>
#include "../utils/Point.hpp"
#include "../utils/Utils.hpp"
#include "../utils/vec2.hpp"

namespace ml {

	class State
	{
	public:
		State() : data_(dimension)
		{
		}

		State(const arma::colvec& data) : data_(data)
		{
		}

		arma::colvec& Data()
		{
			return data_;
		}

		void setData(const arma::colvec& newData)
		{
			data_ = newData;
		}

		const arma::colvec& Encode() const
		{
			return data_;
		}

		utils::Point<std::size_t> position() const
		{
			return utils::Point<std::size_t> { static_cast<std::size_t>(data_[utils::commonConstants::ml::inputParameters::snake_head_x] * 32), static_cast<std::size_t>(data_[utils::commonConstants::ml::inputParameters::snake_head_y] * 32) };
		}

		void position(const utils::Point<std::size_t> newPosition)
		{
			data_[utils::commonConstants::ml::inputParameters::snake_head_x] = newPosition.x_ / 32.0;
			data_[utils::commonConstants::ml::inputParameters::snake_head_y] = newPosition.y_ / 32.0;
		}

		utils::Point<std::size_t> foodPosition() const
		{
			return utils::Point<std::size_t> { static_cast<std::size_t>(data_[utils::commonConstants::ml::inputParameters::food_x] * 32), static_cast<std::size_t>(data_[utils::commonConstants::ml::inputParameters::food_y] * 32) };
		}

		void foodPosition(const utils::Point<std::size_t> newPosition)
		{
			data_[utils::commonConstants::ml::inputParameters::food_x] = newPosition.x_ / 32.0;
			data_[utils::commonConstants::ml::inputParameters::food_y] = newPosition.y_ / 32.0;
		}

		unsigned int step() const
		{
			return step_;
		}

		unsigned int& step()
		{
			return step_;
		}

		utils::Vec2<double> directionVector() const
		{
			//return utils::Vec2<double>(data_[utils::commonConstants::ml::inputParameters::direction_vector_x], data_[utils::commonConstants::ml::inputParameters::direction_vector_y]);
			return utils::Vec2<double>();
		}

		void directionVector(const utils::Vec2<double> direction)
		{
			data_[utils::commonConstants::ml::inputParameters::direction_vector_x] = 0;//direction.x;
			data_[utils::commonConstants::ml::inputParameters::direction_vector_y] = 0;//direction.y;
		}

		static constexpr size_t dimension{ utils::commonConstants::ml::inputParameters::count };

	private:
		arma::colvec data_;
		unsigned int step_{ 0 };
	};
}