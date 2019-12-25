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

void Show_conections(vector<vector<int> >vertex_to_vertexes_conections,vector<vector<int> >vertex_to_edges_conections)
{
	cout << "Example: X---edge_nr---Y" << endl<<endl;
	for (int x = 0; x < vertex_to_vertexes_conections.size(); x++)//cout conections
	{
		cout << "NR_X=" << x + 1 << "  NR_Y:";
		for (int y = 0; y < vertex_to_vertexes_conections[x].size(); y++)
		{
			cout << vertex_to_vertexes_conections[x][y] + 1 << "-";
		}
		cout << endl;
		cout << "NR_X=" << x + 1 << " include Edges_NR:";
		for (int y = 0; y < vertex_to_edges_conections[x].size(); y++)
		{
			cout << vertex_to_edges_conections[x][y] + 1 << "-";
		}
		cout << endl<<endl;
	}
}

void Save_solution_if_the_best(vector<int> solution, float current_solution_score,float& best_solution_score, vector<int >& best_selected_vertexes)
{
	if (current_solution_score > best_solution_score) {
		best_selected_vertexes = solution;
		best_solution_score = current_solution_score;
		cout << "best_solution_score= " << best_solution_score << endl;
	}
	cout << endl;
}

void Show_best_solution(vector<int> best_selected_vertexes,float best_solution_score)
{
	cout << endl<<"------------------------------------------"<<endl;
	cout << "The best found solution:" << endl;
	cout << "Score of best_solution_score= " << best_solution_score << endl;

	cout << "Vertexes in solution:" << endl;
	for (int x = 0; x < best_selected_vertexes.size(); x++)
	{
		cout << x +1<<":" << best_selected_vertexes[x] << endl;
	}
	cout << "------------------------------------------" << endl;
}

void Save_solution_in_tabu_vector(vector<vector<int>>& tabu_solutins, vector<int> random_solution, int& number_of_tabu_solution,int tabu_max_size)
{	
	bool increment = true;
	if (tabu_solutins.size()< tabu_max_size || number_of_tabu_solution==0)
	{
		tabu_solutins.resize(number_of_tabu_solution + 1);
	}else 
	{
		number_of_tabu_solution -= tabu_max_size;
		increment=false;
	}
	if(tabu_solutins[number_of_tabu_solution].size()>0)
	{tabu_solutins[number_of_tabu_solution].clear();}
	for (int x = 0; x < random_solution.size(); x++)
	{
		tabu_solutins[number_of_tabu_solution].push_back(random_solution[x]);
	}
	if (increment==true)
	{
		number_of_tabu_solution++;
	}
}

bool Check_if_solution_is_tabu(vector<vector<int>> tabu_solutins, vector<int> random_solution)
{
	bool the_same_solution = true;
	for (int x=0; x< tabu_solutins.size();x++)
	{
		if (tabu_solutins[x].size() == random_solution.size())
		{
			for (int y = 0; y < random_solution.size(); y++)
			{
				if (tabu_solutins[x][y] != random_solution[y])//if solutions are different
				{
					y == random_solution.size();
					the_same_solution = false;
				}
			}
			if (the_same_solution==true)
			{return true;}
		}
	}
	return false;
}

