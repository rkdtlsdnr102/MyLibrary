#include "DijkstraAlgorithm.h"
#include <array>
#include <iostream>

/// <summary>
/// Example
/// </summary>

typedef DijkstraAlgorithm<int> DijkstraAlgorithm_With_IntItem;

void main()
{
	//nodeItem : 1,2,3,4,5

	DijkstraAlgorithm_With_IntItem::MAP_ITEM_CONN adjacencyMap;

	// make adjacency of each node to node, m x m for m items
	{
		// 1 - 2, 3
		adjacencyMap.insert({ 1, { DijkstraAlgorithm_With_IntItem::ConnWeightItem(2, 10), DijkstraAlgorithm_With_IntItem::ConnWeightItem(3, 5) } });
		// 2 - 1, 3, 4, 5
		adjacencyMap.insert({ 2, {
			DijkstraAlgorithm_With_IntItem::ConnWeightItem(1, 10), 
			DijkstraAlgorithm_With_IntItem::ConnWeightItem(3, 20),
			DijkstraAlgorithm_With_IntItem::ConnWeightItem(4, 5),
			DijkstraAlgorithm_With_IntItem::ConnWeightItem(5, 5)
			} });
		// 3 - 1, 2, 5
		adjacencyMap.insert({ 3, {
			DijkstraAlgorithm_With_IntItem::ConnWeightItem(1, 5),
			DijkstraAlgorithm_With_IntItem::ConnWeightItem(2, 20),
			DijkstraAlgorithm_With_IntItem::ConnWeightItem(5, 10)
			} });
		// 4 - 2, 5
		adjacencyMap.insert({ 4, {
			DijkstraAlgorithm_With_IntItem::ConnWeightItem(2, 5),
			DijkstraAlgorithm_With_IntItem::ConnWeightItem(5, 6),
			} });
		// 5 - 2, 3, 4
		adjacencyMap.insert({ 5, {
			DijkstraAlgorithm_With_IntItem::ConnWeightItem(2, 5),
			DijkstraAlgorithm_With_IntItem::ConnWeightItem(3, 10),
			DijkstraAlgorithm_With_IntItem::ConnWeightItem(4, 6)
			} });
	}
	
	std::set<int> nodeItems = { 1,2,3,4,5 };
	std::vector<int> resPath;

	// initialize shortest path finder with items and its adjacency map
	DijkstraAlgorithm_With_IntItem shortestPath_int(nodeItems, adjacencyMap);

	// find shortest path from node 1 to 5
	if (false == shortestPath_int.Find(1, 4, resPath))
	{
		std::cout << "No Path Found\n";
		return;
	}

	//print shortest path
	std::cout << "Shortest Path : ";

	for (int pathItem : resPath)
	{
		std::cout << pathItem << ' ';
	}

	std::cout << '\n';
}

