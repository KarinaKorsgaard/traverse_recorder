
#include "ofMain.h"
#include "ofxLibsndFileRecorder.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"

#define NUM_CHANNELS 1
#define SAMPLE_RATE 44100
#define BUFFER_SIZE 1024


class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		void keyPressed  (int key);
		void audioReceived(float * input, int bufferSize, int nChannels);
		bool recording;
		ofxLibsndFileRecorder audioRecorder;
    
    ofxPanel gui;
    ofParameter<float>thresholdLOW;
    ofParameter<float>minSampleLength;
    ofParameter<float>wait;
    float inVol;
    
    double silentSec,sampleLength;
    int audioCount;
    
    string filePath;
    string message;
    

};


