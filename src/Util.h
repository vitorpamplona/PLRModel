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


#ifndef UTIL_H_
#define UTIL_H_

float totalTime;

double EPSLON = 1.0E-4;

bool equals(float a, float b) {
	return fabs(a - b) < EPSLON;
}

bool equals(float a, float b, float epslon) {
	return fabs(a - b) <= epslon;
}

bool equals(double a, double b) {
	return fabs(a - b) < EPSLON;
}

inline bool zero(double a) {
	return fabs(a) < EPSLON;
}

int upperCase(int c) {
	return _toupper(c);
}

int lowerCase(int c) {
	return _tolower(c);
}

double degToRad(double degree) {
	return degree *(M_PI / 180);
}

double radToDeg(double radians) {
	return radians * (180/M_PI);
}

char * convert(std::string teste) {
	return const_cast<char *>(teste.c_str());
}

void initTime() {
	totalTime = clock();
}

void printTime(char * msg) {
	totalTime = (double)(clock()-totalTime)/CLOCKS_PER_SEC;
	std::cout << msg << " Total Time: " << totalTime << std::endl;
}

#endif /*UTIL_H_*/
