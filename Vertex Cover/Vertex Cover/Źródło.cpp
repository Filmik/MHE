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

float Best_solution(vector<int> solution) //solution contains selected vertexes
{
	int edges_in_soluton=0;
	int vertex_in_solution = 0;
	for (int x=0; x < solution.size(); x++) {
		cout << "SOLUTIONS:" << solution[x] << endl;
		int b = solution[x];
		b--;
		//if h[x]== sol[x]
		for (int y = 0; y < h[b].size(); y++)//for evry edge in x 
		{
					cout << "H XY" << h[b][y]+1 << endl;
					edges_in_soluton++;
		}
		
		vertex_in_solution++;
	}
	cout << "V" << vertex_in_solution << "E" << edges_in_soluton<<endl;
	return best_solution;
}

void Climbing_Algorythm(int number_of_vertex, int number_of_edges)
{
	//random solution
	vector<int>random_solution;
	random_solution.resize(number_of_edges);
	random_solution.push_back(2);
	random_solution.push_back(4);
	Best_solution(random_solution);//rate random solution
	//algorythm, rate every new solution
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

vector<int> sol_vertex(int n, int e) {
	vector<int> S;
	for (int i = 0; i < n; i++) {
		if (!v[i]) {
			for (int j = 0; j < (int)g[i].size(); j++) {
				if (!v[g[i][j]]) {
					v[i] = true;
					v[g[i][j]] = true;
					break;
				}
			}
		}
	}
	for (int i = 0; i < n; i++)
		if (v[i])
			S.push_back(i);
	return S;
}

tuple<int, int> data_from_file(int number_of_vertex, int number_of_edges, int conection_start, int conection_end)
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
	output_data.open("output_data.txt", ios::out | ios::app);
	output_data << "Numer of vertex: " << number_of_vertex << endl;
	output_data << "Number of edges: " << number_of_edges << endl;
	
	output_data.close();
}

/*tuple<int,int,int,int> generate_graph()
{
	int number_of_vertex, number_of_edges, conection_start, conection_end;
	cout << "enter number of vertex: " ;
	cin >> number_of_vertex;
		if (number_of_vertex<2)
		{
			cout << "number of vertex > 2 !";
			exit(0);
		}
	number_of_edges = rand() % ((number_of_vertex * (number_of_vertex - 1)) / 2);
	cout << "The graph has " << number_of_edges << " edges."<<endl;
	
	g.resize(number_of_vertex);
	memset(v, 0, sizeof(v));
	int current_start, current_end;
	for (int x = 0; x < number_of_edges; x++) {
		current_start = rand() % number_of_vertex + 1;
		current_end = rand() % number_of_vertex + 1;
		if (current_end == current_start) 
		{
			current_end++;
		}
		if(find(g.begin(), g.end(), current_start) != g.end())
		{
			if (find(g.begin(), g.end(), current_end) != g.end())
			{
				// elem exists in the vector
			}
		}
		else {
			current_start--; current_end--;
			g[current_start].push_back(current_end);
			g[current_end].push_back(current_start);
		}
		g[x].push_back(current_edge);
		g[current_edge].push_back(x);
		cout << g[x][current_edge];
		current_edge--;*
	}
	return { number_of_vertex, number_of_edges, conection_start, conection_end };
}*/

int main() {
	clock_t start = clock();
	//printf("Czas wykonywania: %lu ms\n", clock() - start);
	int number_of_vertex=0, number_of_edges=0, conection_start=0, conection_end=0, minimum_vertex_number=0;

	data_from_file(number_of_vertex, number_of_edges, conection_start, conection_end);
	//tie(number_of_vertex, number_of_edges)= data_from_file(number_of_vertex, number_of_edges,conection_start , conection_end);
	//tie(number_of_vertex, number_of_edges, conection_start, conection_end)= generate_graph();
	
	//cout <<"OOOOOOOOOOOOOOOOOOOOOOOOOOO=="<< g[0][2]<<endl;

	///*vector<int> solution =*/BruteForceVertexCover(number_of_vertex, number_of_edges);//JESLI dziala to dziala reszta
	
	Show_conections();
	Climbing_Algorythm(number_of_vertex, number_of_edges);

	data_to_file(number_of_vertex, number_of_edges);
	
	//vector<int> solution = sol_vertex(number_of_vertex, number_of_edges);//zrób rozwi¹zanie si³owe

	//best_solution(minimum_vertex_number);//FUNKCJA CELU,

	cout << "The required vertex cover is as follows:\n";//Exit
	for (int i = 0; i < (int)solution.size(); i++) {
		cout << solution[i] + 1 << " ";
		minimum_vertex_number = minimum_vertex_number + 1;
	}
	cout << endl;
	cout <<"Number on needed edges: "<< minimum_vertex_number;
	minimum_vertex_number = minimum_vertex_number;

	
	cout << endl <<"minimum vertex=" << minimum_vertex_number;
	printf("Czas wykonywania: %lu ms\n", clock() - start);
	return 0;
}
/*Enter number of vertices:4
Enter number of Edges:5
Enter the end-points of edge 1 : 2 1
Enter the end-points of edge 2 : 3 2
Enter the end-points of edge 3 : 4 3
Enter the end-points of edge 4 : 1 4
Enter the end-points of edge 5 : 1 3
The required vertex cover is as follows:
1 2 3 4 */

//brute force- sprawdzanie czy dany vertex zaznacza wszystkie number_of_edges-krawiendzie jeœli nie to nastêpny
//jeœli ¿aden pojednyñcy nie pokrywa to sprawdzam dla vertex 1 + vertex 2 pó¿niej 1 + 3 itd.
//edge 1-2 == 2-1 !!!