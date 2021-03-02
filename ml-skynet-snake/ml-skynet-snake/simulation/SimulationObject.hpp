#pragma once

#include <cstdint>

class simulationObject {
public:
	virtual void update(const uint32_t delta) noexcept = 0;
};
