#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 800, 400, OF_WINDOW);
	//ofSetupOpenGL(&window, 1440, 900, OF_FULLSCREEN);
	ofRunApp(new testApp());
}
