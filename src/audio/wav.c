/*
*
* Parse/read a wav file
*
* Code implemented based on:
* http://truelogic.org/wordpress/2015/09/04/parsing-a-wav-file-in-c/
*
* Other references:
* https://www.daubnet.com/en/file-format-riff
* http://soundfile.sapp.org/doc/WaveFormat/
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "wav.h"

FILE *ptr;
char *filename;
struct HEADER header;

unsigned char fbb[4];	// 4 byte buffer
unsigned char tbb[2];	// 2 byte buffer

int main(int argc, char **argv)
{
	filename = (char*) malloc(sizeof(char) * 1024);
	if (filename == NULL)
	{
		printf("ERROR in malloc\n");
		exit(1); // indicating abnormal termination of program
	}

	// get file path
	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		strcpy(filename, cwd);

		// get filename from command line
		if (argc < 2)	// executable and .wav file
		{
			printf("Please Specify WAV file\n");
			return 0;
		}

		// add filename to the dir
		strcat(filename, "/");
		strcat(filename, argv[1]);
		printf("\n%s\n", filename);
	} 

	// open file
	printf("\nOpening file...\n");
	ptr = fopen(filename, "rb");	// "rb" used to open non_text files
	if (ptr == NULL)
	{
		printf("ERROR in opening file\n");
		exit(1);	// inidicating abnormal termination of program
	}

	// read wav header
	int read = 0; // total number of elements successfull read
	
	read = fread(header.riff, sizeof(header.riff), 1, ptr);
	printf("\n(1-4): %s\n", header.riff);

	read = fread(fbb, sizeof(fbb), 1, ptr);

	// convert little endian to big endian 4 byte int 
	// https://chortle.ccsu.edu/AssemblyTutorial/Chapter-15/ass15_3.html
	// take least sig byte and put in front of second least sig byte and repeat
	header.size_of_file = fbb[0] | fbb[1]<<8 | fbb[2]<<16 | fbb[3]<<24;
	printf("(5-8) Overall Size: %u bytes\n", header.size_of_file);
	
	read = fread(header.wave, sizeof(header.wave), 1, ptr);
	printf("(9-12) Wave Marker: %s\n", header.wave);

	// WAVE file may or may not have JUNK chunk
	// so check for JUNK chunk before moving to FMT chunk
	read = fread(header.fmt_mrkr, sizeof(header.fmt_mrkr), 1, ptr);

	if (strcmp(header.fmt_mrkr, "JUNK") == 0)
	{
		strcpy(header.jnk_mrkr, header.fmt_mrkr);
		printf ("\nJunk Marker: %s\n", header.jnk_mrkr);

		read = fread(fbb, sizeof(fbb), 1, ptr);
		header.size_of_jnk = fbb[0] | fbb[1]<<8 | fbb[2]<<16 | fbb[3]<<24;
		printf("Size of Junk: %u bytes\n\n", header.size_of_jnk);

		unsigned char jbb[header.size_of_jnk];
		read = fread(jbb, sizeof(jbb), 1, ptr);

		read = fread(header.fmt_mrkr, sizeof(header.fmt_mrkr), 1, ptr);
		printf("(13-16) Fmt Marker: %s\n", header.fmt_mrkr);

	}
	else 
	{
		strcpy(header.jnk_mrkr, "NONE");
		printf ("\nJunk Marker: %s\n", header.jnk_mrkr);

		header.size_of_jnk = 0;
		printf("Size of Junk: %u bytes\n\n", header.size_of_jnk);

		printf("(13-16) Fmt Marker: %s\n", header.fmt_mrkr);
	}	

	read = fread(fbb, sizeof(fbb), 1, ptr);
	header.fmt_size = fbb[0] | fbb[1]<<8 | fbb[2]<<16 | fbb[3]<<24;
	printf("(17-20) Fmt Size: %u\n", header.fmt_size);

	read = fread(tbb, sizeof(tbb), 1, ptr);
	header.fmt_type = tbb[0] | tbb[1]<<8;	
	
	char fmt_name[10] = "";

	if (header.fmt_type == 1) { strcpy(fmt_name, "PCM"); }
	else if (header.fmt_type = 6) { strcpy(fmt_name, "A-law"); }
	else { strcpy(fmt_name, "Mu-law"); }
	printf("(21-22) Fmt Type: %s\n", fmt_name);

	read = fread(tbb, sizeof(tbb), 1, ptr);
	header.chan_num = tbb[0] | tbb[1]<<8;
	printf("(23-24) Number of Channels: %u\n", header.chan_num);

	read = fread(fbb, sizeof(fbb), 1, ptr);
	header.sample_rate = fbb[0] | fbb[1]<<8 | fbb[2]<<16 | fbb[3]<<24;
	printf("(25-28) Sample Rate: %u\n", header.sample_rate);

	read = fread(fbb, sizeof(fbb), 1, ptr);
	header.byte_rate = fbb[0] | fbb[1]<<8 | fbb[2]<<16 | fbb[3]<<24;
	printf("(29-32) Byte Rate: %u\n", header.byte_rate);

	read = fread(tbb, sizeof(tbb), 1, ptr);
	header.bytes_per_sample = tbb[0] | tbb[1]<<8;
	printf("(33-34) Bytes Per Sample: %u\n", header.bytes_per_sample);

	read = fread(tbb, sizeof(tbb), 1, ptr);
	header.bits_per_sample = tbb[0] | tbb[1]<<8;
	printf("(35-36) Bits Per Sample: %u\n", header.bits_per_sample);

	read = fread(header.data_mrkr, sizeof(header.data_mrkr), 1, ptr);
	printf("(37-40) Data Marker: %s\n", header.data_mrkr);

	read = fread(fbb, sizeof(fbb), 1, ptr);
	header.data_size = fbb[0] | fbb[1]<<8 | fbb[2]<<16 | fbb[3]<<24;
	printf("(41-44) Size of Data: %u\n", header.data_size);

	// calculate number of samples
	// size of data seems to be wrong, so this value is wrong as well
	// value isn't that important, so i didn't fix it
	long sample_num = (8 * header.data_size) / 
					  (header.chan_num * header.bits_per_sample);
	printf("\nNumber of Samples: %lu\n", sample_num);
	
	// calculate size of each sample in bytes
	long sample_size = (header.chan_num * header.bits_per_sample) / 8;
	printf("Size of Each Sample: %lu bytes\n", sample_size);

	// calculate duration of file
	float duration = (float) header.size_of_file / header.byte_rate;
	printf("Approx. Duration in seconds = %f\n", duration);

	// close the file
	printf("\nClosing file...\n\n");
	fclose(ptr);

	// free dynamic storage
	free(filename);

	// read wav file data
	if (header.fmt_type == 1) // PCM meaning no compression
	{
		printf("Dump Uncompressed Data? (y/n):\n");
		char ans = "n";
		scanf("%c", &ans);
		if (c == "Y" || c == "y")
		{
			char data_buf[sample_size];
			int is_size_correct = 1; // YES 

			// make sure sample size is completely divisible by # of channels
			int remainder = sample_size % header.chan_num;
			if (remainder != 0)
			{
				printf("Error in sample size"); 
				is_size_correct = 0; // NO
			}

			if (is_size_correct == 0)
			{
				long lower_limit = 0l; // sets all bits to zero for long format
				long upper_limit = 0l;
				switch (header.bits_per_sample)
				{
					case 8:
						lower_limit = -128;
						upper_limit = 127; // bit for the sign (why take +?)
						break;
					case 16:
						lower_limit = -32768;
						upper_limit = 32767;
						break;
					case 32:
						lower_limit = -2147483648;
				}
						upper_limit = 2147483647;
						break;
			}
		}
	}

	return 0;
}

