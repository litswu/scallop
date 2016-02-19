#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include "splice_graph.h"
#include "path.h"

class assembler
{
public:
	assembler(const splice_graph &g);
	virtual ~assembler();

public:
	splice_graph gr;				// splice graph
	vector<path> paths;				// transcripts

public:
	virtual int assemble() = 0;
	int print(const string &prefix) const;

protected:
	int update_weights();

	double compute_bottleneck_weight(const path &p) const;
	path compute_maximum_forward_path() const;
	path compute_maximum_path() const;

	int decrease_path(const path &p);
	int increase_path(const path &p);
	int add_backward_path(const path &p);
	int remove_backward_path(const path &p);

	int backup_edge_weights(MED &med) const;
	int recover_edge_weights(const MED &med);

};

#endif
