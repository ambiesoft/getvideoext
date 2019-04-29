// getvideoext.cpp : Defines the entry point for the application.
//

#include "getvideoext.h"
#include "CommandLineParser.h"
#include "RunCommandGetResult.h"

using namespace std;
using namespace Ambiesoft;

int main(int argc, const char* argv[])
{
	CCommandLineParser parser;

	string ffprobe = "ffprobe";
	parser.AddOption("-f", 1, &ffprobe);

	parser.Parse(argc, argv);

	DWORD nRet;
	string output, err;
	DWORD dwLE;
	RunCommandGetResult(
		ffprobe.c_str(),
		"-h",
		&nRet,
		&output,
		&err,
		&dwLE);
	
	if (nRet == 0)
		cout << output << endl;
	else
		cerr << err << endl;

	return 0;
}
