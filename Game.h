#ifndef GAME_H
#define GAME_H

#include <iostream>

#include <glew.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "Font.h"

class Game
{
public:
	Game() {}
	~Game() {}
	
	void init( unsigned short width = 1024,
			   unsigned short height = 768,
			   bool fullscreen = false );
	void initGL();
	void render();
	void render_text( std::u32string text, float x, float y );
	void update();
	void handleEvents();
	void clean();
	
	bool running()
	{
		return m_bRunning;
	}
	
	unsigned short getWidth()
	{
		return this->window.width;
	}
	unsigned short getHeight()
	{
		return this->window.height;
	}
	
private:
	bool m_bRunning;
	
	SDL_Window* m_pWindow;      // = nullptr;
	SDL_Renderer* m_pRenderer;  // = nullptr;
	SDL_GLContext m_pGlcontext; // = nullptr;
	
	//GLuint texture; // Texture object handle
	
	/*SDL_Texture**/ SDL_Surface* m_pTexture; // the new SDL_Texture variable
	SDL_Rect m_sourceRectangle; // the first rectangle
	SDL_Rect m_destinationRectangle; // another rectangle
	
	struct Window
	{
		unsigned short width;
		unsigned short height;
	} window;
	
	Font* font;
};


#endif // GAME_H
