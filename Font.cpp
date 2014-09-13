#include "Font.h"

void Font::draw_bitmap( FT_Bitmap*  bitmap,
			 FT_Int      x,
			 FT_Int      y )
{
	FT_Int  i, j, p, q;
	FT_Int  x_max = x + bitmap->width;
	FT_Int  y_max = y + bitmap->rows;
	
	
	for( i = x, p = 0; i < x_max; i++, p++ )
	{
		for( j = y, q = 0; j < y_max; j++, q++ )
		{
			if( i < 0      || j < 0       ||
					i >= WIDTH || j >= HEIGHT )
				continue;
				
			image[j][i] |= bitmap->buffer[q * bitmap->width + p];
		}
	}
}

void Font::show_image( void )
{
	int  i, j;
	
	
	for( i = 0; i < HEIGHT; i++ )
	{
		for( j = 0; j < WIDTH; j++ )
			putchar( image[i][j] == 0 ? ' '
					 : image[i][j] < 128 ? '+'
					 : '*' );
		putchar( '\n' );
	}
}

Font::Font()
{
	bool error;
	
	error = FT_Init_FreeType( &library );
	
	if( error )
	{
		std::cerr << "" << std::endl;
		return;
	}
	
	error = FT_New_Face( library,
                         "assets/dejavu.ttf",
						 0,
						 &face );
						 
	if( error )
	{
		std::cerr << "" << std::endl;
		return;
	}
	
	if( error )
	{
		std::cout << "There was an error loading the font.\n";
	}

    /*
	error = FT_Set_Char_Size(
                face,    // handle to face object
                0,       // char_width in 1/64th of points
                16 * 64, // char_height in 1/64th of points
                300,     // horizontal device resolution
                300 );   // vertical device resolution
    */

	if( error )
	{
		std::cerr << "" << std::endl;
		return;
    }
	
	error = FT_Set_Pixel_Sizes(
				face,   /* handle to face object */
                0,      /* pixel_width           */
				16 );   /* pixel_height          */
				
	if( error )
	{
        std::cerr << "There was an error setting pixel sizes." << std::endl;
		return;
    }
}


// TODO: return letter '0' on error
Glyph Font::getGlyph(const char* letter)
{
    FT_UInt  glyph_index;
    Glyph glyph;
    bool error;

    FT_Set_Pixel_Sizes(face, 0, 450);

    /* retrieve glyph index from character code */
    glyph_index = FT_Get_Char_Index( face, 282 );

    /* load glyph image into the slot (erase previous one) */
    error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );

    if( error )
    {
        std::cerr << "ERROR: Could not load glyph" << std::endl;
    }

    /* convert to an anti-aliased bitmap */
    error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );

    if( error )
    {
        std::cerr << "ERROR: Could not render glyph" << std::endl;
    }

    FT_GlyphSlot slot = face->glyph;

    draw_bitmap( &slot->bitmap, slot->bitmap_left, HEIGHT - slot->bitmap_top );
    show_image();

    glyph.buffer = slot->bitmap.buffer;
    glyph.height = slot->bitmap.rows;
    glyph.width = slot->bitmap.width;
    glyph.pen_left = slot->bitmap_left;
    glyph.pen_top = slot->bitmap_top;

    std::cout << "glyph.buffer: " << glyph.buffer << std::endl;
    std::cout << "glyph.height: " << glyph.height << std::endl;
    std::cout << "glyph.width: " << glyph.width << std::endl;
    std::cout << "glyph.pen_left: " << glyph.pen_left << std::endl;
    std::cout << "glyph.pen_top: " << glyph.pen_top << std::endl;

    return glyph;
}

Font::~Font()
{
	FT_Done_Face( face );
	FT_Done_FreeType( library );
}
