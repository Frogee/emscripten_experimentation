#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
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
SDL_Surface *asset = NULL;
SDL_Surface *message = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;

//The event structure that will be used
SDL_Event event;

//The font that's going to be used
TTF_Font *font = NULL;

//The color of the font
SDL_Color textColor = { 128, 0, 128 };

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
    SDL_WM_SetCaption( "Tests", NULL );

     //Initialize SDL_ttf
    if( TTF_Init() == -1 ) {return false;}

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

  background = load_image("asset_dir/Background.png");
  asset = load_image("asset_dir/dino_transp_resized.png");

  //Open the font
  std::cout << "Trying to open font" << std::endl;
  //font = TTF_OpenFont("sans-serif", 28);
  font = TTF_OpenFont("fantasy", 28);
  if( font == NULL ) {
        std::cout << "couldn't open font" << std::endl;
        return false;
  }
  if( font != NULL ) {
        std::cout << "opened font?" << std::endl;
  }

  //Render the text
  message = TTF_RenderText_Solid(font, "DinoGurl's Big Adventure!", textColor);

  SDL_BlitSurface (background, NULL, screen, NULL);


  //Apply the background to the screen
  apply_surface( 0, 0, background, screen );

  //Apply the asset to the screen
  apply_surface( 180, 260, asset, screen );

  //Apply the message to the screen
  apply_surface(220, 140, message, screen);

  SDL_Flip(screen);

  emscripten_set_main_loop(one_iter, 1, 1);

  std::cout << "This image was generated in part by the Lazy Foo code." << std::endl;

  //Quit SDL
  SDL_Quit();
  //Free the surfaces
  SDL_FreeSurface( background );
  SDL_FreeSurface( asset);
  SDL_FreeSurface( message );
  //Close the font that was used
  TTF_CloseFont( font );
  //Quit SDL_ttf
  TTF_Quit();
  //Quit SDL
  SDL_Quit();
  return 0;
}
