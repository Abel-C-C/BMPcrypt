#include <iostream>
#include <SDL2/SDL.h>
#include "input.h"
#include "prng.h"

int main() {

        constexpr int WIN_MULTIPLY = 1;
        constexpr int RGB_VAL = 256;

        //input.h
        int size_w, size_l;

        uint16_t extraIter;

        uint64_t seedNum, seedNumR, seedNumG, seedNumB;
        uint16_t countNum, countNumR, countNumG, countNumB;

        //width and height of image
        size_w = dimensionIn("width");
        size_l = dimensionIn("length");

        //number of extra pixels to generate
        extraIter = iterNumIn();

        //starting conditions that decide how the key is generated
        seedNum = seedIn("point");
        countNum = iterateIn();

        seedNumR = seedIn("red");
        countNumR = iterateIn();

        seedNumG = seedIn("green");
        countNumG = iterateIn();

        seedNumB = seedIn("blue");
        countNumB = iterateIn();

        const unsigned int WIDTH = size_w;
        const unsigned int LENGTH = size_l;

        uint64_t RNG_mod1 = randGen(seedNum, countNum);
        uint64_t RNG_mod2 = randGen(RNG_mod1, countNum);

        uint64_t RNG_red = randGen(seedNumR, countNumR);
        uint64_t RNG_green = randGen(seedNumG, countNumG) + WIDTH;
        uint64_t RNG_blue = randGen(seedNumB, countNumB) + LENGTH;
  
        //choose file name
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::string file = fileIn();
        const char *bmpFile = file.c_str();

        //initialize SDL window
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;

        SDL_Init(SDL_INIT_EVERYTHING);

        SDL_CreateWindowAndRenderer(WIDTH*WIN_MULTIPLY, LENGTH*WIN_MULTIPLY, 0, &window, &renderer);
        SDL_RenderSetScale(renderer, WIN_MULTIPLY, WIN_MULTIPLY);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Delay(10);

        std::cout << "\nSuccessfully initialized SDL window." << '\n';

        int point1 = 0;
        int point2 = 0;

        //pixel by pixel generation
        for (int w = 0; w < WIDTH; w++) {

                //SDL_RenderClear(renderer);
                point1 = w;
                std::cout << "Generating column " << (w + 1) << " out of " << WIDTH << "..." << '\r';

                for (int l = 0; l < LENGTH; l++) {

                        point2 = l;

                        RNG_red =   randGen(RNG_green, RGB_VAL - (RNG_green % RGB_VAL));
                        RNG_green = randGen(RNG_blue, RGB_VAL - (RNG_blue % RGB_VAL));
                        RNG_blue =  randGen(RNG_red, RGB_VAL - (RNG_red % RGB_VAL));


                        //generate RGB values
                        SDL_SetRenderDrawColor(renderer,
                                        RNG_red % RGB_VAL,
                                        RNG_green % RGB_VAL,
                                        RNG_blue % RGB_VAL, 255);

                        //update backbuffer
                        SDL_RenderDrawPoint(renderer, w, l);
                        SDL_Delay(0.1);                                                                                                           
                }
        }

        point1 = RNG_mod1;
        point2 = RNG_mod2;

        for (int i = 0; i < extraIter; i++) {

                RNG_mod1 =  randGen(RNG_mod2, 100);
                RNG_mod2 =  randGen(RNG_mod1, 100);

                RNG_red =   randGen(RNG_green, RGB_VAL - (RNG_green % RGB_VAL));
                RNG_green = randGen(RNG_blue, RGB_VAL - (RNG_blue % RGB_VAL));
                RNG_blue =  randGen(RNG_red, RGB_VAL - (RNG_red % RGB_VAL));

                point1 = randGen(RNG_mod1, (RNG_mod2 % RGB_VAL) + 1) % WIDTH;
                point2 = randGen(RNG_mod2, (RNG_mod1 % RGB_VAL) + 1) % LENGTH;

                //generate RGB values
                SDL_SetRenderDrawColor(renderer,
                                RNG_red % RGB_VAL,
                                RNG_green % RGB_VAL,
                                RNG_blue % RGB_VAL, 255);

                //update backbuffer
                SDL_RenderDrawPoint(renderer, point1, point2);
                SDL_Delay(0.1);
        }

        //render final image
        SDL_RenderPresent(renderer);

        //save SDL surface to BMP image.
        SDL_Surface *windowSurface = nullptr;
        SDL_Surface *saveSurface = nullptr;

        windowSurface = SDL_GetWindowSurface(window);

        //get surface data
        unsigned char *pixels = new(std::nothrow)
                unsigned char[windowSurface->w * windowSurface->h
                * windowSurface->format->BytesPerPixel];

        SDL_RenderReadPixels(renderer, NULL, 0,  pixels, windowSurface->w
                        * windowSurface->format->BytesPerPixel);

        saveSurface = SDL_CreateRGBSurfaceFrom(pixels, windowSurface->w, windowSurface->h,
                        windowSurface->format->BitsPerPixel, windowSurface->w *
                        windowSurface->format->BytesPerPixel, windowSurface->format->Rmask,
                        windowSurface->format->Gmask, windowSurface->format->Bmask,
                        windowSurface->format->Amask);

        //save file to disk     
        if (SDL_SaveBMP(saveSurface, bmpFile) != 0) {

                std::cout << "\n\nFailed to save file to disk." << '\n';
                std::cout << '\t' << SDL_GetError() << '\n' << '\n';

                SDL_FreeSurface(saveSurface);
                saveSurface = nullptr;

                delete[] pixels;

                SDL_FreeSurface(windowSurface);
                windowSurface = nullptr;

                return 1;
        }

        //free memory
        SDL_FreeSurface(saveSurface);

        saveSurface = nullptr;
        delete[] pixels;

        SDL_FreeSurface(windowSurface);
        windowSurface = nullptr;

        std::cout << "\n\nSuccessfully generated and saved image." << '\n';
        SDL_Delay(3000);

        std::cout << "\nExiting..." << '\n';
        return 0;
}
