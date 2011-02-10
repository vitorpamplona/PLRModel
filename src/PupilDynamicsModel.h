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


#ifndef PUPILDYNAMICSMODEL_H_
#define PUPILDYNAMICSMODEL_H_

/** Abstract class to all dynamic models. 
 */
class PupilDynamicsModel {
	std::string name;
	
public:
	PupilDynamicsModel(std::string _name) { name = _name; }
	virtual ~PupilDynamicsModel() {}
	
	/**
	 * Intensity in cd/m2
	 * return in mm. 
	 */
	virtual float pupilDiameterAt(float lightIntensity) {
		return 1.0;
	} 
	
	virtual float pupilDiameterAt(float intensity, float latency, float time) {
		return pupilDiameterAt(intensity);
	}	
	
	std::string getName() {
		return name; 
	}
	
	virtual bool isInLumens() { return false; }
};

#endif /*PUPILDYNAMICSMODEL_H_*/
