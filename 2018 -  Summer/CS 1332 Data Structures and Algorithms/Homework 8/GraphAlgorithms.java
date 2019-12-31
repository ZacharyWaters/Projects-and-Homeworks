import java.util.List;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.ArrayList;
import java.util.Queue;
import java.util.PriorityQueue;

/**
 * Your implementation of various different graph algorithms.
 *
 * @author Zachary Waters
 * @userid zwaters3
 * @GTID 903116871
 * @version 1.0
 */
public class GraphAlgorithms {

    /**
     * Performs a depth first search (dfs) on the input graph, starting at
     * {@code start} which represents the starting vertex.
     *
     * When deciding which neighbors to visit next from a vertex, visit the
     * vertices in the order presented in that entry of the adjacency list.
     *
     * *NOTE* You MUST implement this method recursively, or else you will lose
     * most if not all points for this method.
     *
     * You may import/use {@code java.util.Set}, {@code java.util.List}, and
     * any classes that implement the aforementioned interfaces, as long as it
     * is efficient.
     *
     * The only instance of {@code java.util.Map} that you may use is the
     * adjacency list from {@code graph}. DO NOT create new instances of Map
     * for DFS (storing the adjacency list in a variable is fine).
     *
     * DO NOT modify the structure of the graph. The graph should be unmodified
     * after this method terminates.
     *
     * @throws IllegalArgumentException if any input
     *  is null, or if {@code start} doesn't exist in the graph
     * @param <T> the generic typing of the data
     * @param start the vertex to begin the dfs on
     * @param graph the graph to search through
     * @return list of vertices in visited order
     */
    public static <T> List<Vertex<T>> depthFirstSearch(Vertex<T> start,
                                                       Graph<T> graph) {
        if (start == null) {
            throw new IllegalArgumentException(
                    "the Starting Vertex Cannot be null");
        }
        if (graph == null) {
            throw new IllegalArgumentException(
                    "the Graph Cannot be null");
        }
        Map adjList = graph.getAdjList();
        if (!adjList.containsKey(start)) {
            throw new IllegalArgumentException(
                    "the Starting Vertex must exist in the graph");
        }
        Set<Vertex<T>> set = new LinkedHashSet<>();
        depthFirstHelper(start, set, adjList);
        List list = new ArrayList(set);
        return list;
    }

    /**
     * @param current the current vertex
     * @param set the set of visited vertexes
     * @param adjList the adjacency lst
     */
    private static void depthFirstHelper(
            Vertex current, Set set, Map adjList) {
        if (set.add(current)) {
            //new Vertex
            Object o = adjList.get(current);
            ArrayList vertexDistanceArray = (ArrayList) o;
            for (int i = 0; i < vertexDistanceArray.size(); i++) {
                Vertex next
                        = ((VertexDistance) vertexDistanceArray
                        .get(i)).getVertex();
                depthFirstHelper(next, set, adjList);
            }
        }
        /**
        if (set.add(current)) {
            //new Vertex
            Object o = adjList.get(current);
            ArrayList vertexDistanceArray = (ArrayList) o;
            for (int i = vertexDistanceArray.size() - 1; i >= 0; i--) {
                stack.push(vertexDistanceArray.get(i));
            }
        }
        if (!stack.empty()) {
            Vertex next = ((VertexDistance) stack.pop()).getVertex();
            depthFirstHelper(next, stack, set, adjList);
        }
         */
    }