float Rating_solution(vector<int> solution, int number_of_vertex, int number_of_edges,vector<vector<int>> vertex_to_edges_conections) //solution contains selected vertexes
{
	int edges_in_soluton=0, vertex_in_solution = 0, number_of_duplicats = 0,b;
	float current_solution_score =0;
	vector<int> entered_edges;
	//how many vertexes and edges
	for (int x=0; x < solution.size(); x++)
	{
		cout << "SOLUTIONS:" << solution[x] << endl;
		b = solution[x];
		b--;
		for (int y = 0; y < vertex_to_edges_conections[b].size(); y++)//for evry edge in x 
		{
			entered_edges.push_back(vertex_to_edges_conections[b][y]);
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
	current_solution_score += (float)number_of_vertex / (float)vertex_in_solution;//points for number of  used vertex
	current_solution_score *= ((float)number_of_edges * pow(((float)edges_in_soluton-(float)number_of_duplicats), 2.0));//points for covering edges
	current_solution_score -= (pow(number_of_duplicats, 2.0) * 4);//minus points for duplicats
	current_solution_score /= 1000;

	return current_solution_score;
}
//ODDAJ solution 1
void Climbing_Algorithm(int number_of_vertex, int number_of_edges,float& best_solution_score,vector<int>& best_selected_vertexes, vector<vector<int> >vertex_to_edges_conections,int number_of_iterations,int type_of_algorithm)
{
	int check_range,remembr_vertex;
	float current_solution_score=0;
	vector<int>random_solution;
	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	uniform_int_distribution<> distr(1, number_of_vertex);// define the range
	for (int x = 0; x < distr(eng); x++)
	{
		random_solution.push_back(distr(eng));
	}
	
	if (type_of_algorithm == 1) {//solution 1
		current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);//rate random solution
		Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
		for (int x = 0; x < number_of_iterations; x++)
		{
			//remove element
			if (random_solution.size() > 1)
			{
				remembr_vertex = random_solution.back();
				random_solution.pop_back();
				current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);//rate random solution
				Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
				random_solution.push_back(remembr_vertex);
			}
			//add element
			random_solution.push_back(distr(eng));
			current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
			Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
			random_solution.pop_back();

			for (int y = 0; y < random_solution.size(); y++)
			{
				check_range = random_solution[y];
				check_range += 1;
				if (check_range < number_of_vertex && check_range>0)
				{
					random_solution[y] = random_solution[y] + 1;
					current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
					Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);

					if (random_solution.size() > 1)
					{
						remembr_vertex = random_solution.back();
						random_solution.pop_back();
						current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);//rate random solution
						Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
						random_solution.push_back(remembr_vertex);
					}

					random_solution.push_back(distr(eng));
					current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
					Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
					random_solution.pop_back();

					random_solution[y] = random_solution[y] - 1;
				}
				check_range = random_solution[y];
				check_range -= 1;
				if (check_range < number_of_vertex && check_range>0)
				{

					random_solution[y] = random_solution[y] - 1;
					current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);//rate random solution
					Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);

					if (random_solution.size() > 1)
					{
						remembr_vertex = random_solution.back();
						random_solution.pop_back();
						current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);//rate random solution
						Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
						random_solution.push_back(remembr_vertex);
					}

					random_solution.push_back(distr(eng));
					current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);//rate random solution
					Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
					random_solution.pop_back();

					random_solution[y] = random_solution[y] + 1;
				}
				if (random_solution == best_selected_vertexes)
				{
					return;
				}
				random_solution = best_selected_vertexes;
			}
		}
	}

	if (type_of_algorithm == 2) {//solution 2
			int vertex_to_change, vertex_new_value, puls_or_minus, permissible_number_of_worse_solutions, current_number_of_worse_solutions=0;
			cout << "Permissible number of worse solutions: ";
			cin >> permissible_number_of_worse_solutions;
			cout << endl;
			uniform_int_distribution<> val_vertex(1, number_of_vertex);
			uniform_int_distribution<> plus_minus(1,2);
			current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);//rate random solution
			Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);

			for (int a = number_of_iterations; a > 0; a--)
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
						current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
						Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
					}else {//add element to solution
						random_solution.push_back(vertex_new_value);
						current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
						Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
					}
				}
				if (puls_or_minus == 2)
				{
					check_range = random_solution[vertex_to_change];
					check_range -= vertex_new_value;
					if (check_range < number_of_vertex && check_range>0)
					{
						random_solution[vertex_to_change] -= vertex_new_value;
						current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);//rate random solution
						Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
					}else {//delete element from solution
						if (random_solution.size()>1) {
							random_solution.pop_back();
							current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);//rate random solution
							Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
						}
					}
				}
				if (current_solution_score >= best_solution_score)
				{
					current_number_of_worse_solutions=0;
				}
				if (current_solution_score < best_solution_score)
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
//Sprawdz i ODDAJ
void Tabu_Algorithm(int number_of_vertex, int number_of_edges, float& best_solution_score, vector<int>& best_selected_vertexes, vector<vector<int> >vertex_to_edges_conections,int number_of_iterations,int tabu_max_size)
{
	int start_solution, check_range, remembr_vertex, number_of_tabu_solution=0 ;
	float current_solution_score = 0;
	vector<int>random_solution;
	vector<vector<int> >tabu_solutins;//vector for tabu solutions, x---number of tabu solution  y---vertexes in solution(solution nr x)

	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	uniform_int_distribution<> distr(1, number_of_vertex);// define the range
	for (int x = 0; x < distr(eng); x++)
	{
		random_solution.push_back(distr(eng));
	}

	current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);//rate random solution
	Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
	
	Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size);//add random_solution to tabu vector
	
	for (int x = 0; x < number_of_iterations; x++)
	{
		//remove element
		if (random_solution.size() > 1)
		{
			remembr_vertex = random_solution.back();
			random_solution.pop_back();
			if (false == Check_if_solution_is_tabu(tabu_solutins, random_solution))
			{
				current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
				Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
				Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size);
			}
			random_solution.push_back(remembr_vertex);
			
		}
		//add element
		random_solution.push_back(distr(eng));
		if (false == Check_if_solution_is_tabu(tabu_solutins, random_solution))
		{
			current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
			Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
			Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size);
		}
		random_solution.pop_back();

		for (int y = 0; y < random_solution.size(); y++)
		{
			check_range = random_solution[y];
			check_range += 1;
			if (check_range < number_of_vertex && check_range>0)
			{
				random_solution[y] = random_solution[y] + 1;
				if (false == Check_if_solution_is_tabu(tabu_solutins, random_solution))
				{
					current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
					Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
					Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size);
				}

				if (random_solution.size() > 1)
				{
					remembr_vertex = random_solution.back();
					random_solution.pop_back();
					if (false == Check_if_solution_is_tabu(tabu_solutins, random_solution))
					{
						current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
						Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
						Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size);
					}
					random_solution.push_back(remembr_vertex);
				}

				random_solution.push_back(distr(eng));
				if (false == Check_if_solution_is_tabu(tabu_solutins, random_solution))
				{
					current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
					Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
					Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size);
				}
				random_solution.pop_back();

				random_solution[y] = random_solution[y] - 1;
			}
			check_range = random_solution[y];
			check_range -= 1;
			if (check_range < number_of_vertex && check_range>0)
			{
				random_solution[y] = random_solution[y] - 1;
				if (false == Check_if_solution_is_tabu(tabu_solutins, random_solution))
				{
					current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
					Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
					Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size);
				}

				if (random_solution.size() > 1)
				{
					remembr_vertex = random_solution.back();
					random_solution.pop_back();
					if (false == Check_if_solution_is_tabu(tabu_solutins, random_solution))
					{
						current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
						Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
						Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size);
					}
					random_solution.push_back(remembr_vertex);
				}

				random_solution.push_back(distr(eng));
				if (false == Check_if_solution_is_tabu(tabu_solutins, random_solution))
				{
					current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
					Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
					Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size);
				}
				random_solution.pop_back();

				random_solution[y] = random_solution[y] + 1;
			}
			if (random_solution == best_selected_vertexes)
			{
				return;
			}
			random_solution = best_selected_vertexes;
		}
	}
}
//ODDAJ
void Brute_Force_2(vector<int> all_vertexes, int reqLen, int start, int currLen, vector<bool> check, int len, int number_of_edges,float& best_solution, vector<int>& best_selected_vertexes,vector<vector<int>>& vertex_to_edges_conections)
{
	float current_solution_score;
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
		current_solution_score=Rating_solution(x_solution, len, number_of_edges, vertex_to_edges_conections);//rate another solution
		Save_solution_if_the_best(x_solution, current_solution_score, best_solution, best_selected_vertexes);
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
	Brute_Force_2(all_vertexes, reqLen, start + 1, currLen + 1, check, len, number_of_edges, best_solution,best_selected_vertexes,vertex_to_edges_conections);
	// Second is, we don't select it, means put false in check[] and only start incremented.
	check[start] = false;
	Brute_Force_2(all_vertexes, reqLen, start + 1, currLen, check, len, number_of_edges, best_solution,best_selected_vertexes,vertex_to_edges_conections);
}

