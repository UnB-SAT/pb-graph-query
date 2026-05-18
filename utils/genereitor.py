import networkx as nx
import random
import sys

num_nodes = int(sys.argv[1]);

G = nx.gnp_random_graph(num_nodes,p=0.02, directed=False)

largest_cc = max(nx.connected_components(G), key=len)
G = G.subgraph(largest_cc).copy()

num_edges = len(G.edges);
num_nodes = len(G.nodes);

file_name = f'cases/{num_nodes}-{num_edges}.in'

print(f'Writing into file {file_name}\n')

has_path = []
shortest_path = []

with open(file_name, 'w') as file:

	file.write(f"{num_nodes} {num_edges}\n");

	for node in G.nodes():
		file.write(f"{node}\n");


	for u, w in G.edges():
		file.write(f"{u} {w}\n");

	one_has = False;

	while (one_has != True):
		inicial = random.choice(list(G.nodes()))
		_final = random.choice(list(G.nodes()))
	
		while _final == inicial:
			_final = random.choice(list(G.nodes()))

		if(nx.has_path(G, inicial, _final)):
			one_has = True;
			print(f'one has {inicial} -> {_final}\n')
			shortest_path = nx.shortest_path(G, inicial, _final);
			print(shortest_path);
			has_path.append(inicial)
			has_path.append(_final)
			file.write(f"0\n{inicial}\n{_final}\n");

	inicial = random.choice(list(G.nodes()))
	_final = random.choice(list(G.nodes()))

	while _final == inicial:
		_final = random.choice(list(G.nodes()))

	file.write(f"0\n{inicial}\n{_final}\n");

	inicial = random.randint(0, len(G.nodes()))
	_final = random.choice(list(G.nodes()))

	while _final == inicial:
		_final = random.choice(list(G.nodes()))

	file.write(f"0\n{inicial}\n{_final}\n");

