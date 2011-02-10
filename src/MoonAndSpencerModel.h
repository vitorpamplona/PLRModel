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


#ifndef MOONANDSPENCERMODEL_H_
#define MOONANDSPENCERMODEL_H_

/** 
 * Moon and Spencer model for pupil Dynamics. 
 * 
 * Luz entra numa distância de 2mm do centro da pupila.
 *   
 * Source: P. Moon and D. Spencer. On the stiles-crawford effect. J. Opt. Soc. Am., 34:319? 329, 1944
 */
class MoonAndSpencerModel : public PupilDynamicsModel
{
public:
	MoonAndSpencerModel() : PupilDynamicsModel("Moon And Spencer") {}
	virtual ~MoonAndSpencerModel() {}
	
	/**
	 * Intensity in Blondels
	 * returns pupil diameter in mm. 
	 */
	virtual float pupilDiameterAt(float lightIntensity) {
		return pupilDiameterWithBlondel(lightIntensity);
	}
		
	/**
	 * Intensity in Blondel 
	 * returns pupil diameter in mm.
	 * 
	 * 1 blondel = 0.318309886 candela/square meter
	 * 1 candela/square meter = 3.141592654 blondel
	 */	
	virtual float pupilDiameterWithBlondel(float lightIntensity) {
		return 4.9f - 3.0f*tanh(0.4f*(log10(lightIntensity)-0.5f));
	} 	
	
	/**
	 * Intensity in Milliamberts 
	 * returns pupil diameter in mm. 
	 */	
	virtual float pupilDiameterWithMillilambert(float lightIntensity) {
		return 4.9f - 3.0f*tanh(0.4f*(log10(lightIntensity)+0.5f));
	} 		
	
	float arcTanH(float x) {
		return 0.5f * (log(1+x) - log(1-x));
	}	
		
	virtual float getLumensPerSquareMillimiter(float diameter) {
		return Conversion::blondelToLumensSquareMillimeter(getBlondel(diameter));
	}

	virtual float getBlondel(float diameter) {
		return powf(10.0f, arcTanH((diameter - 4.9f) / -3.0f) / 0.4f + 0.5f);
	}
};

#endif /*MOONANDSPENCERMODEL_H_*/
