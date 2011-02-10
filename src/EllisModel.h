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


#ifndef ELLISMODEL_H_
#define ELLISMODEL_H_

/**
 * Ellis Latency Model: 
 * 
 * Source: C. J. Ellis. The pupillary light reflex in normal subjects. 
 *           British Journal of Ophthalmology, 65(11):754-759, Nov 1981
 */
class EllisModel : public LatencyModel 
{
public:
	EllisModel() : LatencyModel("Ellis") {}
	virtual ~EllisModel() {}
	
	/**
	 * Light instensity in Blondels
	 */
	float pupilLatencyAt(float intensityInBlondels) {
		//return 445.7 - 22.9 * log10(intensity) + 76.2 * powf(log10(intensity),2);
		
		float intensity = Conversion::blondelToCandelaSquareMeter(intensityInBlondels);
		
		return   429.9226 -61.3027*log10(intensity) +  4.8738*powf(log10(intensity),2);
	}
};

#endif /*LINKANDSTARKMODEL_H_*/