    /**
     * Finds the single-source shortest distance between the start vertex and
     * all vertices given a weighted graph (you may assume non-negative edge
     * weights).
     *
     * Return a map of the shortest distances such that the key of each entry
     * is a node in the graph and the value for the key is the shortest distance
     * to that node from start, or Integer.MAX_VALUE (representing infinity)
     * if no path exists.
     *
     * You may import/use {@code java.util.PriorityQueue},
     * {@code java.util.Map}, and {@code java.util.Set} and any class that
     * implements the aforementioned interfaces, as long as it's efficient.
     *
     * You should implement the version of Dijkstra's where you use two
     * termination conditions in conjunction.
     *
     * 1) Check that not all vertices have been visited.
     * 2) Check that the PQ is not empty yet.
     *
     * DO NOT modify the structure of the graph. The graph should be unmodified
     * after this method terminates.
     *
     * @throws IllegalArgumentException if any input is null, or if start
     *  doesn't exist in the graph.
     * @param <T> the generic typing of the data
     * @param start index representing which vertex to start at (source)
     * @param graph the graph we are applying Dijkstra's to
     * @return a map of the shortest distances from start to every other node
     *         in the graph
     */
    public static <T> Map<Vertex<T>, Integer> dijkstras(Vertex<T> start,
                                                        Graph<T> graph) {
        //EXCEPTION CHECKING
        if (start == null) {
            throw new IllegalArgumentException(
                    "the Starting Vertex Cannot be null");
        }
        if (graph == null) {
            throw new IllegalArgumentException(
                    "the Graph Cannot be null");
        }
        Map adjList = graph.getAdjList();
        if (!adjList.containsKey(start)) {
            throw new IllegalArgumentException(
                    "the Starting Vertex must exist in the graph");
        }

        //HANDLING INFINITE DISTANCES IN DISJOINTED GRAPHS
        Map<Vertex<T>, Integer> hm = new HashMap<>();
        Set<Vertex<T>> vertices = graph.getVertices();
        Object[] objects = vertices.toArray();
        for (int i = 0; i < objects.length; i++) {
            hm.put(((Vertex<T>) objects[i]), Integer.MAX_VALUE);
        }

        //initializing the PRIORITY QUEUE
        //PriorityQueue<VertexDistance<T>> queue = new PriorityQueue<>();
        //Stack<VertexDistance<T>> queue = new Stack<>();
        Queue<VertexDistance<T>> queue =  new LinkedList<>();
        //initializing the stack of distances
        //Stack<Integer> distances = new Stack<>();
        Queue<Integer> distances =  new LinkedList<>();
        //Initialize the set of visited Nodes
        Set<Vertex<T>> visitSet = new LinkedHashSet<>();
        //Puts the starting value in the queue at 0
        int distance = 0;
        Vertex<T> current = start;
        hm.put(current, distance);
        visitSet.add(current);

        //get the initial neighboring points
        Object o = adjList.get(current);
        ArrayList vertexDistanceArray = (ArrayList) o;
        //add them to the queue with rolling distance of 0.
        for (int i = vertexDistanceArray.size() - 1; i >= 0; i--) {
            queue.add((VertexDistance) vertexDistanceArray.get(i));
            distances.add(0);
            //the Stack distances
            //distances.push(0);
        }

        while (!queue.isEmpty()) {
            VertexDistance<T> poll = queue.poll();
            //VertexDistance<T> poll = queue.pop();
            distance = poll.getDistance();
            int rollDistance = distances.poll();
            //int rollDistance = distances.pop();
            current = poll.getVertex();
            if (visitSet.add(current)
                    || (hm.get(current) > distance + rollDistance)) {

                hm.put(current, distance + rollDistance);
                //get this nodes neighboring nodes
                o = adjList.get(current);
                vertexDistanceArray = (ArrayList) o;
                //add them to the queue with rolling distance of 0.
                for (int i = vertexDistanceArray.size() - 1; i >= 0; i--) {
                    queue.add((VertexDistance) vertexDistanceArray.get(i));
                    //queue.push((VertexDistance) vertexDistanceArray.get(i));
                    distances.add(distance + rollDistance);
                    //distances.push(distance + rollDistance);
                }
            }
        }
        /**
        PriorityQueue<VertexDistance<T>> queue = new PriorityQueue<>();
        int distance = 0;
        Vertex<T> current = start;
        hm.put(current, distance);
        Object o = adjList.get(current);
        ArrayList vertexDistanceArray = (ArrayList) o;
        for (int i = vertexDistanceArray.size() - 1; i >= 0; i--) {
            queue.add((VertexDistance) vertexDistanceArray.get(i));
        }
        while (!queue.isEmpty()) {
            VertexDistance<T> poll = queue.poll();
            distance = poll.getDistance();
            current = poll.getVertex();
            hm.put(current, distance);
        }
         */
        return hm;
    }

    /**
     Set<Edge<T>> set = new HashSet<>(graph.getEdges());
     Object[] objects = set.toArray();
     Arrays.sort(objects);
     Queue<Edge<T>> queue = new PriorityQueue<>();
     for (int i = 0; i < objects.length; i++) {
     queue.add((Edge<T>) objects[i]);
     }
     */


    /**
     * Runs Kruskal's algorithm on the given graph and returns the Minimal
     * Spanning Tree (MST) in the form of a set of Edges. If the graph is
     * disconnected and therefore no valid MST exists, return null.
     *
     * You may assume that the passed in graph is undirected. In this framework,
     * this means that if (u, v, 3) is in the graph, then the opposite edge
     * (v, u, 3) will also be in the graph, though as a separate Edge object.
     *
     * The returned set of edges should form an undirected graph. This means
     * that every time you add an edge to your return set, you should add the
     * reverse edge to the set as well. This is for testing purposes. This
     * reverse edge does not need to be the one from the graph itself; you can
     * just make a new edge object representing the reverse edge.
     *
     * You may assume that there will only be one valid MST that can be formed.
     *
     * Kruskal's will also require you to use a Disjoint Set which has been
     * provided for you. A Disjoint Set will keep track of which vertices are
     * connected given the edges in your current MST, allowing you to easily
     * figure out whether adding an edge will create a cycle. Refer
     * to the {@code DisjointSet} and {@code DisjointSetNode} classes that
     * have been provided to you for more information.
     *
     * You should NOT allow self-loops into the MST.
     *
     * You may import/use {@code java.util.PriorityQueue},
     * {@code java.util.Set}, and any class that implements the aforementioned
     * interface.
     *
     * DO NOT modify the structure of the graph. The graph should be unmodified
     * after this method terminates.
     *
     * @throws IllegalArgumentException if any input is null
     * @param <T> the generic typing of the data
     * @param graph the graph we are applying Kruskals to
     * @return the MST of the graph or null if there is no valid MST
     */
    public static <T> Set<Edge<T>> kruskals(Graph<T> graph) {
        if (graph == null) {
            throw new IllegalArgumentException(
                    "the Graph Cannot be null");
        }
        Queue<Edge<T>> queue = new PriorityQueue<>(graph.getEdges());
        //System.out.println(queue);
        Set<Edge<T>> answerSet = new LinkedHashSet<>();
        DisjointSet disjointSet = new DisjointSet(graph.getVertices());
        while (!queue.isEmpty()) {
            Edge<T> poll = queue.poll();
            Vertex<T> u = poll.getU();
            Vertex<T> v = poll.getV();
            if (disjointSet.find(u) != disjointSet.find(v)) {
                disjointSet.union(u, v);
                answerSet.add(poll);
                answerSet.add(new Edge<>(v, u, poll.getWeight()));
            }
        }
        return answerSet;
    }
}
