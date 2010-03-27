#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, BITS, 512, OF_WINDOW);
	ofRunApp(new testApp());
}
