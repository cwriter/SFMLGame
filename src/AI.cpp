#include "AI.h"


namespace SFG 
{
	Pointer< AIPath > AIPathGraph::getShortestPath(const Pointer< AIPathVertex >& start, const Pointer< AIPathVertex >& target) const
	{
		Util::printLog(Util::Warning, __FILE__, __LINE__, "Untested function called");
		Pointer<AIPath> ptr(new AIPath());
			
		//Create a n*1 table containing the cost to get to the vertex in m_vertices[i] from start
		std::vector<float> T(m_vertices.size() * 2, std::numeric_limits< float >::max());
			
		//Fill table (iterate through all points and get costs)
		//As the points must be metric, i.e. dist(a to b) <= dist(a to c) + dist (c to b).
		//we can do a DFS
		recursive_gsp(T, start.getElement());
		
		//Now, go from start to end until we have finished
		size_t finish_node = target->getPosInArray();
		size_t i = start->getPosInArray();
		while(i != finish_node)
		{
			ptr->addNode(m_vertices[i]);
			i = T[T.size() + i];
		}
			
			
		return ptr;
	}
	
	void AIPathGraph::recursive_gsp(std::vector< float >& T, AIPathVertex* vtx) const
	{
		if(vtx == nullptr) return;
		
		AIPathVertex* current = vtx;
		for(size_t i = 0; i < current->edges().size() / 2; i++)
		{
			size_t pos = current->getPosInArray();
			size_t nextpos = current->edges()[i]->target->getPosInArray();
			
			if(T[pos] == std::numeric_limits<float>::max()) T[pos] = 0;

			//Set which node is taken next
			if(T[nextpos] < T[pos] + current->edges()[i]->cost)
			{
				//Do nothing; the next node is already correctly set
			}
			else 
			{
				T[T.size() / 2 + pos] = nextpos;
			}
			T[nextpos] = std::min(T[nextpos], T[pos] + current->edges()[i]->cost);
			
		
			
			//Go recurse!
			recursive_gsp(T, current->edges()[i]->target);
		}
	}
	
};