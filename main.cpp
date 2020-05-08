#include "SDL.h"
#include "SDL_image.h"
#include "Sprite.h"
#include <iostream>
#include <vector>
//`sdl2-config --cflags --libs`
//g++ main.cpp Sprite.cpp `sdl2-config --cflags --libs` -lSDL2_image


#define window_width 600
#define window_height 600
#define fps 60
using namespace std;
void cap_framerate(Uint32 starting_tick){

     if(( 1000 / fps > SDL_GetTicks() - starting_tick)){
     	SDL_Delay(1000 / fps - (SDL_GetTicks() - starting_tick));
     }
}


class SpriteGroup{
    private:
    	vector <Sprite*> SpriteSTORAGE;
    	int Size_SpriteSTORAGE;


	public:
		SpriteGroup copy(){
			SpriteGroup new_Group;
			for(int i = 0; i < Size_SpriteSTORAGE; ++i){
				new_Group.add(SpriteSTORAGE[i]);
			}
			return new_Group;
		}
		void add(Sprite *sprite){
			SpriteSTORAGE.push_back(sprite);
			Size_SpriteSTORAGE = SpriteSTORAGE.size();


		}
		void remove( Sprite sprite_object){
			for(int i = 0; i < Size_SpriteSTORAGE; ++i){
				if(*SpriteSTORAGE[i] == sprite_object){
					SpriteSTORAGE.erase(SpriteSTORAGE.begin()+i);

				}
			}
			Size_SpriteSTORAGE = SpriteSTORAGE.size();
		}
		bool has(Sprite sprite_object){
			for(int i = 0; i < Size_SpriteSTORAGE; ++i){
				if(*SpriteSTORAGE[i] == sprite_object){
					return true;
				}
			}
			return false;
		}
		void update(){

			if(!SpriteSTORAGE.empty()){
				for(int i = 0; i < Size_SpriteSTORAGE; ++i){
					SpriteSTORAGE[i]->update();
					}
				}
		}
		void draw(SDL_Surface *destination){

			if(!SpriteSTORAGE.empty()){
				for(int i = 0; i < Size_SpriteSTORAGE; ++i){
					SpriteSTORAGE[i]->draw(destination);
				}
			}
		}
		void empty(){
			SpriteSTORAGE.clear();
			Size_SpriteSTORAGE = SpriteSTORAGE.size();


		}
		int size(){
			return Size_SpriteSTORAGE;
		}


		vector <Sprite*> get_Sprites(){
			return SpriteSTORAGE;
		}

};

class Block : public Sprite{


    public:

	Block(Uint32 color,int x,int y,int w = 50,int h = 50) :

	Sprite(color,x,y){
		update_properties();
	}
	void update_properties(){
      set_ox(0);
      set_oy(0);
      set_position(get_rect()->x,get_rect()->y);
	}
	void set_position(int x,int y){
		get_rect()->x = x - get_ox();
		get_rect()->y = y - get_oy();

	}
	void set_image(const char filename[] = NULL){
		if(filename != NULL){
			SDL_Surface *loaded_image = NULL;
			loaded_image = IMG_Load(filename);

			if(loaded_image != NULL){
        set_Image(loaded_image);
				int old_x = get_rect()->x;
				int old_y = get_rect()->y;
        set_rect();
				get_rect()->x = old_x;
				get_rect()->y = old_y;

				update_properties();



			}
		}else {
//check for errors while image loading
cout << "error" << SDL_GetError() <<endl;
}

	}


};
Block *createBlock(Uint32 color,char image[100],int posX,int posY){
	Block *block = new Block(color,posX,posY);
	block->set_image(image);
	return block;
}


int main ( int argc , char const *argv[] ){

	SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = NULL;
     window = SDL_CreateWindow(
        "2048",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        window_width,                               // width, in pixels
        window_height,                               // height, in pixels
        0                  // flags - see below
    );
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Surface *screen = SDL_GetWindowSurface(window);

    Uint32 gray = SDL_MapRGB(screen->format, 128,128,128);//unsigned int 32
    Uint32 red = SDL_MapRGB(screen->format, 255,0,0);//unsigned int 32
    Uint32 blue = SDL_MapRGB(screen->format, 0,0,255);//unsigned int 32
    Uint32 white = SDL_MapRGB(screen->format, 255,255,255);//unsigned int 32

    SDL_FillRect(screen,NULL,white);

    Block block(blue,100,100);
    SpriteGroup active_sprites;
    for(int i = 0; i < 5; ++i){
    	active_sprites.add(createBlock(blue,"tenor.gif",100*i,100*i));

    }
    active_sprites.draw(screen);

    SDL_UpdateWindowSurface(window);



    SDL_Event event;
    Uint32 starting_tick;

    bool running = true;
    while(running){

    	starting_tick = SDL_GetTicks();

    	 while(SDL_PollEvent(&event)){
    	 	if(event.type == SDL_QUIT){
    	 		running = false;
    	 		break;
    	 	}
    	 }
    	 cap_framerate(starting_tick);

    }



	SDL_Quit();


	return 0;
}