void Brute_Force_1(int number_of_vertex, int number_of_edges, float& best_solution_score, vector<int>& best_selected_vertexes, vector<vector<int>>& vertex_to_edges_conections)
{
	int i;
	vector<bool>check;
	check.resize(number_of_vertex);
	float current_solution_score = 0;
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
		Brute_Force_2(all_vertexes, i, 0, 0, check, number_of_vertex, number_of_edges, best_solution_score,best_selected_vertexes,vertex_to_edges_conections);
	}
}

tuple<int, int> data_from_file(int& number_of_vertex, int& number_of_edges, int conection_start, int conection_end, vector<vector<int>>& vertex_to_vertexes_conections, vector<vector<int>>& vertex_to_edges_conections)
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

	vertex_to_vertexes_conections.resize(number_of_vertex);
	vertex_to_edges_conections.resize(number_of_vertex);
	//memset(v, 0, sizeof(v));
	for (int i = 0; i < number_of_edges; i++) {
		//cout << "Enter the end-points of edge " << i + 1 << " : ";//Enter conections 
		//cin >> a >> b;
		input_data >> conection_start >> conection_end;
		conection_start--; conection_end--;
		vertex_to_vertexes_conections[conection_start].push_back(conection_end);
		vertex_to_vertexes_conections[conection_end].push_back(conection_start);
		
		vertex_to_edges_conections[conection_start].push_back(i);
		vertex_to_edges_conections[conection_end].push_back(i);

	}
	input_data.close();
	return { number_of_vertex,number_of_edges };
}

