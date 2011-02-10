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


#ifndef LATENCYMODEL_H_
#define LATENCYMODEL_H_

/**
 * Abstract Class to define Latency Model of the eye
 */
class LatencyModel {
	
	std::string name;
	
public:
	LatencyModel(std::string _name) { name = _name; }
	virtual ~LatencyModel() { }
	
	/**
	 * Light instensity in Blondels
	 * Response latency in Milliseconds (ms)
	 */
	virtual float pupilLatencyAt(float intensity) {
		return 0;
	}
	
	std::string getName() {
		return name;
	}
	
		
};

#endif /*LATENCYMODEL_H_*/

