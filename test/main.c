#include "SDL.h"
#include "SDL_FontCache.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// UTF-8 Sample from http://www.columbia.edu/~fdc/utf8/


#ifdef SDL_GPU_VERSION_MAJOR
GPU_Target* screen;
#else
SDL_Window* window;
SDL_Renderer* renderer;
#endif


void draw_rect(FC_Rect rect, SDL_Color color)
{
    #ifdef SDL_GPU_VERSION_MAJOR
    GPU_Rectangle(screen, rect.x, rect.y, rect.x + rect.w - 1, rect.y + rect.h - 1, color);
    #else
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    #endif
}

void fill_rect(FC_Rect rect, SDL_Color color)
{
    #ifdef SDL_GPU_VERSION_MAJOR
    GPU_RectangleFilled(screen, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, color);
    #else
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    #endif
}

void set_clip(FC_Rect rect)
{
    #ifdef SDL_GPU_VERSION_MAJOR
    GPU_SetClipRect(screen, rect);
    #else
    SDL_RenderSetClipRect(renderer, &rect);
    #endif
}

void unset_clip()
{
    #ifdef SDL_GPU_VERSION_MAJOR
    GPU_UnsetClip(screen);
    #else
    SDL_RenderSetClipRect(renderer, NULL);
    #endif
}

char* get_string_from_file(const char* filename)
{
    char* result;
    int size = 0;
    int i;
    SDL_RWops* rwops = SDL_RWFromFile(filename, "r");

    char c;
    while(SDL_RWread(rwops, &c, 1, 1) > 0)
    {
        ++size;
    }

    result = (char*)malloc(size+1);
    memset(result, 0, size+1);
    SDL_RWseek(rwops, 0, RW_SEEK_SET);

    i = 0;
    while(SDL_RWread(rwops, &c, 1, 1) > 0)
    {
        result[i] = c;
        ++i;
    }

    SDL_RWclose(rwops);
    return result;
}