void simple_graph_visualization_to_file(int number_of_vertex, int number_of_edges, vector<vector<int>> vertex_to_vertexes_conections, vector<vector<int>> vertex_to_edges_conections)
{
	fstream output_data;
	output_data.open("output_data.txt", ios::out | ios::app);
	output_data << "-------------------------------------------------------------------------------------------" << endl;
	output_data << "Numer of vertex: " << number_of_vertex << endl;
	output_data << "Number of edges: " << number_of_edges << endl;
	output_data << "Example: X---edge_nr---Y" << endl;
	for (int x = 0; x < vertex_to_vertexes_conections.size(); x++)//cout conections
	{
		output_data << "NR_X=" << x + 1 << "  NR_Y:";
		for (int y = 0; y < vertex_to_vertexes_conections[x].size(); y++)
		{
			output_data << vertex_to_vertexes_conections[x][y] + 1 << "-";
		}
		output_data << endl;
		output_data << "NR_X=" << x + 1 << " include Edges_NR:";
		for (int y = 0; y < vertex_to_edges_conections[x].size(); y++)
		{
			output_data << vertex_to_edges_conections[x][y] + 1 << "-";
		}
		output_data << endl;
	}
	output_data <<"-------------------------------------------------------------------------------------------"<<endl;
	output_data.close();
}
//Dokoñcz
void data_for_graphviz(int number_of_vertex, int number_of_edges, vector<vector<int>> vertex_to_vertexes_conections, vector<vector<int>> vertex_to_edges_conections, vector<int> best_selected_vertexes)
{
	bool ala = false, sasaas=false;
	fstream used_graph_data;
	fstream best_solution_graph;
	used_graph_data.open("used_graph_data.txt", ios::out | ios::trunc);
	best_solution_graph.open("best_solution_graph.txt", ios::out | ios::trunc);
	used_graph_data << "graph used{" << endl;
	for (int x = 0; x < vertex_to_vertexes_conections.size(); x++)
	{
		for (int y = 0; y < vertex_to_vertexes_conections[x].size(); y++)
		{
			if (vertex_to_vertexes_conections[x][y] > x || x == 0)
			{
				used_graph_data << x + 1 << " -- " << vertex_to_vertexes_conections[x][y] + 1 << endl;//dodaj nr krawendzi
			}
		}
	}
	used_graph_data << "}";
	best_solution_graph << "graph best{" << endl;
	for (int x = 0; x < vertex_to_vertexes_conections.size(); x++)
	{
		for (int y = 0; y < vertex_to_vertexes_conections[x].size(); y++)
		{
			if (vertex_to_vertexes_conections[x][y] > x || x == 0)
			{//nie zapisuje wszystkich zaznaczonych krawendzi
				if(find(best_selected_vertexes.begin(), best_selected_vertexes.end(), vertex_to_vertexes_conections[x][y]) == best_selected_vertexes.end())//if element don't exist
				{
					best_solution_graph << x + 1 << " -- " << vertex_to_vertexes_conections[x][y] + 1 << endl;
				}else{
					best_solution_graph << "S" << x + 1 << " -- " << vertex_to_vertexes_conections[x][y] + 1 << " [color = \"red\"]" << endl;
				}
			}
		}
	}
	best_solution_graph << "}";
	used_graph_data.close();
	best_solution_graph.close();
}

