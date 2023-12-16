#pragma once
#include <map>
#include <set>
#include <vector>
#include <climits>
#include <cmath>

template <typename T>
class DijkstraAlgorithm
{
public:
	//------------------------------------------------------------------------
	// Adjacency Item, contains item and weight
	//------------------------------------------------------------------------
	struct ConnWeightItem
	{
		T connItem ;
		double weight ;

		ConnWeightItem( const T &_connItem, double _weight = DBL_MAX ) : connItem( _connItem ), weight( _weight )
		{

		}

		bool operator<( const ConnWeightItem &rhs ) const
		{
			if( connItem == rhs.connItem )
			{
				return weight < rhs.weight ;
			}
			else
				return connItem < rhs.connItem ;
		}
	};

	typedef std::set<ConnWeightItem> SET_CONN_WEIGHT_ITEM ;

public:
	typedef std::map<T,SET_CONN_WEIGHT_ITEM> MAP_ITEM_CONN ;

private:
	std::set<T> m_ItemSet ;
	MAP_ITEM_CONN m_ConnItemMap ;

private:
	bool FindMinNode( const std::map<T,double> &weightToNode, std::set<T> &visited, T &minNode )
	{
		typename std::map<T,double>::const_iterator weightToNodeIter, weightToNodeEnd = weightToNode.end() ;

		double minWeight = DBL_MAX ;

		for( weightToNodeIter = weightToNode.begin(); weightToNodeIter != weightToNodeEnd; ++weightToNodeIter )
		{
			const T &node = weightToNodeIter->first ;
			double weight = weightToNodeIter->second ;

			//------------------------------------------------------------------------
			// find node with minimum weight that's not visited yet
			//------------------------------------------------------------------------
			if( visited.end() != visited.find( node ) )
				continue ;

			if( weight < minWeight )
			{
				minNode = node ;
				minWeight = weight ;
			}
		}

		if( DBL_MAX == minWeight )
			return false ;

		return true ;
	}

	void UpdateConnWeight( const T &node, std::map<T, double> &weightToNode, std::map<T,T> &prevNodeToEachNode )
	{
		//------------------------------------------------------------------------
		// node까지 오는 최단거리 가져옴
		//------------------------------------------------------------------------
		double weightToCurNode = weightToNode[node] ;

		typename MAP_ITEM_CONN::const_iterator connItemMapIter = m_ConnItemMap.find( node ) ;

		if( connItemMapIter == m_ConnItemMap.end() )
			return ;

		const SET_CONN_WEIGHT_ITEM &connWeightItemSet = connItemMapIter->second ;
		typename SET_CONN_WEIGHT_ITEM::const_iterator connWeightItemSetIter, connWeightItemSetEnd = connWeightItemSet.end() ;
		typename std::map<T, double>::iterator weightToNodeIter, weightToNodeEnd = weightToNode.end() ;

		for( connWeightItemSetIter = connWeightItemSet.begin(); connWeightItemSetIter != connWeightItemSetEnd; ++connWeightItemSetIter )
		{
			const ConnWeightItem &connNodeWeight = *connWeightItemSetIter ;

			weightToNodeIter = weightToNode.find( connNodeWeight.connItem ) ;

			//------------------------------------------------------------------------
			// should there be all nodes in weightToNodeMap, otherwist it's error
			//------------------------------------------------------------------------
			if( weightToNodeIter == weightToNodeEnd )
				return ;

			double &weightToConnNode = weightToNodeIter->second ;

			//------------------------------------------------------------------------
			// update weight to each node
			//------------------------------------------------------------------------
			double newWeight = weightToCurNode + connNodeWeight.weight ;

			if( newWeight < weightToConnNode )
			{
				weightToConnNode = std::min( weightToCurNode + connNodeWeight.weight, weightToConnNode ) ;
				prevNodeToEachNode[connNodeWeight.connItem] = node ;
			}
		}
	}


public:
	//------------------------------------------------------------------------
	// constructor
	//------------------------------------------------------------------------
	DijkstraAlgorithm( std::set<T> &itemSet, MAP_ITEM_CONN &itemConnMap ) : m_ItemSet( itemSet ), m_ConnItemMap( itemConnMap )
	{

	}

	bool Find( const T &start, const T &end, std::vector<T> &route )
	{
		//------------------------------------------------------------------------
		// if start and end is equal, return
		//------------------------------------------------------------------------
		if( start == end )
		{
			route.push_back( start ) ;
			return true ;
		}

		typename std::set<T>::const_iterator itemSetIter, itemSetEnd = m_ItemSet.end() ;

		if( m_ItemSet.find( start ) == itemSetEnd || m_ItemSet.find( end ) == itemSetEnd )
			return false ;

		std::set<T> visited ;

		std::map<T,double> weightToNode ;
		std::map<T,T> prevNearestVisitNode ;

		//------------------------------------------------------------------------
		// initialize weight to node with DBL_MAX value
		//------------------------------------------------------------------------
		typename std::set<T>::const_iterator nodeIter, nodeEnd = m_ItemSet.end() ;

		for( nodeIter = m_ItemSet.begin(); nodeIter != nodeEnd; ++nodeIter )
		{
			const T &node = *nodeIter ;

			weightToNode[node] = DBL_MAX ;
		}

		//------------------------------------------------------------------------
		// set start node weight to 0
		//------------------------------------------------------------------------
		weightToNode[start] = 0 ;
		prevNearestVisitNode[start] = start ;

		T minNode ;

		while( 1 )
		{

			//------------------------------------------------------------------------
			// if all node been visited, end search
			//------------------------------------------------------------------------
			if( false == FindMinNode( weightToNode, visited, minNode ) )
				break ;

			visited.insert( minNode ) ;

			//------------------------------------------------------------------------
			// if it reached end node, break
			//------------------------------------------------------------------------
			if( minNode == end )
				break ;

			//------------------------------------------------------------------------
			// update weight to each node from start, also marking previous node for each connected node
			//------------------------------------------------------------------------
			UpdateConnWeight( minNode, weightToNode, prevNearestVisitNode ) ;
		}

		//------------------------------------------------------------------------
		// get route from start to end node
		//------------------------------------------------------------------------

		double weightToEnd = weightToNode[end] ;

		//------------------------------------------------------------------------
		// end까지 가는 길이 없으면 false 반환
		//------------------------------------------------------------------------
		if( DBL_MAX == weightToEnd )
			return false ;

		route.push_back( end ) ;

		T lastNode = end ;

		while( 1 )
		{
			const T &prevNode = prevNearestVisitNode[lastNode] ;

			if( prevNode == start )
				break ;

			route.push_back( prevNode ) ;

			lastNode = prevNode ;
		}

		route.push_back( start ) ;

		//------------------------------------------------------------------------
		// set route to start
		//------------------------------------------------------------------------
		std::reverse( route.begin(), route.end() ) ;

		return true ;
	}

};