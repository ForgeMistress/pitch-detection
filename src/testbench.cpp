#include <iostream>
#include <fstream>
#include "sinegenerator.h"
#include "pitch_detector.h"

#include "testbench.h"
#ifdef FFMPEG_ENABLED
#include "mp3read.h"
#endif

void testbench(std::string testbench_type, PitchDetector *pitch_detector)
{
	if (testbench_type == "mp3") {
#ifdef FFMPEG_ENABLED
		read_mp3_file((char *) "./tests/guitar_eadgbe.mp3", pitch_detector);
#else
		std::cout << "Compiled without ffmpeg/libav, cannot use this feature\n";
		exit(-1);
#endif
	} else if (testbench_type == "sinewave") {
		std::ifstream input("./tests/pitches.txt");
		double sine_freq;
		while (input >> sine_freq) {
			sinegenerator sinegen = sinegenerator(48000, sine_freq);
			sinegen.generate_tone();
			pitch_detector->init(48000, sinegen.size_single_channel);
			double pitch = pitch_detector->get_pitch(sinegen.tone_single_channel);
			printf("Sinewave freq: %f\tpitch: %f\n", sine_freq, pitch);
			pitch_detector->cleanup();
			sinegen.cleanup();
		exit(0);
		}
	} else {
		std::cout << testbench_type << " is not a valid testbench\n";
		exit(-1);
	}
}