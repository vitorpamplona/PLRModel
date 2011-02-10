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


#ifndef CONVERSION_H_
#define CONVERSION_H_

/**
 * Physical Conversion Class.
 * Based On: http://www.unitconversion.org/unit_converter/luminance.html
 */ 
class Conversion
{
public:
	Conversion() {}
	virtual ~Conversion() {}
	
	// 1 blondel = 0.318309886 candela/square meter
	static float blondelToCandelaSquareMeter(float blondel) {
		return blondel * 0.318309886;
	}

	// 1 candela/square meter = 3.141592654 blondel	
	static float candelaSquareMeterToBlondel(float candelaSquareMeter) {
		return candelaSquareMeter * 3.141592654;
	}	
	
	// 1 lux = 1 lumens / m2
	// 1 lux = 0.000001 lumens / mm2
	static float luxToLumensSquareMillimeter(float lux) {
		return lux * 0.000001;
	}
	
	// 1 lux = 1 lumens / m2	
	static float luxToBlondel(float lux) {
		return   lumensSquareMillimeterToBlondel(luxToLumensSquareMillimeter(lux));
	}	

	static float blondelToLux(float blondel) {
		return lumensSquareMillimeterToLux(blondelToLumensSquareMillimeter(blondel));  
	}	
	
	// 1 lux = 1 lumens / m2	
	static float lumensSquareMillimeterToLux(float lumens) {
		return lumens * 1000000;
	}
	
	// 1 blondel = 0.1 millilambert
	static float blondelToMillilambert(float blondel) {
		return blondel * 0.1;
	}

	// 1 millilambert = 10 blondel	
	static float millilambertToBlondel(float millilambert) {
		return millilambert * 10;
	}		
	

	// 1 foot-lambert = 3.4262591 candela/square meter
	static float footLambertToCandelaSquareMeter(float footLambert) {
		return footLambert * 3.4262591;
	}

	// 1 candela/square meter = 0.291863508 foot-lambert	
	static float candelaSquareMeterToFootLambert(float cancelaSquareMeter) {
		return cancelaSquareMeter * 0.291863508;
	}	

	static float blondelToFootLambert(float blondel) {
		return candelaSquareMeterToFootLambert(blondelToCandelaSquareMeter(blondel));
	}	
	
	// 1 millilambert = 3.183098862 candela/square meter
	static float millilambertToCandelaSquareMeter(float millilambert) {
		return millilambert * 3.183098862;
	}

	// 1 candela/square meter = 0.314159265 millilambert	
	static float candelaSquareMeterToMillilambert(float cancelaSquareMeter) {
		return cancelaSquareMeter * 0.314159265;
	}		
	
	// Troland = 10 * pupilRadius^2 * luminanceInMilliamberts
	// Simplified Version
	static float milliambertsToTrolandsSimplified(float pupilRadius, float luminanceInMilliamberts) {
		return 10 * powf(pupilRadius, 2) * luminanceInMilliamberts;
	}
	
	// Troland = 10 * pupilRadius^2 * luminanceInMilliamberts
	// Simplified Version
	static float candelaPerSquareMeterToTrolandsSimplified(float pupilRadius, float candelaPerSquareMeter) {
		return milliambertsToTrolandsSimplified(pupilRadius, candelaSquareMeterToMillilambert(candelaPerSquareMeter));
	}	
	
	// Troland = 10 * pupilRadius^2 * luminanceInMilliamberts (1 - 0.0425 * pupilRadius^2 + 0.00067 * pupilRadius^4)
	// Stiles Crawford Version
	static float millilambertsToTrolandsStilesCrawford(float pupilRadius, float luminanceInMilliamberts) {
		return milliambertsToTrolandsSimplified(pupilRadius, luminanceInMilliamberts) * (1 - 0.0425 * powf(pupilRadius, 2) + 0.00067 * powf(pupilRadius, 4));
	}
	
	// Troland = 10 * pupilRadius^2 * luminanceInMilliamberts
	// Simplified Version
	static float candelaPerSquareMeterToTrolandsStilesCrawford(float pupilRadius, float candelaPerSquareMeter) {
		return millilambertsToTrolandsStilesCrawford(pupilRadius, candelaSquareMeterToMillilambert(candelaPerSquareMeter));
	}		
	
	// 1 diameter = area
	// mm => mm^2
	// m  => m^2  
	
	static float diameterToArea(float diameter) {
		return M_PI * powf(diameter/2.0f, 2);
	}
	
	// 1 area = 3.14 area
	static float areaToDiameter(float area) {
		return sqrt(area / M_PI) * 2;
	}
	
	// 1 Lambert = 10 lumens / cm^2
	// 1 millilambert = 10 lumens / m^2
	// Emissor lambertiano;
	static float millilambertToLumensSquareMillimeter(float millilamberts) {
		return millilamberts  * 0.00001;
	}
	
	// 1 Lambert = 10 lumens / cm^2
	// 1 millilambert = 10 lumens / m^2 
	// 1 millilambert = 0.0001 lumens/mm^2
	// Emissor lambertiano;
	static float  lumensSquareMillimeterToMillilambert(float lumensSquareMillimeter) {
		return lumensSquareMillimeter * 100000;
	}	
	
	static float blondelToLumensSquareMillimeter(float blondel) {
		//return blondel * 0.000001;
		return millilambertToLumensSquareMillimeter(blondelToMillilambert(blondel));
	}
	
	static float lumensSquareMillimeterToBlondel(float lumensSquareMillimeter) {
		//return lumensSquareMillimeter * 1000000;
		return millilambertToBlondel(lumensSquareMillimeterToMillilambert(lumensSquareMillimeter));
	}
	
	static float lumensSquareMillimeterToCandelaSquareMeter(float lumensSquareMillimeter) {
		return millilambertToCandelaSquareMeter(lumensSquareMillimeterToMillilambert(lumensSquareMillimeter));
	}
	
	static float candelaSquareMeterToLumensSquareMillimeter(float candelasSquareMeter) {
		return millilambertToLumensSquareMillimeter(candelaSquareMeterToMillilambert(candelasSquareMeter));
	}
};

#endif /*CONVERSION_H_*/
