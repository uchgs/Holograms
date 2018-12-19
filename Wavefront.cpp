//#include "stdafx.h"
#include "Wavefront.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#define _USE_MATH_DEFINES

#include <math.h>

const std::complex<double> im(0, 1);

Wavefront::Wavefront()
{
    _xDim = 512;
    _yDim = 512;
    _xDimDisplay = 512;
    _yDimDisplay = 512;
    _xCoordinate = _xDimDisplay / 2;
    _yCoordinate = _yDimDisplay / 2;
    _topologicalCharge = 1;
    numPolynomials = 15;
    zernikeAmplitudes.resize(numPolynomials);
    Zernikes.resize(numPolynomials);
    setVectorSizes(_xDim, _yDim, _xDimDisplay, _yDimDisplay);
    initialize();
    calcZernikes();
    calcAberrationPhase();
    calcWFPhase();
    updateAberrationFunction(2,50);
    maxDisplayValue = 255;
    calculateWrappedPhaseDisplay(maxDisplayValue);
    copySubArray(0, 0);
}

Wavefront::Wavefront(int xDim, int yDim)
{
    _xDim = xDim;
    _yDim = yDim;
    _xDimDisplay = xDim;
    _yDimDisplay = yDim;
    _xCoordinate = _xDimDisplay / 2;
    _yCoordinate = _yDimDisplay / 2;
    _topologicalCharge = 1;
    numPolynomials = 15;
    zernikeAmplitudes.resize(numPolynomials);
    Zernikes.resize(numPolynomials);
    setVectorSizes(_xDim, _yDim, _xDimDisplay, _yDimDisplay);
    initialize();
    calcZernikes();
    calcAberrationPhase();
    calcWFPhase();
    updateAberrationFunction(2,50);
    maxDisplayValue = 255;
    calculateWrappedPhaseDisplay(maxDisplayValue);
    copySubArray(0, 0);
}

Wavefront::Wavefront(int xDim, int yDim, int xDimDisplay, int yDimDisplay)
{
	_xDim = xDim;
	_yDim = yDim;
	_xDimDisplay = xDimDisplay;
	_yDimDisplay = yDimDisplay;
	_xCoordinate = _xDimDisplay / 2;
	_yCoordinate = _yDimDisplay / 2;
    _topologicalCharge = 1;
	numPolynomials = 15;
	zernikeAmplitudes.resize(numPolynomials);
	Zernikes.resize(numPolynomials);
	setVectorSizes(_xDim, _yDim, _xDimDisplay, _yDimDisplay);
	initialize();
	calcZernikes();
	calcAberrationPhase();
	calcWFPhase();
    updateAberrationFunction(2,50);
	maxDisplayValue = 255;
	calculateWrappedPhaseDisplay(maxDisplayValue);
    copySubArray(_xCoordinate, _yCoordinate);
}

Wavefront::~Wavefront()
{
}

void Wavefront::updateAberrationFunction(int index, double amplitude)
{
	double currentAmp = zernikeAmplitudes[index];
	double ampDifference = amplitude - currentAmp;

    #pragma omp parallel for
	for (int i = 0; i < _numElements; i++)
	{
		_Aberration[i] += ampDifference * Zernikes[index][i];
		_Phase[i] = _Phase[i] + ampDifference * Zernikes[index][i];
        Field[i] = _Mask[i] * std::exp(im*_Phase[i]);
	}

	calculateWrappedPhaseDisplay(maxDisplayValue);
	zernikeAmplitudes[index] = amplitude;
	updatePhaseDisplay();

}

void Wavefront::updateCentrePosition(int xCentre, int yCentre)
{	
	_xCoordinate = xCentre;
	_yCoordinate = yCentre;
	copySubArray(_xCoordinate, _yCoordinate);
}

void Wavefront::setBeamType(beamType beam)
{
	_beam = beam;
	calcWFPhase();
	calculateWrappedPhaseDisplay(maxDisplayValue);
	copySubArray(_xCoordinate, _yCoordinate);
}

void Wavefront::resizeHologram(int xDim, int yDim)
{
	_xDim = xDim;
	_yDim = yDim;

	setVectorSizes(_xDim, _yDim, _xDimDisplay, _yDimDisplay);
    std::fill(_Aberration.begin(),_Aberration.end(),0);
	initialize();
	calcZernikes();
	calcAberrationPhase();
	calcWFPhase();
	calculateWrappedPhaseDisplay(maxDisplayValue);
    copySubArray(_xCoordinate, _yCoordinate);
}

