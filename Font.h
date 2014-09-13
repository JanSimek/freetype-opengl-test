#ifndef FONT_H
#define FONT_H

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#define WIDTH   640
#define HEIGHT  480

struct Glyph
{
    int width;
    int height;
    int pen_left;
    int pen_top;
    unsigned char* buffer;
};

class Font
{
public:
	Font();
    ~Font();

    void show_image( void );
    void draw_bitmap( FT_Bitmap*  bitmap,
                 FT_Int      x,
                 FT_Int      y );

    Glyph getGlyph(const char* letter);

private:
	FT_Library  library;   /* handle to library     */
	FT_Face     face;      /* handle to face object */

    /* origin is the upper left corner */
    unsigned char image[HEIGHT][WIDTH];
};

#endif // FONT_H
