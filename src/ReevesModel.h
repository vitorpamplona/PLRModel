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


#ifndef REEVESMODEL_H_
#define REEVESMODEL_H_

/** 
 * Reeves model for pupil Dynamics
 *   
 * Source: P. Moon and D. Spencer. On the stiles-crawford effect. J. Opt. Soc. Am., 34:319? 329, 1944
 * Based on: Reeves, P. The response of the average pupil to various intensities of light. Journal of the Optical Society of America, 1920, 4(2), 35-43
 */
class ReevesModel : public PupilDynamicsModel
{
public:
	ReevesModel() : PupilDynamicsModel("Reeves") {}
	virtual ~ReevesModel() {}
	
	virtual float pupilDiameterAt(float lightIntensity) {
		return pupilDiameterWithBlondel(lightIntensity);
	}
	
	/**
	 * Intensity in Blondel
	 * returns pupil diameter in mm. 
	 */
	virtual float pupilDiameterWithBlondel(float lightIntensity) {
		return 4.90 - 3.20*tanh(0.471*(log10(lightIntensity)-1.30));
	} 	
};

#endif /*MOONANDSPENCERMODEL_H_*/
