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

#ifndef PamplonaAndOliveiraWithEnvelopeMODEL_H_
#define PamplonaAndOliveiraWithEnvelopeMODEL_H_


class HistoryItem {
public: 
	Vector3f atual;
	Vector3f anterior;
	
	HistoryItem() {}
};

/**
 * Pamplona's Model for Pupil Light Reflex. Implementing our pupil light reflex model with an envelope.  
 * 
 * Source: Pamplona, V. F., Oliveira, M. M., and Baranoski, G. V. G. (2009). Photorealistic models for pupil light reflex and iridal pattern deformation. ACM Trans. Graph. 28, 4 (Aug. 2009), 106:1-106:12. DOI= http://doi.acm.org/10.1145/1559755.1559763
 * 
 * Equation 20;
 */ 
class PamplonaAndOliveiraWithEnvelopeModel : public PupilDynamicsModel {
	
	// x = time (milliseconds) , 
	// y = intensity (lumens), 
	// z = pupil diameter (mm).
	HistoryFifo<Vector3f, 1000> history;
	
	float dt;
	float phiBar;
	float subjectBias;
	float age;
	bool withEnvelope;
	
public:
	PamplonaAndOliveiraWithEnvelopeModel() : PupilDynamicsModel("Our Model With Envelope") {		
		init();
	}
	virtual ~PamplonaAndOliveiraWithEnvelopeModel() {}
	
	void init() {
		dt = 0.3f;
		subjectBias = 0.42; // Vitor
		withEnvelope = false;
		age = 20;

		phiBar = evalPhiBar();
	}

	float evalPhiBar() {
		// Calculando PHI BARRA.
		MoonAndSpencerModel *  moon = new MoonAndSpencerModel();
		// Lower intensity
		float phiBarIntensityBlondels = powf(10, -5);
		// To Lumens per Square MM
		float phiBarIntensityLumensMM = Conversion::blondelToLumensSquareMillimeter(phiBarIntensityBlondels);
		// Get pupil diameter
		float phiBarDiameter = moon->pupilDiameterWithBlondel(phiBarIntensityBlondels);
		// apply subject variatio
		phiBarDiameter = applySubjectPupilVariation(phiBarDiameter, subjectBias);
		// find phiBar = Area * lumens/MM2
		float phiBar = Conversion::diameterToArea(phiBarDiameter) * phiBarIntensityLumensMM;
		return phiBar;
	}
	
	void setWithEnvelope(bool v) {
		withEnvelope = v;
		phiBar = evalPhiBar();
	}
	
	void setSubjectBias(float bias) {
		subjectBias = bias;
		phiBar = evalPhiBar();
	}
	
	float getSubjectBias() {
		return subjectBias;
	}
	
	
	void setAge(float bias) {
		age = bias;
	}
	
	float getAge() {
		return age;
	}
		
	
	virtual bool isInLumens() { return true; }
	
	HistoryFifo<Vector3f, 1000> getHistory() {
		return history;
	}
	
	void setDt(float _dt) {
		dt = _dt;
	}

	float getDt() {
		return dt;
	}	
	
	

	/** 
         * Polynomial generated from Moon and Spencer data
         */
	float evaluateMoonUpperBound(float diameterMM) {
		return -0.0126725930676599 * powf(diameterMM,5)
		     + 0.3221924543867559 * powf(diameterMM,4)
		     + -3.0962731561456747 * powf(diameterMM,3)
		     + 13.6546122786699637  * powf(diameterMM,2)
		     + -25.3475379179258162 * diameterMM
		     + 18.1791129454502212; 
	}
	
	/** 
         * Polynomial generated from Moon and Spencer data
         */
	float evaluateMoonLowerBound(float diameterMM) {
		return -5.44158133181249E-3 * powf(diameterMM,5)
		     + 1.38697487323348E-1 * powf(diameterMM,4)
		     + -1.34343594792456 * powf(diameterMM,3)
		     + 6.21951063864639 * powf(diameterMM,2)
		     + -1.31667597501514E+1 * diameterMM
		     + 1.21911721275106E+1; 
	}	
	
	void addPulse(float mSeconds, float intensity, float area) {		
		if (area < 2.7000f) area = 2.7001;
		if (area > 48.890f) area = 48.889;
		
		history.add(Vector3f(mSeconds, intensity, area));
	}
	
	/**
	 * Returns the flux in the retina. 
	 * 
	 * lightItensity in Lumens mm^-2
	 * pupilArea in mm^2
	 */
	float retinalFlux(float lightIntensity, float pupilArea) {
		return lightIntensity * pupilArea;
	}
	
	HistoryItem * getFromHistory(float latencyInMilliseconds) {
		int size = history.history.size()-1;
		
		double time = history.history[size].x();
		double fromTime = time - latencyInMilliseconds;
		
		HistoryItem * item = new HistoryItem();
		for (int i=size; i>=0; i--) {
			if (!(fromTime < history.history[i].x())) {
				item->atual = history.history[i];
				item->anterior = history.history[i+1];
				
				if (i == size) {
					item->anterior = item->atual;
				}
				
				return item;
			}
		}
		return NULL;
	}
	
	float intensityAt(float latency) {
		int size = history.history.size()-1;
		
		double time = history.history[size].x();
		double fromTime = time - latency;
		
		HistoryItem * item = getFromHistory(latency);
		Vector3f iAtual = item->atual;
		Vector3f iAnterior = item->anterior;

		if (item == NULL) return 0;
		
		float deltaTime = iAnterior.x() - iAtual.x();
		float resto = fromTime - iAtual.x();

		float percent = 0.1;

		if (fabs(deltaTime) > 0.01)
			percent = resto / deltaTime;
				
		// linear filter
		float intensity = iAtual.y() + (iAnterior.y() - iAtual.y()) * percent;
		return intensity;
	}
	
