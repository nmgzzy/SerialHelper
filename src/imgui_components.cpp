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

void Demo_RealtimePlots()
{
    ImGui::BulletText("Move your mouse to change the data!");
    ImGui::BulletText("This example assumes 60 FPS. Higher FPS requires larger buffer size.");
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

    static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

    if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1, 150)))
    {
        ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
        ImPlot::PlotShaded("Mouse X", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), -INFINITY, 0, sdata1.Offset, 2 * sizeof(float));
        ImPlot::PlotLine("Mouse Y", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }
    if (ImPlot::BeginPlot("##Rolling", ImVec2(-1, 150)))
    {
        ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, 0, history, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
        ImPlot::PlotLine("Mouse X", &rdata1.Data[0].x, &rdata1.Data[0].y, rdata1.Data.size(), 0, 0, 2 * sizeof(float));
        ImPlot::PlotLine("Mouse Y", &rdata2.Data[0].x, &rdata2.Data[0].y, rdata2.Data.size(), 0, 0, 2 * sizeof(float));
        ImPlot::EndPlot();
    }
}

void Demo_DragLines()
{
    ImGui::BulletText("Click and drag the horizontal and vertical lines.");
    static double x1 = 0.2;
    static double x2 = 0.8;
    static double y1 = 0.25;
    static double y2 = 0.75;
    static double f = 0.1;
    static ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;
    ImGui::CheckboxFlags("NoCursors", (unsigned int *)&flags, ImPlotDragToolFlags_NoCursors);
    ImGui::SameLine();
    ImGui::CheckboxFlags("NoFit", (unsigned int *)&flags, ImPlotDragToolFlags_NoFit);
    ImGui::SameLine();
    ImGui::CheckboxFlags("NoInput", (unsigned int *)&flags, ImPlotDragToolFlags_NoInputs);
    if (ImPlot::BeginPlot("##lines", ImVec2(-1, 0)))
    {
        ImPlot::SetupAxesLimits(0, 1, 0, 1);
        ImPlot::DragLineX(0, &x1, ImVec4(1, 1, 1, 1), 1, flags);
        ImPlot::DragLineX(1, &x2, ImVec4(1, 1, 1, 1), 1, flags);
        ImPlot::DragLineY(2, &y1, ImVec4(1, 1, 1, 1), 1, flags);
        ImPlot::DragLineY(3, &y2, ImVec4(1, 1, 1, 1), 1, flags);
        double xs[1000], ys[1000];
        for (int i = 0; i < 1000; ++i)
        {
            xs[i] = (x2 + x1) / 2 + fabs(x2 - x1) * (i / 1000.0f - 0.5f);
            ys[i] = (y1 + y2) / 2 + fabs(y2 - y1) / 2 * sin(f * i / 10);
        }
        ImPlot::PlotLine("Interactive Data", xs, ys, 1000);
        ImPlot::DragLineY(120482, &f, ImVec4(1, 0.5f, 1, 1), 1, flags);
        ImPlot::EndPlot();
    }
}