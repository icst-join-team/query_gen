/*=============================================================================
# Filename: IO.h
# Author: Bookug Lobert 
# Mail: 1181955272@qq.com
# Last Modified: 2016-10-24 22:55
# Description: 
=============================================================================*/

#ifndef _IO_IO_H
#define _IO_IO_H

#include "../util/Util.h"
#include "../graph/Graph.h"

class IO
{
public:
	double var_ratio;

	std::set<std::string> names;
	std::map<std::string,int> name2id;
	std::map<int,std::string> id2name;


	IO();
	IO(std::string query, std::string data, std::string file,double _var_ratio);
	bool input(std::vector<int>& node_list, std::vector<int>& edge_list, std::vector<int>& query_list);
	bool input(Graph*& data_graph);
	Graph* input_data();
	bool output(int qid);
	bool output();
	bool output(int* m, int size);
	void flush();
	~IO();

    std::string getOutputDIR() const
    {
        return this->output_dir;
    }

	void construct_index();

	void create_sql(std::vector<int> _vlabel, std::vector< std::pair<int,int>* >_edge,
	std::vector<int> elabel,FILE* _sql_p, FILE* _res_p);

	int rand_if_var(double ratio);

private:
	std::string line;
	int data_id;
	// FILE* dfp;
	FILE* ofp;

	std::string query_req_path;
	std::string data_path;
	std::string output_dir;


};

#endif

