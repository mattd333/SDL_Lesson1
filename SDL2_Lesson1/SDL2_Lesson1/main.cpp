/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <iostream>

//Window dimension constants
int WINDOW_WIDTH = 300;
int WINDOW_HEIGHT = 400;
int WINDOW_POSITION_X = 100;
int WINDOW_POSITION_Y = 200;

//Pointer to the window
SDL_Window* window;
//Pointer to its renderer
SDL_Renderer* renderer;

//Function that initializes all the overhead -- SDL, window, renderer
int Initialize_everything();

//Function that intializes SDL
bool Initialize_SDL();

//Function that creates and initializes the window
bool Initialize_window();

//Function that creates renderer
bool Create_renderer();
//Function that sets up renderer
void Set_up_renderer();

//Function that renders the image to the screen at the end of each main game loop
void Render();

//The game engine
void Run_game();

SDL_Rect Player_position;

using namespace std;



//******************************************************************************************************
//******************************************************************************************************
//******************************************************************************************************
int main( int argc, char* args[] )
{
    int Initialization_return_value = Initialize_everything();
	if ( Initialization_return_value != 0 ) 
	{	
       cout<< "ERROR: In main, error code " << Initialization_return_value << "\n";
       return(-1);
    }

	// Initialize player position
	Player_position.x = 20;
	Player_position.y = 20;
	Player_position.w = 20;
	Player_position.h = 20;

	Run_game();
    
    
    return(0);
}
//******************************************************************************************************
//******************* END OF MAIN FUNCTION *************************************************************
//******************************************************************************************************


//******************************************************************************************************
int Initialize_everything()
{
 bool SDL_initialized_successfully = Initialize_SDL();
 if ( SDL_initialized_successfully == false )
 {
    return(-1);
 }
 
 bool Window_created_successfully = Initialize_window();
 if ( Window_created_successfully == false )
 {
    return(-2);
 }
 
 bool Renderer_created_successfully = Create_renderer();
 if ( Renderer_created_successfully == false )
 {
    return(-3);
 }
 
 return(0);

}
//******************************************************************************************************


//******************************************************************************************************
bool Initialize_SDL()
{
   int SDL_initialized_successfully = SDL_Init( SDL_INIT_EVERYTHING );
   if ( SDL_initialized_successfully != 0 )
   {
      cout<< "ERROR: In 'Initialize_SDL()', SDL failed to initialize: " << SDL_GetError() << "\n";
      return( false );  
   }
   
   return(true);
}
//******************************************************************************************************


//******************************************************************************************************
bool Initialize_window()
{
   window = SDL_CreateWindow( "Window title", WINDOW_POSITION_X, WINDOW_POSITION_Y, WINDOW_WIDTH, WINDOW_HEIGHT, 0 );
   
   if ( window == nullptr )
   {
      cout<< "ERROR: In 'Create_window()', Failed to create window: " << SDL_GetError() << "\n";
      return( false );
   }
   
   return( true );
}
//******************************************************************************************************


//******************************************************************************************************
bool Create_renderer()
{
 renderer = SDL_CreateRenderer( window, -1, 0 );
 
 if ( renderer == nullptr )
 {
    cout<< "ERROR: In 'Initialize_renderer()', Failed to create renderer: " << SDL_GetError() << "\n";
    return( false ); 
 }
 
 return( true );

}
//******************************************************************************************************


//******************************************************************************************************
void Set_up_renderer()
{
   //Set size of renderer to same size as window
   SDL_RenderSetLogicalSize( renderer, WINDOW_WIDTH, WINDOW_HEIGHT );
   
   //Set color of renderer to green
   SDL_SetRenderDrawColor( renderer, 0 , 255, 0, 255 ); 
}
//******************************************************************************************************


//******************************************************************************************************
void Render()
{
   //Clear the window and make it all green
   SDL_RenderClear( renderer );
   
   //Change color to blue
   SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
   
   // Render player
   SDL_RenderFillRect( renderer, &Player_position );
   
   //Change color to green 
   SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255 );
   
   //Render the changes above
   SDL_RenderPresent( renderer );
   
}
//******************************************************************************************************


//******************************************************************************************************
void Run_game()
{

 bool Main_loop = true;
 
 while( Main_loop == true )
 {
    SDL_Event event;
    while ( SDL_PollEvent( &event ) )
    {  
       if ( event.type == SDL_QUIT )
       {
          Main_loop = false;
          
          //Wait two seconds
          SDL_Delay( 2000 );
          
       } //end of quit check
       else if ( event.type == SDL_KEYDOWN )
       {
          switch ( event.key.keysym.sym )
          {
             case SDLK_RIGHT:
                Player_position.x = Player_position.x + 5;
                break;
             
             case SDLK_LEFT:
                Player_position.x = Player_position.x - 5;
                break;
                
             //NOTE: In SDL, (0,0) is left,top corner, as y increases coordinate goes down screen
             case SDLK_DOWN:
                Player_position.y = Player_position.y + 5;
                break;
                
             case SDLK_UP:
                Player_position.y = Player_position.y - 5;
                break;
                
             default:
                break;                                                        
          } //end of keydown switch cases
       } //end of keydown check
       
    } //end of event polling
    
    Render();
    
    //Add a 16 ms delay to make game run at approximately 60 fps
    SDL_Delay( 16 );
  
 } // end of main game loop

}
//******************************************************************************************************
