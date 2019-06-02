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

	this->data_id = -1;
}

IO::IO(string _query_req_path, string _data_path, string _output_dir,double _var_ratio)
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
    this->ofp = NULL;
    // this->output_directory = _dir;
    Util::create_dir(_output_dir);
	this->var_ratio=_var_ratio;
}


void IO::construct_index()
{
	printf("data path: %s\n",data_path.c_str());
	FILE* data_file_ptr=fopen(data_path.c_str(),"r");
	if(data_file_ptr == NULL)
	{
		cerr<<"data file open error!"<<endl;
		return;
	}

	while(true)
	{
		
		char* s=new char[200];
		char* p=new char[200];
		char* o=new char[200];
		int fscanf_ret=fscanf(data_file_ptr, "%s %s %s .\n", s,p,o);

		if(fscanf_ret<0)
		{
			break;
		}

		string subject=s;
		string predicate=p;
		string object=o;

		printf("s:%s p:%s o:%s\n",s,p,o);

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
		return NULL;
	}

	// we have known how many vertices in this graph
	int v_num=this->names.size();
	for(int i=0;i<v_num;++i)
	{
		ng->addVertex(i);
	}

	while(true)
	{
		
		char* s=new char[200];
		char* p=new char[200];
		char* o=new char[200];
		
		int fscanf_ret=fscanf(data_file_ptr, "%s %s %s .\n", s,p,o);

		if(fscanf_ret<0)
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

    if(this->ofp != NULL)
    {
        //NOTICE: fclose(NULL) will cause error, while fflush(NULL) is ok
        fclose(this->ofp);
        this->ofp = NULL;
    }
}

void IO::create_sql(std::vector<int> _vlabel, vector< pair<int,int>* >_edge,vector<int> _elabel,
FILE* _sql_p, FILE* _res_p)
{
	set<int> id_set;
	map<int,string> id2query_name;
	vector<string> select_vec;
	vector<string> res_vec;
	for(vector<int>::iterator iter=_vlabel.begin();iter!=_vlabel.end();++iter)
	{
		id_set.insert(*iter);
	}
	for(vector<int>::iterator iter=_elabel.begin();iter!=_elabel.end();++iter)
	{
		id_set.insert(*iter);
	}
	int var_cnt=0;
	
	for(set<int>::iterator iter=id_set.begin();iter!=id_set.end();++iter)
	{
		int tmp_id=*iter;
		int rand_ret=rand_if_var(this->var_ratio);
		bool is_var;
		if(rand_ret)
		{
			is_var=true;
		}
		else
		{
			is_var=false;
		}
		if(is_var)
		{
			string tmp_name;
			stringstream ss;
			ss<<"?a";
			ss<<var_cnt;
			ss>>tmp_name;
			++var_cnt;
			id2query_name.insert(pair<int,string>(tmp_id,tmp_name));
			select_vec.push_back(tmp_name);
			res_vec.push_back(this->id2name[tmp_id]);
		}
		else
		{
			string tmp_name=this->id2name.at(tmp_id);
			id2query_name.insert(pair<int,string>(tmp_id,tmp_name));
		}	
	}
	if(select_vec.size()==0)
	{
		int tmp_id=_vlabel[0];
		string tmp_name;
		stringstream ss;
		ss<<"?a";
		ss<<var_cnt;
		ss>>tmp_name;
		++var_cnt;
		id2query_name.at(tmp_id)=tmp_name;
		select_vec.push_back(tmp_name);
		res_vec.push_back(this->id2name[tmp_id]);
	}

	int edge_cnt=0;
	for(vector<string>::iterator iter=res_vec.begin();iter!=res_vec.end();++iter)
	{
		fprintf(_res_p,"%s ",(*iter).c_str());
	}
	fprintf(_res_p,"\n");
	fprintf(_sql_p,"select ");
	for(vector<string>::iterator iter=select_vec.begin();iter!=select_vec.end();++iter)
	{
		fprintf(_sql_p,"%s ",(*iter).c_str());
	}
	fprintf(_sql_p,"where\n");
	fprintf(_sql_p,"{\n");
	for(vector< pair<int,int>* >::iterator iter=_edge.begin();iter!=_edge.end();++iter)
	{
		pair<int,int>* pair_ptr=*iter;
		int v_left_pos=pair_ptr->first;
		int v_right_pos=pair_ptr->second;
		int s_id=_vlabel[v_left_pos];
		int o_id=_vlabel[v_right_pos];
		int p_id=_elabel[edge_cnt];
		++edge_cnt;
		string s_sql_name=id2query_name[s_id];
		string p_sql_name=id2query_name[p_id];
		string o_sql_name=id2query_name[o_id];
		fprintf(_sql_p,"%s %s %s .\n",s_sql_name.c_str(),p_sql_name.c_str(),o_sql_name.c_str());
	}
	fprintf(_sql_p,"}\n");
}

int IO::rand_if_var(double ratio)
{
	int tmp_range=int(1/ratio);
	srand((unsigned)time(NULL)); 
	int ret=rand()%tmp_range;
	if(ret)
	{
		ret=0;
	}
	else
	{
		ret=1;
	}
	
	return ret;
}
