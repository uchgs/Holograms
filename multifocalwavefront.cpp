#include "multifocalwavefront.h"
#include <omp.h>
#include <iostream>
#define _USE_MATH_DEFINES

#include <math.h>

const std::complex<double> im(0, 1);

MultiFocalWavefront::MultiFocalWavefront()
{
    _numFoci = 64;

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

    /*calcZernikes();
    calcAberrationPhase();
    calcWFPhase();
    updateAberrationFunction(2,50);*/

    calcPropogators();
    calcRandomPhase();
    calcInitialV();
    updateHologram();

    maxDisplayValue = 255;
    calculateWrappedPhaseDisplay(maxDisplayValue);
    copySubArray(0, 0);


}

MultiFocalWavefront::MultiFocalWavefront(int xDim, int yDim)
{
    _numFoci = 64;

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

    calcPropogators();
    calcRandomPhase();
    calcInitialV();
    updateHologram();

    maxDisplayValue = 255;
    calculateWrappedPhaseDisplay(maxDisplayValue);
    copySubArray(0, 0);
}

MultiFocalWavefront::MultiFocalWavefront(int xDim, int yDim, int xDimDisplay, int yDimDisplay)
{
    _numFoci = 16;

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

    //calcZernikes();
    //calcAberrationPhase();
    //calcWFPhase();
    //updateAberrationFunction(2,50);


    calcRandomPhase();
    calcPropogators();
    //calcInitialV();
    //updateHologram();
    getPhase();
    maxDisplayValue = 255;
    calculateWrappedPhaseDisplayA(maxDisplayValue);
    copySubArray(_xCoordinate, _yCoordinate);
}

MultiFocalWavefront::~MultiFocalWavefront()
{
}

void MultiFocalWavefront::setNumFoci(int numFoci)
{
    _numFoci = numFoci;

    _V.resize(_numFoci);
    //_Propogators.resize(_numFoci);

    calcPropogators();

}

void MultiFocalWavefront::setVectorSizes(int xDim, int yDim, int xDimDisplay, int yDimDisplay)
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
    _RandomPhase.resize(_numElements);
    _Hologram.resize(_numElements);
    PhaseDisplay.resize(_numElementsDisplay);
    Field.resize(_numElements);

    #pragma omp parallel for
    for (int i = 0; i < numPolynomials; i++)
    {
        Zernikes[i].resize(_numElements);
    }

    //_Propogators.resize(_numFoci);
    _Propogators.resize(_numElements);
    _V.resize(_numFoci);
    _Weights.resize(_numFoci);
    std::fill(_Weights.begin(), _Weights.end(),1);

    /*#pragma omp parallel for
    for (int i = 0; i < _numFoci; i++)
    {
        _Propogators[i].resize(_numElements);
    }*/
}

