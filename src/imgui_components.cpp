#include "imgui_components.h"
#include "imgui.h"
#include <SDL.h>

void ShowUartConfig()
{
	static BaudRateComboConfig baud_rate;
    if (ImGui::Combo(baud_rate.label.c_str(), &baud_rate.choice, baud_rate.list_str.c_str()))
    {
        // TODO
    }

    if (ImGui::Button(GetText("OPEN").c_str()))
    {
        // TODO
    }
}