void Wavefront::setTopologicalCharge(int topologicalCharge)
{
    _topologicalCharge = topologicalCharge;
    setBeamType(_beam);
}

void Wavefront::setStepRadius(double stepRadius)
{
    _stepRadius = stepRadius;
    setBeamType(_beam);
}

void Wavefront::setMaxBlaze(int maxBlaze)
{
    maxDisplayValue = maxBlaze;
    calculateWrappedPhaseDisplay(maxDisplayValue);
    copySubArray(_xCoordinate, _yCoordinate);
}

void Wavefront::setVectorSizes(int xDim, int yDim, int xDimDisplay, int yDimDisplay)
{
	_numElements = xDim * yDim;
	_xDimDisplay = xDimDisplay;
	_yDimDisplay = yDimDisplay;

	_numElementsDisplay = _xDimDisplay * _yDimDisplay;	

	_X.resize(xDim);
	_Y.resize(yDim);
	_R.resize(_numElements);
	_Mask.resize(_numElements);
	_Phase.resize(_numElements);
	_WrappedPhaseDisplay.resize(_numElements);
	_Aberration.resize(_numElements);
	_Theta.resize(_numElements);
	PhaseDisplay.resize(_numElementsDisplay);
	Field.resize(_numElements);	

    #pragma omp parallel for
	for (int i = 0; i < numPolynomials; i++)
	{
		Zernikes[i].resize(_numElements);
	}
}

void Wavefront::initialize()
{
	double currentVal;
	double xStartVal = (-1 * (double)_xDim / 2.0);
	double yStartVal = (-1 * (double)_yDim / 2.0);
    #pragma omp parallel for
	for (int i = 0; i < _xDim; i++)
	{
		currentVal = xStartVal + (double)i;
		_X[i] = currentVal;		
	}	
    #pragma omp parallel for
	for (int i = 0; i < _yDim; i++)
	{
		currentVal = yStartVal + (double)i;		
		_Y[i] = currentVal;
	}

    #pragma omp parallel for ordered
    for (int i = 0; i < _yDim; i++)
    {
        #pragma omp ordered
        {
            #pragma omp parallel for ordered
            for (int j = 0; j < _xDim; j++)
            {
                #pragma omp ordered
                {
                    int idx = (j * _yDim) + i;
                    currentVal = sqrt(_X[j] * _X[j] + _Y[i] * _Y[i])/(double)(_xDim/2);

                    _R[idx] = currentVal;
                    _Mask[idx] = (double)(currentVal <= 1);
                    _Theta[idx] = atan2(_Y[i], _X[j]);
                }
            }
        }
    }


}

void Wavefront::calcZernikes()
{
	
	double r2, r3, r4;	
    #pragma omp parallel for
	for (int i = 0; i < _yDim; i++)
	{
        #pragma omp parallel for
		for (int j = 0; j < _xDim; j++)
		{
			int idx = (j * _yDim) + i;				

			r2 = pow(_R[idx], 2);
			r3 = pow(_R[idx], 3);
			r4 = pow(_R[idx], 4);

			Zernikes[0][idx] = 1; //1: Piston

			Zernikes[1][idx] = (1 / M_PI) * 2 * _R[idx] * cos(_Theta[idx]); //2: X Tilt 

			Zernikes[2][idx] = (1 / M_PI) * 2 * _R[idx] * sin(_Theta[idx]); //3: Y Tilt

			Zernikes[3][idx] = (1 / M_PI) * sqrt(3) * (2 * r2 - 1); //4: Defocus

			Zernikes[4][idx] = (1 / M_PI) * sqrt(6) * r2 * sin(2.0*_Theta[idx]); //5: Oblique Astigmatism

			Zernikes[5][idx] = (1 / M_PI) * sqrt(6) * r2 * cos(2.0*_Theta[idx]); //6: Vertical Astigmatism

			Zernikes[6][idx] = (1 / M_PI) * sqrt(8) * (3.0 * r3 - 2.0 * _R[idx]) * sin(_Theta[idx]); //7: Vertical Coma

			Zernikes[7][idx] = (1 / M_PI) * sqrt(8) * (3.0 * r3 - 2.0 * _R[idx]) * cos(_Theta[idx]); //8: Horizontal Coma

			Zernikes[8][idx] = (1 / M_PI) * sqrt(8) * r3 * sin(3.0 * _Theta[idx]); //9: Vertical Trefoil

			Zernikes[9][idx] = (1 / M_PI) * sqrt(8) * r3 * cos(3.0 * _Theta[idx]); //10: Oblique Trefoil

			Zernikes[10][idx] = (1 / M_PI) * sqrt(5) * (6.0 * r4 - 6 * r2 + 1); //11: Primary spherical

			Zernikes[11][idx] = (1 / M_PI) * sqrt(10) * (4 * r4 - 3 * r2) * cos(2.0 * _Theta[idx]); //12: Vertical secondary astigmatism

			Zernikes[12][idx] = (1 / M_PI) * sqrt(10) * (4 * r4 - 3 * r2) * sin(2.0 * _Theta[idx]); //13: Oblique secondary astigmatism

			Zernikes[13][idx] = (1 / M_PI) * sqrt(10) * r4 * cos(4 * _Theta[idx]); //14: Vertical quadrafoil

			Zernikes[14][idx] = (1 / M_PI) * sqrt(10) * r4 * sin(4 * _Theta[idx]); //15: Oblique quadrafoil

		}
	}
	

}

