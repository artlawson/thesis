#class

class Node:
    def __init__(self,i_d,edges,interests):
        self.id = i_d
        self.edges = edges
        self.other_interests = interests #if data has attribute for profile interests
        self.neighbors = []
    def set_neighbors(self):
        for edge in self.edges:
            self.neighbors.append(edge.neighhbor_helper(self))
            
class Edge:
    def __init__(self,nodes):
        self.nodes = set(nodes.sort()) #each edge can be indentified by the nodes it connects
    def edge_nodes(self,node): #returns whether or not a node is in an edge
        return self.nodes[0] == node or self.nodes[1] == node
    def neighbor_helper(self,node): #returns the 'other' node in an edge pair
        if self.nodes[0] == node:
            return self.nodes[1]
        return self.nodes[0]
        