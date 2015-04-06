/*
SDL_FontCache v0.0.1: A font cache for SDL and SDL_ttf
by Jonathan Dearborn
Dedicated to the memory of Florian Hufsky

License:
    The short:
    Use it however you'd like, but keep the copyright and license notice 
    whenever these files or parts of them are distributed in uncompiled form.
    
    The long:
Copyright (c) 2015 Jonathan Dearborn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef _SDL_FONTCACHE_H__
#define _SDL_FONTCACHE_H__

#include "SDL.h"
#include "SDL_ttf.h"

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif


// Let's pretend this exists...
#define TTF_STYLE_OUTLINE	16


// SDL_FontCache types

typedef enum
{
    FC_ALIGN_LEFT,
    FC_ALIGN_CENTER,
    FC_ALIGN_RIGHT
} FC_AlignEnum;

typedef struct FC_Scale
{
    float x;
    float y;
    
} FC_Scale;

typedef struct FC_Effect
{
    FC_AlignEnum alignment;
    FC_Scale scale;
    SDL_Color color;
    
} FC_Effect;

// Opaque type
typedef struct FC_Font FC_Font;




#ifdef FC_USE_GPU
#define FC_Rect GPU_Rect
#define FC_Target GPU_Target
#define FC_Image GPU_Image
#define FC_Log GPU_LogError
#else
#define FC_Rect SDL_Rect
#define FC_Target SDL_Renderer
#define FC_Image SDL_Texture
#define FC_Log SDL_Log
#endif


// Object creation

FC_Rect FC_MakeRect(float x, float y, float w, float h);

FC_Scale FC_MakeScale(float x, float y);

SDL_Color FC_MakeColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

FC_Effect FC_MakeEffect(FC_AlignEnum alignment, FC_Scale scale, SDL_Color color);


FC_Font* FC_CreateFont(void);

Uint8 FC_LoadFont(FC_Font* font, SDL_Renderer* renderer, const char* filename_ttf, Uint32 pointSize, SDL_Color color, int style);

Uint8 FC_LoadFontFromTTF(FC_Font* font, SDL_Renderer* renderer, TTF_Font* ttf, SDL_Color color);

Uint8 FC_LoadFont_RW(FC_Font* font, SDL_Renderer* renderer, SDL_RWops* file_rwops_ttf, Uint8 own_rwops, Uint32 pointSize, SDL_Color color, int style);

void FC_ClearFont(FC_Font* font);

void FC_FreeFont(FC_Font* font);



// UTF-8 to SDL_FontCache codepoint conversion

/*!
Returns the Uint32 codepoint (not UTF-32) parsed from the given UTF-8 string.
\param c A pointer to a string of proper UTF-8 character values.
\param advance_pointer If true, the source pointer will be incremented to skip the extra bytes from multibyte codepoints.
*/
Uint32 FC_GetCodepointFromUTF8(const char** c, Uint8 advance_pointer);

/*!
Parses the given codepoint and stores the UTF-8 bytes in 'result'.  The result is NULL terminated.
\param result A memory buffer for the UTF-8 values.  Must be at least 5 bytes long.
\param codepoint The Uint32 codepoint to parse (not UTF-32).
*/
void FC_GetUTF8FromCodepoint(char* result, Uint32 codepoint);


// String operations

/*! Allocates a copy of the given string. */
char* U8_strdup(const char* string);
/*! Erases the UTF-8 character at the given position, moving the subsequent characters down. */
void U8_strdel(char* string, int position);
void U8_free(char* string);
/*! Returns the number of UTF-8 characters in the given string. */
int U8_strlen(const char* string);
/*! Returns the number of bytes in the UTF-8 multibyte character pointed at by c. */
int U8_charsize(const char* c);
/*! Inserts a UTF-8 string into 'string' at the given position.  Use a position of -1 to append.  Returns 0 when unable to insert the string. */
int U8_strinsert(char* string, int position, const char* source, int max_bytes);

/*! Changes the size of the internal buffer which is used for unpacking variadic text data. */
void FC_SetBufferSize(unsigned int size);
void FC_SetRenderCallback(FC_Rect (*callback)(FC_Image* src, FC_Rect* srcrect, FC_Target* dest, float x, float y, float xscale, float yscale));
FC_Rect FC_DefaultRenderCallback(FC_Image* src, FC_Rect* srcrect, FC_Target* dest, float x, float y, float xscale, float yscale);

// Drawing
SDL_Rect FC_Draw(FC_Font* font, SDL_Renderer* dest, float x, float y, const char* formatted_text, ...);
SDL_Rect FC_DrawAlign(FC_Font* font, SDL_Renderer* dest, float x, float y, FC_AlignEnum align, const char* formatted_text, ...);
SDL_Rect FC_DrawScale(FC_Font* font, SDL_Renderer* dest, float x, float y, FC_Scale scale, const char* formatted_text, ...);
SDL_Rect FC_DrawColor(FC_Font* font, SDL_Renderer* dest, float x, float y, SDL_Color color, const char* formatted_text, ...);
SDL_Rect FC_DrawEffect(FC_Font* font, SDL_Renderer* dest, float x, float y, FC_Effect effect, const char* formatted_text, ...);

SDL_Rect FC_DrawBox(FC_Font* font, SDL_Renderer* dest, SDL_Rect box, const char* formatted_text, ...);
SDL_Rect FC_DrawBoxAlign(FC_Font* font, SDL_Renderer* dest, SDL_Rect box, FC_AlignEnum align, const char* formatted_text, ...);
SDL_Rect FC_DrawColumn(FC_Font* font, SDL_Renderer* dest, float x, float y, Uint16 width, const char* formatted_text, ...);
SDL_Rect FC_DrawColumnAlign(FC_Font* font, SDL_Renderer* dest, float x, float y, Uint16 width, FC_AlignEnum align, const char* formatted_text, ...);

// Getters
Uint16 FC_GetLineHeight(FC_Font* font);
Uint16 FC_GetHeight(FC_Font* font, const char* formatted_text, ...);
Uint16 FC_GetWidth(FC_Font* font, const char* formatted_text, ...);
FC_Rect FC_GetCharacterOffset(FC_Font* font, Uint16 position_index, int column_width, const char* formatted_text, ...);
Uint16 FC_GetColumnHeight(FC_Font* font, Uint16 width, const char* formatted_text, ...);

int FC_GetAscent(FC_Font* font, const char* formatted_text, ...);
int FC_GetDescent(FC_Font* font, const char* formatted_text, ...);
int FC_GetBaseline(FC_Font* font);
int FC_GetSpacing(FC_Font* font);
int FC_GetLineSpacing(FC_Font* font);
Uint16 FC_GetMaxWidth(FC_Font* font);
SDL_Color FC_GetDefaultColor(FC_Font* font);

// Setters
void FC_SetSpacing(FC_Font* font, int LetterSpacing);
void FC_SetLineSpacing(FC_Font* font, int LineSpacing);
void FC_SetDefaultColor(FC_Font* font, SDL_Color color);


#ifdef __cplusplus
}
#endif



#endif