void Wavefront::calcAberrationPhase()
{
    #pragma omp parallel for ordered
	for (int i = 0; i < numPolynomials; i++)
	{
        #pragma omp ordered
        {
            #pragma omp parallel for ordered
            for (int j = 0; j < _numElements; j++)
                {
                    #pragma omp ordered
                    {
                        _Aberration[j] += zernikeAmplitudes[i] * Zernikes[i][j];
                    }

                }
        }
	}
}

void Wavefront::calcWFPhase()
{
	if (_beam == Normal)
	{
        std::complex<double> complexMask(0,0);
        std::complex<double> complexPhase(0,0);
        //#pragma omp parallel for
		for (int i = 0; i < _numElements; i++)
		{
            complexMask.real(_Mask[i]);
            complexPhase.real(_Mask[i] *_Aberration[i]);
			_Phase[i] = _Mask[i] *_Aberration[i];
            Field[i] = _Mask[i] * std::exp(im*_Phase[i]);
		}
	}

	else if(_beam == Helical)
	{
        #pragma omp parallel for
		for (int i = 0; i < _numElements; i++)
		{
            _Phase[i] = _Mask[i] * ((double)_topologicalCharge*(_Theta[i]) + _Aberration[i]);
			Field[i] = _Mask[i] * std::exp(im*_Phase[i]);
		}
	}

	else
	{
        std::complex<double> complexMask;
        std::complex<double> complexPhase;

        #pragma omp parallel for
		for (int i = 0; i < _numElements; i++)
		{
            double stepPhase = (double)(_R[i] <= _stepRadius);
			_Phase[i] = _Mask[i] * (stepPhase*M_PI) + _Aberration[i];
			Field[i] = _Mask[i] * std::exp(im*_Phase[i]);
		}
	}
	
}

void Wavefront::calculateWrappedPhaseDisplay(int maxDisplayValue)
{
    #pragma omp parallel for
	for (int i = 0; i < _numElements; i++)
	{
        //_WrappedPhaseDisplay[i] = ((uint8_t)(_Mask[i] * (double)maxDisplayValue * (remainder(_Phase[i], (2.0 * M_PI)) / (2.0 * M_PI))));
        _WrappedPhaseDisplay[i] = _Mask[i] * (((double)std::arg(Field[i]) + M_PI)/ (2.0 * M_PI));
	}

}

void Wavefront::updatePhaseDisplay()
{
	copySubArray(_xCoordinate, _yCoordinate);
	
}

void Wavefront::copySubArray(int xCentre, int yCentre)
{

	int xPos = (xCentre - _xDim / 2) + _xDimDisplay;
	int yPos = (yCentre - _yDim / 2) + _yDimDisplay;

	uint8_t zero = 0;
	std::fill(PhaseDisplay.begin(), PhaseDisplay.end(), zero);
	
    //int startIndex = (_xDimDisplay * yPos) + xPos;
    #pragma omp parallel for
	for (int i = 0; i < _yDim; i++)
	{
        #pragma omp parallel for
		for (int j = 0; j < _xDim; j++)
		{
			int x = (xPos + j) % _xDimDisplay;
			int y = (yPos + i) % _yDimDisplay;

			//int idx = startIndex + j + (i*_xDimDisplay);
			int idx =  (y*_xDimDisplay) + x;
			int idx2 = (j * _yDim) + i;
			
            PhaseDisplay[idx] = (uint8_t)((double)maxDisplayValue * _WrappedPhaseDisplay[idx2]);
			
		}
	}
}
