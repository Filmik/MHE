#include<iostream>
#include<math.h>
#include<stdio.h>
#include<vector>
#include<fstream>
#include<cstdlib>
#include<tuple>
#include<string>
#include<random>
#include <ctime>
#include <algorithm>

using namespace std;

vector<vector<int> > g;//vector for edge conections, x-edge_1 y-edge_2
vector<vector<int> > h;// x-edge y-number of edge
vector<int >solution;
bool v[11110];
float best_solution=0;

void Show_conections()
{
	cout << "Example: X---edge_nr---Y" << endl;
	for (int x = 0; x < g.size(); x++)//cout conections
	{
		cout << "NR_X=" << x + 1 << "  NR_Y:";
		for (int y = 0; y < g[x].size(); y++)
		{
			cout << g[x][y] + 1 << "-";
		}
		cout << endl;
		cout << "NR_X=" << x + 1 << " include Edges_NR:";
		for (int y = 0; y < h[x].size(); y++)
		{
			cout << h[x][y] + 1 << "-";
		}
		cout << endl;
	}
}

float Best_solution(vector<int> solution, int number_of_vertex, int number_of_edges) //solution contains selected vertexes
{
	int edges_in_soluton=0, vertex_in_solution = 0, number_of_duplicats = 0,b;
	float current_solution=0;
	vector<int> entered_edges;
	//how many vertexes and edges
	for (int x=0; x < solution.size(); x++) {
		cout << "SOLUTIONS:" << solution[x] << endl;
		b = solution[x];
		b--;
		for (int y = 0; y < h[b].size(); y++)//for evry edge in x 
		{
			entered_edges.push_back(h[b][y]);
			edges_in_soluton++;
		}
		vertex_in_solution++;
	}

	//how meny duplicats
	for (int x = 0; x < entered_edges.size(); x++)
	{
		int current_number_of_duplicats = 0;
		int current_edge= entered_edges[x];
		int y = x;
		for (y; y<entered_edges.size(); y++)
		{
			if (current_edge == entered_edges[y])
			{
				current_number_of_duplicats++;
			}
		}
		if (current_number_of_duplicats > 1)
		{
			number_of_duplicats+= current_number_of_duplicats-1;
		}
	}

	cout << "V=" << vertex_in_solution << " E=" << edges_in_soluton<<" D="<< number_of_duplicats <<endl;
	current_solution += (float)number_of_vertex / (float)vertex_in_solution;//points for number of  used vertex
	current_solution *= ((float)number_of_edges * pow(((float)edges_in_soluton-(float)number_of_duplicats), 2.0));//points for covering edges
	current_solution -= (pow(number_of_duplicats, 2.0) * 4);//minus points for duplicats
	if (current_solution > best_solution) {
		best_solution= current_solution;
		cout << "best_solution= " << best_solution << endl;
	}
	return current_solution;
}

void Climbing_Algorythm(int number_of_vertex, int number_of_edges)
{
	float current_solution;
	vector<int>random_solution;
	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	uniform_int_distribution<> distr(1, number_of_vertex);// define the range
	for (int x = 0; x < distr(eng); x++)
	{
		random_solution.push_back(distr(eng));
	}
	//random_solution.push_back(1);
	//random_solution.push_back(1);
	for (int x = random_solution.size(); x < random_solution.size() * 2; x++)//alternatywna 1
	{
		current_solution=Best_solution(random_solution, number_of_vertex, number_of_edges);//rate random solution

		if (current_solution == best_solution)
		{
			if (random_solution[0]< number_of_vertex)
			{
			random_solution[0] = random_solution[0] + 1;
			}
		}
		if(current_solution<best_solution)
		{
			x = random_solution.size() * 2;
		}
	}
	/*if (random_solution.size() > 1)//alternatywna 2
	{
		int range=2,new_solution,start_solution;
		uniform_int_distribution<> close_solution(1, range);
		current_solution = Best_solution(random_solution, number_of_vertex, number_of_edges);//rate random solution
		start_solution = random_solution[0];
		for (int x = 0; x < range; x++)
		{
			new_solution = close_solution(eng);
			if (new_solution % 2 == 0) 
			{
				if (new_solution + random_solution[0] < random_solution.size())
				{
					random_solution[0] += new_solution;
					current_solution = Best_solution(random_solution, number_of_vertex, number_of_edges);//rate random solution
					random_solution[0] = start_solution;
				}
			}
			else {
				if ( random_solution[0] - new_solution > 1)
				{
					random_solution[0] -= new_solution;
					current_solution = Best_solution(random_solution, number_of_vertex, number_of_edges);//rate random solution
					random_solution[0] = start_solution;
				}
			}
		}
	}
	else { current_solution = Best_solution(random_solution, number_of_vertex, number_of_edges); }*/
	
	cout << "Best solution for climbing algorythm=" << best_solution << endl;
}