void MultiFocalWavefront::calcPropogators()
{
    dblVec xCoordinates, yCoordinates;
    int noCols = (int)sqrt(_numFoci);
    int noRows = (int)sqrt(_numFoci);
    xCoordinates.resize(noCols);
    yCoordinates.resize(noRows);

    double spacing = _focalPlaneWidth/(double)(noCols - 1);
    double startCoord = -_focalPlaneWidth/2.0;
    std::fill(Field.begin(),Field.end(),(0,0));
    #pragma omp parallel for
    for(int i = 0; i < noCols; i++)
    {
        double coord = startCoord + ((double)i)*spacing;

        xCoordinates[i] = coord;
        yCoordinates[i] = coord;
    }

    double k = 2.0 * M_PI/_waveLength;
    double pupilRadius = _pupilDiameter/2;
    std::complex<double> current(0,0);

    #pragma omp parallel for ordered
    for(int i = 0; i < noCols; i++)
    {
        #pragma omp ordered
        {
            #pragma omp parallel for ordered
            for(int j = 0; j < noRows; j++)
            {
                #pragma omp ordered
                {
                    #pragma omp parallel for ordered
                    for(int y = 0; y < _yDim; y++)
                    {
                        current.real(0);
                        current.imag(0);
                        #pragma omp ordered
                        {
                            #pragma omp parallel for ordered
                            for(int x = 0; x < _xDim; x++)
                            {
                                int idx = (x * _yDim) + y;
                                _Propogators[idx] = (0,0);
                                //_Propogators[i*noCols + j][idx] = 0;//(k/_lensFocalLength)*((_X[x]/(double)(_xDim/2.0))*pupilRadius*xCoordinates[i] + (_Y[y]/(double)(_xDim/2.0))*pupilRadius*yCoordinates[j]);
                                _Propogators[idx] += (k/_lensFocalLength)*((_X[x]/(double)(_xDim/2.0))*pupilRadius*xCoordinates[i] + (_Y[y]/(double)(_xDim/2.0))*pupilRadius*yCoordinates[j]);
                                current += std::exp(im*(static_cast<std::complex<double>>(_RandomPhase[i]) - _Propogators[idx]));
                            }
                            _V[i*noCols + j] = current/(static_cast<std::complex<double>>(_numFoci));

                            #pragma omp parallel for ordered
                            for(int y = 0; y < _yDim; y++)
                            {
                                #pragma omp ordered
                                {
                                    #pragma omp parallel for ordered
                                    for(int x = 0; x < _xDim; x++)
                                    {
                                        int idx = (x * _yDim) + y;
                                        Field[idx] += std::exp(im*(_Propogators[idx] * _V[i*noCols + j]/std::abs(_V[i*noCols + j])));
                                        //_Propogators[i*noCols + j][idx] = 0;//(k/_lensFocalLength)*((_X[x]/(double)(_xDim/2.0))*pupilRadius*xCoordinates[i] + (_Y[y]/(double)(_xDim/2.0))*pupilRadius*yCoordinates[j]);
                                        //_Propogators[idx] += (k/_lensFocalLength)*((_X[x]/(double)(_xDim/2.0))*pupilRadius*xCoordinates[i] + (_Y[y]/(double)(_xDim/2.0))*pupilRadius*yCoordinates[j]);
                                        //current += std::exp(im*(static_cast<std::complex<double>>(_RandomPhase[i]) - _Propogators[idx]));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void MultiFocalWavefront::calcRandomPhase()
{
    int v;
    double phaseVal;

    #pragma omp parallel for
    for(int i = 0; i < _numElements; i++)
    {
        v = rand() % 200;
        phaseVal = (((double)v)/(200.0))*2.0*M_PI;
        _RandomPhase[i] = phaseVal;
    }
}

void MultiFocalWavefront::calcInitialV()
{
    std::complex<double> current(0,0);
    #pragma omp parallel for ordered
    for(int index = 0; index < _numFoci; index++)
    {
        #pragma omp ordered
        {
            current.real(0);
            current.imag(0);
            #pragma omp parallel for ordered
            for(int i = 0; i < _numElements; i++)
            {
                #pragma omp ordered
                {
                current += std::exp(im*(static_cast<std::complex<double>>(_RandomPhase[i]) - _Propogators[i]));
                }
            }

            _V[index] = current/(static_cast<std::complex<double>>(_numFoci));
            }
    }

}

void MultiFocalWavefront::updateHologram()
{
    std::complex<double> currentVal(0,0);
    #pragma omp parallel for ordered
    for(int i = 0; i < _numElements; i++)
    {
        #pragma omp ordered
        {
            currentVal = (0,0);

            #pragma omp parallel for ordered
            for(int index = 0; index < _numFoci; index++)
            {
                #pragma omp ordered
                {
                    currentVal += 0;//std::exp(im * _Propogators[index][i]) * static_cast<std::complex<double>>((_Weights[index]*_V[index])/((double)std::abs(_V[index])));
                }
            }

            _Phase[i] = (double)std::arg(currentVal);
        }
    }

}

void MultiFocalWavefront::getPhase()
{
    #pragma omp parallel for
    for (int i = 0; i < _numElements; i++)
    {
        _Phase[i] = std::arg(Field[i]);
    }

}

void MultiFocalWavefront::calculateWrappedPhaseDisplayA(int maxDisplayValue)
{
    #pragma omp parallel for
    for (int i = 0; i < _numElements; i++)
    {
        _WrappedPhaseDisplay[i] = (uint8_t)(_Mask[i] * (double)maxDisplayValue * (_Phase[i]/ (2.0 * M_PI)));
    }

}
