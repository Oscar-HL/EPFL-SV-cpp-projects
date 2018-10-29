#include "network.h"
#include <algorithm>
#include <functional>
#include <iterator>
#include <iostream>
#include "random.h"

//Resizes the list of nodes


typedef std::multimap<size_t, size_t>::iterator MIT;
typedef std::multimap<size_t, size_t>::const_iterator cMIT;

void Network::resize(const size_t& nodes) {
	
	if(nodes>=0) {
		
		if(values.size()==0) {
			values=std::vector<double>(nodes,0);
			RNG.normal(values);
		} 
		else if (values.size()< nodes) {
			for(size_t i(values.size()); i<nodes; i++) {
				values.push_back(RNG.uniform_double());
			}
		}
		else if(values.size()>nodes) {
			for(size_t i(values.size()); i>nodes; i--) {
				values.pop_back();
			}
		}
	}
}

		
//Adds a bidirectional link between two nodes


bool Network::add_link(const size_t& a, const size_t& b) {
	
	if(a!=b and a>=0 and b>=0 and b<values.size() and a<values.size()) {
		
		std::pair<MIT, MIT> bounds=links.equal_range(a);
		
		for(MIT it=bounds.first;it!=bounds.second; it++) {
			if(it->second==b) {
				return false;
			}
		}
		
		links.insert(std::make_pair(a,b));
		links.insert(std::make_pair(b,a));
		return true;
	}
	
	else {
		return false;
	}
}
		
			



// Creates random connections between nodes: each node n will be linked with degree(n) 
//other nodes (randomly chosen) where degree(n) is Poisson-distributed.

size_t Network::random_connect(const double& mean_deg) {
	
	size_t nLinks(0);
	links.clear();
	std::vector<int> nodes(values.size());
	std::vector<int> degrees(values.size());
	RNG.poisson(degrees, mean_deg);
	
	for(size_t i(0) ; i < values.size() ; i++) {
	size_t degree(degrees[i]);
	if(degree > nodes.size()) degree = nodes.size();
			for(size_t j(0) ; j < degree ; j++)  {
				RNG.uniform_int(nodes, 0, values.size());
				if(add_link(i,nodes[j]))  { nLinks++;}
				else if(degree < nodes.size()) degree++;
			}
	}
	return nLinks;
	
}

//Resets all node values.

size_t Network::set_values(const std::vector<double>& n_val) {
	
	for(size_t i(0); i<values.size(); i++) {
		values[i]=0.0;
	}
	
	if(n_val.size()==values.size()) {
		values=n_val;
		return values.size();
	}
	
	else if(n_val.size()>values.size()) {
		for(size_t i(0); i<values.size(); i++) {
			values[i]=n_val[i];
		}
			return values.size();
		
	}
	
	else if(n_val.size()<values.size()) {
		for(size_t i(0); i<n_val.size(); i++) {
			values[i]=n_val[i];
		}
			return n_val.size();
		
	}
}

//Number of nodes

size_t Network::size() const {
	
	return values.size();
	
}

//Degree (number of links) of node no *n*

size_t Network::degree(const size_t &_n) const {
	
	if(_n>=0 and _n<values.size()) {
		return links.count(_n);
	}
	else {
		return 0;
	}
	
}

//Value of node no "n"

double Network::value(const size_t &_n) const {
	
	if(_n>=0 and _n<values.size()){
	return values[_n];
	}
	else {
		return 0;
	}
}


//All node values in descending order
std::vector<double> Network::sorted_values() const {
	
	std::vector<double> sortedValues(values);
	std::sort(sortedValues.begin(), sortedValues.end(), std::greater<double>());
	return sortedValues;
		
}


//All neighbors (linked) nodes of node no *n*
std::vector<size_t> Network::neighbors(const size_t& a) const {
	
	std::vector<size_t> Neighbors;
	if(a < values.size() and a >= 0) {
	
	std::pair<cMIT, cMIT> bounds;
	bounds=links.equal_range(a);
	
		for (cMIT it = bounds.first; it != bounds.second; it++) {
			Neighbors.push_back(it->second);
		}
	}
	
	return Neighbors;
}
