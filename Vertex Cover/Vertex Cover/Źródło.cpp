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
	if (current_solution_score > best_solution_score){
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

void Save_solution_in_tabu_vector(vector<vector<int>>& tabu_solutins, vector<int> random_solution, int& number_of_tabu_solution,int tabu_max_size, bool clear_oldest_solution)
{	
	if (number_of_tabu_solution == tabu_max_size)
	{
		clear_oldest_solution = true;
		number_of_tabu_solution = 0;
	}

	if (clear_oldest_solution == true)
	{
		tabu_solutins[number_of_tabu_solution].clear();
	}

	for (int x = 0; x < random_solution.size(); x++)
	{
		tabu_solutins[number_of_tabu_solution].push_back(random_solution[x]);
	}
	number_of_tabu_solution++;
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
	int edges_in_soluton=0, vertex_in_solution = 0, doubled_edges = 0, doubled_vertexes=0,b;
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

	//how meny duplicats(edges)
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
			doubled_edges += current_number_of_duplicats-1;
		}
	}

	//how meny duplicats(vertexes)
	for (int x = 0; x < solution.size(); x++)
	{
		int current_number_of_doubled_vertexes = 0;
		int current_vertex_in_solution = solution[x];

		for (int y = 0; y < solution.size(); y++)
		{
			if (current_vertex_in_solution == solution[y])
			{
				current_number_of_doubled_vertexes++;
			}
		}
		if (current_number_of_doubled_vertexes > 1)
		{
			doubled_vertexes += current_number_of_doubled_vertexes - 1;
		}
	}

	cout << "V=" << vertex_in_solution << " E=" << edges_in_soluton<<" E-D="<< doubled_edges <<" V-D="<< doubled_vertexes <<endl;

	if ((edges_in_soluton - doubled_edges) == number_of_edges)//if all edges covered
	{
		if ((number_of_vertex - vertex_in_solution)>=0 && (edges_in_soluton - doubled_edges)>0 && doubled_vertexes==0)
		{
			current_solution_score += pow(((number_of_vertex - vertex_in_solution) + 1), 4);//points for number of  used vertex
			current_solution_score *= (float)number_of_edges*pow(((float)edges_in_soluton - (float)doubled_edges), 2.0);//points for covering edges
		}
		current_solution_score -= (pow(doubled_edges, 2.0) * 4);//minus points for duplicats
		current_solution_score /= 10000;
	}else 
	{
		if ((number_of_vertex - vertex_in_solution) >= 0 && (edges_in_soluton - doubled_edges) > 0 && doubled_vertexes == 0)
		{
			current_solution_score += pow(((number_of_vertex - vertex_in_solution) + 1), 4);//points for number of  used vertex
			current_solution_score *= (float)number_of_edges * pow(((float)edges_in_soluton - (float)doubled_edges), 2.0);//points for covering edges
		}
		current_solution_score -= (pow(doubled_edges, 2.0) * 4);//minus points for duplicats
		current_solution_score /= 1000000;//two more 0 for lower quality
	}
	return current_solution_score;
}

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

			for (int y = 0; y < random_solution.size(); y++)
			{
				check_range = random_solution[y];
				check_range += 1;
				if (check_range < number_of_vertex && check_range>0)
				{
					random_solution[y] = random_solution[y] + 1;
					current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
					Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);

					if (random_solution.size() > 1)//remove element
					{
						remembr_vertex = random_solution.back();
						random_solution.pop_back();
						current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);//rate random solution
						Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
						random_solution.push_back(remembr_vertex);
					}

					if ((y + 1) < number_of_vertex)//add element
					{
						random_solution.push_back(y + 1);
						current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
						Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
						random_solution.pop_back();
					}

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

					if ((y + 1) < number_of_vertex)//add element
					{
						random_solution.push_back(y + 1);
						current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
						Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
						random_solution.pop_back();
					}

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

