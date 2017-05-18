#include "ofApp.h"




//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	recording=false;
    
	ofSoundStreamSetup(0,NUM_CHANNELS,this, SAMPLE_RATE,BUFFER_SIZE,4);
    
    gui.setup();
    gui.add(thresholdLOW.set("volume threshold",0.26,0,0.5));
    gui.add(wait.set("allowed pauses",0.80,0,2));
    gui.add(minSampleLength.set("Min Sample Length",3.,0,20));
    
    gui.loadFromFile("settings.xml");
    
    ofxXmlSettings xml;
    if(xml.load("config.xml"))
       filePath = xml.getValue("config:path", "");
    else filePath = "Users/annelieberner/Dropbox/sounds/outro/";
    
    ofDirectory dir;
    string path =filePath;
    dir.allowExt("wav");
    dir.listDir(path);
    dir.sort();
    
    message = dir.isDirectory()? "directory found, looking in: "+filePath:"error: directory not found. Change path in config.xml";
    
    
    if(dir.size()>0){
        cout << dir.getName(dir.size()-1)<< endl;
        vector<string> name = ofSplitString(dir.getName(dir.size()-1), ".");
        audioCount = ofToInt(name.front())+1;
    }else{
        audioCount=0;
    }
    cout << audioCount << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
    double dt = ofGetLastFrameTime();
    
    if(recording)sampleLength+=dt;
    
    if(inVol >= thresholdLOW){
        silentSec = 0.f;
        
        if(!recording){
            cout<<"Start recording\n";
            audioCount++;
            string pt=filePath+ofToString(audioCount,0)+".wav";
        
            cout << pt<<"----\n";
            audioRecorder.setup(pt);
            audioRecorder.setFormat(SF_FORMAT_WAV | SF_FORMAT_PCM_16);
            recording=true;
            
        }
    }
    else if(recording){
        silentSec+=dt;
        
        if(silentSec>wait){
            cout<<"Stop recording\n";
            recording=false;
            
            bool save = sampleLength>minSampleLength;
            if(save){
                audioRecorder.finalize();
            }
            else{
                audioRecorder.initialized=false;
                audioRecorder.recordingSize=0;
                delete audioRecorder.outFile;
                ofFile::removeFile(filePath+ofToString(audioCount,0)+".wav", false);
                cout << filePath+ofToString(audioCount,0)+".wav" << endl;
                //ofFile(filePath+ofToString(audioCount,0)+".wav", false).remove();
                //audioCount--;
            }
            sampleLength=0;
        }
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255);
    
	
	if(recording){
		ofSetColor(100,255,100);
        ofDrawBitmapString("Sample length: "+std::to_string(sampleLength), 10, 300);
	}
    else{
        ofDrawBitmapString("volume: "+std::to_string(inVol), 10, 300);
        ofSetColor(255,100,100);
    }
    ofDrawBitmapString(message, 10, 320);
    ofDrawCircle(400,160,50);
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels){
    
	if (recording)
		audioRecorder.addSamples(input, bufferSize * nChannels);
    

    inVol = accumulate(input, input + bufferSize * nChannels, 0., []( float a, float b ) {
        return std::max(a, b);
    });

    //cout << inVol <<endl;
}

