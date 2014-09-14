#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Game.h"
#include "GLSLShader.h"
#include "Utility.h"

//shader reference
GLSLShader shader;

//vertex array and vertex buffer object IDs
GLuint vaoID;
GLuint vboVerticesID;
GLuint vboIndicesID;

//texture ID
GLuint textureID;

//quad vertices and indices
glm::vec4 vertices[4];
GLushort indices[6];

//projection and modelview matrices
glm::mat4  P = glm::mat4( 1 );
glm::mat4 MV = glm::mat4( 1 );

GLuint texture;

void Game::init( unsigned short width, unsigned short height, bool fullscreen )
{
	m_bRunning = true;
	
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		std::cout << SDL_GetError() << std::endl;
		m_bRunning = false;
	}
	else
	{
		std::cout << "SDL initialization complete" << std::endl;
	}
	
	/* Request opengl 3.2 context. */
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	
	/* Turn on double buffering with a 24bit Z buffer.
	 * You may need to change this to 16 or 32 for your system */
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
	
    m_pWindow = SDL_CreateWindow( "OpenGL + Freetype",
								  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								  width, height,
								  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
								  
    this->window.height = height;
    this->window.width = width;

	if( m_pWindow == nullptr )
	{
		std::cout << SDL_GetError() << std::endl;
		m_bRunning = false;
	}
	
	m_pGlcontext = SDL_GL_CreateContext( m_pWindow );
	
	if( m_pGlcontext == nullptr )
	{
		std::cout << SDL_GetError() << std::endl;
		m_bRunning = false;
	}
	
	glewExperimental = GL_TRUE;
	
	// check for errors
	GL_CHECK_ERROR();
	
	if( glewInit() != GLEW_OK )
	{
		std::cerr << "GLEW initialization failed: " << glewGetErrorString( glewInit() ) << std::endl;
		m_bRunning = false;
	}
	else
	{
		if( GLEW_VERSION_3_3 )
		{
			std::cout << "Driver supports OpenGL 3.3\nDetails:" << std::endl;
		}
	}
	// check for errors
	glGetError(); // glewInit() produces a GL error, skip it
	GL_CHECK_ERROR();
	
	std::cout << "GLEW version " << glewGetString( GLEW_VERSION ) << std::endl;
	std::cout << "Vendor: " << glGetString( GL_VENDOR ) << std::endl;
	std::cout << "Renderer: " << glGetString( GL_RENDERER ) << std::endl;
	std::cout << "Version: " << glGetString( GL_VERSION ) << std::endl;
	std::cout << "GLSL: " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl;

    font = new Font();

	initGL();
}

void Game::initGL()
{
	/* This makes our buffer swap syncronized with the monitor's vertical refresh */
	SDL_GL_SetSwapInterval( 1 );
	
	GL_CHECK_ERROR();
	
    //load shader
    shader.LoadFromFile( GL_VERTEX_SHADER, "shaders/shader.vert" );
    shader.LoadFromFile( GL_FRAGMENT_SHADER, "shaders/shader.frag" );
	
	//compile and link shader
	shader.CreateAndLinkProgram();
    shader.Use();
        //add attributes and uniforms
        shader.AddAttribute("vVertex");
        shader.AddUniform("textureMap");
        //pass values of constant uniforms at initialization
        glUniform1i(shader("textureMap"), 0);
	shader.UnUse();

    GL_CHECK_ERROR();
	
	//setup quad geometry
    //setup quad vertices

    vertices[0] = glm::vec4(-1.0,-1.0, 0,1);
    vertices[1] = glm::vec4(1.0,1.0, 1,0);
    vertices[2] = glm::vec4(1.0,-1.0, 1,1);
    vertices[3] = glm::vec4(-1.0,1.0, 0,0);
	
	//fill quad indices array
    GLushort* id = &indices[0];
    *id++ = 0;
    *id++ = 1;
    *id++ = 2;
    *id++ = 0;
    *id++ = 1;
    *id++ = 3;
	
	GL_CHECK_ERROR();

	//setup quad vao and vbo stuff
	glGenVertexArrays( 1, &vaoID );
	glGenBuffers( 1, &vboVerticesID );
    glGenBuffers( 1, &vboIndicesID );
	
	glBindVertexArray( vaoID );

    glBindBuffer (GL_ARRAY_BUFFER, vboVerticesID);
    //pass quad vertices to buffer object
    glBufferData (GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_DYNAMIC_DRAW);
    GL_CHECK_ERROR();

    //enable vertex attribute array for position
    glEnableVertexAttribArray(shader["vVertex"]);
    glVertexAttribPointer(shader["vVertex"], sizeof(shader["vVertex"]), GL_FLOAT, GL_FALSE,0,0);
    GL_CHECK_ERROR();

    //pass quad indices to element array buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
    GL_CHECK_ERROR();

	//setup OpenGL texture and bind to texture unit 0
    glGenTextures( 1, &textureID );
    glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, textureID );

    //set texture parameters
    /*
	GL_CHECK_ERROR();
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	GL_CHECK_ERROR();
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    GL_CHECK_ERROR();
    */

    /* We require 1 byte alignment when uploading texture data */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /* Clamping to edges is important to prevent artifacts when scaling */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    /* Linear filtering usually looks best for text */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    Glyph letter = font->getGlyph("Ř");

    //allocate texture
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 letter.width,
                 letter.height,
                 0,
                 GL_RED,
                 GL_UNSIGNED_BYTE,
                 letter.buffer
                 );

	std::cout << "OpenGL Initialization successfull" << std::endl;
    GL_CHECK_ERROR();
}

void Game::handleEvents()
{
	SDL_Event e;
	
	if( SDL_PollEvent( &e ) )
	{
		switch( e.type )
		{
		
			case SDL_QUIT:
				m_bRunning = false;
				break;
				
			case SDL_KEYDOWN:
				switch( e.key.keysym.sym )
				{
					case SDLK_ESCAPE:
						m_bRunning = false;
						break;
					default:
						break;
				}
				
			default:
				break;
		}
	}
}

void Game::render()
{
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    /* White background */
    glClearColor(1, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Enable blending, necessary for our alpha texture */

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // bind shader
    shader.Use();
        //draw the full screen quad
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_SHORT, 0);
    // unbind shader
    shader.UnUse();

    GL_CHECK_ERROR();

    // swap front and back buffers to show the rendered result
    SDL_GL_SwapWindow( m_pWindow );
}

void Game::update()
{

}

void Game::clean()
{
    //Destroy shader
    shader.DeleteShaderProgram();

    //Destroy vao and vbo
    glDeleteBuffers( 1, &vboVerticesID );
    glDeleteBuffers( 1, &vboIndicesID );
    glDeleteVertexArrays( 1, &vaoID );

    //Delete textures
    glDeleteTextures( 1, &textureID );

    SDL_GL_DeleteContext( m_pGlcontext );
    SDL_DestroyWindow( m_pWindow );
    SDL_Quit();

    delete font;

    std::cout << "Exitting...\n";
}
