#ifndef WAVEFRONT_H
#define WAVEFRONT_H

#include <vector>
#include <complex>


typedef std::vector<int> iVec;
typedef std::vector<double> dblVec;
typedef std::vector<std::complex<double>> complexVec;

class Wavefront
{
public:
	Wavefront();
	Wavefront(int xDim, int yDim);
	Wavefront(int xDim, int yDim, int xDimDisplay, int yDimDisplay);
	~Wavefront();

	enum beamType
	{
		Normal,
		Helical,
		StepPhase,
	};

	complexVec Field;
	std::vector<uint8_t> PhaseDisplay;

	dblVec zernikeAmplitudes;

	void updateAberrationFunction(int index, double amplitude);
	void updateCentrePosition(int xCentre, int yCentre);
	void setBeamType(beamType beam);
	void resizeHologram(int xDim, int yDim);
    void setTopologicalCharge(int topologicalCharge);
    void setStepRadius(double stepRadius);
    void setMaxBlaze(int maxBlaze);

protected:
	int _xDim;
	int _yDim;
	int _xDimDisplay;
	int _yDimDisplay;
	int _numElements;
	int _numElementsDisplay;
	int _xCoordinate;
	int _yCoordinate;
	int _holoRadius;
	int numPolynomials = 15;
	int maxDisplayValue;
    int _topologicalCharge;

	beamType _beam = Normal;

    double _stepRadius = 0.707;


	dblVec _X;
	dblVec _Y;
	dblVec _R;
	dblVec _Mask;
	dblVec _Theta;	
		
	dblVec _Phase;
	dblVec _WrappedPhaseDisplay;
	dblVec _Aberration;
	dblVec _Amplitude;	 

	std::vector<std::vector<double>> Zernikes;


	void initialize();
	void calcZernikes();
	void calcAberrationPhase();
	void calcWFPhase();
	void calculateWrappedPhaseDisplay(int maxValue);
	void updatePhaseDisplay();	
	void copySubArray(int xPos, int yPos);

private:
    void setVectorSizes(int xDim, int yDim, int xDimDisplay, int yDimDisplay);

};

#endif // WAVEFRONT_H
