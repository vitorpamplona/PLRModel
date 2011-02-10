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

#ifndef PamplonaAndOliveiraMODEL_H_
#define PamplonaAndOliveiraMODEL_H_

/**
 * Pamplona's Model for Pupil Light Reflex
 *
 * Source: Pamplona, V. F., Oliveira, M. M., and Baranoski, G. V. G. (2009). Photorealistic models for pupil light reflex and iridal pattern deformation. ACM Trans. Graph. 28, 4 (Aug. 2009), 106:1-106:12. DOI= http://doi.acm.org/10.1145/1559755.1559763
 * 
 * Equation 16
 */ 
class PamplonaAndOliveiraModel : public PupilDynamicsModel {
	
	// x = time (milliseconds) , 
	// y = intensity (lumens), 
	// z = pupil diameter (mm).
	HistoryFifo<Vector3f, 1000> history;
	
	float dt;
	double minimumThreshold;
	
public:
	PamplonaAndOliveiraModel() : PupilDynamicsModel("Our Model") {		
		init();
	}
	virtual ~PamplonaAndOliveiraModel() {}
	
	void init() {
		dt = 0.3f;
		minimumThreshold = evalPhiBar(); //4.8118f * pow(10, -10.0f);
	}
	
	float evalPhiBar() {
		MoonAndSpencerModel *  moon = new MoonAndSpencerModel();
		// Lower intensity
		float phiBarIntensityBlondels = powf(10, -5);
		// To Lumens per Square MM
		float phiBarIntensityLumensMM = Conversion::blondelToLumensSquareMillimeter(phiBarIntensityBlondels);
		// Get pupil diameter
		float phiBarDiameter = moon->pupilDiameterWithBlondel(phiBarIntensityBlondels);
		// find phiBar = Area * lumens/MM2
		float phiBar = Conversion::diameterToArea(phiBarDiameter) * phiBarIntensityLumensMM;
		return phiBar;
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
	
	float retinalFlux(float latencyInMilliseconds) {
		int size = history.history.size()-1;
		
		double time = history.history[size].x();
		double fromTime = time - latencyInMilliseconds;
		
		for (int i=size; i>=0; i--) {
			if (!(fromTime < history.history[i].x())) {
				Vector3f iAtual = history.history[i];
				Vector3f iAnterior = history.history[i+1];

				if (i == size) {
					iAnterior = iAtual;
				}

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
		}

		return 0;
	}
	
	float logarithmOfRetinalFluxRate(float latency) {
		return log(retinalFlux(latency)/minimumThreshold);
	}
	
	/** Returns the afferent or efferent neural action potential per time */
	float muscleActivity(float latency) {
		return 5.2 - 0.45 * logarithmOfRetinalFluxRate(latency); 
	}
		
	float arcTanH(float x) {
		return 0.5f * (log(1+x) - log(1-x));
	}

	float m(float diameter) {
		return arcTanH((diameter - 4.9) / 3);
	}

	float evaluateLeftSide(float time, float dD) {
		float dT = (time - (history.history.end()-1)->x()) / 500.0f;
		float prevDiammeter = Conversion::areaToDiameter(history.history[history.history.size()-1].z());
		
		float diameter = prevDiammeter + dD;
		float prevM = m(prevDiammeter);
		float dM = m(diameter) - prevM;

		if (dD > 0) {
			dT /= 3.0f;
		}
			
		if (equals(dD, 0.000f,0.0001f) || equals(dT, 0.000f,0.0001f))
			return 2.3025*m(diameter);
		else
			return dM/dT + 2.3025*m(diameter);
	}
	
	/**
	 * 
	 */ 
	float evaluateDiameter(float latency, float time) {
		// Compute the right side of the equation. This will not change.
		float rightSide = muscleActivity(latency);
		float leftSide;
		
		double dD = 0;
		float pass = 10.0f;
		float leftSideAnt = 0;
		float operation = 1;
		
		for (int i=0; i<100; i++) {
			leftSide = evaluateLeftSide(time, dD);
			
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
				
				// check to decrease the step size.
				if (i>0 && pass > 0.0000001f) {
					dD = dD + operation *  pass; // back one.
					
					// if the new value is equal to the previous one, can be a solution
					if (equals(leftSide, leftSideAnt, 0.00001)) {
						
						// check if a solution is possible
						if ((operation < 0 && leftSide > rightSide)
						||  (operation > 0 && leftSide < rightSide)) {
							float prevDiameter = Conversion::areaToDiameter((history.history.end()-1)->z());
							return prevDiameter+dD;
						}
						
						// go back one step
						dD = dD + operation *  pass; // back one.	
					}		
										
					// decrease the step size.
					pass /= 2.0f;
				}
			}
			
			dD = dD + operation * pass;
			
			// store the value
			leftSideAnt = leftSide;
		}
		
		std::cout << "The Equation Diverged: " << dD << " " << pass << " "<< leftSide << " "<< rightSide << " " << std::endl;
		
		// If it fails, return the last pupil diameter.
		float prevDiameter = Conversion::areaToDiameter((history.history.end()-1)->z());
		return prevDiameter;
	}
	
	float pupilDiameterAt(float intensity, float latency, float time) {
		float diameter = evaluateDiameter(latency, time);
		
		//std::cout << intensity << " " << diameter << std::endl;
		
		addPulse(time,intensity, Conversion::diameterToArea(diameter));
		return diameter;
	}

	float getLumens(float diameter) {
		return 0;
	}

	
};

#endif 
