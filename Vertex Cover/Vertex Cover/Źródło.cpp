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

vector<vector<int> > g;//vector for vertex conections, x-vertex_1 y-vertex_2
vector<vector<int> > h;// x-vertex y-number of edge
vector<int >solution;
vector<int >best_selected_vertexes;
float best_solution=0;

void Show_conections()
{
	cout << "Example: X---edge_nr---Y" << endl<<endl;
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
		cout << endl<<endl;
	}
}

void Best_solution()//Show best solution
{
	cout << "best_solution= " << best_solution << endl;

	for (int x = 0; x < best_selected_vertexes.size(); x++)
	{
		cout << "SOLUTIONS:" << best_selected_vertexes[x] << endl;
	}
}

float Rating_solution(vector<int> solution, int number_of_vertex, int number_of_edges) //solution contains selected vertexes
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

	cout << "V=" << vertex_in_solution << " E=" << edges_in_soluton<<" D="<< number_of_duplicats <<endl<<endl;
	current_solution += (float)number_of_vertex / (float)vertex_in_solution;//points for number of  used vertex
	current_solution *= ((float)number_of_edges * pow(((float)edges_in_soluton-(float)number_of_duplicats), 2.0));//points for covering edges
	current_solution -= (pow(number_of_duplicats, 2.0) * 4);//minus points for duplicats
	current_solution /= 1000;
	if (current_solution > best_solution) {
		best_selected_vertexes = solution;
		best_solution= current_solution;
		cout << "best_solution= " << best_solution << endl;
	}
	return current_solution;
}

void Climbing_Algorithm(int number_of_vertex, int number_of_edges)
{
	int type_of_algorithm=0, range;
	float current_solution=0;
	vector<int>random_solution;
	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	uniform_int_distribution<> distr(1, number_of_vertex);// define the range
	for (int x = 0; x < distr(eng); x++)
	{
		random_solution.push_back(distr(eng));
	}

	for (type_of_algorithm; type_of_algorithm < 1 || type_of_algorithm > 2;) {
		cout << "Select type of algorithm"<<endl<<"1 or 2"<<endl;
		cin >> type_of_algorithm;
		if (type_of_algorithm < 1 || type_of_algorithm > 2)
		{
			cout <<endl<< "Wrong nuber"<<endl;
		}
	}

	cout << "Select range of algorythm: ";
	cin >> range;
	cout << endl;

	if (type_of_algorithm == 1) {//solution 1
		current_solution = Rating_solution(random_solution, number_of_vertex, number_of_edges);//rate random solution
		for (int x = 0; x < range; x++)
		{
			if (current_solution == best_solution)
			{
				if (random_solution.size()>x)
				{ 
					if (random_solution[x] < number_of_vertex)
					{
						random_solution[x] = random_solution[x] + 1;
						current_solution = Rating_solution(random_solution, number_of_vertex, number_of_edges);//rate random solution
						if (current_solution < best_solution)
						{
							return;
						}
						random_solution[x] = random_solution[x] - 1;
					}
					if (random_solution[x] > 1)
					{
						random_solution[x] = random_solution[x] - 1;
						current_solution = Rating_solution(random_solution, number_of_vertex, number_of_edges);//rate random solution
						if (current_solution < best_solution)
						{
							return;
						}
						random_solution[x] = random_solution[x] + 1;
					}
				}
			}
			if (current_solution < best_solution)
			{
				return;
			}
		}
	}

	if (type_of_algorithm == 2) {//solution 2
			int new_solution, start_solution,vertex_to_change, vertex_new_value, max_posible_range, check_range, puls_or_minus, permissible_number_of_worse_solutions, current_number_of_worse_solutions=0;
			cout << "Permissible number of worse solutions: ";
			cin >> permissible_number_of_worse_solutions;
			cout << endl;
			uniform_int_distribution<> val_vertex(1, number_of_vertex);
			uniform_int_distribution<> plus_minus(1,2);
			current_solution = Rating_solution(random_solution, number_of_vertex, number_of_edges);//rate random solution

			for (int a = range; a > 0; a--)
			{
				uniform_int_distribution<> nr_vertex(0, random_solution.size() - 1);
				vertex_to_change = nr_vertex(eng);
				vertex_new_value = val_vertex(eng);
				puls_or_minus = plus_minus(eng);
				
				if (puls_or_minus == 1)
				{
					check_range = random_solution[vertex_to_change];
					check_range += vertex_new_value;
					if (check_range < number_of_vertex && check_range>0)
					{
						random_solution[vertex_to_change] += vertex_new_value;
						current_solution= Rating_solution(random_solution, number_of_vertex, number_of_edges);//rate random solution
					}else {//add element to solution
						random_solution.push_back(vertex_new_value);
						current_solution= Rating_solution(random_solution, number_of_vertex, number_of_edges);//rate random solution
					}
				}
				if (puls_or_minus == 2)
				{
					check_range = random_solution[vertex_to_change];
					check_range -= vertex_new_value;
					if (check_range < number_of_vertex && check_range>0)
					{
						random_solution[vertex_to_change] -= vertex_new_value;
						current_solution= Rating_solution(random_solution, number_of_vertex, number_of_edges);//rate random solution
					}else {//delete element from solution
						if (random_solution.size()>1) {
							random_solution.pop_back();
							current_solution= Rating_solution(random_solution, number_of_vertex, number_of_edges);//rate random solution
						}
					}
				}
				if (current_solution >= best_solution)
				{
					current_number_of_worse_solutions=0;
				}
				if (current_solution < best_solution)
				{
					current_number_of_worse_solutions ++;
				}
				if (current_number_of_worse_solutions == permissible_number_of_worse_solutions)
				{
					a = 0;
				}
			}
	}
}
//Popraw
void Tabu_Algorithm(int number_of_vertex, int number_of_edges)
{
	int range,start_solution;
	vector<int>random_solution;
	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	uniform_int_distribution<> distr(1, number_of_vertex);// define the range
	for (int x = 0; x < distr(eng); x++)
	{
		random_solution.push_back(distr(eng));
	}
	cout << "Select range of algorythm: ";
	cin >> range;
	cout << endl;

	//wektor na rozwi¹zania tabu
	//wektor na aktualnych sonsiadow + sprawdzanie czy sonsiad nie jest w tabu
	//sprawdzanie wszystkich aktualnych sonsiadow i wybieranie najlepszego, reszta do tabu

	start_solution = random_solution[0];
	
}

