#find all graphlets of size 3
import classes

#two different shapes a set of 3 nodes can take

#triangle (all connected to each other)
#3 star (2 connected to 1)

#brute force: check each node and their children recursively
nodes = []
edges = []
triangles = 0 
triangle_sets = set()
two_stars = 0
two_star_sets = set()

for node in nodes:
    for neigh in node.neighbors:
        for tri in neigh.neighbors:
            if tri == node:
                continue
            elif tri in node.neighbors:
                triangle_sets.add ((node,neigh,tri).sort()) #adds t
            else:
                two_star_sets

triangles = len(triangle_sets)
two_stars = len(two_star_sets)
#next step, figure out way to get rid of duplicates

                