#include "imgui_components.h"
#include <sstream>
#include "serial_config.h"

void ComboConfig::ListString()
{
    std::stringstream tmp;
    for (std::string_view i : list)
    {
        tmp << i << '\0';
    }
    tmp << '\0';
    list_str = tmp.str();
}

void ComPortComboConfig::Refresh()
{
    SerialEnumeratePorts(port_list);
    list.clear();
    for (int i = 0; i < port_list.size(); i++)
    {
        list.emplace_back(std::string_view(port_list[i]));
    }
    ListString();
}

void ShowUartConfig()
{
    static ComPortComboConfig com_port;
    static BaudRateComboConfig baud_rate;
    static DataBitsComboConfig data_bits;
    static ParityComboConfig parity;
    static StopBitsComboConfig stop_bits;

    if (ImGui::Combo(com_port.label.c_str(), &com_port.choice, com_port.list_str.c_str()))
    {
        // TODO
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("Re"))
    {
        com_port.Refresh();
    }
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

    if (ImGui::Button(GetText("OPEN").c_str(), ImVec2(-1, 0)))
    {
        // TODO
    }
}

// utility structure for realtime plot
struct ScrollingBuffer
{
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000)
    {
        MaxSize = max_size;
        Offset = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y)
    {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x, y));
        else
        {
            Data[Offset] = ImVec2(x, y);
            Offset = (Offset + 1) % MaxSize;
        }
    }
    void Erase()
    {
        if (Data.size() > 0)
        {
            Data.shrink(0);
            Offset = 0;
        }
    }
};

struct RollingBuffer
{
    float Span;
    ImVector<ImVec2> Data;
    RollingBuffer()
    {
        Span = 10.0f;
        Data.reserve(2000);
    }
    void AddPoint(float x, float y)
    {
        float xmod = fmodf(x, Span);
        if (!Data.empty() && xmod < Data.back().x)
            Data.shrink(0);
        Data.push_back(ImVec2(xmod, y));
    }
};

static void Demo_RealtimePlots()
{
    static int cnt = 0;
    static ScrollingBuffer sdata1, sdata2;
    static RollingBuffer rdata1, rdata2;
    ImVec2 mouse = ImGui::GetMousePos();
    static float t = 0;
    t += ImGui::GetIO().DeltaTime;
    sdata1.AddPoint(t, mouse.x * 0.0005f);
    rdata1.AddPoint(t, mouse.x * 0.0005f);
    sdata2.AddPoint(t, mouse.y * 0.0005f);
    rdata2.AddPoint(t, mouse.y * 0.0005f);

    static float history = 10.0f;
    ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");
    rdata1.Span = history;
    rdata2.Span = history;

    static ImPlotAxisFlags flags = 0;

    if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1, -1)))
    {
        ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
        ImPlot::PlotLine("Mouse X", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), 0, sdata1.Offset, 2 * sizeof(float));
        ImPlot::PlotLine("Mouse Y", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }
}

// void Demo_DragLines()
//{
//     static double x1 = 0.2;
//     static double x2 = 0.8;
//     static double y1 = 0.25;
//     static double y2 = 0.75;
//     static double f = 0.1;
//     static ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;
//     ImGui::CheckboxFlags("NoCursors", (unsigned int *)&flags, ImPlotDragToolFlags_NoCursors);
//     ImGui::SameLine();
//     ImGui::CheckboxFlags("NoFit", (unsigned int *)&flags, ImPlotDragToolFlags_NoFit);
//     ImGui::SameLine();
//     ImGui::CheckboxFlags("NoInput", (unsigned int *)&flags, ImPlotDragToolFlags_NoInputs);
//     if (ImPlot::BeginPlot("##lines", ImVec2(-1, 0)))
//     {
//         ImPlot::SetupAxesLimits(0, 1, 0, 1);
//         ImPlot::DragLineX(0, &x1, ImVec4(1, 1, 1, 1), 1, flags);
//         ImPlot::DragLineX(1, &x2, ImVec4(1, 1, 1, 1), 1, flags);
//         ImPlot::DragLineY(2, &y1, ImVec4(1, 1, 1, 1), 1, flags);
//         ImPlot::DragLineY(3, &y2, ImVec4(1, 1, 1, 1), 1, flags);
//         double xs[1000], ys[1000];
//         for (int i = 0; i < 1000; ++i)
//         {
//             xs[i] = (x2 + x1) / 2 + fabs(x2 - x1) * (i / 1000.0f - 0.5f);
//             ys[i] = (y1 + y2) / 2 + fabs(y2 - y1) / 2 * sin(f * i / 10);
//         }
//         ImPlot::PlotLine("Interactive Data", xs, ys, 1000);
//         ImPlot::DragLineY(120482, &f, ImVec4(1, 0.5f, 1, 1), 1, flags);
//         ImPlot::EndPlot();
//     }
// }

int input_cb(ImGuiInputTextCallbackData *data)
{
    SDL_Log("callback");
    return 0;
}

void ShowTextWindow()
{
    ImGui::Begin("TextWindow");
    static bool enable_text = true;
    ImGui::Checkbox(GetText("ENABLE").c_str(), &enable_text);
    float height = ImGui::GetWindowHeight() - 5 * ImGui::GetFrameHeight();
    static char bufferr[100] = {0};
    ImGui::InputTextMultiline("##ReadText", bufferr, 100, ImVec2(-1, 0.7f * height), ImGuiInputTextFlags_AllowTabInput, input_cb);
    ImGui::Separator();
    static char bufferw[100] = {0};
    ImGui::InputTextMultiline("##WriteText", bufferw, 100, ImVec2(-1, 0.3f * height), ImGuiInputTextFlags_AllowTabInput, input_cb);
    if (ImGui::Button(GetText("SEND").c_str()))
    {
        // TODO
        SDL_Log("Send Button");
    }
    ImGui::End();
}

void ShowPlottingWindow()
{
    ImGui::Begin("PlottingWindow");
    static bool enable_chart = false;
    ImGui::Checkbox(GetText("ENABLE").c_str(), &enable_chart);
    Demo_RealtimePlots();
    ImGui::End();
}

void ShowProfessionalWindow()
{
    ImGui::Begin("ProfessionalWindow");

    ImGui::End();
}

void ShowHelpWindow()
{
    ImGui::Begin("HelpWindow");

    ImGui::End();
}