void Brute_Force_2(vector<int> all_vertexes, int reqLen, int start, int currLen, vector<bool> check, int len, int number_of_edges)
{
	vector<int>x_solution;
	x_solution.clear();
	// Return if the currLen is more than the required length.
	if (currLen > reqLen)
		return;
	// If currLen is equal to required length then print the sequence.
	else if (currLen == reqLen)
	{
		//cout << "\t";
		for (int i = 0; i < len; i++)
		{
			if (check[i] == true)
			{
				//cout << all_vertexes[i] << " ";
				x_solution.push_back(all_vertexes[i]);
			}
		}
		//cout << "\n";
		Rating_solution(x_solution, len, number_of_edges);//rate another solution
		return;
	}
	// If start equals to len then return since no further element left.
	if (start == len)
	{
		return;
	}
	// For every index we have two options.
	// First is, we select it, means put true in check[] and increment currLen and start.
	check[start] = true;
	Brute_Force_2(all_vertexes, reqLen, start + 1, currLen + 1, check, len, number_of_edges);
	// Second is, we don't select it, means put false in check[] and only start incremented.
	check[start] = false;
	Brute_Force_2(all_vertexes, reqLen, start + 1, currLen, check, len, number_of_edges);
}

void Brute_Force_1(int number_of_vertex, int number_of_edges)
{
	int i;
	vector<bool>check;
	check.resize(number_of_vertex);
	float current_solution = 0;
	vector<int>all_vertexes;
	all_vertexes.resize(number_of_vertex);
	

	// Take the input of the array.
	for (i = 0; i < number_of_vertex; i++)
	{
		all_vertexes[i]=i+1;
		check[i] = false;
	}

	// For each length of sub-array, call the Brute_Force_2
	for (i = 1; i <= number_of_vertex; i++)
	{
		Brute_Force_2(all_vertexes, i, 0, 0, check, number_of_vertex, number_of_edges);
	}
}

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
	cout << "Number of vertexes(from file):" << number_of_vertex << endl;
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

tuple<int, int> generate_graph(int& number_of_vertex, int& number_of_edges, int conection_start, int conection_end)
{
	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator

	cout << "enter number of vertex: ";
	cin >> number_of_vertex;
	if (number_of_vertex < 2)
	{
		cout << "number of vertex > 2 !";
		exit(0);
	}

	uniform_int_distribution<> random_nr_of_edges(1, number_of_vertex * (number_of_vertex - 1) / 2);//no loops
	number_of_edges = random_nr_of_edges(eng);
	cout << "The graph has " << number_of_edges << " edges." << endl;

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
			if (current_end == number_of_vertex)
			{
				current_end--;
			}
			else
			{
				current_end++;
			}
		}

		current_start--; current_end--;
		if (find(g[current_start].begin(), g[current_start].end(), current_end) == g[current_start].end())//if element don't exist
		{
			g[current_start].push_back(current_end);
			g[current_end].push_back(current_start);

			h[current_start].push_back(x);
			h[current_end].push_back(x);
		}else { x--;}
	}
	return { number_of_vertex, number_of_edges };
}


int main(){
	int number_of_vertex = 0, number_of_edges = 0, conection_start = 0, conection_end = 0, minimum_vertex_number = 0, menu = -1, graph_select = 0;

	cout << "Select graph" << endl << "1 --- From file" << endl << "2 --- Generate" << endl;
	for (graph_select; graph_select < 1 || graph_select > 2;)
	{
		cin >> graph_select;
		if (graph_select < 1 || graph_select > 2)
		{
			cout << "Wrong number" << endl;
		}
	}
	cout << endl;
	if (graph_select == 1)
	{
		data_from_file(number_of_vertex, number_of_edges, conection_start, conection_end);
	}
	if (graph_select == 2)
	{
		generate_graph(number_of_vertex, number_of_edges, conection_start, conection_end);
	}

	Show_conections();

	for (menu; menu != 0; )
	{
		cout << endl;
		cout << "Select algorithm:" << endl << "1 --- Climbing" << endl << "2 --- Tabu" << endl <<  "3 --- Brute Force" << endl << "0 --- exit"<<endl;
		cin >> menu;
		cout << endl;

		if (menu == 1)
		{
			clock_t start = clock();
			Climbing_Algorithm(number_of_vertex, number_of_edges);
			printf("Czas wykonywania: %lu ms\n", clock() - start);
		}
		if (menu == 2)
		{
			clock_t start = clock();
			Tabu_Algorithm(number_of_vertex, number_of_edges);
			printf("Czas wykonywania: %lu ms\n", clock() - start);
		}
		if (menu == 3)
		{
			clock_t start = clock();
			Brute_Force_1(number_of_vertex, number_of_edges);
			printf("Czas wykonywania: %lu ms\n", clock() - start);
		}
		Best_solution();
	}

	//data_to_file(number_of_vertex, number_of_edges);
	return 0;
}