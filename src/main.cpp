#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <imgui.h>
#include <imgui_impl_SDL3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <memory>

#include <iostream>

constexpr int WINDOW_MINIMUM_WIDTH = 1280;
constexpr int WINDOW_MINIMUM_HEIGHT = 720;
constexpr char WINDOW_TITLE[] = "SDL3 template";
constexpr auto WINDOW_FLAGS = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_AUDIO| SDL_INIT_VIDEO);

    auto window = 
        SDL_CreateWindow(WINDOW_TITLE, WINDOW_MINIMUM_WIDTH, WINDOW_MINIMUM_HEIGHT, WINDOW_FLAGS);

    SDL_SetWindowMinimumSize(window, WINDOW_MINIMUM_WIDTH, WINDOW_MINIMUM_HEIGHT);

    auto renderer = SDL_CreateRenderer(window, "opengl");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    bool is_running{true};
    while (is_running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
            {
                is_running = false;
                break;
            }
        }

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
        SDL_Delay(1);
    }

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}
