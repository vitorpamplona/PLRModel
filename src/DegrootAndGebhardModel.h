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


#ifndef DEGROOTANDGEBHARDMODEL_H_
#define DEGROOTANDGEBHARDMODEL_H_

/** 
 * Groot and Gebhard model for pupil Dynamics
 *   
 * Source: S. G. de Groot and J. W. Gebhard. Pupil size as determined by adapting luminance.
 *                J. Opt. Soc. Am., 42:492-495, 1952.
 */
class DegrootAndGebhardModel : public PupilDynamicsModel
{
public:
	DegrootAndGebhardModel() : PupilDynamicsModel("Degroot And Gebhard") {}
	virtual ~DegrootAndGebhardModel() {}
	
	/**
	 * Intensity in Blondels
	 * return diameter in mm. 
	 */
	virtual float pupilDiameterAt(float lightIntensity) {
		return pupilDiameterWithMillilamberts(Conversion::blondelToMillilambert(lightIntensity));
	}	
	
	/**
	 * Intensity in milliamberts
	 * return diameter in mm. 
	 */
	virtual float pupilDiameterWithMillilamberts(float lightIntensity) {
		return powf(10, 0.8558 -0.000401 * powf(log10(lightIntensity)+8.1, 3));
	} 	
};

#endif /*MOONANDSPENCERMODEL_H_*/
