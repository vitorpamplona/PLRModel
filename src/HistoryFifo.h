#ifndef HISTORYFIFO_H_
#define HISTORYFIFO_H_

#include <vector> 

/**
 * Storage for pupil size and intensity. 
 * This is used in the *delay* differential equation
 */
template <class T, int limit>
class HistoryFifo {
			
public:
	std::vector<T> history;
	
	HistoryFifo() {}
	virtual ~HistoryFifo() {}
	
	void add(T value) {
		history.push_back(value);
		if (history.size()>limit) {
			history.erase(history.begin());
		}
	}
	
	
	int size() {
		return history.size();
	}	
	
	T operator [] (int index) {
		history[index];
	}
};

#endif /*HISTORYFIFO_H_*/
