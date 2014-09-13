#include "Utility.h"

std::string GLErrorString( GLenum errorCode )
{
	std::vector<std::pair<GLenum, std::string> > errors;
	errors.push_back( std::pair<GLenum, std::string>( GL_NO_ERROR, "no error" ) );
	errors.push_back( std::pair<GLenum, std::string>( GL_INVALID_ENUM, "invalid enumerant" ) );
	errors.push_back( std::pair<GLenum, std::string>( GL_INVALID_VALUE, "invalid value" ) );
	errors.push_back( std::pair<GLenum, std::string>( GL_INVALID_OPERATION, "invalid operation" ) );
	errors.push_back( std::pair<GLenum, std::string>( GL_STACK_OVERFLOW, "stack overflow" ) );
	errors.push_back( std::pair<GLenum, std::string>( GL_STACK_UNDERFLOW, "stack underflow" ) );
	errors.push_back( std::pair<GLenum, std::string>( GL_OUT_OF_MEMORY, "out of memory" ) );
	//errors.push_back(std::pair<GLenum, std::string>(0, NULL));
	
	for( auto it = std::begin( errors ); it != std::end( errors ); ++it )
	{
		if( errorCode == it->first ) return it->second;
	}
	return "unknown error";
}
