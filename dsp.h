#ifndef DSP_H_
#define DSP_H_

#include <sndfile.h>

#define N 1024 /* Order 10 FFT */
#define PI 3.14159265358979323846264
#define MIN 0.000000001
#define HAMM_ALPHA 0.53836
#define HAMM_BETA 0.46164

void process(SNDFILE *sf, SF_INFO *inf);
void hann(double *in, int n);
void hamming(double *in, int n);
void seperate_channels(double *in, double *in_l, double *in_r, SF_INFO *inf, size_t count);
void fft(double *in, double *out, size_t size, double *max, double *min);

#endif /* DSP_H_ */