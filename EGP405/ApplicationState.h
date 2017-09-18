#ifndef _APPLICATIONSTATE_H_
#define _APPLICATIONSTATE_H_

class ApplicationState abstract
{
public:
	virtual void updateInput() = 0;

	virtual void updateNetworking() = 0;
	
	virtual void updateState() = 0;

	virtual void display() = 0;

protected:
	ApplicationState *next, *prev;

	struct ApplicationStateData
	{
		char doesUpdateInput;
		char doesUpdateNetworking;
		char doesUpdateState;
		char doesDisplay;
	} data;

	virtual void goToNextState(ApplicationStateData *passData) = 0;
	virtual void onArriveFromPrevious(ApplicationStateData *passData) = 0;

};

#endif // !_APPLICATIONSTATE_H_