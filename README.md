# Introduction

PLRModel is a library for photorealistic Pupil Light Reflex and Pupil Latency models for facial animations.

# Running the Demo

	$ ./make.sh
	$ bin/PLRModel

# Usage

The folling code shows how to declare and use the Pamplona's Model: http://bit.ly/duD1oA  

	#include "PupilLifecycle.h"

	float pupilDiameterInMM = 7.1; // Starts with a large pupil in mm. 
	float lightIntensityInBlondels = -2; // Light intensity reaching the retina in Blondels
	float timeInMilliseconds = 100; // time

	PamplonaAndOliveiraModel model; // PupilLightReflex Model.

	float getIntensityInLumens() {
	    return Conversion::blondelToLumensSquareMillimeter(powf(10,lightIntensityInBlondels));
	}

	float evaluateDiameter() {
	    pupilDiameterInMM = model.pupilDiameterAt(getIntensityInLumens(), 250, timeInMilliseconds);
	    timeInMilliseconds += 100;
	    	 
	    return pupilDiameterInMM;
	}

	int main(int argc, char *argv[]) {

	    // Fill t<0 data. 
	    for (int i=0; i<10; i ++) {
	 	 model.addPulse(timeInMilliseconds, getIntensityInLumens(), Conversion::diameterToArea(pupilDiameterInMM));
	 	 timeInMilliseconds += 100;
	    }

	    lightIntensityInBlondels = -2;

	    for (int i=0; i<20; i ++) {	
		    std::cout << "In time " << timeInMilliseconds 
			      << " with light intensity of " << lightIntensityInBlondels
			      << ", the pupil diameter is " << evaluateDiameter() << std::endl;
	    }

	    lightIntensityInBlondels = 2;

	    for (int i=0; i<20; i ++) {	
		    std::cout << "In time " << timeInMilliseconds 
			      << " with light intensity of " << lightIntensityInBlondels
			      << ", the pupil diameter is " << evaluateDiameter() << std::endl;
	    }

	    return 0;
	}

