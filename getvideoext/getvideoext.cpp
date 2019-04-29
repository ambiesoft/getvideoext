// getvideoext.cpp : Defines the entry point for the application.
//
#include <sstream>
#include <regex>
#include <vector>
#include <map>

#include "getvideoext.h"
#include "CommandLineParser.h"
#include "RunCommandGetResult.h"
#include "stdosd/stdosd.h"

using namespace std;
using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;

string launchAndGet(
	const string& ffprobe,
	const string& arg)
{
	string output;
	DWORD nRet;
	string err;
	DWORD dwLE;
	if (!RunCommandGetResult(
		ffprobe.c_str(),
		arg.c_str(),
		&nRet,
		&output,
		&err,
		&dwLE))
	{
		cerr << "Failed to launch ffprobe:" << dwLE << endl;
		exit(dwLE);
	}

	if (nRet != 0)
	{
		cerr << err << endl;
		exit(nRet);
	}
	return output;
}
int main(int argc, const char* argv[])
{
	CCommandLineParser parser;

	string ffprobe = "ffprobe";
	parser.AddOption("-f", 1, &ffprobe);

	parser.Parse(argc, argv);

	string output = launchAndGet(ffprobe, "-demuxers");

	vector<string> demuxers;
	{
		stringstream ss(output);
		string line;
		bool entryStarted = false;
		std::regex reg("^ D\\s+(\\w+)\\s+");

		while (getline(ss, line))
		{
			line = stdTrimEnd(line, "\r\n");
			if (!entryStarted)
			{
				if (stdTrim(line) == "--")
				{
					entryStarted = true;
				}
			}
			else
			{
				// entry started
	/*
	 D  3dostr          3DO STR
	 D  4xm             4X Technologies
	 D  aa              Audible AA format files
	 D  aac             raw ADTS AAC (Advanced Audio Coding)
	 D  ac3             raw AC-3
	*/

				smatch match;
				if (regex_search(line, match, reg))
				{
					vector<string> muxes = stdSplitString(match[1], ",");
					for(auto&& mux : muxes)
						demuxers.push_back(mux);
				}
			}
		}
	}

	if (demuxers.empty())
	{
		cerr << "demuxers are empty." << endl;
		return 1;
	}

	map<string, vector<string>> demuxToExts;
	regex regExts("^\\s+Common extensions: ([\\w,]+)\\.");
	for (auto&& demuxer : demuxers)
	{
		string output = launchAndGet(ffprobe, "-h demuxer=" + demuxer);
		stringstream ss(output);
		string line;
		while (getline(ss, line))
		{
			line = stdTrimEnd(line, "\r\n");

			/*
				Common extensions: mov,mp4,m4a,3gp,3g2,mj2.
			*/
			
			smatch match;
			if (regex_search(line, match, regExts))
			{
				vector<string> exts = stdSplitString(match[1], ",");
				assert(demuxToExts[demuxer].empty());
				demuxToExts[demuxer] = exts;
				break;
			}
		}
	}

	for (auto&& elem : demuxToExts)
	{
		cout << elem.first << "\t";
		for (int i = 0; i < elem.second.size(); ++i)
		{
			cout << elem.second[i];
			if ((i + 1) != elem.second.size())
				cout << ',';
		}
		cout << '\n';
	}
	return 0;
}
