#pragma once

// Set to true to enable
#define DEBUG_MODE true

#define DEBUG_WRAP(block) if constexpr(DEBUG_MODE) {\
	block\
}