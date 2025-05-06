#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <imgui.h>
#include <imgui_impl_SDL3.h>
#include <imgui_impl_sdlrenderer3.h>

constexpr int WINDOW_MINIMUM_WIDTH = 1280;
constexpr int WINDOW_MINIMUM_HEIGHT = 720;
constexpr char WINDOW_TITLE[] = "SDL3 template";
constexpr auto WINDOW_FLAGS = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;

SDL_Renderer *renderer;
SDL_Window *window;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_MINIMUM_WIDTH, WINDOW_MINIMUM_HEIGHT, WINDOW_FLAGS);
    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Unable to create a new window.");
        return SDL_APP_FAILURE;
    }

    SDL_SetWindowMinimumSize(window, WINDOW_MINIMUM_WIDTH, WINDOW_MINIMUM_HEIGHT);

    renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Unable to create a new renderer.");
        return SDL_APP_FAILURE;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    bool is_running = true;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_RenderClear(renderer);

    ImGui_ImplSDL3_NewFrame();
    ImGui_ImplSDLRenderer3_NewFrame();

    ImGui::NewFrame();
    {
        if (ImGui::Begin("Hello"))
        {
            if (ImGui::Button("Hello World"))
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Hello", "Hello World", window);
            }
        }
        ImGui::End();
    }

    ImGui::EndFrame();
    ImGui::Render();

    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    ImGui_ImplSDL3_ProcessEvent(event);
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}