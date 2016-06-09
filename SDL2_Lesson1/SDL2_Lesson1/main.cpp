/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

using namespace std;

//Window dimension constants
int WINDOW_WIDTH_X = 300;
int WINDOW_HEIGHT_Y = 400;
int WINDOW_POSITION_X = 900;
int WINDOW_POSITION_Y = 300;

//Movement constants
//Number of pixels per key press:
int Movement_increment = 15;
int Last_enemy_position = 50;

//Set number of enemies
int Number_of_enemies = 13;

SDL_Rect Player_position;
SDL_Rect Top_bar;
SDL_Rect Bottom_bar;

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

//Function that adds an enemy to the screen
void Add_enemy();

//Function that moves the enemies
void Move_enemies();

//Function that puts player back to start position, either due to winning or losing
void Reset_player_position();

//Function that tests whether two rectangles have collided
bool Check_collision( const SDL_Rect &input_rectangle_1, const SDL_Rect &input_rectangle_2 );

//Function that checks whether enemies have collided with player, uses Check_collision
bool Check_enemy_collisions();


enum class Direction
{
 Left,
 Right
};

class Enemy
{
 public:
    Enemy( SDL_Rect Initial_position, int Initial_speed, Direction Initial_direction )
    {  
       Position = Initial_position;
       Speed = Initial_speed;
       Enemy_direction = Initial_direction;
    }
    
    SDL_Rect Position;
    int Speed;
    Direction Enemy_direction;  
};

vector< Enemy > Enemies_vector;



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
    
    //Add the enemies
    for ( int counter = 0; counter < Number_of_enemies; counter++ )
    {
        Add_enemy();
    }
    
    //Initialize top and bottom bar
    Top_bar.x = 0;
    Top_bar.y = 0;
    Top_bar.w = WINDOW_WIDTH_X;
    Top_bar.h = 20;
    
    Bottom_bar.x = 0;
    Bottom_bar.y = WINDOW_HEIGHT_Y - 20;
    Bottom_bar.w = WINDOW_WIDTH_X;
    Bottom_bar.h = 20;

	// Initialize player
	Player_position.w = 20;
	Player_position.h = 20;
    Reset_player_position();

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
   window = SDL_CreateWindow( "Window title", WINDOW_POSITION_X, WINDOW_POSITION_Y, WINDOW_WIDTH_X, WINDOW_HEIGHT_Y, 0 );
   
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
   SDL_RenderSetLogicalSize( renderer, WINDOW_WIDTH_X, WINDOW_HEIGHT_Y );
   
   //Set color of renderer to green
   SDL_SetRenderDrawColor( renderer, 0 , 255, 0, 255 ); 
}
//******************************************************************************************************


//******************************************************************************************************
void Render()
{
   //Clear the window and make it all white
   SDL_RenderClear( renderer );
   
   //Change color to black
   SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
   
   //Render top and bottom bar
   SDL_RenderFillRect( renderer, &Bottom_bar );
   SDL_RenderFillRect( renderer, &Top_bar );
   
   //Change color to blue
   SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
   
   // Render player
   SDL_RenderFillRect( renderer, &Player_position );
   
   //Change color to red
   SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
   
   //Render the enemies
   for ( const auto &p : Enemies_vector )
   {
       SDL_RenderFillRect( renderer, &p.Position );
   }
   
   //Change the color to green 
   SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
   
   //Render the changes above to the window
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
                Player_position.x = Player_position.x + Movement_increment;
                break;
             
             case SDLK_LEFT:
                Player_position.x = Player_position.x - Movement_increment;
                break;
                
             //NOTE: In SDL, (0,0) is left,top corner, as y increases coordinate goes down screen
             case SDLK_DOWN:
                Player_position.y = Player_position.y + Movement_increment;
                break;
                
             case SDLK_UP:
                Player_position.y = Player_position.y - Movement_increment;
                break;
                
             default:
                break;                                                        
          } //end of keydown switch cases
       } //end of keydown check
       
    } //end of event polling
    
    //After player has moved, check periodic horizontal boundaries
    //Check right boundary
    int Right_edge_of_player = Player_position.x + Player_position.w;
    if ( Right_edge_of_player > WINDOW_WIDTH_X )
    {
        Player_position.x = Player_position.x - WINDOW_WIDTH_X;
    }
    //Check left boundary
    int Left_edge_of_player = Player_position.x;
    if ( Left_edge_of_player < 0 )
    {
        Player_position.x = Player_position.x + WINDOW_WIDTH_X;
    }
    
    Move_enemies();
    
    //Check collisions against enemies
    if ( Check_enemy_collisions() == true )
    {
        cout<< "LOSE\n";
        Reset_player_position();
    }
    
    //Check collision against top bar
    int Bottom_of_top_bar = Top_bar.h + Top_bar.y;
    if ( Player_position.y < Bottom_of_top_bar )
    {
        cout<< "WIN!\n";
        Reset_player_position();
    }
    
    //Check collision against bottom bar
    int Top_of_bottom_bar = Bottom_bar.y;
    if (Player_position.y > Top_of_bottom_bar+1 )
    {
        cout<< "WARNING: Left play area\n"; 
        Reset_player_position();
    } 
    
    Render();
    
    //Add a 16 ms delay to make game run at approximately 60 fps
    SDL_Delay( 16 );
  
 } // end of main game loop

}
//******************************************************************************************************



