#ifndef PUPILLIFECYCLEINTERFACE_H_
#define PUPILLIFECYCLEINTERFACE_H_

class PupilLifecycleInterface
{
public:
	PupilLifecycleInterface() {}
	virtual ~PupilLifecycleInterface() {}
	
	virtual float getDiameter(int time, float intensity) { return 0; } 
};

#endif /*PUPILDYNAMICSINTERFACE_H_*/
