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


#ifndef _SINGLETON_
#define _SINGLETON_

/*******************************************************************************************************
 * Singleton class definitions
 *******************************************************************************************************/

// Template class for creating single-instance global classes
template<typename T>
class Singleton
{
public:

	// Returns a refecente to the single-instance of the class
	static T& getSingleton()
	{
		if (m_singleton == NULL) 
		    m_singleton = new T();
		return *m_singleton;
	}
	
	static T& getInstance()
	{
		return getSingleton();
	}
	
	// Returns a pointer to the single-instance of the class
	static T* getSingletonPtr() {
		if (m_singleton == NULL) 
		    m_singleton = new T();
		return m_singleton;
	}

	static T* getInstancePtr()
	{
		return getSingletonPtr();
	}

	// Default constructor
	Singleton()
	{
		m_singleton = static_cast<T*>( this );
	}

	// Destructor
	virtual ~Singleton()
	{
		m_singleton = NULL;
	}

protected:

	// The single instance of the class
	static T* m_singleton;

};

/*******************************************************************************************************
 * Singleton class static initializations
 *******************************************************************************************************/

template<typename T>
T*
Singleton<T>::m_singleton = NULL;

#endif // !_SINGLETON_
