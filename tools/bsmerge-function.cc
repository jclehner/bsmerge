#include <iostream>
#include <getopt.h>
#include "config.hh"
#include "util.hh"
using namespace std;

namespace {
	void print_usage()
	{
		cerr << "usage: bsmerge-function [global args] function [function args]" << endl;
		cerr << endl;
		cerr << 
			"Global options:\n"
			"\n"
			"  -F FILE         Use FILE as config file. Default is /etc/bsmerge.conf.\n";
		cerr << endl;
		exit(1);
	}
}

extern "C" int find_best_slave_main(int argc, char* argv[], const bsmerge::Config& config);

int main(int argc, char* argv[])
{
	int opt;

	while((opt = getopt(argc, argv, "F:")) != -1)
	{
		switch(opt)
		{
			case 'F':
				// ignore for now
				break;
			default:
				print_usage();
				break;
		}
	}

	if(optind >= argc)
		print_usage();

	bsmerge::Config config;
	find_best_slave_main(argc, argv, config);
}