tuple<int, int> generate_graph(int& number_of_vertex, int& number_of_edges, int conection_start, int conection_end, vector<vector<int>>& vertex_to_vertexes_conections, vector<vector<int>>& vertex_to_edges_conections)
{
	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator

	uniform_int_distribution<> random_nr_of_edges(1, number_of_vertex * (number_of_vertex - 1) / 2);//no loops
	number_of_edges = random_nr_of_edges(eng);
	cout << "The graph has " << number_of_edges << " edges." << endl;

	vertex_to_vertexes_conections.resize(number_of_vertex);
	vertex_to_edges_conections.resize(number_of_vertex);
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
		if (find(vertex_to_vertexes_conections[current_start].begin(), vertex_to_vertexes_conections[current_start].end(), current_end) == vertex_to_vertexes_conections[current_start].end())//if element don't exist
		{
			vertex_to_vertexes_conections[current_start].push_back(current_end);
			vertex_to_vertexes_conections[current_end].push_back(current_start);

			vertex_to_edges_conections[current_start].push_back(x);
			vertex_to_edges_conections[current_end].push_back(x);
		}else { x--;}
	}
	return { number_of_vertex, number_of_edges };
}

void Make_stat(int number_of_vertex, int number_of_edges, int  conection_start, int conection_end, vector<vector<int> > vertex_to_vertexes_conections, vector<vector<int> > vertex_to_edges_conections, vector<int> solution, vector<int> best_selected_vertexes, float best_solution_score,int number_of_iterations,int type_of_algorithm,int tabu_max_size)
{
	int nr_of_algorithm_iterations = 25;

	type_of_algorithm = 1;
	for (int x = 0; x < nr_of_algorithm_iterations; x++)
	{
		vertex_to_vertexes_conections.clear(), vertex_to_edges_conections.clear(), solution.clear(), best_selected_vertexes.clear();
		best_solution_score = 0;
		number_of_vertex = x + 2;
		number_of_iterations = x + 1;
		tabu_max_size = x + 2;

		generate_graph(number_of_vertex, number_of_edges, conection_start, conection_end, vertex_to_vertexes_conections, vertex_to_edges_conections);
	
		clock_t start = clock();
		Climbing_Algorithm(number_of_vertex, number_of_edges, best_solution_score, best_selected_vertexes, vertex_to_edges_conections, number_of_iterations, type_of_algorithm);
		fstream Climbing_Algorithm_stat;
		Climbing_Algorithm_stat.open("Climbing_Algorithm_stat.txt", ios::out | ios::app);
		Climbing_Algorithm_stat << "-------------------------------------------------------------------------------------------" << endl;
		Climbing_Algorithm_stat << "Numer of vertex: " << number_of_vertex << endl;
		Climbing_Algorithm_stat << "Time= " << clock() - start << endl;
		Climbing_Algorithm_stat << "Score= " << best_solution_score << endl;
		Climbing_Algorithm_stat << "-------------------------------------------------------------------------------------------" << endl;
		Climbing_Algorithm_stat.close();

		solution.clear(), best_selected_vertexes.clear();
		best_solution_score = 0;

		start = clock();
		Tabu_Algorithm(number_of_vertex, number_of_edges, best_solution_score, best_selected_vertexes, vertex_to_edges_conections, number_of_iterations, tabu_max_size);
		fstream Tabu_Algorithm_stat;
		Tabu_Algorithm_stat.open("Tabu_Algorithm_stat.txt", ios::out | ios::app);
		Tabu_Algorithm_stat << "-------------------------------------------------------------------------------------------" << endl;
		Tabu_Algorithm_stat << "Numer of vertex: " << number_of_vertex << endl;
		Tabu_Algorithm_stat << "Time= " << clock() - start << endl;
		Tabu_Algorithm_stat << "Score= " << best_solution_score << endl;
		Tabu_Algorithm_stat << "-------------------------------------------------------------------------------------------" << endl;
		Tabu_Algorithm_stat.close();

		if (x < 8) 
		{
			solution.clear(), best_selected_vertexes.clear();
			best_solution_score = 0;

			start = clock();
			Brute_Force_1(number_of_vertex, number_of_edges, best_solution_score, best_selected_vertexes, vertex_to_edges_conections);
			fstream Brute_Force_stat;
			Brute_Force_stat.open("Brute_Force_stat.txt", ios::out | ios::app);
			Brute_Force_stat << "-------------------------------------------------------------------------------------------" << endl;
			Brute_Force_stat << "Numer of vertex: " << number_of_vertex << endl;
			Brute_Force_stat << "Time= " << clock() - start << endl;
			Brute_Force_stat << "Score= " << best_solution_score << endl;
			Brute_Force_stat << "-------------------------------------------------------------------------------------------" << endl;
			Brute_Force_stat.close();
		}
	}
}


