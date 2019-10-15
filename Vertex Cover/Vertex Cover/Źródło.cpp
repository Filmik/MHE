#include<iostream>
#include<math.h>
#include<stdio.h>
#include<vector>

using namespace std;

vector<vector<int> > g;//vector for edge conections
bool v[11110];
int i, j;
vector<int> sol_vertex(int n, int e) {
	vector<int> S;
	for (i = 0; i < n; i++) {
		if (!v[i]) {
			for (j = 0; j < (int)g[i].size(); j++) {
				if (!v[g[i][j]]) {
					v[i] = true;
					v[g[i][j]] = true;
					break;
				}
			}
		}
	}
	for (i = 0; i < n; i++)
		if (v[i])
			S.push_back(i);
	return S;
}
int main() {
	int n, e, a, b, s=0;//bardzielj logicznwe nazwy
	cout << "Enter number of vertices:";//Enter vertexes
	cin >> n;
	cout << "Enter number of Edges:";//Enter edges
	cin >> e;
	g.resize(n);
	memset(v, 0, sizeof(v));
	for (i = 0; i < e; i++) {
		cout << "Enter the end-points of edge " << i + 1 << " : ";//Enter conections 
		cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	vector<int> S = sol_vertex(n, e);
	cout << "The required vertex cover is as follows:\n";//Exit
	for (i = 0; i < (int)S.size(); i++) {
		cout << S[i] + 1 << " ";
		s = s + 1;
	}
	cout << endl;
	cout <<"Number on needed edges: "<< s;
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