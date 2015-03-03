#include "WaveFile.h"

namespace Wave
{

void assign(char* assignee, const std::string& data)
{
	for(size_t i = 0; i < data.size(); i++)
	{
		assignee[i] = data[i];
	}
}

void write(const std::string& filename, int buffer_size, int channels, int sampleRate, double* data)
{
	FILE* file = fopen(filename.c_str(), "wb");
	if(!file) return;

	wave_header header;
	assign(header.chunk_id, "RIFF");
	header.chunk_size = 36 + buffer_size;
	assign(header.format, "WAVE");
	assign(header.sub_chunk1_id, "fmt ");
	header.sub_chunk1_size = 16;
	header.audio_format = 1;
	header.num_channels = channels;
	header.sample_rate = sampleRate;
	header.byte_rate = sampleRate * channels * sizeof(double);
	header.block_align = channels * sizeof(double);
	header.bits_per_sample = 8 * sizeof(double);
	assign(header.sub_chunk2_id, "data");
	header.sub_chunk2_size = buffer_size;
	fwrite(&header, sizeof(header), 1, file);
	fwrite(data, buffer_size, 1, file);
	fclose(file);
};

bool read(const std::string& filename)
{
	std::shared_ptr<wave_header> header(new wave_header());

	FILE* file = fopen(filename.c_str(), "rb");
	if(!file) return false;
	short int buffer[256];
	int nb, count;
	fread(header.get(), 1, sizeof(wave_header), file);
	std::vector<double> data;

	while((nb = fread(buffer, sizeof(short int), 256, file)) > 0)
	{
		for(int i = 0; i < nb; i+=2)
		{
			int hi = (signed char) buffer[i + 1];
			int c = (hi << 8) | buffer[i + 0];
			double t = c / 32768.0;
			data.push_back(t);
		}
	}
	fclose(file);

	std::cout << "Sample rate of " << filename << " is : " << header->sample_rate << std::endl;
	std::cout << "Data size of " << filename << " is : " << header->sub_chunk2_size << std::endl;
	return true;
}

}
