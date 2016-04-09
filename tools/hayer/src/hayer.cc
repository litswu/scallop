#include "hayer.h"
#include "util.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>

using namespace std;

int hayer::process(const string &file, const string &s)
{
	if(s == "T1")
	{
		read_genome(file);
	}
	else if(s == "ER" || s == "EP")
	{
		read_transcripts(file);
		build_genome();
	}

	gm.sort();
	gm.build_index();

	return 0;
}

int hayer::read_genome(const string &file)
{
	ifstream fin(file.c_str());
	if(fin.fail()) return 0;

	char buf[102400];
	char s0[102400];
	char s1[102400];
	char s2[102400];

	gene gn;
	transcript tt;
	tt.gene_id = "shaomingfu";
	tt.transcript_id = "shaomingfu";
	tt.source = "hayer";
	tt.feature = "transcript";

	while(fin.getline(buf, 102400, '\n'))
	{
		stringstream sstr(buf);
		sstr>>s0;
		string id(s0);
		if(id.substr(0, 4) == "GENE")
		{
			if(tt.exons.size() >= 1 && tt.expression >= 1)
			{
				tt.expression /= tt.length();
				if(tt.expression >= 1)
				{
					gn.add_transcript(tt);
				}
			}

			tt.exons.clear();
			sstr>>s1;
			tt.strand = s1[0];
			tt.transcript_id = id;

			string gid = parse_transcript_id(tt.transcript_id);
			if(tt.gene_id == gid) continue;

			tt.gene_id = gid;

			if(gn.transcripts.size() >= 1)
			{
				gm.add_gene(gn);
				gn.transcripts.clear();
			}
		}
		else if(id == "transcript")
		{
			sstr>>s1>>tt.expression;
			tt.expression *= 100;
		}
		else if(id == "exon")
		{
			sstr>>s1>>s2;
			int s, t;
			parse_location(s2, tt.seqname, s, t);
			tt.add_exon(s, t);
		}
	}
	fin.close();
	return 0;
}


int hayer::read_transcripts(const string &file)
{
	ifstream fin(file.c_str());
	if(fin.fail()) return 0;

	char buf[102400];
	char s0[102400];
	char s1[102400];
	char s2[102400];

	transcript tt;
	tt.gene_id = "shaomingfu";
	tt.transcript_id = "shaomingfu";
	tt.source = "hayer";
	tt.feature = "transcript";

	while(fin.getline(buf, 102400, '\n'))
	{
		stringstream sstr(buf);
		sstr>>s0;
		string id(s0);
		if(id.substr(0, 4) == "GENE")
		{
			if(tt.exons.size() >= 1 && tt.expression >= 1)
			{
				tt.expression /= tt.length();
				if(tt.expression >= 1)
				{
					transcripts.push_back(tt);
				}
			}

			tt.exons.clear();
			sstr>>s1;
			tt.strand = s1[0];
			tt.transcript_id = id;
		}
		else if(id == "transcript")
		{
			sstr>>s1>>tt.expression;
			tt.expression *= 100;
		}
		else if(id == "exon")
		{
			sstr>>s1>>s2;
			int s, t;
			parse_location(s2, tt.seqname, s, t);
			//printf("%s: exon %s (%d, %d)\n", tt.transcript_id.c_str(), s1, s, t);
			assert(s <= t);
			tt.add_exon(s, t + 1);
		}
	}
	fin.close();
	return 0;
}

int hayer::build_genome()
{
	if(transcripts.size() == 0) return 0;

	for(int i = 0; i < transcripts.size(); i++)
	{
		transcripts[i].sort();
	}

	sort(transcripts.begin(), transcripts.end());

	int index = 0;
	gene gn;
	gn.add_transcript(transcripts[0]);
	for(int i = 1; i < transcripts.size(); i++)
	{
		transcript &s = transcripts[i - 1];
		transcript &t = transcripts[i + 0];
		int l = s.get_bounds().second;
		int r = t.get_bounds().first;
		if(s.seqname != t.seqname || l <= r)
		{
			gn.set_gene_id(string("HAYER.") + tostring(index));
			gm.add_gene(gn);
			gn.clear();
			index++;
		}
		gn.add_transcript(transcripts[i]);
	}
	return 0;
}

int hayer::write(const string &file)
{
	gm.write(file);
	return 0;
}

int hayer::parse_location(const string &s, string &chrm, int &start, int &end)
{
	int p1 = -1;
	int p2 = -1;
	for(int i = 0; i < s.size(); i++)
	{
		if(s[i] == ':') p1 = i;
		if(s[i] == '-') p2 = i;
	}
	assert(p1 != -1);
	assert(p2 != -1);
	assert(p2 > p1);
	chrm = s.substr(0, p1);
	start = atoi(s.substr(p1 + 1, p2 - p1 - 1).c_str());
	end = atoi(s.substr(p2 + 1, s.size() - p2 - 1).c_str());
	return 0;
}

string hayer::parse_transcript_id(const string &s)
{
	int p1 = -1;
	int p2 = -1;
	for(int i = 0; i < s.size(); i++)
	{
		if(s[i] == '.' && p1 == -1) p1 = i;
		if(s[i] == '.' && p1 != -1) p2 = i;
	}
	assert(p1 != -1);
	assert(p2 != -1);
	assert(p2 > p1);

	return s.substr(0, p2);
}
