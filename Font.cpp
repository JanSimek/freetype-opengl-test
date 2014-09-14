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

// Unicode & UTF-8 handling

/**
 * @brief never returns more than 4
 * @author Unvanquished
 */
int Q_UTF8_Width( const char *str )
{
  int                 ewidth;
  const unsigned char *s = (const unsigned char *)str;

  if( !str )
    return 0;

  if     (               *s <= 0x7F )
    ewidth = 0;
  else if( 0xC2 <= *s && *s <= 0xDF )
    ewidth = 1;
  else if( 0xE0 <= *s && *s <= 0xEF )
    ewidth = 2;
  else if( 0xF0 <= *s && *s <= 0xF4 )
    ewidth = 3;
  else
    ewidth = 0;

  for( ; *s && ewidth > 0; s++, ewidth-- );

  return s - (const unsigned char *)str + 1;
}

int Q_UTF8_WidthCP( int ch )
{
    if ( ch <=   0x007F ) { return 1; }
    if ( ch <=   0x07FF ) { return 2; }
    if ( ch <=   0xFFFF ) { return 3; }
    if ( ch <= 0x10FFFF ) { return 4; }
    return 0;
}

int Q_UTF8_Strlen( const char *str )
{
  int l = 0;

  while( *str )
  {
    l++;

    str += Q_UTF8_Width( str );
  }

  return l;
}

int Q_UTF8_PrintStrlen( const char *str )
{
  int l = 0;

  while( *str )
  {
    /*
      if( Q_IsColorString( str ) )
    {
      str += 2;
      continue;
    }
    if( *str == Q_COLOR_ESCAPE && str[1] == Q_COLOR_ESCAPE )
    {
      ++str;
    }
    */

    l++;

    str += Q_UTF8_Width( str );
  }

  return l;
}

bool Q_UTF8_ContByte( char c )
{
  return (unsigned char )0x80 <= (unsigned char)c && (unsigned char)c <= (unsigned char )0xBF;
}

static bool getbit(const unsigned char *p, int pos)
{
  p   += pos / 8;
  pos %= 8;

  return (*p & (1 << (7 - pos))) != 0;
}

static void setbit(unsigned char *p, int pos, bool on)
{
  p   += pos / 8;
  pos %= 8;

  if( on )
    *p |= 1 << (7 - pos);
  else
    *p &= ~(1 << (7 - pos));
}

static void shiftbitsright(unsigned char *p, unsigned long num, unsigned long by)
{
  int step, off;
  unsigned char *e;

  if( by >= num )
  {
    for( ; num > 8; p++, num -= 8 )
      *p = 0;

    *p &= (~0x00) >> num;

    return;
  }

  step = by / 8;
  off  = by % 8;

  for( e = p + (num + 7) / 8 - 1; e > p + step; e-- )
    *e = (*(e - step) >> off) | (*(e - step - 1) << (8 - off));

  *e = *(e - step) >> off;

  for( e = p; e < p + step; e++ )
    *e = 0;
}

unsigned long Q_UTF8_CodePoint( const char *str )
{
  int i, j;
  int n = 0;
  int size = Q_UTF8_Width( str );
  unsigned long codepoint = 0;
  unsigned char *p = (unsigned char *) &codepoint;

  if( size > sizeof( codepoint ) )
    size = sizeof( codepoint );
  else if( size < 1 )
    size = 1;

  for( i = (size > 1 ? size + 1 : 1); i < 8; i++ )
    setbit(p, n++, getbit((const unsigned char *)str, i));
  for( i = 1; i < size; i++ )
    for( j = 2; j < 8; j++ )
      setbit(p, n++, getbit(((const unsigned char *)str) + i, j));

  shiftbitsright(p, 8 * sizeof(codepoint), 8 * sizeof(codepoint) - n);

#ifndef Q3_BIG_ENDIAN
  for( i = 0; i < sizeof(codepoint) / 2; i++ )
  {
    p[i] ^= p[sizeof(codepoint) - 1 - i];
    p[sizeof(codepoint) - 1 - i] ^= p[i];
    p[i] ^= p[sizeof(codepoint) - 1 - i];
  }
#endif

  return codepoint;
}

// TODO: return letter '0' on error
Glyph Font::getGlyph(const char* letter)
{
    FT_UInt  glyph_index;
    Glyph glyph;
    bool error;

    FT_Set_Pixel_Sizes(face, 0, 450);

    /* retrieve glyph index from character code */
    glyph_index = FT_Get_Char_Index( face, Q_UTF8_CodePoint(letter)); //282 );

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

    //draw_bitmap( &slot->bitmap, slot->bitmap_left, HEIGHT - slot->bitmap_top );
    //show_image();

    glyph.buffer = slot->bitmap.buffer;
    glyph.height = slot->bitmap.rows;
    glyph.width = slot->bitmap.width;
    glyph.pen_left = slot->bitmap_left;
    glyph.pen_top = slot->bitmap_top;

/*
    std::cout << "glyph.buffer: " << glyph.buffer << std::endl;
    std::cout << "glyph.height: " << glyph.height << std::endl;
    std::cout << "glyph.width: " << glyph.width << std::endl;
    std::cout << "glyph.pen_left: " << glyph.pen_left << std::endl;
    std::cout << "glyph.pen_top: " << glyph.pen_top << std::endl;
*/
    return glyph;
}

Font::~Font()
{
	FT_Done_Face( face );
	FT_Done_FreeType( library );
}
