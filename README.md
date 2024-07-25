# A-Star-3D
A* algorithm that works on 3D space


Create a graph file using graphCreate.cpp, and use functions inside astar.cpp to find way between waypoints.

Compile them both using any compiler.

For creating graph file, use this parameters to createGraph program: EXECUTABLE [output file name]  [Max X] [Max Y] [Max Z] [Min X] [Min Y] [Min Z] [Resolution]

After creating graph file, use this parameters to find ways between given points: Usage: EXECUTABLE [graph file name] [used node file] [start X] [start Y] [start Z] [end X] [end Y] [end Z]

About findWay function inside astar.cpp: If you are willing to find way without crossing previous ways, give last parameter true, if not, give false. This parameter restricts used nodes for future usage.

drawWay is the function needed to visualize waypoints with my graphic engine, you can omit that for now. 
