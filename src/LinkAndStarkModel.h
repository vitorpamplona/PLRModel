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


#ifndef LINKANDSTARKMODEL_H_
#define LINKANDSTARKMODEL_H_

/**
 * Link And Stark Latency Model: 
 * 
 * Source: N. Link and L. Stark. Latency of the pupillary response.
 *       IEEE Transactions on Biomedical Engineering, 35(3):214-218, 1988
 */
class LinkAndStarkModel : public LatencyModel 
{
	float frequency;
	
public:
	LinkAndStarkModel(float _frequency) : LatencyModel("Link And Stark") {
		frequency = _frequency;
	};
	virtual ~LinkAndStarkModel() {};
	
	/**
	 * Frequency in Hertz (Hz)
	 */ 
	void setFrequency(float _frequency) {
		frequency = _frequency;
	};	
	
	/**
	 * Light instensity in Blondels
	 * Response latency in Milliseconds (ms)
	 */
	float pupilLatencyAt(float intensityInBlondels) {
		return pupilLatencyWithFootLamberts(Conversion::blondelToFootLambert(intensityInBlondels));
	}
	
	/**
	 * Frequency in Hertz (Hz) 
	 * Light instensity in Foot-Laberts (FL)
	 * Response latency in Milliseconds (ms)
	 */	
	float pupilLatencyWithFootLamberts(float intensityInFootLamberts) {
						
		return +253   													//A1 - Calcium activation process in muscle 
			   -14 * log10(intensityInFootLamberts)  					//A2 - light intensity up, latency down (Retina)
			   +70 * frequency   										//A3 - frequency up, latency up.
			   -29 * frequency * log10(intensityInFootLamberts)        //A4 - cross product (not well understood)
			   +0.13 * powf(frequency,2)								//A5
			   -0.35 * powf(log10(intensityInFootLamberts),2);  		//A6
	}	
};

#endif /*LINKANDSTARKMODEL_H_*/
