#include <SDL.h>
//#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <string.h>

bool init();

void close();

// render target
SDL_Window* gWindow = NULL;

// renderer
SDL_Renderer* gRenderer = NULL;

SDL_Rect gScreenRect = {0, 0, 320, 240};
SDL_Rect box = {0, 0, 100, 100};

SDL_Color color = {0x00, 0x00, 0x00, 0xFF};

bool init()
{
    // init flag
    bool success = true;

    // init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL couldn't be initialized! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        // get the device starting mode
        SDL_DisplayMode displayMode;
        
        if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0) {
            gScreenRect.w = displayMode.w;
            gScreenRect.h = displayMode.h;
        }

        // create the window
        gWindow = SDL_CreateWindow( "TridoFlow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gScreenRect.w, gScreenRect.h, SDL_WINDOW_SHOWN );
		
        if( gWindow == NULL ) {
			SDL_Log( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else {
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED  );
			if( gRenderer == NULL )
			{
				SDL_Log( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			}
		}
	}

	return success;
}

bool loadMedia() {
    return true;
}

void close() {
    // destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	// quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char* args[]) {
    // check it opened
    if (!init()) {
        SDL_Log("Failed to initialize!\n");
    }
    else {
        bool quit = false;

        SDL_Event e;
        int laste;

        SDL_Point touchLocation = {gScreenRect.w / 2, gScreenRect.h / 2};

        while (!quit) {
            while( SDL_PollEvent( &e ) != 0 ) {
                //User requests quit
                if( e.type == SDL_QUIT ) {
                    quit = true;
                }
                //Window event
                else if( e.type == SDL_WINDOWEVENT ) {
                    //Window resize/orientation change
                    if( e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED ) {
                        //Get screen dimensions
                        gScreenRect.w = e.window.data1;
                        gScreenRect.h = e.window.data2;
                        
                        //Update screen
                        SDL_RenderPresent( gRenderer );
                    }
                }
                //Touch down
                else if( e.type == SDL_FINGERDOWN )
                {
                    touchLocation.x = e.tfinger.x * gScreenRect.w;
                    touchLocation.y = e.tfinger.y * gScreenRect.h;
                    //currentTexture = &gTouchDownTexture;
                    //SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0xFF);

                    laste = e.type;
                }
                //Touch motion
                else if( e.type == SDL_FINGERMOTION )
                {
                    touchLocation.x = e.tfinger.x * gScreenRect.w;
                    touchLocation.y = e.tfinger.y * gScreenRect.h;
                    //currentTexture = &gTouchMotionTexture;
                    //SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);

                    laste = e.type;
                }
                //Touch release
                else if( e.type == SDL_FINGERUP )
                {
                    touchLocation.x = e.tfinger.x * gScreenRect.w;
                    touchLocation.y = e.tfinger.y * gScreenRect.h;
                    //currentTexture = &gTouchUpTexture;
                    //SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);

                    laste = e.type;
                }

                //Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render touch texture
				//currentTexture->render( touchLocation.x - currentTexture->getWidth() / 2, touchLocation.y - currentTexture->getHeight() / 2 );

                box.x = touchLocation.x - 100 / 2;
                box.y = touchLocation.y - 100 / 2;

                if (laste == SDL_FINGERDOWN) {
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                } else if( laste == SDL_FINGERMOTION ) {
                    SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );
                } else if( laste == SDL_FINGERUP ) {
                    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
                }

                SDL_RenderFillRect(gRenderer, &box);

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
        }
    }

    close();

    return 0;
}