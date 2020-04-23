#include "game.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "controlls.h"
#include "Scenes/TestScene.h"

#include <cmath>

Game* Game::instance = NULL;

Color bgcolor(130, 80, 100);

Image minifont;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;

	score = 0;
	current_scene_index = 0;
	prev_scene_index = -1;

	/*font.loadTGA("data/bitmap-font-white.tga"); //load bitmap-font image
	minifont.loadTGA("data/mini-font-white-4x6.tga"); //load bitmap-font image
	sprite.loadTGA("data/spritesheet.tga"); //example to load an sprite
	*/
	minifont.loadTGA("data/bitmap-font-white.tga"); //load bitmap-font image

	//enableAudio(); //enable this line if you plan to add audio to your application
	//synth.playSample("data/coin.wav",1,true);
	//synth.osc1.amplitude = 0.5;
	//Sprite char_spr = Sprite(character);

	game_scenes.push_back((Scene*) new MenuScene(&current_scene_index));
	game_scenes.push_back( (Scene*) new TestScene(&current_scene_index));
}

//what to do when the image has to be draw
void Game::render(void)
{
	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	Image framebuffer(160, 120); //do not change framebuffer size

	//add your code here to fill the framebuffer
	//...

	//some new useful functions
		framebuffer.fill( bgcolor );								//fills the image with one color
		//framebuffer.drawLine( 0, 0, 100,100, Color::RED );		//draws a line
		//framebuffer.drawImage( sprite, 0, 0 );					//draws full image
		//framebuffer.drawImage( sprite, 0, 0, framebuffer.width, framebuffer.height );			//draws a scaled image
		//framebuffer.drawImage( sprite, 0, 0, Area(0,0,14,18) );	//draws only a part of an image
		//framebuffer.drawText( "Hello World", 0, 0, font );				//draws some text using a bitmap font in an image (assuming every char is 7x9)
		//framebuffer.drawText( toString(time), 1, 10, minifont,4,6);	//draws some text using a bitmap font in an image (assuming every char is 4x6)
	game_scenes[current_scene_index]->render(&framebuffer);
	//Image* tile_set = ImageManager::getImage("data/tileset.tga");
	//Image im = tile_set->getArea(0,16,16,16);
	//framebuffer.drawImage(im, 0,0, 0, 0, 16, 16);
	//send image to screen
	//framebuffer.drawText("FPS: " + fps, 0, 0, minifont);
	showFramebuffer(&framebuffer);
	//std::cout << fps <<std::endl;
}

void Game::update(double seconds_elapsed)
{
	//Add here your update method
	//...

	if (current_scene_index != prev_scene_index) {
		// Clean prev scene
		if (prev_scene_index > 0 && game_scenes.size() > prev_scene_index)
			game_scenes[prev_scene_index]->close();
		// Init the new scene
		game_scenes[current_scene_index]->init();
		prev_scene_index = current_scene_index;
	}

	int pressed_controls = 0;
	//to read the gamepad state
	if (Input::gamepads[0].isButtonPressed(A_BUTTON) || Input::isKeyPressed(SDL_SCANCODE_A)) //if the A button is pressed
	{
		pressed_controls ^= A;
	}
	if (Input::gamepads[0].isButtonPressed(B_BUTTON) || Input::isKeyPressed(SDL_SCANCODE_S)) //if the A button is pressed
	{
		pressed_controls ^= B;
	}

	if ((Input::gamepads[0].direction & PAD_UP) || Input::isKeyPressed(SDL_SCANCODE_UP)) //left stick pointing up
	{
		pressed_controls ^= UP;
	}
	if ((Input::gamepads[0].direction & PAD_DOWN) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) //left stick pointing up
	{
		pressed_controls ^= DOWN;
	}
	if ((Input::gamepads[0].direction & PAD_RIGHT) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //left stick pointing up
	{
		pressed_controls ^= RIGHT;
	}
	if ((Input::gamepads[0].direction & PAD_LEFT)  || Input::isKeyPressed(SDL_SCANCODE_LEFT)) //left stick pointing up
	{
		pressed_controls ^= LEFT;
	}
	game_scenes[current_scene_index]->button_press_events(pressed_controls);
	game_scenes[current_scene_index]->update(seconds_elapsed, time);
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseMove(SDL_MouseMotionEvent event)
{
}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	window_width = width;
	window_height = height;
}

//sends the image to the framebuffer of the GPU
void Game::showFramebuffer(Image* img)
{
	static GLuint texture_id = -1;
	static GLuint shader_id = -1;
	if (!texture_id)
		glGenTextures(1, &texture_id);

	//upload as texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

	glDisable(GL_CULL_FACE); glDisable(GL_DEPTH_TEST); glEnable(GL_TEXTURE_2D);
	float startx = -1.0; float starty = -1.0;
	float width = 2.0; float height = 2.0;

	//center in window
	float real_aspect = window_width / (float)window_height;
	float desired_aspect = img->width / (float)img->height;
	float diff = desired_aspect / real_aspect;
	width *= diff;
	startx = -diff;

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(startx, starty + height);
	glTexCoord2f(1.0, 0.0); glVertex2f(startx + width, starty + height);
	glTexCoord2f(1.0, 1.0); glVertex2f(startx + width, starty);
	glTexCoord2f(0.0, 1.0); glVertex2f(startx, starty);
	glEnd();

	/* this version resizes the image which is slower
	Image resized = *img;
	//resized.quantize(1); //change this line to have a more retro look
	resized.scale(window_width, window_height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (1) //flip
	{
	glRasterPos2f(-1, 1);
	glPixelZoom(1, -1);
	}
	glDrawPixels( resized.width, resized.height, GL_RGBA, GL_UNSIGNED_BYTE, resized.pixels );
	*/
}

//AUDIO STUFF ********************

SDL_AudioSpec audio_spec;

void AudioCallback(void*  userdata,
	Uint8* stream,
	int    len)
{
	static double audio_time = 0;

	memset(stream, 0, len);//clear
	if (!Game::instance)
		return;

	Game::instance->onAudio((float*)stream, len / sizeof(float), audio_time, audio_spec);
	audio_time += len / (double)audio_spec.freq;
}

void Game::enableAudio()
{
	SDL_memset(&audio_spec, 0, sizeof(audio_spec)); /* or SDL_zero(want) */
	audio_spec.freq = 48000;
	audio_spec.format = AUDIO_F32;
	audio_spec.channels = 1;
	audio_spec.samples = 1024;
	audio_spec.callback = AudioCallback; /* you wrote this function elsewhere. */
	if (SDL_OpenAudio(&audio_spec, &audio_spec) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	SDL_PauseAudio(0);
}

void Game::onAudio(float *buffer, unsigned int len, double time, SDL_AudioSpec& audio_spec)
{
	//fill the audio buffer using our custom retro synth
	synth.generateAudio(buffer, len, audio_spec);
}
