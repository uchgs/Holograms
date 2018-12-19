#ifndef MULTIFOCALWAVEFRONT_H
#define MULTIFOCALWAVEFRONT_H
#include "Wavefront.h"
#include <vector>
#include <complex>


/*typedef std::vector<int> iVec;
typedef std::vector<double> dblVec;
typedef std::vector<std::complex<double>> complexVec;*/

class MultiFocalWavefront : public Wavefront
{
public:
    MultiFocalWavefront();
    MultiFocalWavefront(int xDim, int yDim);
    MultiFocalWavefront(int xDim, int yDim, int xDimDisplay, int yDimDisplay);
    ~MultiFocalWavefront();

    void setNumFoci(int numFoci);

private:
    int _numFoci;

    double _waveLength = 0.488;
    double _pxSize = 11;
    double _focalPlaneWidth = 10;
    double _lensFocalLength = 2000;
    double _pupilDiameter = 5600;
    dblVec _RandomPhase;
    dblVec _Hologram;
    dblVec _WrappedHologram;
    dblVec _Weights;
    complexVec _V;

    //std::vector<std::vector<double>> _Propogators;
    dblVec _Propogators;


    void setVectorSizes(int xDim, int yDim, int xDimDisplay, int yDimDisplay);
    void calcPropogators();
    void calcRandomPhase();
    void calcInitialV();
    void updateHologram();
    void getPhase();
    void calculateWrappedPhaseDisplayA(int maxDisplayValue);
};

#endif // MULTIFOCALWAVEFRONT_H
