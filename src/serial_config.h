#pragma once
#include "common.h"
#include "imgui_components.h"

class SerialConfig
{
public:
	ComboConfig port{ "PORT" };
	ComboConfig baud_rate{ "BAUD_RATE" };
	ComboConfig data_bits{ "DATA_BITS" };
	ComboConfig parity{ "PARITY" };
	ComboConfig stop_bits{ "STOP_BITS" };
	bool is_open = false;
};