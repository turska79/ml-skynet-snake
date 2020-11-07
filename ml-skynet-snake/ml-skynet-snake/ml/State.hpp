#pragma once

#pragma warning(push)  
#pragma warning(disable : 26819 26495 4244 26451 6011 26439 26812 4458 4267 4801 4081)
#include <ensmallen.hpp>
#pragma warning( pop )

#include <array>
#include "../utils/Point.hpp"

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
			return utils::Point<std::size_t> { static_cast<std::size_t>(data_[24]), static_cast<std::size_t>(data_[25]) };
		}
		
		double& x()
		{
			return data_[24];
		}

		double& y()
		{
			return data_[25];
		}

		double& directionX()
		{
			return data_[26];
		}

		double& directionY()
		{
			return data_[27];
		}

		static constexpr size_t dimension{ 28 };

	private:
		arma::colvec data_;
	};
}