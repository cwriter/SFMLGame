#pragma once
#include "stdafx.h"

namespace SFG
{
	enum CoordinateType
	{
		cartesian,
		polar
	};
	
	class AIPathVertex;
	
	///<summary>
	///Simple class representing edges. WARNING: This class does not make a copy of the
	///Vertex it connects to and only contains fundamental types. The lifetime of this object
	///must not exceed the lifetime of the targetted vertex.
	///</summary>
	class AIPathEdge
	{
	public:
		
		AIPathVertex* target;
		float cost;
		
	private:
		
	};
	
	class AIPathVertex
	{
	public:
		
		inline size_t getPosInArray() const { return m_pos_in_array; }
		
		inline void setPosInArray(size_t i) { m_pos_in_array = i; }
		
		inline const std::vector<Pointer<AIPathEdge>>& edges() const { return m_edges; }
		
	private:
		CoordinateType m_ct;
		
		//A quick reference to find out which point in the array this vertex is.
		size_t m_pos_in_array;
		
		//In a cartesian system, it's the normal point.
		//In a polar system, x is \phi and y is r.
		sf::Vector2f m_position;
		
		//The edges connecting to other points. This 
		std::vector<Pointer<AIPathEdge>> m_edges;
	};
	
	///<summary>
	///Represents an array of edges that lead from one vertex to another
	///</summary>
	class AIPath
	{
	public:
		
		void addNode(const Pointer<AIPathVertex>& e)
		{
			m_nodes.push_back(e);
		}
		
	private:
		std::vector<Pointer<AIPathVertex>> m_nodes;
	};
	
	///<summary>
	///Represents a pathfinding graph
	///</summary>
	class AIPathGraph
	{
	public:
		
		Pointer<AIPath> getShortestPath(
			const Pointer<AIPathVertex>& start,
			const Pointer<AIPathVertex>& target) const;
		
	private:
		
		void recursive_gsp(std::vector< float >& T, SFG::AIPathVertex* vtx) const;
		
		std::vector<Pointer<AIPathVertex>> m_vertices;
	};
	
	///<summary>
	///Provides coarse-grained AI functionality including pathfinding
	///</summary>
	class AI
	{
	public:
		
		
	private:
		//std::vector<> m_pending_orders;
	};
	
};
