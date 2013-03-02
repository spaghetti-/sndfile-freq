#include "dsp.h"
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>

void hann(double *in, int n)
{
	int i;
	double f;
	for(i = 0; i < n; i++)
	{
		f =  0.5 * (1 - cos(2 * PI * in[i] / (n - 1)));
		in[i] *= f;
	}
}

/* a - b (cos((2 * PI * n)/ (N - 1))) */
void hamming(double *in, int n)
{
	int i;
	double f;
	double l = n - 1;
	for(i = 0; i < n; i++)
	{
		f = HAMM_ALPHA - (HAMM_BETA * (cos((2 * PI * in[i]) / l)));
		in[i] *= f;
	}
}

void fft(double *in, double *out, size_t size, double *max, double *min)
{
	int i;
	double t;
	fftw_plan p;
	fftw_complex *out_c = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * size);
	*min = 0;
	*max = 0;
	p = fftw_plan_dft_r2c_1d(N, in, out_c, FFTW_ESTIMATE);
	fftw_execute(p);
	for(i = 0; i < N; i++)
	{
		t = sqrt((out_c[i][0] * out_c[i][0]) + (out_c[i][1] * out_c[i][1]));
		out[i] = t;
		if(t > *max)
			*max = t;
		if(t <= *min)
			*min = t;
	}
	out[0] = 0;
	fftw_destroy_plan(p);
	fftw_free(out_c);
}

void seperate_channels(double *in, double *in_l, double *in_r, SF_INFO *inf, size_t count)
{
	int i, j;
	for(i = 0, j = 0; i < count; i++, j++)
	{
		in_l[j] = in[i];
		in_r[j] = in[++i];
	}
}


void process(SNDFILE *sf, SF_INFO *inf)
{
	int i;
	size_t data_size, c;
	double *data, *output_l, *output_r, *left, *right, min, max_l, dB_l;
	double max_r, dB_r;

	data_size = inf->channels * N; /* N samples */
	data = (double *) malloc(sizeof(double) * data_size);
	left = (double *) malloc(sizeof(double) * N);
	right = (double *) malloc(sizeof(double) * N);
	output_l = (double *) malloc(sizeof(double) * N);
	output_r = (double *) malloc(sizeof(double) * N);

	while((c = sf_readf_double(sf, data, N)) > 0)
	{
		hamming(data, data_size);
		seperate_channels(data, left, right, inf, data_size);
		//hann(data, data_size);
		//fft(data, output, data_size, &max, &min);
		fft(left, output_l, N, &max_l, &min);
		fft(right, output_r, N, &max_r, &min);
		for(i = 0; i < N; i++)
		{
			output_l[i] = output_l[i] == 0 ? MIN : output_l[i];
			output_r[i] = output_r[i] == 0 ? MIN : output_r[i];
			dB_l = 10 * log10(output_l[i] / max_l);
			dB_r = 10 * log10(output_r[i] / max_r);
			fprintf(stdout, "%d, ", i * inf->samplerate / (N * 2));
			fprintf(stdout, "%g ", dB_l);
			fprintf(stdout, "%g\n", dB_r);
		}
	}

	sf_close(sf);
	free(data);
	free(output_l);
	free(output_r);
}
