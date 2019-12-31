'''
 * Copyright (c) 2014, 2015 Entertainment Intelligence Lab, Georgia Institute of Technology.
 * Originally developed by Mark Riedl.
 * Last edited by Mark Riedl 05/2015
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
'''

import sys, pygame, math, numpy, random, time, copy
from pygame.locals import *

from constants import *
from utils import *
from core import *


# Creates a path node network that connects the midpoints of each nav mesh together
def myCreatePathNetwork(world, agent = None):
	nodes = []
	edges = []
	polys = []
	### YOUR CODE GOES BELOW HERE ###

	def is_a_duplicate_Triangle(all_Possible_Triangles, current_Triangle):
		new_a = current_Triangle[0]
		new_b = current_Triangle[1]
		new_c = current_Triangle[2]
		for existing_triangle in all_Possible_Triangles:
			old_a = existing_triangle[0]
			old_b = existing_triangle[1]
			old_c = existing_triangle[2]
			if (new_a == old_a) and (new_b == old_b) and (new_c == old_c):
				return True
			if (new_a == old_a) and (new_b == old_c) and (new_c == old_b):
				return True
			if(new_a == old_b) and (new_b == old_c) and (new_c == old_a):
				return True
			if(new_a == old_b) and (new_b == old_a) and (new_c == old_c):
				return True
			if(new_a == old_c) and (new_b == old_a) and (new_c == old_b):
				return True
			if(new_a == old_c) and (new_b == old_b) and (new_c == old_a):
				return True
		return False

	def triangle_crosses_another_triangle(all_Possible_Triangles, current_Triangle):
		new_a = current_Triangle[0]
		new_b = current_Triangle[1]
		new_c = current_Triangle[2]
		new_line_AB = (new_a, new_b)
		new_line_AB_reverse = (new_b, new_a)
		new_line_AC = (new_a, new_c)
		new_line_AC_reverse = (new_c, new_a)
		new_line_BC = (new_b, new_c)
		new_line_BC_reverse = (new_c, new_b)
		for existing_triangle in all_Possible_Triangles:
			old_a = existing_triangle[0]
			old_b = existing_triangle[1]
			old_c = existing_triangle[2]
			old_line_AB = (old_a, old_b)
			old_line_AC = (old_a, old_c)
			old_line_BC = (old_b, old_c)
			if rayTraceNoEndpoints(new_a, new_b, old_line_AB) is not None:
				if(new_line_AB != old_line_AB):
					if (new_line_AB_reverse != old_line_AB):
						return True
			if rayTraceNoEndpoints(new_a, new_b, old_line_AC) is not None:
				if (new_line_AB != old_line_AC):
					if (new_line_AB_reverse != old_line_AC):
						return True
			if rayTraceNoEndpoints(new_a, new_b, old_line_BC) is not None:
				if (new_line_AB != old_line_BC):
					if (new_line_AB_reverse != old_line_BC):
						return True
			if rayTraceNoEndpoints(new_a, new_c, old_line_AB) is not None:
				if (new_line_AC != old_line_AB):
					if(new_line_AC_reverse != old_line_AB):
						return True
			if rayTraceNoEndpoints(new_a, new_c, old_line_AC) is not None:
				if (new_line_AC != old_line_AC):
					if (new_line_AC_reverse != old_line_AC):
						return True
			if rayTraceNoEndpoints(new_a, new_c, old_line_BC) is not None:
				if (new_line_AC != old_line_BC):
					if (new_line_AC_reverse != old_line_BC):
						return True
			if rayTraceNoEndpoints(new_b, new_c, old_line_AB) is not None:
				if (new_line_BC != old_line_AB):
					if (new_line_BC_reverse != old_line_AB):
						return True
			if rayTraceNoEndpoints(new_b, new_c, old_line_AC) is not None:
				if (new_line_BC != old_line_AC):
					if (new_line_BC_reverse != old_line_AC):
						return True
			if rayTraceNoEndpoints(new_b, new_c, old_line_BC) is not None:
				if (new_line_BC != old_line_BC):
					if (new_line_BC_reverse != old_line_BC):
						return True
		return False

	def triangle_crosses_obstacle_line(world, current_Triangle):
		obsticle_Lines = world.getLines()
		new_a = current_Triangle[0]
		new_b = current_Triangle[1]
		new_c = current_Triangle[2]
		new_line_AB = (new_a, new_b)
		new_line_AB_reverse = (new_b, new_a)
		new_line_AC = (new_a, new_c)
		new_line_AC_reverse = (new_c, new_a)
		new_line_BC = (new_b, new_c)
		new_line_BC_reverse = (new_c, new_b)
		if rayTraceWorldNoEndPoints(new_a, new_b, obsticle_Lines) is not None:
			if (new_line_AB not in obsticle_Lines):
				if (new_line_AB_reverse not in obsticle_Lines):
					return True
		if rayTraceWorldNoEndPoints(new_a, new_c, obsticle_Lines) is not None:
			if (new_line_AC not in obsticle_Lines):
				if (new_line_AC_reverse not in obsticle_Lines):
					return True
		if rayTraceWorldNoEndPoints(new_b, new_c, obsticle_Lines) is not None:
			if (new_line_BC not in obsticle_Lines):
				if (new_line_BC_reverse not in obsticle_Lines):
					return True
		return False

	def triangle_is_Inside_Obstacle(world, current_Triangle):
		all_Obstacles = world.getObstacles()
		new_a = current_Triangle[0]
		new_b = current_Triangle[1]
		new_c = current_Triangle[2]
		for obstacle in all_Obstacles:
			obsticle_points = obstacle.getPoints()
			if new_a in obsticle_points:
				if new_b in obsticle_points:
					if new_c in obsticle_points:
						return True
		return False

	def obstacle_is_Inside_Triangle(world, current_Triangle):
		all_obstacles = world.getObstacles()
		for obstacle in all_obstacles:
			obsticle_Points = obstacle.getPoints()
			counter = 0
			for single_obsticle_point in obsticle_Points:
				if pointInsidePolygonPoints(single_obsticle_point, current_Triangle):
					counter = counter + 1
				if counter >= 3:
					return True
		return False

	def isValidTriangle(all_Possible_Triangles, current_Triangle, world):
		if is_a_duplicate_Triangle(all_Possible_Triangles,current_Triangle) == True:
			return False
		if triangle_crosses_obstacle_line(world,current_Triangle) == True:
			return False
		if triangle_crosses_another_triangle(all_Possible_Triangles, current_Triangle) == True:
			return False
		if triangle_is_Inside_Obstacle(world, current_Triangle) == True:
			return False
		if obstacle_is_Inside_Triangle(world, current_Triangle) == True:
			return False
		return True

	def getAllPossibleTriangles(world):
		all_Points = world.getPoints()
		#all_points = random.shuffle(all_Points)
		all_Possible_Triangles = set()
		for point_1 in all_Points:
			for point_2 in all_Points:
				for point_3 in all_Points:
					if point_1 != point_2 and point_1 != point_3 and point_2 != point_3:
						new_Triangle = (point_1, point_2, point_3)
						if(isValidTriangle(all_Possible_Triangles, new_Triangle, world) == True):
							all_Possible_Triangles.add(new_Triangle)
		return list(all_Possible_Triangles)

	def mergePolygons(polygon1, polygon2):
		polygon1 = list(polygon1)
		polygon2 = list(polygon2)
		my_p1 = None
		my_p2 = None
		for point1 in polygon1:
			for point2 in polygon1:
				if point1 != point2:
					if point1 in polygon2 and point2 in polygon2:
						my_p1 = point1
						my_p2 = point2
		[point1, point2] = [my_p2, my_p1]
		polygon1_point1_index = polygon1.index(point1)
		polygon1_point2_index = polygon1.index(point2)
		if polygon1_point1_index == 0 and polygon1_point2_index == len(polygon1) - 1:
			point1, point2 = point2, point1
		sorted_poly1 = polygon1[polygon1.index(point1):] + polygon1[:polygon1.index(point1)]
		polygon2_point1_index = polygon2.index(point1)
		polygon2_point2_index = polygon2.index(point2)
		if polygon2_point1_index < polygon2_point2_index and (polygon2_point1_index != 0 or polygon2_point2_index != len(polygon2) - 1):
			polygon2.reverse()
		sorted_poly2 = polygon2[polygon2.index(point2):] + polygon2[:polygon2.index(point2)]
		section_of_poly1 = sorted_poly1[1:]
		section_of_poly2 = sorted_poly2[1:]
		new_combined_poly = section_of_poly1 + section_of_poly2
		return new_combined_poly

	def single_mergePolygons_inArray(polygon_Array):
		mergebucket = [0] * len(polygon_Array)
		new_Polygons = []
		polygon_1_Index = 0
		a_Merge_happened = False
		while polygon_1_Index < len(polygon_Array):
			polygon_2_Index = 0
			while polygon_2_Index < len(polygon_Array):
				if mergebucket[polygon_1_Index] == 0 and mergebucket[polygon_2_Index] == 0:
					polygon_1 = polygon_Array[polygon_1_Index]
					polygon_2 = polygon_Array[polygon_2_Index]
					if polygon_1 != polygon_2:
						if polygonsAdjacent(polygon_1, polygon_2):
							merged_Polygon = mergePolygons(polygon_1, polygon_2)
							if isConvex(merged_Polygon):
								a_Merge_happened = True
								new_Polygons.append(merged_Polygon)
								mergebucket[polygon_1_Index] = 1
								mergebucket[polygon_2_Index] = 1
				polygon_2_Index = polygon_2_Index + 1
			polygon_1_Index = polygon_1_Index + 1
		mergebucket_index = 0
		while mergebucket_index < len(mergebucket):
			ifzero_then_NoMerge = mergebucket[mergebucket_index]
			if(ifzero_then_NoMerge == 0):
				polygon_toAdd = polygon_Array[mergebucket_index]
				new_Polygons.append(polygon_toAdd)
			mergebucket_index = mergebucket_index + 1
		return (new_Polygons, a_Merge_happened)

	def continuous_MergePolygons_inArray(polygon_Array):
		value = single_mergePolygons_inArray(polygon_Array)
		array = value[0]
		maximum_Merge = value[1]
		while(maximum_Merge == True):
			value = single_mergePolygons_inArray(array)
			array = value[0]
			maximum_Merge = value[1]
		return array

	def establish_nodes(world, polygon_Array):
		nodes = list()
		polygon_parents = list()
		for polygon_1 in polygon_Array:
			for polygon_2 in polygon_Array:
				if polygon_1 != polygon_2:
					if(polygonsAdjacent(polygon_1, polygon_2)):
						sharedEdge = (polygonsAdjacent(polygon_1, polygon_2))
						midpoint_i = ((sharedEdge[0][0] + sharedEdge[1][0]) / 2)
						midpoint_j = ((sharedEdge[0][1] + sharedEdge[1][1]) / 2)
						midPoint = (midpoint_i, midpoint_j)
						if midPoint not in nodes:
							nodes.append(midPoint)
							parent_polygons = (polygon_1,polygon_2)
							polygon_parents.append(parent_polygons)
		combined_data = (nodes, polygon_parents)
		return list(combined_data)

	def edge_Validity_Checker(world, polygon_Array, edge_array, current_edge):
		current_edge_A = current_edge[0]
		current_edge_B = current_edge[1]
		obstacles = world.getObstacles()
		for obstacle in obstacles:
			for obstacle_point in obstacle.getPoints():
				if minimumDistance((current_edge_A, current_edge_B), obstacle_point) <= agent.getMaxRadius():
					return False

		for edge in edge_array:
			if rayTraceNoEndpoints(current_edge_A, current_edge_B, edge) is not None:
				return False
		return True

	def establish_edges(world, polygon_Array, node_array, parent_array):
		edges = list()
		index = 0
		while index < len(parent_array):
			parents = parent_array[index]
			parent1 = parents[0]
			parent2 = parents[1]
			second_index = 0
			while(second_index< len(parent_array)):
				if(index != second_index):
					second_parents = parent_array[second_index]
					second_parent1 = second_parents[0]
					second_parent2 = second_parents[1]
					if(second_parent1 == parent1) or (second_parent2 == parent1) or (second_parent1 == parent2) or (second_parent2 == parent2):
						edge_to_add = (node_array[index], node_array[second_index])
						if edge_Validity_Checker(world, polygon_Array, edges, edge_to_add) == True:
							edges.append(edge_to_add)
				second_index = second_index + 1
			index = index + 1
		return edges

	def establish_nodes_and_edges(world, polygon_Array):
		combined_data = establish_nodes(world,polygon_Array)
		node_array = combined_data[0]
		parent_array = combined_data[1]
		edges_array = establish_edges(world, polygon_Array, node_array, parent_array)
		return (node_array, edges_array)


	allpossibleTrianges = getAllPossibleTriangles(world)
	polys = continuous_MergePolygons_inArray(allpossibleTrianges)
	nodesAndEdges = establish_nodes_and_edges(world, polys)
	nodes = nodesAndEdges[0]
	edges = nodesAndEdges[1]

	### YOUR CODE GOES ABOVE HERE ###
	return nodes, edges, polys

	
