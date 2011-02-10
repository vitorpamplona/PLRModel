#ifndef LONGTINANDMILTONMODEL_H_
#define LONGTINANDMILTONMODEL_H_

/* Longtin and Milton model for PLR
 * Source: Longtin, A. and Milton, J.G. (1989) Modelling autonomous oscillations in the human pupil light reflex using nonlinear delay-differential equations, Bull. Math. Biol. 51: 605-624.
 */
class LongtinAndMiltonModel : public PupilDynamicsModel {
	
	
	
	// x = time (milliseconds) , 
	// y = intensity (lumens), 
	// z = pupil area (mm ^2).
	HistoryFifo<Vector3f, 1000> history;
	
	float gamma;
	float minimumThreshold;
	float alpha;
	float minArea;
	float maxArea;
	float theta;
	float dt;
	float n;
	
public:
	LongtinAndMiltonModel(float _gamma, float _minimumThreshold) : PupilDynamicsModel("Longtin And Milton") {
		init();
		gamma = _gamma;
		minimumThreshold = _minimumThreshold;	
	}
		
	LongtinAndMiltonModel(float _alpha, float _gamma, float _minimumThreshold) : PupilDynamicsModel("Longtin And Milton") {
		init ();

		gamma = _gamma;
		minimumThreshold = _minimumThreshold;
		alpha = _alpha;
	}
			
	
	LongtinAndMiltonModel() : PupilDynamicsModel("Longtin And Milton") {
		init ();
	}

	virtual bool isInLumens() { return true; }
	
	void init () {
		//gamma = 0.45;
		
		gamma = 0.83f;
		alpha = 1/0.171;
		minArea = 2.7000f;
		minimumThreshold = 4.8118f * pow(10, -10.0f);
		maxArea = 48.890f - minArea;
		theta = 10;
		dt = 0.01;
		n = 55;
		

		/*
		gamma = 58.11f;
		alpha = 58.18;
		minArea = 1.0f;
		minimumThreshold = 4.8;
		maxArea = 55.0f - minArea;
		theta = 5.81;
		dt = 0.1;
		n = 4;
		*/
	}
	
	virtual ~LongtinAndMiltonModel() {}
	
	HistoryFifo<Vector3f, 1000> getHistory() {
		return history;
	}
	
	float getGamma() {
		return gamma;
	}

	float getAlpha() {
		return alpha;
	}
	
	float getMinimumThreshold() {
		return minimumThreshold;
	}
	
	void setGamma(float rate) {
		gamma = rate;
	}

	void setAlpha(float rate) {
		alpha = rate;
	}
	
	void setMinimumThreshold(float threshold) {
		minimumThreshold = threshold;
	}	
	
	void setMinArea(float area) {
		minArea = area;
	}

	void setMaxArea(float area) {
		maxArea = area;
	}

	float getMinArea() {
		return minArea;
	}

	float getMaxArea() {
		return maxArea;
	}
	
	void setTheta(float _theta) {
		theta = _theta;
	}

	float getTheta() {
		return theta;
	}
	
	void setDt(float _dt) {
		dt = _dt;
	}

	float getDt() {
		return dt;
	}	
	
