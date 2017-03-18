/*
 *	Copyright (C) 2016  Hannes Haberl
 *
 *	This file is part of GLMViz.
 *
 *	GLMViz is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	GLMViz is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with GLMViz.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "FFT.hpp"

FFT::FFT(const size_t fft_size){
	size = fft_size;
	size_t output_size = size/2 +1;
	input = reinterpret_cast<float*>(fftwf_malloc(sizeof(float) * size));
	output = reinterpret_cast<fftwf_complex*>(fftwf_malloc(sizeof(fftwf_complex) * output_size));
	plan = fftwf_plan_dft_r2c_1d(size, input, output, FFTW_ESTIMATE);
}

FFT::~FFT(){
	fftwf_free(output);
	fftwf_free(input);
	fftwf_destroy_plan(plan);
}

void FFT::calculate(){
	fftwf_execute(plan);
}

void FFT::fill(std::vector<float>& buffer){
	for(unsigned i = 0; i < buffer.size(); i++){
		// apply hann window with corrected factors (a * 2)
		input[i] = static_cast<float>(buffer[i]);
	}
}

void FFT::calculate_window(const size_t w_size){
	window.resize(w_size);
	float N_1 = 1.0 / (float)(w_size-1);

	// compensated Blackman window constants
	const float a1 = 4620.0 / 3969.0;
	const float a2 = 715.0 / 3969.0;

	for(unsigned int i = 0; i < w_size; i++){
		/* Hann window */
		//window[i] = 1.0 - cos(2.0 * M_PI * (float)i * N_1);

		/* exact Blackman window */
		window[i] = 1.0 - a1 * cos(2*M_PI * i * N_1) + a2 * cos(4*M_PI * i * N_1);
	}
}
