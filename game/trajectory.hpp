#ifndef __TRAJECTORY
#define __TRAJECTORY

#include <vector>
#include "foliage.hpp"

class Trajectory
{
public:
	Trajectory(const Sint32 nbSteps)
		: _steps(nbSteps)
	{
		_currentStep = _steps.begin();
	}

	void setStep(const Sint32 step, const Foliage::Speed s)
	{
		_steps[step] = s;
	}

	Foliage::Speed nextStep()
	{
		if (ended())
		{
			return Foliage::Speed();
		}
		else
		{
			const Foliage::Speed s = *_currentStep;
			++_currentStep;
			return s;
		}
	}

	bool ended() const
	{
		return (_currentStep == _steps.end());
	}

private:
	std::vector<Foliage::Speed> _steps;
	std::vector<Foliage::Speed>::const_iterator _currentStep;
};

#endif //__TRAJECTORY