void Tabu_Algorithm(int number_of_vertex, int number_of_edges, float& best_solution_score, vector<int>& best_selected_vertexes, vector<vector<int> >vertex_to_edges_conections,int number_of_iterations,int tabu_max_size)
{
	int start_solution, check_range, remembr_vertex, number_of_tabu_solution=0 ;
	float current_solution_score = 0;
	bool clear_oldest_solution = false;
	vector<int>random_solution;
	vector<vector<int> >tabu_solutins;//vector for tabu solutions, x---number of tabu solution  y---vertexes in solution(solution nr x)
	tabu_solutins.resize(tabu_max_size);
	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	uniform_int_distribution<> distr(1, number_of_vertex);// define the range
	for (int x = 0; x < distr(eng); x++)
	{
		random_solution.push_back(distr(eng));
	}

	current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);//rate random solution
	Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
	
	Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size, clear_oldest_solution);//add random_solution to tabu vector
	
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
				Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size, clear_oldest_solution);
			}
			random_solution.push_back(remembr_vertex);
			
		}
		
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
					Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size, clear_oldest_solution);
				}

				if (random_solution.size() > 1)//remove element
				{
					remembr_vertex = random_solution.back();
					random_solution.pop_back();
					if (false == Check_if_solution_is_tabu(tabu_solutins, random_solution))
					{
						current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
						Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
						Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size, clear_oldest_solution);
					}
					random_solution.push_back(remembr_vertex);
				}

				if ((y + 1) < number_of_vertex)//add element
				{
					random_solution.push_back(y + 1);
					if (false == Check_if_solution_is_tabu(tabu_solutins, random_solution))
					{
						current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
						Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
						Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size, clear_oldest_solution);
					}
					random_solution.pop_back();
				}

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
					Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size, clear_oldest_solution);
				}

				if (random_solution.size() > 1)//remove element
				{
					remembr_vertex = random_solution.back();
					random_solution.pop_back();
					if (false == Check_if_solution_is_tabu(tabu_solutins, random_solution))
					{
						current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
						Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
						Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size, clear_oldest_solution);
					}
					random_solution.push_back(remembr_vertex);
				}

				if ((y + 1) < number_of_vertex)//add element
				{
					random_solution.push_back(y + 1);
					if (false == Check_if_solution_is_tabu(tabu_solutins, random_solution))
					{
						current_solution_score = Rating_solution(random_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
						Save_solution_if_the_best(random_solution, current_solution_score, best_solution_score, best_selected_vertexes);
						Save_solution_in_tabu_vector(tabu_solutins, random_solution, number_of_tabu_solution, tabu_max_size, clear_oldest_solution);
					}
					random_solution.pop_back();
				}

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

void Simulated_annealing(int number_of_vertex, int number_of_edges, float& best_solution_score, vector<int>& best_selected_vertexes, vector<vector<int> >vertex_to_edges_conections, int number_of_iterations, int temperature)
{
	int minimum_temperature=0, check_range, vertex_to_change, vertex_new_value, puls_or_minus;
	float current_solution_score = 0, score_of_new_solution = 0, probability_of_change=0;
	vector<int>main_solution, new_solution;
	uniform_int_distribution<> val_vertex(1, number_of_vertex);
	uniform_int_distribution<> plus_minus(1, 2);

	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	uniform_int_distribution<> distr(1, number_of_vertex);// define the range
	for (int x = 0; x < distr(eng); x++)
	{
		main_solution.push_back(distr(eng));
	}

	current_solution_score = Rating_solution(main_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
	Save_solution_if_the_best(main_solution, current_solution_score, best_solution_score, best_selected_vertexes);


	new_solution = main_solution;

	
		for (temperature; temperature > minimum_temperature; temperature--)
		{
			uniform_int_distribution<> nr_vertex(0, new_solution.size() - 1);
			vertex_to_change = nr_vertex(eng);
			vertex_new_value = val_vertex(eng);
			puls_or_minus = plus_minus(eng);

			uniform_real_distribution<> random_u(1, 2);
			uniform_real_distribution<> random_a(0, 1);
			float a = random_a(eng);

			if (puls_or_minus == 1)
			{
				check_range = new_solution[vertex_to_change];
				check_range += vertex_new_value;
				if (check_range < number_of_vertex && check_range>0)
				{
					new_solution[vertex_to_change] += vertex_new_value;
					score_of_new_solution = Rating_solution(new_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
					Save_solution_if_the_best(new_solution, current_solution_score, best_solution_score, best_selected_vertexes);
				}
				else {//add element to solution
					new_solution.push_back(vertex_new_value);
					score_of_new_solution = Rating_solution(new_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);
					Save_solution_if_the_best(new_solution, current_solution_score, best_solution_score, best_selected_vertexes);
				}
			}
			if (puls_or_minus == 2)
			{
				check_range = new_solution[vertex_to_change];
				check_range -= vertex_new_value;
				if (check_range < number_of_vertex && check_range>0)
				{
					new_solution[vertex_to_change] -= vertex_new_value;
					score_of_new_solution = Rating_solution(new_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);//rate random solution
					Save_solution_if_the_best(new_solution, current_solution_score, best_solution_score, best_selected_vertexes);
				}
				else {//delete element from solution
					if (new_solution.size() > 1) {
						new_solution.pop_back();
						score_of_new_solution = Rating_solution(new_solution, number_of_vertex, number_of_edges, vertex_to_edges_conections);//rate random solution
						Save_solution_if_the_best(new_solution, current_solution_score, best_solution_score, best_selected_vertexes);
					}
				}
			}

			if (score_of_new_solution > current_solution_score)
			{
				main_solution = new_solution;
				current_solution_score = score_of_new_solution;
				for (int x = 1; x < temperature; x++)
				{
					a *= a;
				}
				temperature = a;
			}
			else {
				if (score_of_new_solution < current_solution_score)
				{
					probability_of_change = exp(-(score_of_new_solution - current_solution_score) / temperature);

					uniform_real_distribution<> random_u(1, 2);
					float u = random_u(eng);
					if (u < probability_of_change)
					{
						main_solution = new_solution;
						current_solution_score = score_of_new_solution;
						for (int x = 1; x < temperature; x++)
						{
							a *= a;
						}
						temperature = a;
					}
				}
			}
		}
	

}

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

void data_for_graphviz(int number_of_vertex, int number_of_edges, vector<vector<int>> vertex_to_vertexes_conections, vector<vector<int>> vertex_to_edges_conections, vector<int> best_selected_vertexes)
{
	bool slelected_vertex = false;
	int nr_of_edge=0;
	fstream used_graph_data;
	fstream best_solution_graph;
	used_graph_data.open("used_graph_data.txt", ios::out | ios::trunc);
	best_solution_graph.open("best_solution_graph.txt", ios::out | ios::trunc);
	used_graph_data << "graph used{" << endl;
	for (int x = 0; x < vertex_to_vertexes_conections.size(); x++)
	{
		if (vertex_to_vertexes_conections[x].size() == 0)
		{
			used_graph_data << x + 1 << endl;
		}
		for (int y = 0; y < vertex_to_vertexes_conections[x].size(); y++)
		{
			if (vertex_to_vertexes_conections[x][y] > x || x == 0)
			{
				nr_of_edge++;
				used_graph_data << x + 1 << " -- " << vertex_to_vertexes_conections[x][y] + 1 <<"[label="<<nr_of_edge<<"]"<< endl;//nr_of_edge different from vertex_to_edges_conections
			}
		}
	}
	used_graph_data << "}";

	nr_of_edge = 0;

	best_solution_graph << "graph best{" << endl;
	for (int x = 0; x < vertex_to_vertexes_conections.size(); x++)
	{
		if (vertex_to_vertexes_conections[x].size() == 0)
		{
			used_graph_data << x + 1 << endl;
		}
		for (int y = 0; y < vertex_to_vertexes_conections[x].size(); y++)
		{
			if (vertex_to_vertexes_conections[x][y] > x || x == 0)
			{
				nr_of_edge++;
				slelected_vertex = false;
				for (int z = 0; z < best_selected_vertexes.size(); z++) 
				{
					if (vertex_to_vertexes_conections[x][y]+1== best_selected_vertexes[z]&& slelected_vertex == false)
					{
						best_solution_graph << x + 1 << " -- " << vertex_to_vertexes_conections[x][y] + 1 << "[color = \"red\" label=" << nr_of_edge << "]" << endl;
						slelected_vertex = true;
					}
					if ( x + 1 == best_selected_vertexes[z]&& slelected_vertex ==false)
					{
						best_solution_graph <<x + 1 <<" -- " << vertex_to_vertexes_conections[x][y] + 1 << "[color = \"red\" label=" << nr_of_edge << "]" << endl;
						slelected_vertex = true;
					}
				}
				if(slelected_vertex==false)
				{
					best_solution_graph << x + 1 << " -- " << vertex_to_vertexes_conections[x][y] + 1 <<"[label=" << nr_of_edge << "]"<<endl;
				}
			}
		}
	}

	for (int x = 0; x < vertex_to_vertexes_conections.size(); x++)
	{
		for (int y = 0; y < best_selected_vertexes.size(); y++)
		{
			if (best_selected_vertexes[y] == x+1)
			{
				best_solution_graph << x + 1 << "[shape= circle, fillcolor=\"blue\", style=filled]" << endl;
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

void Make_median(vector<float>time_results, vector<float>score_results,float& time_median, float& score_median)
{
	for (int x = 0; x < time_results.size(); x++)
	{
		time_median += time_results[x];
	}
	time_median /= time_results.size();

	for (int x = 0; x < score_results.size(); x++)
	{
		score_median += score_results[x];
	}
	score_median /= score_results.size();
}
//oddaj
void Make_stat(int number_of_vertex, int number_of_edges, int  conection_start, int conection_end, vector<vector<int> > vertex_to_vertexes_conections, vector<vector<int> > vertex_to_edges_conections, vector<int> solution, vector<int> best_selected_vertexes, float best_solution_score,int number_of_iterations,int type_of_algorithm,int tabu_max_size)
{
	int nr_of_algorithm_iterations = 24, iterations_for_median=12, numer_of_edges_in_last_solution = 0;
	float time, time_median, score_median,best_score_for_climbing=0, best_score_for_tabu=0, best_score_for_brute=0;
	vector<float>time_results,score_results;
	fstream Climbing_Algorithm_stat, Tabu_Algorithm_stat, Brute_Force_stat, Data_for_gnuplot, Best_solution_for_algorithm;
	Climbing_Algorithm_stat.open("Climbing_Algorithm_stat.txt", ios::out | ios::trunc);
	Tabu_Algorithm_stat.open("Tabu_Algorithm_stat.txt", ios::out | ios::trunc);
	Brute_Force_stat.open("Brute_Force_stat.txt", ios::out | ios::trunc);
	Data_for_gnuplot.open("Data.dat", ios::out | ios::trunc);
	Data_for_gnuplot.open("Best_solution_for_algorithm.txt", ios::out | ios::trunc);
	Climbing_Algorithm_stat.close();
	Tabu_Algorithm_stat.close();
	Brute_Force_stat.close();
	Data_for_gnuplot.close();
	Best_solution_for_algorithm.close();

	clock_t start = clock();

	type_of_algorithm = 1;
	for (int x = 0; x < nr_of_algorithm_iterations; x++)
	{
		time_results.clear(), score_results.clear();//vertex_to_vertexes_conections.clear(), vertex_to_edges_conections.clear(), solution.clear(), best_selected_vertexes.clear(),
		time_median = 0, score_median = 0;
		best_solution_score = 0;
		number_of_vertex = x + 2;
		number_of_iterations = x + 1;
		tabu_max_size = x + 2;

		for (int x=0;x<= numer_of_edges_in_last_solution;)
		{
			vertex_to_vertexes_conections.clear(), vertex_to_edges_conections.clear(), solution.clear(), best_selected_vertexes.clear(),
			generate_graph(number_of_vertex, number_of_edges, conection_start, conection_end, vertex_to_vertexes_conections, vertex_to_edges_conections);
			x = number_of_edges;
		}
		numer_of_edges_in_last_solution = number_of_edges;
		for (int x = 0; x < iterations_for_median; x++)
		{
			best_solution_score = 0;
			start = clock();
			Climbing_Algorithm(number_of_vertex, number_of_edges, best_solution_score, best_selected_vertexes, vertex_to_edges_conections, number_of_iterations, type_of_algorithm);
			time = clock() - start;
			time_results.push_back(time);
			score_results.push_back(best_solution_score);
			if (best_score_for_climbing < best_solution_score)
			{
				best_score_for_climbing = best_solution_score;
			}
		}
		Make_median(time_results, score_results, time_median, score_median);

		Climbing_Algorithm_stat.open("Climbing_Algorithm_stat.txt", ios::out | ios::app);
		Climbing_Algorithm_stat << "-------------------------------------------------------------------------------------------" << endl;
		Climbing_Algorithm_stat << "Numer of vertex: " << number_of_vertex << endl;
		Climbing_Algorithm_stat << "Time_median= " << time_median << endl;
		Climbing_Algorithm_stat << "Score_median= " << score_median << endl;
		Climbing_Algorithm_stat << "number of edges in graph= " << number_of_edges << endl;
		Climbing_Algorithm_stat << "-------------------------------------------------------------------------------------------" << endl;
		Climbing_Algorithm_stat.close();

		Data_for_gnuplot.open("Data.dat", ios::out | ios::app);
		Data_for_gnuplot << number_of_vertex <<" ";
		Data_for_gnuplot << time_median <<" ";
		Data_for_gnuplot << score_median <<" ";
		Data_for_gnuplot.close();

		solution.clear(), best_selected_vertexes.clear(), time_results.clear(), score_results.clear();
		time_median=0, score_median=0;

		for (int x = 0; x < iterations_for_median; x++)
		{
			best_solution_score = 0;
			start = clock();
			Tabu_Algorithm(number_of_vertex, number_of_edges, best_solution_score, best_selected_vertexes, vertex_to_edges_conections, number_of_iterations, tabu_max_size);
			time = clock() - start;
			time_results.push_back(time);
			score_results.push_back(best_solution_score);
			if (best_score_for_tabu < best_solution_score)
			{
				best_score_for_tabu = best_solution_score;
			}
		}
		Make_median(time_results, score_results, time_median, score_median);

		Tabu_Algorithm_stat.open("Tabu_Algorithm_stat.txt", ios::out | ios::app);
		Tabu_Algorithm_stat << "-------------------------------------------------------------------------------------------" << endl;
		Tabu_Algorithm_stat << "Numer of vertex: " << number_of_vertex << endl;
		Tabu_Algorithm_stat << "Time_median= " << time_median << endl;
		Tabu_Algorithm_stat << "Score_median= " << score_median << endl;
		Tabu_Algorithm_stat << "number of edges in graph= " << number_of_edges << endl;
		Tabu_Algorithm_stat << "-------------------------------------------------------------------------------------------" << endl;
		Tabu_Algorithm_stat.close();

		Data_for_gnuplot.open("Data.dat", ios::out | ios::app);
		Data_for_gnuplot << time_median << " ";
		Data_for_gnuplot << score_median << " ";
		Data_for_gnuplot.close();

		if (x < 9) 
		{
			solution.clear(), best_selected_vertexes.clear(), time_results.clear(), score_results.clear();
			time_median = 0, score_median = 0;

			for (int x = 0; x < iterations_for_median; x++)
			{
				best_solution_score = 0;
				start = clock();
				Brute_Force_1(number_of_vertex, number_of_edges, best_solution_score, best_selected_vertexes, vertex_to_edges_conections);
				time = clock() - start;
				time_results.push_back(time);
				score_results.push_back(best_solution_score);
				if (best_score_for_brute < best_solution_score)
				{
					best_score_for_brute = best_solution_score;
				}
			}
			Make_median(time_results, score_results, time_median, score_median);

			Brute_Force_stat.open("Brute_Force_stat.txt", ios::out | ios::app);
			Brute_Force_stat << "-------------------------------------------------------------------------------------------" << endl;
			Brute_Force_stat << "Numer of vertex: " << number_of_vertex << endl;
			Brute_Force_stat << "Time_median= " << time_median << endl;
			Brute_Force_stat << "Score_median= " << score_median << endl;
			Brute_Force_stat << "number of edges in graph= " << number_of_edges << endl;
			Brute_Force_stat << "-------------------------------------------------------------------------------------------" << endl;
			Brute_Force_stat.close();

			Data_for_gnuplot.open("Data.dat", ios::out | ios::app);
			Data_for_gnuplot << time_median << " ";
			Data_for_gnuplot << score_median << " ";
			Data_for_gnuplot.close();
		}
		Data_for_gnuplot.open("Data.dat", ios::out | ios::app);
		Data_for_gnuplot <<endl;
		Data_for_gnuplot.close();
	}
	/*Best_solution_for_algorithm.open("Best_solution_for_algorithm.txt", ios::out | ios::app);
	Best_solution_for_algorithm << "-------------------------------------------------------------------------------------------" << endl;
	Best_solution_for_algorithm << "Best scors: " << endl;
	Best_solution_for_algorithm << "Climbing: " << best_score_for_climbing << endl;
	Best_solution_for_algorithm << "Tabu= " << best_score_for_tabu << endl;
	Best_solution_for_algorithm << "Brute= " << best_score_for_brute << endl;
	Best_solution_for_algorithm << "-------------------------------------------------------------------------------------------" << endl;
	Best_solution_for_algorithm.close();*/
}
//oddaj
void Finding_best_solution_by_simulated_annealing(int number_of_vertex, int number_of_edges, float best_solution_score, vector<int> best_selected_vertexes, int  conection_start, int conection_end, vector<vector<int> > vertex_to_vertexes_conections, vector<vector<int> > vertex_to_edges_conections)
{
	
	int number_of_iterations= number_of_vertex,temperature= number_of_vertex;
	float best_score_for_brute = 0, best_score_for_annealing = 0, time_annealing,time_brute,annealing_time_table[10];// best_score_for_current_params, current_score_for_params, time, time_median, score_median, minimum_value;
	//vector<float>time_results, score_results;
	fstream  Time_to_find_best_solution_for_annealing; //Parameters_for_Simulated_Annealing
	//Parameters_for_Simulated_Annealing.open("Parameters_for_Simulated_Annealing.txt", ios::out | ios::trunc);
	//Parameters_for_Simulated_Annealing.close();

	Time_to_find_best_solution_for_annealing.open("Time_to_find_best_solution_for_annealing.txt", ios::out | ios::trunc);
	Time_to_find_best_solution_for_annealing.close();


	clock_t start = clock();

	vertex_to_vertexes_conections.clear(), vertex_to_edges_conections.clear(), best_selected_vertexes.clear();
	generate_graph(number_of_vertex, number_of_edges, conection_start, conection_end, vertex_to_vertexes_conections, vertex_to_edges_conections);

	
	start = clock();
	Brute_Force_1(number_of_vertex, number_of_edges, best_solution_score, best_selected_vertexes, vertex_to_edges_conections);
	time_brute = clock() - start;
	best_score_for_brute = best_solution_score;
	/*temperature = number_of_vertex*2;
	number_of_iterations = temperature;
	vector<int>tamp_number;
	vector<int>iterations_number;
	vector<float>time_results_median;
	vector<float>top_scors_for_different_params;
	
	vertex_to_vertexes_conections.clear(), vertex_to_edges_conections.clear(), best_selected_vertexes.clear();
	generate_graph(number_of_vertex, number_of_edges, conection_start, conection_end, vertex_to_vertexes_conections, vertex_to_edges_conections);

	for (int a = 0; a < temperature; temperature--)
	{
		number_of_iterations = temperature;
		for (int b = 0; b < number_of_iterations; number_of_iterations--)
		{
			time_median = 0, score_median = 0;
			best_selected_vertexes.clear(), time_results.clear(), score_results.clear();
			for (int x = 0; x < iterations_for_median; x++)
			{
				best_solution_score = 0;
				start = clock();
				Simulated_annealing(number_of_vertex, number_of_edges, best_solution_score, best_selected_vertexes, vertex_to_edges_conections, number_of_iterations, temperature);
				time = clock() - start;
				time_results.push_back(time);
				score_results.push_back(best_solution_score);
			}
			Make_median(time_results, score_results, time_median, score_median);
			
			if (top_scors_for_different_params.size() == size_of_top_scors)
			{
				/*for (const auto& i : top_scors_for_different_params)
					cout << i << ' ' << endl;
				cout << "------------------" << endl;*/
	/*
				minimum_value = top_scors_for_different_params[0];
				for (int i = 0; i < size_of_top_scors; i++)//find smallest score
				{
					if (top_scors_for_different_params[i] < minimum_value)
						minimum_value = top_scors_for_different_params[i];
				}
				if (minimum_value < score_median)//change smallest value to new score
				{
					for (int x = 0; x < top_scors_for_different_params.size(); x++)
					{
						if (top_scors_for_different_params[x] < minimum_value)
						{
							top_scors_for_different_params[x] = minimum_value;
							tamp_number[x] = temperature;
							iterations_number[x] = number_of_iterations;
							time_results_median[x] = time_median;
							x = top_scors_for_different_params.size();
						}
					}
				}

			}else{
				top_scors_for_different_params.push_back(score_median);
				tamp_number.push_back(temperature);
				iterations_number.push_back(number_of_iterations);
				time_results_median.push_back(time_median);
			}
		}
	}
	for (int x = 0; x < size_of_top_scors; x++)
	{
		Parameters_for_Simulated_Annealing.open("Parameters_for_Simulated_Annealing.txt", ios::out | ios::app);
		Parameters_for_Simulated_Annealing << "-------------------------------------------------------------------------------------------" << endl;
		Parameters_for_Simulated_Annealing << "Numer of vertex: " << number_of_vertex << endl;
		Parameters_for_Simulated_Annealing << "Score_median= " << top_scors_for_different_params[x] << endl;
		Parameters_for_Simulated_Annealing << "Time_median= " << time_results_median[x] << endl;
		Parameters_for_Simulated_Annealing << "Temperature= " << tamp_number[x] << endl;
		Parameters_for_Simulated_Annealing << "Number_of_iterations= " << iterations_number[x] << endl;
		Parameters_for_Simulated_Annealing << "-------------------------------------------------------------------------------------------" << endl;
		Parameters_for_Simulated_Annealing.close();
	}*/

	for (int x = 0; x<10; x++)
	{
		best_score_for_annealing = 0;
		best_solution_score = 0;
		start = clock();
		for (int y = 0; best_score_for_annealing < best_score_for_brute;)
		{
			Simulated_annealing(number_of_vertex, number_of_edges, best_solution_score, best_selected_vertexes, vertex_to_edges_conections, number_of_iterations, temperature);
			best_score_for_annealing = best_solution_score;

		}
		time_annealing = clock() - start;
		annealing_time_table[x] = time_annealing;
	}
	
	Time_to_find_best_solution_for_annealing.open("Time_to_find_best_solution_for_annealing.txt", ios::out | ios::app);
	Time_to_find_best_solution_for_annealing << "-------------------------------------------------------------------------------------------" << endl;
	Time_to_find_best_solution_for_annealing << "Numer of vertex: " << number_of_vertex << endl;
	Time_to_find_best_solution_for_annealing << "Time to find best solution: " << number_of_vertex << endl;
	for (int x = 0; x < 10; x++) 
	{
		Time_to_find_best_solution_for_annealing << "annealing= " << annealing_time_table[x] << "ms" << endl;
	}
	Time_to_find_best_solution_for_annealing << "brute= " << time_brute <<"ms"<< endl;
	Time_to_find_best_solution_for_annealing << "-------------------------------------------------------------------------------------------" << endl;
	Time_to_find_best_solution_for_annealing.close();
}

int main(){
	int number_of_vertex = 0, number_of_edges = 0, conection_start = 0, conection_end = 0, minimum_vertex_number = 0, menu = 5, graph_select = 0, number_of_iterations=0, type_of_algorithm=0, tabu_max_size=0, temperature=0, iterations_for_median, size_of_top_scors;
	vector<vector<int> > vertex_to_vertexes_conections,vertex_to_edges_conections;// vertex_conections= x-vertex_1 y-vertex_2, vertex_to_edges_conections= x-vertex y-number of edge
	vector<int >solution, best_selected_vertexes;
	float best_solution_score = 0;

	for (menu; menu != 0; )
	{
		if (menu == 5)
		{
			vertex_to_vertexes_conections.clear(), vertex_to_edges_conections.clear(), solution.clear(), best_selected_vertexes.clear();
			graph_select = 0, best_solution_score = 0;
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

				for (number_of_vertex = 0; number_of_vertex < 2;)
				{
					cout << "enter number of vertex: ";
					cin >> number_of_vertex;
					if (number_of_vertex < 2)
					{
						cout << "number of vertex > 2 !" << endl;
					}
				}
				generate_graph(number_of_vertex, number_of_edges, conection_start, conection_end, vertex_to_vertexes_conections, vertex_to_edges_conections);
			}

			Show_conections(vertex_to_vertexes_conections, vertex_to_edges_conections);
		}
		cout << "------------------------------------------" << endl;
		cout << "Select algorithm:                        |" << endl << "1 --- Climbing                           |" << endl << "2 --- Tabu                               |" << endl << "3 --- Simulated annealing                |" << endl << "4 --- Brute Force                        |" << endl << "5 --- Select new graph                   |" << endl << "6 --- Save simple graph visualization    |" << endl << "7 --- Save graph for graphviz            |" << endl << "8 --- Create algorithms statistics       |" <<endl<<"9 --- Find best solution, sim. annealing |"<< endl << "0 --- exit                               |" << endl; ;
		cout << "------------------------------------------" << endl;
		cin >> menu;
		cout << endl;

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
			type_of_algorithm = 0;
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
			cout << "Select number of iterations: ";
			cin >> number_of_iterations;
			cout << endl;
			cout << "Select temperature: ";
			cin >> temperature;
			cout << endl;
			clock_t start = clock();
			Simulated_annealing(number_of_vertex, number_of_edges, best_solution_score, best_selected_vertexes, vertex_to_edges_conections, number_of_iterations, temperature);
			printf("Czas wykonywania: %lu ms\n", clock() - start);
		}
		if (menu == 4)
		{
			clock_t start = clock();
			Brute_Force_1(number_of_vertex, number_of_edges, best_solution_score, best_selected_vertexes, vertex_to_edges_conections);
			printf("Czas wykonywania: %lu ms\n", clock() - start);
		}
		if (menu == 1 || menu == 2 || menu == 3 || menu == 4)
		{
			Show_best_solution(best_selected_vertexes, best_solution_score);
			best_solution_score = 0;
		}
		if (menu == 6)
		{
			simple_graph_visualization_to_file(number_of_vertex, number_of_edges, vertex_to_vertexes_conections, vertex_to_edges_conections);
		}
		if (menu == 7)
		{
			data_for_graphviz(number_of_vertex, number_of_edges, vertex_to_vertexes_conections, vertex_to_edges_conections, best_selected_vertexes);
		}
		if (menu == 8)
		{
			Make_stat(number_of_vertex, number_of_edges, conection_start, conection_end, vertex_to_vertexes_conections, vertex_to_edges_conections, solution, best_selected_vertexes, best_solution_score, number_of_iterations, type_of_algorithm, tabu_max_size);
		}
		if (menu == 9)
		{
			cout << "Select size of solution: ";
			cin >> number_of_vertex;
			
			Finding_best_solution_by_simulated_annealing(number_of_vertex, number_of_edges, best_solution_score, best_selected_vertexes, conection_start, conection_end, vertex_to_vertexes_conections, vertex_to_edges_conections);
		}
	}
	return 0;
}