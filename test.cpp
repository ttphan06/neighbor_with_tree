#include <iostream>
#include <fstream>
#include <map>
#include <array>
#include <vector>
#include <math.h>
#include <ctime>
using namespace std;


bool sortbysec(const pair<double,double> &a, 
              const pair<double,double> &b) 
{ 
    return (a.second < b.second); 
} 


int main()
{
	std::clock_t start;
    double duration;
    start = clock();
	ifstream ifs;
	string ifsVal;
	ifs.open("w.1.node");

	// set up hash map for node	
	double ins[4];
	using coordinate = array<double, 3>;
	ifs >> ins[0];
	ifs >> ins[1];
	ifs >> ins[2];
	ifs >> ins[3];
	map<double, coordinate> point;
	coordinate c;

	while (ifs >> ins[0]) 
	{
		ifs >> ins[1];
		ifs >> ins[2];
		ifs >> ins[3];
		//cout << ins[0] << ' ' << ins[1] << ' ' << ins[2] << ' ' << ins[3] << endl;
		c = {ins[1], ins[2], ins[3]};
		point.insert({ins[0], c});
	}

	/*
	for (auto it = point.begin(); it != point.end(); ++it)
	{
		cout << it->first << ' ' << it->second[0] 
		<< ' ' << it->second[1] << ' ' << it->second[2] << endl;
	}*/
  	ifs.close();



  	// get edge
  	ifs.open("w.1.edge");
  	map<double, vector<pair<double, double>>> edge;
  	pair<double, double> pointB;
  	double dist = 0;
  	ifs >> ins[0];
  	ifs >> ins[1];
  	//cout << point[2][2] << endl;
  	/*
  	edge[0].push_back({1,2});
  	edge[0].push_back({3,4});
  	cout << edge[0].at(1).second << endl;
  	*/
  	while (ifs >> ins[0])
  	{
		ifs >> ins[1];
		ifs >> ins[2];
		ifs >> ins[3];

		dist = pow(point[ins[1]][0] - point[ins[2]][0], 2)
			+ pow(point[ins[1]][1] - point[ins[2]][1], 2)
			+ pow(point[ins[1]][2] - point[ins[2]][2], 2);
		dist = sqrt(dist);
		edge[ins[1]].push_back({ins[2], dist});
		//sort(edge[ins[1]].begin(), edge[ins[1]].end(), sortbysec);
  	}

  	ifs.close();

  	for (auto it = edge.begin(); it != edge.end(); ++it)
  	{
  		sort(it->second.begin(), it->second.end(), sortbysec);
  	}

  	int index = 0;
  	for (auto it = edge.begin(); it != edge.end(); ++it)
  	{
  		index = 0;
  		while (it->second.size() < 6)
  		{
  			cout << "edge: " << it->first << ' '
  				 << edge[it->second.at(index).first].at(0).first << endl;
  			it->second.push_back(edge[it->second.at(index).first].at(0));
  			++index;
  		}

  		/*
  		cout << it->first << ' ';
  		for (int i = 0; i < it->second.size(); ++i)
  		{
  			cout << ' ' << it->second.at(i).first << ' ' << it->second.at(i).second << ' ';
		}
		cout << endl;*/
  	}

	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"printf: "<< duration <<'\n';
    return 0;
}
