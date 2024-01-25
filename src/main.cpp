#include "common.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "imgui_components.h"
#include "serial_config.h"
#include "language.h"

#if !SDL_VERSION_ATLEAST(2, 0, 17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

const int base_size = 20;
std::string init_language = u8"中文";

int input_cb(ImGuiInputTextCallbackData *data)
{
    SDL_Log("callback");
    return 0;
}

// Main code
int main(int, char **)
{
    InitLanguage(init_language);

    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        SDL_Log("Error: %s\n", SDL_GetError());
        return -1;
    }

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("SerialHelper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    if (window == nullptr)
    {
        SDL_Log("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return 0;
    }
    // SDL_RendererInfo info;
    // SDL_GetRendererInfo(renderer, &info);
    // SDL_Log("Current SDL_Renderer: %s", info.name);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    // Load Fonts
    ImFontConfig config;
    config.MergeMode = true;
    io.Fonts->AddFontFromFileTTF("res/DroidSans.ttf", base_size);
    ImFont *font = io.Fonts->AddFontFromFileTTF("res/DroidSansFallback.ttf", base_size, &config, io.Fonts->GetGlyphRangesChineseFull());

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //////////////////////////////////////////
    CommunicateMethodComboConfig method;
    //////////////////////////////////////////

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // my code
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar);
        if (1)
        {
            ImGui::Begin("ConfigWindow");

            if (ImGui::Combo(method.label.c_str(), &method.choice, method.list_str.c_str()))
            {
                // TODO
            }

            switch (method.choice)
            {
            case CommunicateMethodComboConfig::COMMUNICATE_SERIAL:
                ShowUartConfig();
                break;
            case CommunicateMethodComboConfig::COMMUNICATE_TCP_SERVER:

                break;
            case CommunicateMethodComboConfig::COMMUNICATE_TCP_CLIENT:

                break;
            case CommunicateMethodComboConfig::COMMUNICATE_UDP:

                break;
            default:
                SDL_Log("Warning: wrone current_communicate_method!");
                break;
            }
            ImGui::Separator();

            ImGui::End();
        }
        if (2)
        {
            ImGui::Begin("TextWindow");
            static bool enable_text = true;
            ImGui::Checkbox(GetText("ENABLE").c_str(), &enable_text);
            float width = ImGui::GetWindowWidth();
            float height = ImGui::GetWindowHeight() - 5 * ImGui::GetFrameHeight();
            static char bufferr[100] = {0};
            ImGui::InputTextMultiline("##ReadText", bufferr, 100, ImVec2(width, 0.7f * height), ImGuiInputTextFlags_AllowTabInput, input_cb);
            ImGui::Separator();
            static char bufferw[100] = {0};
            ImGui::InputTextMultiline("##WriteText", bufferw, 100, ImVec2(width, 0.3f * height), ImGuiInputTextFlags_AllowTabInput, input_cb);
            if (ImGui::Button(GetText("SEND").c_str()))
            {
                // TODO
                SDL_Log("Send Button");
            }
            ImGui::End();
        }
        if (3)
        {
            ImGui::Begin("PlottingWindow");
            static bool enable_chart = false;
            ImGui::Checkbox(GetText("ENABLE").c_str(), &enable_chart);

            ImGui::End();
        }
        if (4)
        {
            ImGui::Begin("ProjectWindow");

            ImGui::End();
        }
        if (5)
        {
            ImGui::Begin("HelpWindow");

            ImGui::End();
        }

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        if (0)
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
