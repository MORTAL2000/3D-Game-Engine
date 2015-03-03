#ifndef WAVE_FILE_H
#define WAVE_FILE_H

#include <iostream>
#include <stdio.h>
#include <memory>
#include <algorithm>
#include <string>

namespace Wave
{

void assign(char* assignee, const std::string& data);

struct wave_header
{
	char chunk_id[4];
	int chunk_size;
	char format[4];
	char sub_chunk1_id[4];
 	int sub_chunk1_size;
	short int audio_format;
    short int num_channels;
    int sample_rate;
    int byte_rate;
    short int block_align;
    short int bits_per_sample;
    char sub_chunk2_id[4];
    int sub_chunk2_size;
};

void write(const std::string& filename, int buffer_size, int channels, int sampleRate, double* data);
bool read(const std::string& filename);

}

#endif
