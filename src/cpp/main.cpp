
#include <stdio.h>
#include <vector>

#include "globals.h"
#include "logging.h"
#include "ns_abbr/fs.h"
#include "settings/root_settings.h"
#include "ui/menubar/menubar.h"
#include <plog/Log.h>

#define SETTINGS_PATH "settings.json"
#define MEDIA_SOURCES_PATH "media_sources.json"
#define TARGET_FPS 60.0f

#if !SDL_VERSION_ATLEAST(2, 0, 17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

// Main code
int main(int argc, char** argv) {
    Global::app_path = std::filesystem::path(argv[0]);
    // Asset manager has the code for accessing the app's data folder:
    Global::asset_manager = new Assets::AssetManager(); 
    Settings::load_settings(&Global::settings, Global::asset_manager->getDataPath(SETTINGS_PATH));
    Logging::setup_logs();


    // Loading Media Sources:
    Global::media_sources = new MediaSource::MediaSourceManager();
    Global::media_sources->loadSources(MEDIA_SOURCES_PATH);
    // Loading Library Database:
    Global::library_db = new Library::Database();

    PLOGD << "App Started";

    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        PLOGE.printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    Global::sdl2_window = SDL_CreateWindow("Trove Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    Global::renderer = SDL_CreateRenderer(Global::sdl2_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (Global::renderer == nullptr) {
        PLOGE.printf("Error creating SDL_Renderer!");
        return 0;
    }

    SDL_RendererInfo info;
    SDL_GetRendererInfo(Global::renderer, &info);
    PLOGI.printf("Current SDL_Renderer: %s", info.name);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    std::string imgui_ini_path = Global::asset_manager->getDataPath("imgui.ini").string();
    io.IniFilename = imgui_ini_path.c_str();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(Global::sdl2_window, Global::renderer);
    ImGui_ImplSDLRenderer2_Init(Global::renderer);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont()
    // to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an
    // assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling
    // ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    // IM_ASSERT(font != nullptr);

    // Our state
    // ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    Global::main_window = new UI::MainWindow();
    Global::sub_window_handler = new UI::SubWindowHandler();

    unsigned int target_frame_length_ms = (1.0f / (float)TARGET_FPS) * 1000;

    // Main loop:
    while (!Global::should_terminate) {
        unsigned long frame_start_time = SDL_GetTicks64();

        Global::main_window->onBackground();

        // Background for sub-windows:
        Global::sub_window_handler->handleNewWindows();
        Global::sub_window_handler->handleBackgrounds();

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of
        // the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy
        // of the keyboard data. Generally you may always pass all inputs to dear imgui, and hide them from your application based on those
        // two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                Global::should_terminate = true;
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(Global::sdl2_window)) {
                Global::should_terminate = true;
            }

            // Only send the event to subwindows if the main window consumed the event:
            if (!Global::main_window->onEvent(&event)) {
                Global::sub_window_handler->handleEvents(&event);
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (Global::draw_demo_window) {
            ImGui::ShowDemoWindow(&Global::draw_demo_window);
        }

        // Draw Application:
        UI::draw_menu_bar();
        Global::main_window->onDraw();
        Global::sub_window_handler->handleDraw();

        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(Global::renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(Global::renderer,
                               (Uint8)(clear_color.x * 255),
                               (Uint8)(clear_color.y * 255),
                               (Uint8)(clear_color.z * 255),
                               (Uint8)(clear_color.w * 255));
        SDL_RenderClear(Global::renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(Global::renderer);

        // Run tasks intended for the end of the frame:
        Global::sub_window_handler->handleDeletion();
        Global::media_sources->runSourceUpdates();

        // Process Frame Delay:
        // We'll calculate how long the frame took to process, and delay for the remainder.
        unsigned long frame_end_time = SDL_GetTicks64();
        unsigned long frame_delta_time = frame_end_time - frame_start_time;

        if (frame_delta_time < target_frame_length_ms) {
            SDL_Delay(target_frame_length_ms - frame_delta_time);
        }
    }

    // Cleanup
    delete Global::main_window;
    delete Global::sub_window_handler;
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // Because asset_manager holds SDL2 textures, it need to be cleaned up before we quit SDL2.
    delete Global::library_db;
    
    Settings::save_settings(&Global::settings, Global::asset_manager->getDataPath(SETTINGS_PATH));
    Global::media_sources->saveSources(Global::asset_manager->getDataPath(MEDIA_SOURCES_PATH));
    delete Global::media_sources;

    delete Global::asset_manager;

    SDL_DestroyRenderer(Global::renderer);
    SDL_DestroyWindow(Global::sdl2_window);
    SDL_Quit();

    return 0;
}