	void addPulse(float mSeconds, float intensity, float area) {
		if (area < minArea) area = minArea;
		if (area > maxArea + minArea) area = maxArea + minArea;		
		
		if (area < 0.001) area = 1;
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
	
	/**
	 * 100
	 * 120
	 * 140
	 * 160
	 * 
	 * 130
	 */
	float retinalFlux(float latencyInMilliseconds) {
		int size = history.history.size()-1;
		
		double time = history.history[size].x();
		double fromTime = time - latencyInMilliseconds;
		
		for (int i=size; i>=0; i--) {
			if (fromTime > history.history[i].x()) {
				Vector3f iAtual = history.history[i];
				Vector3f iAnterior = history.history[i+1];

				if (i == size)
					iAnterior = iAtual;				
				
				float deltaTime = iAnterior.x() - iAtual.x();
				float resto = fromTime - iAtual.x();
				
				float percent = 0;

				if (fabs(deltaTime) > 0.01)
					percent = resto / deltaTime;
				
				// linear filter
				float intensity = iAtual.y() + (iAnterior.y() - iAtual.y()) * percent;
				float area 		= iAtual.z() + (iAnterior.z() - iAtual.z()) * percent;

				//std::cout << iAnterior.print() << " - " << iAtual.print() << std::endl;
				
				return retinalFlux(intensity, area);
			}
		}

		return 0;
	}
	
	float logarithmOfRetinalFluxRate(float latency) {
		return log(retinalFlux(latency)/minimumThreshold);
	}
	
	/** Returns the afferent or efferent neural action potential per time */
	float neuralActionPotentialRate(float latency) {
		return gamma * logarithmOfRetinalFluxRate(latency);
	}
		
	/**
	 * max 30 mm^2
	 * min  0 mm^2
	 * teta = 10 mm^2
	 * n = 4
	 */
	float hillFunction(float value) {
		return minArea + (maxArea * powf(theta,n)) / (powf(theta,n) + powf(value, n));    
	}
	
	/**
	 * area in mm^2
	 */
	float hillFunctionInverse(float area) {
		if (area < minArea) area = minArea;
		if (area > maxArea + minArea) area = maxArea + minArea;

		long double powTethaN = pow(((long double)theta), ((long double) n));

		return pow( (maxArea * powTethaN) / (area -minArea) - powTethaN , (long double) 1.0f/n);    
	}
	
	float evaluateLeftSide(float time, float dA) {
		//float dT = dt;//time - (history.history.end()-1)->x();
		float dT = (time - (history.history.end()-1)->x()) / 540.0f;
		float prevArea = history.history[history.history.size()-1].z();
		
		float hillFunc = hillFunctionInverse(prevArea + dA);
		float dG = hillFunc - hillFunctionInverse(prevArea);
		
		if (equals(dA, 0.000f,0.0001f))
			return -54.0 + alpha *  hillFunc;
		else
			return dG/dA * dA/dT + -54.0 + alpha * hillFunc;
	}
	
	/**
	 * Leandro: talvez isso ajude pro teu chute inicial de dA
	 * 	Leandro: A( t ) = h( M( t ) - ((1/alpha) * dg/dt) )
	 * 	Leandro: dg/dt vc tem explícita
	 * 	Leandro: h também
	 * 	Leandro: M( t ) vc consegue tabelar
	 * 	Leandro: logo.. imagino que isso te dá A( t )
	 * 	Leandro: aí vc faz um A( t1 ) - A( t2 ).. pra t1 e t2 bem próximos
	 * 	Leandro: e consegue um dA razoável
	 * 	Leandro: aliás.. se a sua precisão pra M( t ) for boa, vc consegue um dA próximo do real
	 * 
	 */ 
	float evaluateArea(float latency, float time) {
		float rightSide = neuralActionPotentialRate(latency);
		float leftSide;
		
		float dA = 0;
		float pass = 10.0f;
		float leftSideAnt = 0;
		float operation = 1;
		
		for (int i=0; i<100; i++) {
			leftSide = evaluateLeftSide(time, dA);

			// se encontrou o tamanho correto, retorne. 
			if (equals(leftSide, rightSide, 0.01)) {
				float prevArea = (history.history.end()-1)->z();
				return prevArea+dA;
			}
			
			if (leftSide - leftSideAnt > 0.001 && rightSide > leftSide) {
				// Continue assim
			} else if (leftSide - leftSideAnt < -0.001 && rightSide < leftSide) {
				// Continue assim
			} else {
				// inverta o processo
				operation = operation * -1;
				
				//diminua o passo.
				if (i>0 && pass > 0.0001) {
					dA = dA + operation *  pass; // back one.
					
					// se for igual, será que é um limite?
					if (equals(leftSide, leftSideAnt, 0.00001)) {
						
						// se não tem como chegar lá.
						if ((operation < 0 && leftSide > rightSide)
						||  (operation > 0 && leftSide < rightSide)) {
							float prevArea = (history.history.end()-1)->z();
							return prevArea+dA;
						}
						
						// caso contrário volta mais um passo para continuar a interação
						dA = dA + operation *  pass; // back one.	
					}
						
					// Diminui o passo
					pass /= 2.0f;
				}
			}
			
			// altera dA com o novo passo na nova direção
			dA = dA + operation * pass;
			
			// armazena o valor anterior. 
			leftSideAnt = leftSide;
		}
		
		std::cout << "Não Convergiu" << std::endl;
		std::cout << " dA: " << dA << "          \t pass: " << pass << "\t L: " << leftSide << "\t R: " << rightSide << std::endl;
		
		// caso não enco							ntre, retorne a área anterior.		
		float prevArea = (history.history.end()-1)->z();
		return prevArea;
	}
	
	
	
	
	float gaussianError(float time) {
		time = ((int)time) % 2000;
		float width = 400;
		float heght = 0.3;
		float pico = 1000;
		return heght * exp(- powf(time-pico,2 ) /  (2 * powf(   width , 2)) ) ;
	}
	
	float pupilDiameterAt(float intensity, float latency, float time) {
		//std::cout << intensity << std::endl;
		
		float area = evaluateArea(latency, time);
		
		//area += gaussianError(time);
		//std::cout << time << " " << gaussianError(time) << std::endl;
		
		addPulse(time,intensity, area);
		return Conversion::areaToDiameter(area);
	}
	
};

#endif /*LONGTINANDMILTONMODEL_H_*/
