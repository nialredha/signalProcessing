#ifndef WAV_H_	// include guard
#define WAV_H_

struct HEADER
{ 
	unsigned	char	riff[4];
	unsigned	int		size_of_file;
	unsigned	char	wave[4];
	unsigned	char	jnk_mrkr[4];
	unsigned	int		size_of_jnk;
	unsigned	char	fmt_mrkr[4];
	unsigned	int		fmt_size;
	unsigned	int		fmt_type;
	unsigned	int		chan_num;
	unsigned	int		sample_rate;
	unsigned	int		byte_rate;
	unsigned	int		bytes_per_sample;
	unsigned	int		bits_per_sample;
	unsigned	char	data_mrkr[4];
	unsigned	int		data_size;	
};

#endif // WAV_H_



