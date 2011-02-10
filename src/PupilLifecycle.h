/** PupilDynamic - Complete model of the human iris.
    Copyright (C) 2007 Vitor Fernando Pamplona (vitor@vitorpamplona.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PUPILLIFECYCLE_H_
#define PUPILLIFECYCLE_H_

#ifndef _WIN32
#include <pthread.h>
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

#include <cmath>

#include <GL/glut.h>

#include "Singleton.h"
#include "Vector.h"
#include "Util.h"
#include "Conversion.h"

#include "PupilDynamicsModel.h"
#include "MoonAndSpencerModel.h"
#include "ReevesModel.h"
#include "PokornyAndSmithModel.h"
#include "DegrootAndGebhardModel.h"

#include "HistoryFifo.h"
#include "LongtinAndMiltonModel.h"
#include "PamplonaAndOliveiraModel.h"
#include "PamplonaAndOliveiraWithEnvelopeModel.h"

#include "LatencyModel.h"
#include "LinkAndStarkModel.h"
#include "EllisModel.h"

#include "PupilLifecycleInterface.h"

/**
 * Holds on all pupil dynamic factors.
 */
class PupilLifecycle : public Singleton<PupilLifecycle>, public PupilLifecycleInterface {
	PupilDynamicsModel * dynamics;
	LatencyModel * latency;

	// Frequency in cd/mm2;
	float intensity;
	//float newIntensity;

	std::vector<Vector2f> latencyFifo;

public:
	PupilLifecycle()  {
		dynamics = new MoonAndSpencerModel();
		latency = new LinkAndStarkModel(0.4);

		intensity = 0.0f;

		setIntensity(powf(10, 1), 0);
		//newIntensity = 0.0f;
	}
	virtual ~PupilLifecycle() {}

	std::string pupilModel() {
		return dynamics->getName();
	}

	std::string latencyModel() {
		return latency->getName();
	}

	PupilDynamicsModel * getDynamics() {
		return dynamics;
	}

	void setEllisModel() {
		delete latency;
		latency = new EllisModel();
	}

	void setLinkModel() {
		delete latency;
		latency = new LinkAndStarkModel(0.4);
	}

	void setMoonModel() {
		delete dynamics;
		dynamics = new MoonAndSpencerModel();
	}

	void setGrootModel() {
		delete dynamics;
		dynamics = new DegrootAndGebhardModel();
	}

	void setPokornyModel() {
		delete dynamics;
		dynamics = new PokornyAndSmithModel();
	}

	void setReevesModel() {
		delete dynamics;
		dynamics = new ReevesModel();
	}

	void setPamplonaModel(float time) {
		delete dynamics;
		dynamics = new PamplonaAndOliveiraModel();

		float area = 48;
		intensity = Conversion::blondelToLumensSquareMillimeter(pow(10.0f, -5.0f));

		for (int i=100; i>0; i--) {
			((PamplonaAndOliveiraModel *)dynamics)->addPulse(time - 100*i, intensity, area);
		}
	}

	void setPamplonaEnvelopeModel(float time) {
		delete dynamics;
		dynamics = new PamplonaAndOliveiraWithEnvelopeModel();
		((PamplonaAndOliveiraWithEnvelopeModel *) dynamics)->setWithEnvelope(true);

		float area = 48;
		intensity = Conversion::blondelToLumensSquareMillimeter(pow(10.0f, -5.0f));

		for (int i=100; i>0; i--) {
			((PamplonaAndOliveiraModel *)dynamics)->addPulse(time - 100*i, intensity, area);
		}
	}

	void setLongtinModel(float time) {
		delete dynamics;

		dynamics = new LongtinAndMiltonModel();

		float area = 48;
		intensity = Conversion::blondelToLumensSquareMillimeter(pow(10.0f, -5.0f));

		for (int i=100; i>0; i--) {
			((LongtinAndMiltonModel *)dynamics)->addPulse(time - 100*i, intensity, area);
		}
	}

	/**
	 * Intensity in Cd/mm2
	 */
	void setIntensity(float _intensity, float time) {
		latencyFifo.push_back(Vector2f(time + (int)latency->pupilLatencyAt(_intensity), _intensity));
	}

	void nextPupilModel(float timeInMilliseconds) {
		if (pupilModel() == "Degroot And Gebhard"){
			setReevesModel();
		} else if (pupilModel() == "Reeves"){
			 setMoonModel();
		} else if (pupilModel() == "Moon And Spencer"){
			 setPamplonaModel(timeInMilliseconds);
		} else if (pupilModel() == "Our Model"){
			 setPamplonaEnvelopeModel(timeInMilliseconds);
		} else if (pupilModel() == "Our Model With Envelope"){
			 setPokornyModel();
		} else if (pupilModel() == "Pokorny And Smith"){
			 setLongtinModel(timeInMilliseconds);
		} else if (pupilModel() == "Longtin And Milton"){
			 setGrootModel();
		}
	}

	void nextLatencyModel() {
		if (latencyModel() == "Ellis"){
			 setLinkModel();
		} else if (latencyModel() == "Link And Stark"){
			 setEllisModel();
		}
	}

	/** Returns the pupil diameter in mm.
	 */
	float getDiameter(float time) {
		std::vector<Vector2f>:: iterator i;
		for (i = latencyFifo.begin(); i< latencyFifo.end(); i++) {
			if (time > i->x()) {
				intensity = i->y();
			}
		}

		if (dynamics->isInLumens()) {
			float freshIntensity = latencyFifo.size() > 0 ? latencyFifo[latencyFifo.size()-1].y() : intensity;
			freshIntensity = Conversion::blondelToLumensSquareMillimeter(freshIntensity);
			return dynamics->pupilDiameterAt(freshIntensity, latency->pupilLatencyAt(intensity), time);
		} else {
			return dynamics->pupilDiameterAt(intensity, latency->pupilLatencyAt(intensity), time);
		}
	}

	virtual float getDiameter(float time, float _intensity) {
		setIntensity(_intensity, time);
		return getDiameter(time);
	}
};

#endif /*PUPILLIFECYCLE_H_*/
