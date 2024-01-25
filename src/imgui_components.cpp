#include "imgui_components.h"
#include "imgui.h"
#include <SDL.h>

void ShowUartConfig()
{
    static BaudRateComboConfig baud_rate;
    static DataBitsComboConfig data_bits;
    static ParityComboConfig parity;
    static StopBitsComboConfig stop_bits;

    if (ImGui::Combo(baud_rate.label.c_str(), &baud_rate.choice, baud_rate.list_str.c_str()))
    {
        // TODO
    }
    if (ImGui::Combo(data_bits.label.c_str(), &data_bits.choice, data_bits.list_str.c_str()))
    {
        // TODO
    }
    if (ImGui::Combo(parity.label.c_str(), &parity.choice, parity.list_str.c_str()))
    {
        // TODO
    }
    if (ImGui::Combo(stop_bits.label.c_str(), &stop_bits.choice, stop_bits.list_str.c_str()))
    {
        // TODO
    }

    if (ImGui::Button(GetText("OPEN").c_str(), ImVec2(ImGui::GetWindowWidth(), 0)))
    {
        // TODO
    }
}