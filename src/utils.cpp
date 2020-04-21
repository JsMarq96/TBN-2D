#include "utils.h"

#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/time.h>
#endif

#include "includes.h"
#include "game.h"

// Custom Overlapping functions
bool inRange(int low, int high, int x) { 
    return ((x-high)*(x-low) <= 0); 
} 

// If check if the areas overlap or touch
 bool partially_inside(Area a1, Area a2) {
	if (inRange(a1.x, a1.x + a1.w, a2.x) || inRange(a1.x, a1.x + a1.w, a2.x + a2.w)) {
		if (inRange(a1.y, a1.y + a1.h, a2.y) || inRange(a1.y, a1.y + a1.h, a2.h + a2.y)) {
			return true;
		}
	}
	
	return false;
	//return ((a1.x <= (a2.x + a2.w) || a2.x <= (a1.x + a1.w)) && (a1.y >= (a2.y + a2.h) || a2.y >= (a1.y + a1.h)));
 }

Vector2 distance_between_area_center(Area a1, Area a2) {
	Vector2 mid_adr1 = Vector2(a1.x + (a1.w/2), a1.y + (a1.h/2));
	
	Vector2 mid_adr2 = Vector2(a2.x + (a2.w/2), a2.y + (a2.h/2));
	
	mid_adr1 -= mid_adr2;
	//mid_adr1 /= sqrt((mid_adr1.x * mid_adr1.x) + (mid_adr1.y * mid_adr1.y) );
	//mid_adr1 /= mid_adr1.length();
	return mid_adr1;
 }

 Vector2 direction_between_areas(Area a1, Area a2) {
	Vector2 mid_adr1 = Vector2(a1.x + (a1.w/2), a1.y + (a1.h/2));
	
	Vector2 mid_adr2 = Vector2(a2.x + (a2.w/2), a2.y + (a2.h/2));
	
	mid_adr1 -= mid_adr2;
	mid_adr1 /= sqrt((mid_adr1.x * mid_adr1.x) + (mid_adr1.y * mid_adr1.y) );
	//mid_adr1 /= mid_adr1.length();

	mid_adr1.x = round(mid_adr1.x);
	mid_adr1.y = round(mid_adr1.y);
	
	return mid_adr1;
}

//returns time in milliseconds
long getTime()
{
	return (long)SDL_GetTicks();
	/*
	#ifdef WIN32
		return GetTickCount();
	#else
		struct timeval tv;
		gettimeofday(&tv,NULL);
		return (int)(tv.tv_sec*1000 + (tv.tv_usec / 1000));
	#endif
	*/
}

long getPrecisionTime()
{
	return SDL_GetPerformanceCounter();
}

//this function is used to access OpenGL Extensions (special features not supported by all cards)
void* getGLProcAddress(const char* name)
{
	return SDL_GL_GetProcAddress(name);
}

//Retrieve the current path of the application
#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

#ifdef WIN32
	#include <direct.h>
	#define GetCurrentDir _getcwd
#else
	#include <unistd.h>
	#define GetCurrentDir getcwd
#endif

std::string getPath()
{
    std::string fullpath;
    // ----------------------------------------------------------------------------
    // This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);
    chdir(path);
    fullpath = path;
#else
	 char cCurrentPath[1024];
	 if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
		 return "";

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
	fullpath = cCurrentPath;

#endif    
    return fullpath;
}

std::string toString(float v) { std::ostringstream ss; ss << v; return std::string(ss.str()); }

bool readFile(const std::string& filename, std::string& content)
{
	content.clear();

	long count = 0;

	FILE *fp = fopen(filename.c_str(), "rb");
	if (fp == NULL)
	{
		std::cerr << "::readFile: file not found " << filename << std::endl;
		return false;
	}

	fseek(fp, 0, SEEK_END);
	count = ftell(fp);
	rewind(fp);

	content.resize(count);
	if (count > 0)
	{
		count = fread(&content[0], sizeof(char), count, fp);
	}
	fclose(fp);

	return true;
}


bool checkGLErrors()
{
	#ifdef _DEBUG
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL Error: " << errString << std::endl;
		return false;
	}
	#endif

	return true;
}

std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


Vector2 getDesktopSize( int display_index )
{
  SDL_DisplayMode current;
  // Get current display mode of all displays.
  int should_be_zero = SDL_GetCurrentDisplayMode(display_index, &current);
  return Vector2( (float)current.w, (float)current.h );
}

std::vector<std::string> tokenize(const std::string& source, const char* delimiters, bool process_strings)
{
	std::vector<std::string> tokens;

	std::string str;
	size_t del_size = strlen(delimiters);
	const char* pos = source.c_str();
	char in_string = 0;
	unsigned int i = 0;
	while (*pos != 0)
	{
		bool split = false;

		if (!process_strings || (process_strings && in_string == 0))
		{
			for (i = 0; i < del_size && *pos != delimiters[i]; i++);
			if (i != del_size) split = true;
		}

		if (process_strings && (*pos == '\"' || *pos == '\''))
		{
			if (!str.empty() && in_string == 0) //some chars remaining
			{
				tokens.push_back(str);
				str.clear();
			}

			in_string = (in_string != 0 ? 0 : *pos);
			if (in_string == 0)
			{
				str += *pos;
				split = true;
			}
		}

		if (split)
		{
			if (!str.empty())
			{
				tokens.push_back(str);
				str.clear();
			}
		}
		else
			str += *pos;
		pos++;
	}
	if (!str.empty())
		tokens.push_back(str);
	return tokens;
}