void loop_drawSomeText()
{
    FC_Font* font = FC_CreateFont();
    FC_Font* font2 = FC_CreateFont();
    FC_Font* font3 = FC_CreateFont();

    //FC_SetLoadingString(font, FC_GetStringASCII_Latin1());

    #ifdef SDL_GPU_VERSION_MAJOR
    FC_LoadFont(font, "fonts/FreeSans.ttf", 20, FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);
    FC_LoadFont(font2, "fonts/FreeSans.ttf", 18, FC_MakeColor(0,200,0,255), TTF_STYLE_NORMAL);
    FC_LoadFont(font3, "fonts/FreeSans.ttf", 22, FC_MakeColor(0,0,200,255), TTF_STYLE_NORMAL);
    #else
    FC_LoadFont(font, renderer, "fonts/FreeSans.ttf", 20, FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);
    FC_LoadFont(font2, renderer, "fonts/FreeSans.ttf", 18, FC_MakeColor(0,200,0,255), TTF_STYLE_NORMAL);
    FC_LoadFont(font3, renderer, "fonts/FreeSans.ttf", 22, FC_MakeColor(0,0,200,255), TTF_STYLE_NORMAL);
    #endif
    char* utf8_string = get_string_from_file("utf8_sample.txt");
    char input_text[2048];
    sprintf(input_text, "Edit this text.");
    int input_position = U8_strlen(input_text);
    FC_Rect input_rect;
    int mode = 0;


    float target_w, target_h;
    #ifdef SDL_GPU_VERSION_MAJOR
    GPU_Target* target = screen;
    target_w = target->w;
    target_h = target->h;
    #else
    SDL_Renderer* target = renderer;
    {
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        target_w = w;
        target_h = h;
    }
    #endif

    FC_Rect leftHalf = {0, 0, 3*target_w/4.0f, target_h};
    FC_Rect rightHalf = {leftHalf.w, 0, target_w/4.0f, target_h};

    FC_Rect box1 = {215, 50, 150, 150};
    FC_Rect box2 = {215, box1.y + box1.h + 50, 150, 150};
    FC_Rect box3 = {215, box2.y + box2.h + 50, 150, 150};

    SDL_Color black = {0, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color gray = {0x77, 0x77, 0x77, 255};
    SDL_Color blue = {0, 0, 127, 255};

    int scroll = 0;

    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    input_rect = FC_MakeRect(rightHalf.x, 175, rightHalf.w, 500);

    SDL_StartTextInput();

    Uint8 done = 0;
	SDL_Event event;
	while(!done)
	{
	    while(SDL_PollEvent(&event))
	    {
	        if(event.type == SDL_QUIT)
                done = 1;
	        else if(event.type == SDL_KEYDOWN)
	        {
	            if(event.key.keysym.sym == SDLK_ESCAPE)
                    done = 1;
	            if(event.key.keysym.sym == SDLK_BACKSPACE)
                {
                    U8_strdel(input_text, input_position-1);

                    --input_position;
                    if(input_position < 0)
                        input_position = 0;
                }
	            else if(event.key.keysym.sym == SDLK_F1)
                {
                    mode++;
                    if(mode > 1)
                        mode = 0;
                }
	            else if(event.key.keysym.sym == SDLK_RETURN)
                {
                    U8_strinsert(input_text, input_position, "\n", 2048);
                    input_position++;
                }
	            else if(event.key.keysym.sym == SDLK_TAB)
                {
                    U8_strinsert(input_text, input_position, "\t", 2048);
                    input_position++;
                }
	            else if(event.key.keysym.sym == SDLK_LEFT)
                {
                    --input_position;
                    if(input_position < 0)
                        input_position = 0;
                }
	            else if(event.key.keysym.sym == SDLK_RIGHT)
                {
                    ++input_position;
                    int len = U8_strlen(input_text);
                    if(input_position >= len)
                        input_position = len;
                }
	        }
	        else if(event.type == SDL_TEXTINPUT)
	        {
	            if(U8_strinsert(input_text, input_position, event.text.text, 2048))
                    input_position += U8_strlen(event.text.text);
	        }
	        else if(event.type == SDL_MOUSEBUTTONDOWN)
	        {
	            if(FC_InRect(event.button.x, event.button.y, input_rect))
                {
                    input_position = FC_GetPositionFromOffset(font, event.button.x - input_rect.x, event.button.y - input_rect.y, input_rect.w, FC_ALIGN_LEFT, "%s", input_text);
                }
	        }
	    }

	    if(keystates[SDL_SCANCODE_UP])
            scroll--;
	    else if(keystates[SDL_SCANCODE_DOWN])
            scroll++;


	    if(mode == 0 || mode == 1)
        {
            fill_rect(leftHalf, white);
            fill_rect(rightHalf, gray);

            FC_Draw(font2, target, 0, 0, "UTF-8 text: %s", utf8_string);

            FC_DrawAlign(font, target, rightHalf.x, 5, FC_ALIGN_LEFT, "draw align LEFT");
            FC_DrawAlign(font, target, rightHalf.x, 25, FC_ALIGN_CENTER, "draw align CENTER");
            FC_DrawAlign(font, target, rightHalf.x, 45, FC_ALIGN_RIGHT, "draw align RIGHT");

            float time = SDL_GetTicks()/1000.0f;

            FC_DrawColor(font, target, rightHalf.x, 65, FC_MakeColor(128 + 127*sin(time), 128 + 127*sin(time/2), 128 + 127*sin(time/4), 128 + 127*sin(time/8)), "Dynamic colored text");
            FC_Draw(font, target, rightHalf.x, 85, "Multi\nline\ntext");

            FC_DrawBox(font, target, input_rect, "%s", input_text);
            draw_rect(FC_MakeRect(input_rect.x, input_rect.y + FC_GetLineHeight(font), FC_GetWidth(font, "%s", input_text), 2), black);

            FC_Rect input_cursor_pos = FC_GetCharacterOffset(font, input_position, input_rect.w, "%s", input_text);
            if(SDL_GetTicks()%1000 < 500)
                fill_rect(FC_MakeRect(input_rect.x + input_cursor_pos.x, input_rect.y + input_cursor_pos.y, input_cursor_pos.w, input_cursor_pos.h), FC_MakeColor(0, 0, 0, 255));

            FC_DrawColumn(font, target, 0, 50, 200, "column align LEFT\nColumn text wraps at the width of the column and has no maximum height.");
            FC_DrawColumnAlign(font, target, 100, 250, 200, FC_ALIGN_CENTER, "column align CENTER\nColumn text wraps at the width of the column and has no maximum height.");
            FC_DrawColumnAlign(font, target, 200, 450, 200, FC_ALIGN_RIGHT, "column align RIGHT\nColumn text wraps at the width of the column and has no maximum height.");


            draw_rect(box1, black);
            draw_rect(box2, black);
            draw_rect(box3, black);

            set_clip(box1);
            FC_Rect box1a = {box1.x, box1.y - scroll, box1.w, box1.h + scroll};
            FC_DrawBox(font, target, box1a, "box align LEFT\nBox text wraps at the width of the box and is clipped to the maximum height.");

            set_clip(box2);
            FC_Rect box2a = {box2.x, box2.y - scroll, box2.w, box2.h + scroll};
            FC_DrawBoxAlign(font, target, box2a, FC_ALIGN_CENTER, "box align CENTER\nBox text wraps at the width of the box and is clipped to the maximum height.");

            set_clip(box3);
            FC_Rect box3a = {box3.x, box3.y - scroll, box3.w, box3.h + scroll};
            FC_DrawBoxAlign(font, target, box3a, FC_ALIGN_RIGHT, "box align RIGHT\nBox text wraps at the width of the box and is clipped to the maximum height.");

            unset_clip();
        }

        if(mode == 1)
        {
            fill_rect(leftHalf, blue);
            fill_rect(rightHalf, blue);

            #ifdef SDL_GPU_VERSION_MAJOR
            GPU_Image* image = FC_GetGlyphCacheLevel(font, 0);
            GPU_SetRGBA(image, 255, 255, 255, 255);
            GPU_Blit(image, NULL, screen, image->w/2, image->h/2);
            #else
            SDL_Texture* image = FC_GetGlyphCacheLevel(font, 0);
            SDL_Rect destrect = {0, 0, 0, 0};
            SDL_QueryTexture(image, NULL, NULL, &destrect.w, &destrect.h);
            SDL_SetTextureColorMod(image, 255, 255, 255);
            SDL_SetTextureAlphaMod(image, 255);
            SDL_RenderCopy(renderer, image, NULL, &destrect);
            #endif
        }

        #ifdef SDL_GPU_VERSION_MAJOR
	    GPU_Flip(screen);
	    #else
	    SDL_RenderPresent(renderer);
	    #endif

	    SDL_Delay(1);
	}

	SDL_StopTextInput();

	free(utf8_string);
	FC_FreeFont(font);
	FC_FreeFont(font2);
	FC_FreeFont(font3);
}

int main(int argc, char* argv[])
{
    int w = 800;
    int h = 600;

    #ifdef SDL_GPU_VERSION_MAJOR
    screen = GPU_Init(w, h, GPU_DEFAULT_INIT_FLAGS);
	if(screen == NULL)
	{
        GPU_LogError("Failed to initialize SDL_gpu.\n");
        return 1;
    }
    #else
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Failed to initialize SDL.\n");
        return 1;
    }

    window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        SDL_Log("Failed to create window.\n");
        return 2;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
    {
        SDL_Log("Failed to create renderer.\n");
        return 3;
    }
    #endif

	loop_drawSomeText();

    #ifdef SDL_GPU_VERSION_MAJOR
    GPU_Quit();
    #else
	SDL_Quit();
	#endif

	return 0;
}