int main(){
	int number_of_vertex = 0, number_of_edges = 0, conection_start = 0, conection_end = 0, minimum_vertex_number = 0, menu = 4, graph_select = 0, number_of_iterations=0, type_of_algorithm=0, tabu_max_size=0;
	vector<vector<int> > vertex_to_vertexes_conections,vertex_to_edges_conections;// vertex_conections= x-vertex_1 y-vertex_2, vertex_to_edges_conections= x-vertex y-number of edge
	vector<int >solution, best_selected_vertexes;
	float best_solution_score = 0;
	//dodaj liczbe wywo³añ a=funkcji celu
	

	for (menu; menu != 0; )
	{
		if (menu == 4)
		{
			vertex_to_vertexes_conections.clear(), vertex_to_edges_conections.clear(), solution.clear(), best_selected_vertexes.clear();
			graph_select = 0, best_solution_score=0;
			cout << endl;
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
				data_from_file(number_of_vertex, number_of_edges, conection_start, conection_end, vertex_to_vertexes_conections, vertex_to_edges_conections);
			}
			if (graph_select == 2)
			{
				
				for (number_of_vertex = 0;number_of_vertex < 2;)
				{
					cout << "enter number of vertex: ";
					cin >> number_of_vertex;
					if (number_of_vertex < 2)
					{cout << "number of vertex > 2 !"<<endl;}
				}
				generate_graph(number_of_vertex, number_of_edges, conection_start, conection_end, vertex_to_vertexes_conections, vertex_to_edges_conections);
			}

			Show_conections(vertex_to_vertexes_conections, vertex_to_edges_conections);
		}
		cout<<"------------------------------------------" << endl;
		cout << "Select algorithm:                        |" << endl << "1 --- Climbing                           |" << endl << "2 --- Tabu                               |" << endl << "3 --- Brute Force                        |" << endl << "4 --- Select new graph                   |" << endl << "5 --- Save simple graph visualization    |" << endl << "6 --- Save graph for graphviz            |" << endl << "0 --- exit                               |" << endl; ;
		cout << "------------------------------------------" << endl;
		cin >> menu;
		cout<< endl;

		if (menu == 1)
		{
			for (type_of_algorithm; type_of_algorithm < 1 || type_of_algorithm > 2;) {
				cout << "Select type of algorithm" << endl << "1 --- next solution from surrounding" << endl << "2 --- random solution from surrounding" << endl;
				cin >> type_of_algorithm;
				if (type_of_algorithm < 1 || type_of_algorithm > 2)
				{
					cout << endl << "Wrong nuber" << endl;
				}
			}
			cout << "Select number of iterations: ";
			cin >> number_of_iterations;
			cout << endl;
			clock_t start = clock();
			Climbing_Algorithm(number_of_vertex, number_of_edges, best_solution_score, best_selected_vertexes, vertex_to_edges_conections, number_of_iterations, type_of_algorithm);
			printf("Czas wykonywania: %lu ms\n", clock() - start);
		}
		if (menu == 2)
		{
			cout << "Select number of iterations: ";
			cin >> number_of_iterations;
			cout << endl;
			cout << "Select size of tabu list: ";
			cin >> tabu_max_size;
			cout << endl;
			clock_t start = clock();
			Tabu_Algorithm(number_of_vertex, number_of_edges, best_solution_score, best_selected_vertexes, vertex_to_edges_conections, number_of_iterations, tabu_max_size);
			printf("Czas wykonywania: %lu ms\n", clock() - start);
		}
		if (menu == 3)
		{
			clock_t start = clock();
			Brute_Force_1(number_of_vertex, number_of_edges, best_solution_score, best_selected_vertexes, vertex_to_edges_conections);
			printf("Czas wykonywania: %lu ms\n", clock() - start);
		}
		if (menu == 1 || menu == 2 || menu == 3)
		{
			Show_best_solution(best_selected_vertexes, best_solution_score);
			best_solution_score = 0;
		}
		if (menu == 5)
		{
			simple_graph_visualization_to_file(number_of_vertex, number_of_edges, vertex_to_vertexes_conections, vertex_to_edges_conections);
		}
		if (menu == 6)
		{
			data_for_graphviz(number_of_vertex, number_of_edges, vertex_to_vertexes_conections, vertex_to_edges_conections, best_selected_vertexes);
		}
	}
	Make_stat(number_of_vertex, number_of_edges, conection_start, conection_end, vertex_to_vertexes_conections,vertex_to_edges_conections,solution,best_selected_vertexes, best_solution_score, number_of_iterations, type_of_algorithm, tabu_max_size);
	return 0;
}