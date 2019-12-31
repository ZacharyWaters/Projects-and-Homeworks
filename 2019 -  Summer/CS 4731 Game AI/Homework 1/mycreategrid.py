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

# Creates a grid as a 2D array of True/False values (True =  traversable). Also returns the dimensions of the grid as a (columns, rows) list.
def myCreateGrid(world, cellsize):
	grid = None
	dimensions = (0, 0)
	### YOUR CODE GOES BELOW HERE ###
	world_Dimensions = world.getDimensions()
	world_dim_width = world_Dimensions[0]
	world_dim_height = world_Dimensions[1]
	number_of_width_cells = int(math.floor(world_dim_width/cellsize))
	number_of_height_cells = int(math.floor(world_dim_height/cellsize))


	dimensions = (number_of_width_cells, number_of_height_cells)
	grid = [[True for y in range(number_of_height_cells)] for x in range(number_of_width_cells)]

	obstacles = world.getObstacles()
	flag = False
	for obstacle in obstacles:
		for point in (obstacle.getPoints()):
			point_x = point[0]
			point_y = point[1]
			x_index = int(math.floor(point_x / cellsize))
			y_index = int(math.floor(point_y / cellsize))
			if point_x != 0 and point_y != 0 and point_x % cellsize != 0 and point_y % cellsize != 0 and x_index < number_of_height_cells and y_index < number_of_height_cells:
				grid[x_index][y_index] = False
				# DEBUG STUFF
				# red_color = (255, 64, 64)
				# grid_box = (x_index * cellsize, y_index * cellsize, cellsize, cellsize)
				# pygame.draw.rect(world.debug, red_color, grid_box, 0)

		for line in (obstacle.getLines()):
			line_start = line[0]
			line_start_x = line_start[0]
			line_start_y = line_start[1]
			start_x_index = int(math.floor(line_start[0] / cellsize))
			start_y_index = int(math.floor(line_start[1] / cellsize))
			line_end = line[1]
			line_end_x = line_end[0]
			line_end_y = line_end[1]
			end_x_index = int(math.floor(line_end[0] / cellsize))
			end_y_index = int(math.floor(line_end[1] / cellsize))

			x_index = start_x_index
			y_index = start_y_index
			adjacentSquares = set()
			value = ((x_index + 1), (y_index + 1))
			adjacentSquares.add(value)
			value = ((x_index - 1), (y_index - 1))
			adjacentSquares.add(value)
			value = ((x_index + 1), (y_index - 1))
			adjacentSquares.add(value)
			value = ((x_index - 1), (y_index + 1))
			adjacentSquares.add(value)
			value = ((x_index + 1), (y_index))
			adjacentSquares.add(value)
			value = ((x_index - 1), (y_index))
			adjacentSquares.add(value)
			value = ((x_index), (y_index + 1))
			adjacentSquares.add(value)
			value = ((x_index), (y_index - 1))
			adjacentSquares.add(value)

			visited = set()
			while adjacentSquares:
				adjacentSquare = adjacentSquares.pop()

				x_index = adjacentSquare[0]
				y_index = adjacentSquare[1]
				if(y_index < number_of_height_cells) and (x_index < number_of_width_cells) and (adjacentSquare not in visited):
					visited.add(adjacentSquare)

					topLeftCorner = (x_index * cellsize, y_index * cellsize)
					topRightCorner = ((x_index + 1) * cellsize, y_index * cellsize)
					bottomLeftCorner = (x_index * cellsize, (y_index + 1) * cellsize)
					bottomRightCorner = ((x_index + 1) * cellsize, (y_index + 1) * cellsize)

					boxlines = []
					topLine = (topLeftCorner, topRightCorner)
					boxlines.append(topLine)
					leftLine = (topLeftCorner, bottomLeftCorner)
					boxlines.append(leftLine)
					rightLine = (topRightCorner, bottomRightCorner)
					boxlines.append(rightLine)
					bottomLine = (bottomLeftCorner, bottomRightCorner)
					boxlines.append(bottomLine)

					extendRange = False
					obsticle_point = obstacle.getPoints()

					if rayTraceWorld(line_start, line_end, boxlines) != None:
						grid[x_index][y_index] = False
						extendRange = True

					if pointInsidePolygonPoints(topLeftCorner, obsticle_point) == True:
						grid[x_index][y_index] = False
						extendRange = True
					if pointInsidePolygonPoints(topRightCorner, obsticle_point) == True:
						grid[x_index][y_index] = False
						extendRange = True
					if pointInsidePolygonPoints(bottomLeftCorner, obsticle_point) == True:
						grid[x_index][y_index] = False
						extendRange = True
					if pointInsidePolygonPoints(bottomRightCorner, obsticle_point) == True:
						grid[x_index][y_index] = False
						extendRange = True

					if(extendRange == True):
						value = ((x_index + 1), (y_index + 1))
						adjacentSquares.add(value)
						value = ((x_index - 1), (y_index - 1))
						adjacentSquares.add(value)
						value = ((x_index + 1), (y_index - 1))
						adjacentSquares.add(value)
						value = ((x_index - 1), (y_index + 1))
						adjacentSquares.add(value)
						value = ((x_index + 1), (y_index))
						adjacentSquares.add(value)
						value = ((x_index - 1), (y_index))
						adjacentSquares.add(value)
						value = ((x_index), (y_index + 1))
						adjacentSquares.add(value)
						value = ((x_index), (y_index - 1))
						adjacentSquares.add(value)


	### YOUR CODE GOES ABOVE HERE ###
	return grid, dimensions