//******************************************************************************************************
void Add_enemy()
{
 Direction Initial_direction;

 if ( ( rand()%2 )==0 )
 {
     Initial_direction = Direction::Left;
 }
 else
 {
     Initial_direction = Direction::Right;
 }

 
 SDL_Rect Initial_position_rectangle;
 Initial_position_rectangle.x = rand()%300;
 Initial_position_rectangle.y = Last_enemy_position;
 Initial_position_rectangle.w = 20;
 Initial_position_rectangle.h = 20; 
 
 int Initial_speed = 1;
 
 Enemies_vector.push_back( Enemy( Initial_position_rectangle, Initial_speed, Initial_direction  ) );
 
 Last_enemy_position = Last_enemy_position + 25;

}
//******************************************************************************************************



//******************************************************************************************************
void Reset_player_position()
{
 int Middle_of_the_screen = WINDOW_WIDTH_X/2;

 //Center player horizontally on the screen
 Player_position.x = Middle_of_the_screen - (Player_position.w)/2;
 
 //Place player's vertical position just above the bottom bar
 Player_position.y = WINDOW_HEIGHT_Y - Bottom_bar.h;
}
//******************************************************************************************************



//******************************************************************************************************
bool Check_collision( const SDL_Rect &input_rectangle_1, const SDL_Rect &input_rectangle_2 )
{
 //Find edges of input_rectangle_1
 int Left_1 = input_rectangle_1.x;
 int Right_1 = input_rectangle_1.x + input_rectangle_1.w;
 int Top_1 = input_rectangle_1.y;
 int Bottom_1 = input_rectangle_1.y + input_rectangle_1.h;
 
 //Find edges of input_rectangle_2
 int Left_2 = input_rectangle_2.x;
 int Right_2 = input_rectangle_2.x + input_rectangle_2.w;
 int Top_2 = input_rectangle_2.y;
 int Bottom_2 = input_rectangle_2.y + input_rectangle_2.h;
 
 //Check edges, see if they are outside each other
 // If Left_1 is right of Right_2
 if ( Left_1 > Right_2 )
 {
     return( false );
 }
 //If Right_1 is left of Left_2
 if ( Right_1 < Left_2 )
 {
     return( false );
 }
 //If Top_1 is below Bottom_2, recalling that in vertical direction, bigger values correspond to lower positions
 if ( Top_1 > Bottom_2 )
 {
     return( false );
 }
 //If Bottom_1 is above Top_2 (again, in vertical direction, smaller values <-> higher positions)
 if ( Bottom_1 < Top_2 )
 {  
     return( false );
 }
 
 return( true );
}
//******************************************************************************************************




//******************************************************************************************************
bool Check_enemy_collisions()
{
 //Returns true if any enemy rectangle has collided with the player, false otherwise
 
 for ( const auto &p : Enemies_vector )
 {
     if ( Check_collision( p.Position, Player_position ) )
     {
         return( true );
     }
 }

 return( false );
}
//******************************************************************************************************



//******************************************************************************************************
void Move_enemies()
{
 for ( auto &p : Enemies_vector )
 {
     if ( p.Enemy_direction == Direction::Right )
     {
         p.Position.x = p.Position.x + p.Speed;
         
         //Make horizontal direction periodic
         int Right_edge_of_enemy = p.Position.x + p.Position.w;
         
         if ( Right_edge_of_enemy > WINDOW_WIDTH_X )
         {
             p.Position.x = p.Position.x - WINDOW_WIDTH_X;
         }
     } //End of right direction case
     else
     {
         p.Position.x = p.Position.x - p.Speed;
         
         //Make horizontal direction periodic
         int Left_edge_of_enemy = p.Position.x;
         
         if ( Left_edge_of_enemy <= 0 )
         {
             p.Position.x = p.Position.x + WINDOW_WIDTH_X;
         }
     
     } //End of left direction case
     
 } //End of looping through vector of enemies

}
//******************************************************************************************************



