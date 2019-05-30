/*=============================================================================
# Filename: IO.cpp
# Author: Bookug Lobert 
# Mail: 1181955272@qq.com
# Last Modified: 2016-10-24 22:55
# Description: 
=============================================================================*/

#include "IO.h"

using namespace std;

IO::IO()
{
	this->dfp = NULL;
	this->ofp = NULL;
	this->data_id = -1;
}

IO::IO(string _query_req_path, string _data_path, string _output_dir)
{
	this->data_id = -1;
	this->line = "============================================================";
	this->query_req_path=_query_req_path;
	this->data_path=_data_path;
	this->output_dir=_output_dir;

	// dfp = fopen(data.c_str(), "r");
	// if(dfp == NULL)
	// {
	// 	cerr<<"input open error!"<<endl;
	// 	return;
	// }
//	ofp = fopen(file.c_str(), "w+");
//	if(ofp == NULL)
//	{
//		cerr<<"output open error!"<<endl;
//		return;
//	}
    // this->ofp = NULL;
    // this->output_directory = _dir;
    Util::create_dir(_output_dir);
}


void IO::construct_index()
{
	FILE* data_file_ptr=fopen(data_path.c_str(),"r");
	if(data_file_ptr == NULL)
	{
		cerr<<"data file open error!"<<endl;
		return;
	}

	while(true)
	{
		char* s,p,o;
		s=new char[200];
		p=new char[200];
		o=new char[200];
		if(!fscanf(data_file_ptr, "%s %s %s\n", s,p,o))
		{
			break;
		}

		string subject=s;
		string predicate=p;
		string object=o;

		delete []s;
		delete []p;
		delete []o;

		this->names.insert(subject);
		this->names.insert(predicate);
		this->names.insert(object);
	}
	int name_cnt=0;
	for(set<string>::iterator iter=names.begin();iter!=names.end();++iter)
	{
		string tmp_name=*iter;
		name2id.insert(pair<string,int>(tmp_name,name_cnt));
		id2name.insert(pair<int,string>(name_cnt,tmp_name));
		++name_cnt;
	}

	fclose(data_file_ptr);

}


Graph* 
IO::input_data()
{
	Graph* ng = new Graph;
	FILE* data_file_ptr=fopen(data_path.c_str(),"r");
	if(data_file_ptr == NULL)
	{
		cerr<<"data file open error!"<<endl;
		return;
	}

	// we have known how many vertices in this graph
	int v_num=this->names.size();
	for(int i=0;i<v_num;++i)
	{
		ng->addVertex(i);
	}

	while(true)
	{
		char* s,p,o;
		s=new char[200];
		p=new char[200];
		o=new char[200];
		if(!fscanf(data_file_ptr, "%s %s %s\n", s,p,o))
		{
			break;
		}

		string subject=s;
		string predicate=p;
		string object=o;

		delete []s;
		delete []p;
		delete []o;

		int s_lb=this->name2id.at(subject);
		int p_lb=this->name2id.at(predicate);
		int o_lb=this->name2id.at(object);

		ng->addEdge(s_lb,o_lb,p_lb);

	}


	fclose(data_file_ptr);
	return ng;
	// char c1, c2;
	// int id0, id1, id2, lb;
	// bool flag = false;
	// Graph* ng = NULL;

	// while(true)
	// {
	// 	fscanf(fp, "%c", &c1);
	// 	if(c1 == 't')
	// 	{
	// 		if(flag)
	// 		{
	// 			fseek(fp, -1, SEEK_CUR);
	// 			return ng;
	// 		}
	// 		flag = true;
	// 		fscanf(fp, " %c %d\n", &c2, &id0);
	// 		if(id0 == -1)
	// 		{
	// 			return NULL;
	// 		}
	// 		else
	// 		{
	// 			ng = new Graph;
	// 			int nodeNum, edgeNum, ln1, ln2;
	// 			fscanf(fp, "%d %d %d %d\n",&nodeNum,&edgeNum, &ln1, &ln2);
	// 		}
	// 	}
	// 	else if(c1 == 'v')
	// 	{
	// 		fscanf(fp, " %d %d\n", &id1, &lb);
	// 		ng->addVertex(lb); 
	// 	}
	// 	else if(c1 == 'e')
	// 	{
	// 		fscanf(fp, " %d %d %d\n", &id1, &id2, &lb);
	// 		//NOTICE:we treat this graph as directed, each edge represents two
	// 		//This may cause too many matchings, if to reduce, only add the first one
	// 		ng->addEdge(id1, id2, lb);
	// //		ng->addEdge(id2, id1, lb);
	// 	}
	// 	else 
	// 	{
	// 		cerr<<"ERROR in input() -- invalid char"<<endl;
	// 		return false;
	// 	}
	// }
	// return NULL;
}

bool 
IO::input(Graph*& data_graph)
{
	// convert names to ids
	this->construct_index();
	data_graph = this->input_data();
	if(data_graph == NULL)
		return false;
	this->data_id++;
	return true;
}

bool 
IO::input(vector<int>& node_list, vector<int>& edge_list, vector<int>& query_list)
{
	int queryNodeNum = 0;
	int queryEdgeNum = 0;
    int queryNum = 0;
	
	std::ifstream ifs(this->query_req_path.c_str());
	while (true)
	{
		ifs >> queryNodeNum;
		if (queryNodeNum == -1) 
		{
			break;
		}
		ifs >> queryEdgeNum;
        ifs >> queryNum;
		//cout << queryNodeNum << " to add!" << endl;
		node_list.push_back(queryNodeNum);
		edge_list.push_back(queryEdgeNum);
        query_list.push_back(queryNum);
	}
	ifs.close();
	return true;
}

bool 
IO::output(int qid)
{
	fprintf(ofp, "query graph:%d    data graph:%d\n", qid, this->data_id);
	fprintf(ofp, "%s\n", line.c_str());
	return true;
}

bool
IO::output()
{
	fprintf(ofp, "t # -1\n");
	return true;
}

bool 
IO::output(int* m, int size)
{
	for(int i = 0; i < size; ++i)
	{
		fprintf(ofp, "(%d, %d) ", i, m[i]);
	}
	fprintf(ofp, "\n");
	return true;
}

void
IO::flush()
{
	fflush(this->ofp);
}

IO::~IO()
{
	fclose(this->dfp);
	this->dfp = NULL;
    if(this->ofp != NULL)
    {
        //NOTICE: fclose(NULL) will cause error, while fflush(NULL) is ok
        fclose(this->ofp);
        this->ofp = NULL;
    }
}

