#pragma once

#pragma warning(push)  
#pragma warning(disable : 26819 26495 4244 26451 6011 26439 26812 4458 4267 4801 4081)
#include <ensmallen.hpp>
#pragma warning( pop )

#include <array>
#include "../utils/Point.hpp"
#include "../utils/Utils.hpp"

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
		/*
		std::array<double, 24> vision() const
		{
			std::array<double, 24> vision{ 0 };
			auto iter = std::begin(data_);
			std::advance(iter, 24);
			std::copy(std::begin(data_), iter, std::begin(vision));
			return vision;
		}*/

		const arma::colvec& Encode() const
		{
			return data_;
		}

		utils::Point<std::size_t> position() const
		{
			return utils::Point<std::size_t> { static_cast<std::size_t>(data_[utils::commonConstants::ml::inputParameters::snake_head_x_in_data]), static_cast<std::size_t>(data_[utils::commonConstants::ml::inputParameters::snake_head_y_in_data]) };
		}

		double& x()
		{
			return data_[utils::commonConstants::ml::inputParameters::snake_head_x_in_data];
		}

		double& y()
		{
			return data_[utils::commonConstants::ml::inputParameters::snake_head_y_in_data];
		}

		double& directionVectorX()
		{
			return data_[utils::commonConstants::ml::inputParameters::direction_vector_x];
		}

		double& directionVectorY()
		{
			return data_[utils::commonConstants::ml::inputParameters::direction_vector_y];
		}

		utils::Point<std::size_t> foodPosition() const
		{
			return utils::Point<std::size_t> { static_cast<std::size_t>(data_[utils::commonConstants::ml::inputParameters::food_x]), static_cast<std::size_t>(data_[utils::commonConstants::ml::inputParameters::food_y]) };
		}

		double& foodPositionX()
		{
			return data_[utils::commonConstants::ml::inputParameters::food_x];
		}

		double& foodPositionY()
		{
			return data_[utils::commonConstants::ml::inputParameters::food_y];
		}

		static constexpr size_t dimension{ utils::commonConstants::ml::inputParameters::count };

	private:
		arma::colvec data_;
	};
}