	float retinalFlux(float latencyInMilliseconds) {
		int size = history.history.size()-1;
		
		double time = history.history[size].x();
		double fromTime = time - latencyInMilliseconds;
		
		HistoryItem * item = getFromHistory(latencyInMilliseconds);
		Vector3f iAtual = item->atual;
		Vector3f iAnterior = item->anterior;

		if (item == NULL) return 0;
		
		float deltaTime = iAnterior.x() - iAtual.x();
		float resto = fromTime - iAtual.x();

		float percent = 0.1;

		if (fabs(deltaTime) > 0.01)
			percent = resto / deltaTime;
				
		// linear filter
		float intensity = iAtual.y() + (iAnterior.y() - iAtual.y()) * percent;
		float area 		= iAtual.z() + (iAnterior.z() - iAtual.z()) * percent;
	
		return retinalFlux(intensity, area);
	}
	
	float logarithmOfRetinalFluxRate(float latency) {
		return log(retinalFlux(latency)/phiBar);
	}
	
	/** Returns the afferent or efferent neural action potential per time */
	float muscleActivity(float latency) {
		return 5.2 - 0.45 * logarithmOfRetinalFluxRate(latency); 
	}
	
	float getLumens(float diameter) {
		return 0;
	}
		
	float arcTanH(float x) {
		return 0.5f * (log(1+x) - log(1-x));
	}

	float m(float diameter) {
		return arcTanH((diameter - 4.9) / 3);
	}

	float evaluateLeftSide(float time, float dD, float latency) {
		int size = history.history.size()-1;
		
		float dT = (time - history.history[size].x()) / 600.0f;
		float prevDiammeter = Conversion::areaToDiameter(history.history[history.history.size()-1].z());

		float diameter = prevDiammeter + dD;
		float prevM = m(prevDiammeter);
		float dM = m(diameter) - prevM;
		
		if (dD > 0) {
			// Dilation Velocity
			dT /= 3.0f;
		} 
		
		if (equals(dM, 0.000f,0.00000000001f) || equals(dT, 0.000f,0.0000000001f))
			return 2.3025*m(diameter);
		else
			return dM/dT + 2.3025*m(diameter);
	}
	
	float applySubjectPupilVariation(float diameter, float subjectBias) {
		if (diameter > evaluateMoonUpperBound(diameter) + 0.1 || diameter < evaluateMoonLowerBound(diameter) - 0.1 ) {
			return diameter;
		}
		if (!withEnvelope)
			return diameter;
		
		return ((evaluateMoonUpperBound(diameter) - evaluateMoonLowerBound(diameter)) 
			   * subjectBias) 
			   + evaluateMoonLowerBound(diameter);
	}
	
	float evaluateDiameter(float latency, float time) {
		// Compute the right side of the equation. This will not change.
		float rightSide = muscleActivity(latency);
		float leftSide;
				
		double dD = 0;
		float pass = 1.0f;
		float leftSideAnt = 0;
		float operation = 1;
		
		std::vector<Vector3f> debug;
		
		for (int i=0; i<100; i++) {
			leftSide = evaluateLeftSide(time, dD, latency);
		
			// If it found the right value, return.
			if (equals(leftSide, rightSide, 0.001)) {
				float prevDiameter = Conversion::areaToDiameter((history.history.end()-1)->z());
				return prevDiameter+dD;
			}
			
			
			if (leftSide - leftSideAnt > 0.001 && rightSide > leftSide) {
				// continue
			} else if (leftSide - leftSideAnt < -0.001 && rightSide < leftSide) {
				// continue
			} else {
				// invert the search
				operation = operation * -1;
				
				// check to decrease the step size
				if (i>0 && pass > 0.0000001f) {
					dD = dD + operation *  pass; // back one.
					
					// if the new value is equal to the previous one, can be a solution
					if (equals(leftSide, leftSideAnt, 0.0001)) {
						
						// check if a solution is possible
						if ((operation < 0 && leftSide > rightSide)
						||  (operation > 0 && leftSide < rightSide)) {
							float prevDiameter = Conversion::areaToDiameter((history.history.end()-1)->z());
							return prevDiameter+dD;
						}
						
						// go back one step
						dD = dD + operation *  pass; 
					}		
										
					// decrease the step size.
					pass /= 2.0f;
				}
			}
			
			dD = dD + operation * pass;
			
			// store the value
			leftSideAnt = leftSide;
			debug.push_back(Vector3f(dD, leftSide, rightSide));
		}
		
		std::vector<Vector3f>::iterator i = debug.begin();
		for (; i != debug.end(); i++) {
			std::cout << "Debug: " << i->x() << " " << i->y() << " " << i->z() << std::endl;
		}
		std::cout << "Não Convergiu: " << dD << " " << pass << " "<< leftSide << " "<< rightSide << " " << std::endl;
		
		// If fails, returns the previous valid area.
		float prevDiameter = Conversion::areaToDiameter((history.history.end()-1)->z());
		return prevDiameter;
	}
	
	float pupilDiameterAt(float intensity, float latency, float time) {
		float diameter = evaluateDiameter(latency, time);
		
		addPulse(time,intensity, Conversion::diameterToArea(diameter));

		diameter = applySubjectPupilVariation(diameter, subjectBias);
		return diameter;
	}
	
};

#endif /*LONGTINANDMILTONMODEL_H_*/