/*vector<int>void BruteForceVertexCover(int number_of_vertex, int number_of_edges)
{
	//brute force- sprawdzanie czy dany vertex zaznacza wszystkie number_of_edges-krawiendzie jeœli nie to nastêpny
//jeœli ¿aden pojednyñcy nie pokrywa to sprawdzam dla vertex 1 + vertex 2 pó¿niej 1 + 3 itd.
//edge 1-2 == 2-1 !!!

	//OCENA ROZWI¥ZANIA W FUNCJI CELU
	
				//krawêdŸ 1-2==krawêdŸ 2-1 przez co nie doda jej do listy zaznaczonych krawêdzi która musi mieæ w sobie number_of_edges krawêdzi
				//if vertex 1 to wtedy 3!=number_of_edges if 1 i 2 4!=number_of_edges if 1 i 3 5==number_of_edges wiêc wyœlij do best_solution() i tak dalej
	
		//drugi vektor 2d? 1d? sprawdzaj¹cy czy krawêdzie zaznaczone
		// wyœlij kazde rozwi¹zanie ale jeœli jest s³abe to dostanie s³ab¹ ocene w funkcji celu
}*/

tuple<int, int> data_from_file(int& number_of_vertex, int& number_of_edges, int conection_start, int conection_end)
{
	fstream input_data;
	input_data.open("input_data.txt", ios::in);
	if (input_data.good() == false) {
		cout << "Missing file input_data.txt!";
		exit(0);
	}
	input_data >> number_of_vertex;
	input_data >> number_of_edges;
	cout << "Number of vertices(from file):" << number_of_vertex << endl;
	cout << "Number of Edges(from file):" << number_of_edges << endl;

	g.resize(number_of_vertex);
	h.resize(number_of_vertex);
	//memset(v, 0, sizeof(v));
	for (int i = 0; i < number_of_edges; i++) {
		//cout << "Enter the end-points of edge " << i + 1 << " : ";//Enter conections 
		//cin >> a >> b;
		input_data >> conection_start >> conection_end;
		conection_start--; conection_end--;
		g[conection_start].push_back(conection_end);
		g[conection_end].push_back(conection_start);
		
		h[conection_start].push_back(i);
		h[conection_end].push_back(i);

	}
	input_data.close();
	return { number_of_vertex,number_of_edges };
}

void data_to_file(int number_of_vertex, int number_of_edges)
{
	fstream output_data;
	output_data << "-------------------------------------------------------------------------------------------" << endl;
	output_data.open("output_data.txt", ios::out | ios::app);
	output_data << "Numer of vertex: " << number_of_vertex << endl;
	output_data << "Number of edges: " << number_of_edges << endl;
	output_data << "Example: X---edge_nr---Y" << endl;
	for (int x = 0; x < g.size(); x++)//cout conections
	{
		output_data << "NR_X=" << x + 1 << "  NR_Y:";
		for (int y = 0; y < g[x].size(); y++)
		{
			output_data << g[x][y] + 1 << "-";
		}
		output_data << endl;
		output_data << "NR_X=" << x + 1 << " include Edges_NR:";
		for (int y = 0; y < h[x].size(); y++)
		{
			output_data << h[x][y] + 1 << "-";
		}
		output_data << endl;
	}
	output_data <<"-------------------------------------------------------------------------------------------"<<endl;
	output_data.close();
}

tuple<int,int> generate_graph(int& number_of_vertex, int& number_of_edges, int conection_start, int conection_end)
{
	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator

	cout << "enter number of vertex: " ;
	cin >> number_of_vertex;
		if (number_of_vertex<2)
		{
			cout << "number of vertex > 2 !";
			exit(0);
		}
		
	uniform_int_distribution<> random_nr_of_edges(1, number_of_vertex *(number_of_vertex - 1)/2);//no loops
	number_of_edges = random_nr_of_edges(eng);
	cout << "The graph has " << number_of_edges << " edges."<<endl;
	
	g.resize(number_of_vertex);
	h.resize(number_of_vertex);
	int current_start, current_end;
	uniform_int_distribution<> random_edges(1, number_of_vertex);
	for (int x = 0; x < number_of_edges; x++) 
	{
		current_start = random_edges(eng);
		current_end = random_edges(eng);

		if (current_end == current_start)
		{
			if (current_end >= number_of_vertex)
			{
				current_end--;
			}
			if (current_end < number_of_vertex)
			{
				current_end++;
			}
		}
		
		current_start--; current_end--;
		g[current_start].push_back(current_end);
		g[current_end].push_back(current_start);

		h[current_start].push_back(x);
		h[current_end].push_back(x);
	}
	return { number_of_vertex, number_of_edges };
}

int main() {
	//clock_t start = clock();
	//printf("Czas wykonywania: %lu ms\n", clock() - start);
	int number_of_vertex=0, number_of_edges=0, conection_start=0, conection_end=0, minimum_vertex_number=0;

	//data_from_file(number_of_vertex, number_of_edges, conection_start, conection_end);
	generate_graph(number_of_vertex, number_of_edges, conection_start, conection_end);
	
	//cout <<"OOOOOOOOOOOOOOOOOOOOOOOOOOO=="<< g[0][2]<<endl;
	
	Show_conections();

	clock_t start = clock();
	Climbing_Algorythm(number_of_vertex, number_of_edges);
	printf("Czas wykonywania: %lu ms\n", clock() - start);

	data_to_file(number_of_vertex, number_of_edges);
	return 0;
}