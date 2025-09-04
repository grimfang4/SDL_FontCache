# SDL\_FontCache

A generic font caching C library with loading and rendering support for SDL.

SDL\_FontCache loads, caches, and renders TrueType fonts using **SDL\_ttf**.
It fully supports **UTF-8 strings** and includes utility functions for manipulating them.

---

## Example Usage

```c
FC_Font* font = FC_CreateFont();
FC_LoadFont(font, renderer, "fonts/FreeSans.ttf", 20, FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);

...

FC_Draw(font, renderer, 0, 0, "This is %s.\nIt works.", "example text");

...

FC_FreeFont(font);
```

---

# Function Documentation

## `FC_CreateFont`

**Purpose**: Creates and initializes a new font object

### Parameters:

* None

### Returns:

* `FC_Font*`: A pointer to a newly allocated font object.

### Description:

Creates a new `FC_Font` structure. Use `FC_LoadFont` to load a TrueType font into it
When you are finished, call `FC_FreeFont` to release memory

---

## `FC_LoadFont`

**Purpose**: Loads a TrueType font from a specified file into memory

### Parameters:

* `font` (FC\_Font\*): A pointer to the font object to be loaded
* `renderer` (SDL\_Renderer\*): The SDL renderer used for loading the font
* `fontPath` (const char\*): Path to the TrueType font file
* `fontSize` (int): The desired size of the font
* `color` (SDL\_Color): The color of the font
* `style` (int): Style flags for the font (e.g., bold, italic)

### Returns:

* `FC_Font*`: A pointer to the newly loaded font object.

### Description:

Loads a font from a specified TrueType file and sets its style and color

---

## `FC_Draw`

**Purpose**: Renders a string of text at specified coordinates using a given font.

### Parameters:

* `font` (FC\_Font\*): A pointer to the font object used to render the text
* `renderer` (SDL\_Renderer\*): The SDL renderer used for drawing
* `x` (float): The X-coordinate where the text will be rendered
* `y` (float): The Y-coordinate where the text will be rendered
* `text` (const char\*): The string of text to be rendered

### Returns:

* `void`

### Description:

Renders a UTF-8 encoded text string at the given position using the provided font

---

## `FC_FreeFont`

**Purpose**: Frees the memory used by a font object

### Parameters:

* `font` (FC\_Font\*): A pointer to the font object to be freed

### Returns:

* `void`

### Description:

Frees the memory allocated for a font object previously created with `FC_CreateFont` and loaded with `FC_LoadFont`. Always call this to avoid memory leaks

---

# Utility Functions

## `FC_MakeRect`

**Purpose**: Creates and returns a rectangle structure.

### Parameters:

* `x` (float): X coordinate of the top left corner
* `y` (float): Y coordinate of the top left corner
* `w` (float): Width of the rectangle
* `h` (float): Height of the rectangle

### Returns:

* `FC_Rect`: A rectangle structure.

---

## `FC_MakeColor`

**Purpose**: Creates an SDL\_Color structure with the given RGBA values

### Parameters:

* `r` (Uint8): Red component (0–255)
* `g` (Uint8): Green component (0–255)
* `b` (Uint8): Blue component (0–255)
* `a` (Uint8): Alpha component (0–255)

### Returns:

* `SDL_Color`: The resulting color structure

---

## `FC_FreeText`

**Purpose**: Frees the memory used by a text object

### Parameters:

* `text` (FC\_Text\*): A pointer to the text object to be freed

### Returns:

* `void`

### Description:

Frees the memory allocated for a text object created by `FC_LoadText`. Always call this when you are done with the text object

---
