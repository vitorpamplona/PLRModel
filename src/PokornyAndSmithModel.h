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

#ifndef POKORNYANDSMITHMODEL_H_
#define POKORNYANDSMITHMODEL_H_

/**
 * Pokorny and Smith model for pupil Dynamics. 
 *  
 * Source: J. Pokorny and V. C. Smith. The verriest lecture. how much light reaches the retina? In C.R. Cavonius (ed), 
 *           Colour Vision Deficiencies XIII. Documenta Ophthalmologica Proceedings Series 59, pages 491-511, 1997.
 */ 

class PokornyAndSmithModel : public PupilDynamicsModel
{
public:
	PokornyAndSmithModel() : PupilDynamicsModel("Pokorny And Smith") {}
	virtual ~PokornyAndSmithModel() {}
	
	/**
	 * Intensity in Blondels
	 * return diameter in mm. 
	 */
	virtual float pupilDiameterAt(float lightIntensity) {
		return 5.0f - 3.0f*tanh(0.4*log10(Conversion::blondelToCandelaSquareMeter(lightIntensity)));
	} 	
};

#endif /*MOONANDSPENCERMODEL_H_*/
