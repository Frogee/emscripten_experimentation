#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <emscripten.h>
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>

class Test {}; // This will fail in C mode

//The attributes of the screen
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces that will be used
SDL_Surface *message = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;

//The event structure that will be used
SDL_Event event;

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Make a temporary rectangle to hold the offsets
    SDL_Rect offset;

    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

SDL_Surface *load_image( std::string filename )
{
    SDL_Surface* loadedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );
    if(!loadedImage) {
        printf("IMG_Load: %s\n", IMG_GetError());
    }
    return loadedImage;
}

bool init() {
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) {return false;}

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL ) {return false;}

    //Set the window caption
    SDL_WM_SetCaption( "Event test", NULL );

    //If everything initialized fine
    return true;
}

void one_iter() {
    std::cout << "Entering iteration" << std::endl;
    while( SDL_PollEvent( &event ) ) {
        if (event.type == SDL_KEYDOWN) {
            std::cout << "Keyboard event!!" << std::endl;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            std::cout << "Mouse button event!" << std::endl;
        }
        if (event.type == SDL_MOUSEMOTION) {
            std::cout << "Mouse moved around event!" << std::endl;
        }
    }
}

int main( int argc, char* args[] ) {
  printf("hello from printf()!\n");
  std::vector<int> test;

  test.push_back(3);
  test.push_back(6);

  std::cout << "Hello from cout!\t" << "\nElement 1 of test vector is: " << test[1]  << std::endl;
//Initialize
    if( init() == false ) {return 1;}

  background = load_image("asset_dir/Test.bmp");
  message = load_image("asset_dir/Test2.bmp");

  SDL_BlitSurface (background, NULL, screen, NULL);


  //Apply the background to the screen
  apply_surface( 0, 0, background, screen );
  apply_surface( 320, 0, background, screen );
  apply_surface( 0, 240, background, screen );
  apply_surface( 320, 240, background, screen );

  //Apply the message to the screen
  apply_surface( 180, 140, message, screen );

  SDL_Flip(screen);

  emscripten_set_main_loop(one_iter, 1, 1);

  std::cout << "This image was generated in part by the Lazy Foo code." << std::endl;

  //Quit SDL
  SDL_Quit();

  return 0;
}
