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
from mycreatepathnetwork import *
from mynavigatorhelpers import *


###############################
### AStarNavigator
###
### Creates a path node network and implements the A* algorithm to create a path to the given destination.
			
class AStarNavigator(NavMeshNavigator):

	def __init__(self):
		NavMeshNavigator.__init__(self)
		

	### Create the path node network.
	### self: the navigator object
	### world: the world object
	def createPathNetwork(self, world):
		self.pathnodes, self.pathnetwork, self.navmesh = myCreatePathNetwork(world, self.agent)
		return None
		
	### Finds the shortest path from the source to the destination using A*.
	### self: the navigator object
	### source: the place the agent is starting from (i.e., its current location)
	### dest: the place the agent is told to go to
	def computePath(self, source, dest):
		self.setPath(None)
		### Make sure the next and dist matrices exist
		if self.agent != None and self.world != None: 
			self.source = source
			self.destination = dest
			### Step 1: If the agent has a clear path from the source to dest, then go straight there.
			###   Determine if there are no obstacles between source and destination (hint: cast rays against world.getLines(), check for clearance).
			###   Tell the agent to move to dest
			### Step 2: If there is an obstacle, create the path that will move around the obstacles.
			###   Find the path nodes closest to source and destination.
			###   Create the path by traversing the self.next matrix until the path node closest to the destination is reached
			###   Store the path by calling self.setPath()
			###   Tell the agent to move to the first node in the path (and pop the first node off the path)
			if clearShot(source, dest, self.world.getLinesWithoutBorders(), self.world.getPoints(), self.agent):
				self.agent.moveToTarget(dest)
			else:
				start = findClosestUnobstructed(source, self.pathnodes, self.world.getLinesWithoutBorders())
				end = findClosestUnobstructed(dest, self.pathnodes, self.world.getLinesWithoutBorders())
				if start != None and end != None:
					# print len(self.pathnetwork)
					newnetwork = unobstructedNetwork(self.pathnetwork, self.world.getGates())
					# print len(newnetwork)
					closedlist = []
					path, closedlist = astar(start, end, newnetwork)
					if path is not None and len(path) > 0:
						path = shortcutPath(source, dest, path, self.world, self.agent)
						self.setPath(path)
						if self.path is not None and len(self.path) > 0:
							first = self.path.pop(0)
							self.agent.moveToTarget(first)
		return None
		
	### Called when the agent gets to a node in the path.
	### self: the navigator object
	def checkpoint(self):
		myCheckpoint(self)
		return None

	### This function gets called by the agent to figure out if some shortcuts can be taken when traversing the path.
	### This function should update the path and return True if the path was updated.
	def smooth(self):
		return mySmooth(self)

	def update(self, delta):
		myUpdate(self, delta)


def unobstructedNetwork(network, worldLines):
	newnetwork = []
	for l in network:
		hit = rayTraceWorld(l[0], l[1], worldLines)
		if hit == None:
			newnetwork.append(l)
	return newnetwork




def astar(init, goal, network):
	path = []
	open = []
	closed = []
	### YOUR CODE GOES BELOW HERE ###

	def getSuccessors(input_Coordinates, input_Network):
		successors = []
		for edge in input_Network:
			edgeA = edge[0]
			edgeB = edge[1]
			if edgeA == input_Coordinates:
				successors.append(edgeB)
			elif edgeB == input_Coordinates:
				successors.append(edgeA)
		return successors

	def rebuildPath(path, parents, init, goal):
		index = goal
		while index is not init:
			path.append(index)
			if index not in parents:
				path = None
				break
			else:
				index = parents[index]
		if path is not None:
			path.append(init)
			path.reverse()
		return path



	# Makes Open a Priority Queue,
	# The Priority Queue Will hold tuples of
	# (Priority, Coordinates)
	# where Priority is = g(n) + h(n)
	from Queue import PriorityQueue
	open = PriorityQueue()


	# closed just holds the coordinates of visited sets
	closed = set([])
	start_Priority = 0
	start_Node = (start_Priority, init)
	open.put(start_Node)

	parentDictonary = {}




	# while our open list is not empty
	while not open.empty():
		current = open.get()
		current_distance_from_start = current[0]
		current_Coordinates = current[1]
		if current_Coordinates == goal:
			break
		else:
			closed.add(current_Coordinates)
			successors = getSuccessors(current_Coordinates, network)
			for successor in successors:
				if successor not in closed:
					distance_from_current_to_Goal = distance(current_Coordinates, successor)
					_Gscore = current_distance_from_start + distance_from_current_to_Goal
					_Hscore = distance(successor, goal)
					_Fscore = _Gscore + _Hscore

					existingNode = None
					existing_node_Value = None
					existing_node_Coordinates = None
					find_existing_index = 0
					found_existing = False
					while(find_existing_index < open.qsize()  and found_existing == False):
						existingNode = open.queue[find_existing_index]
						existing_node_Coordinates = existingNode[1]
						if(successor ==existing_node_Coordinates):
							existing_node_Value = existingNode[0]
							found_existing = True
						find_existing_index = find_existing_index + 1

					if(found_existing == False):
						open.put((_Fscore, successor))
						parentDictonary[successor] = current_Coordinates
					else:
						if _Gscore < existing_node_Value:
							open.queue.remove(existingNode)
							open.queue.append((_Fscore,existing_node_Coordinates))
							parentDictonary[successor] = current_Coordinates


	path = rebuildPath(path, parentDictonary, init, goal)
	closed = list(closed)
	### YOUR CODE GOES ABOVE HERE ###
	return path, closed




def myUpdate(nav, delta):
	### YOUR CODE GOES BELOW HERE ###
	agent = nav.agent
	current_position = agent.position
	move_target = agent.getMoveTarget()
	final_destination = nav.getDestination()
	worldlines = nav.world.getLinesWithoutBorders()
	worldpoints = nav.world.getPoints()

	if not clearShot(current_position, move_target, worldlines, worldpoints, agent):
		nav.agent.stopMoving()
	### YOUR CODE GOES ABOVE HERE ###
	return None



def myCheckpoint(nav):
	### YOUR CODE GOES BELOW HERE ###

	agent = nav.agent
	current_position = nav.agent.getLocation()
	move_target = agent.getMoveTarget()
	final_destination = nav.getDestination()
	worldlines = nav.world.getLinesWithoutBorders()
	worldpoints = nav.world.getPoints()

	if not clearShot(current_position, move_target, worldlines, worldpoints, agent):
		nav.agent.stopMoving()
		agent.navigateTo(final_destination)

	### YOUR CODE GOES ABOVE HERE ###
	return None


### Returns true if the agent can get from p1 to p2 directly without running into an obstacle.
### p1: the current location of the agent
### p2: the destination of the agent
### worldLines: all the lines in the world
### agent: the Agent object
def clearShot(p1, p2, worldLines, worldPoints, agent):
	### YOUR CODE GOES BELOW HERE ###
	if p1 == p2:
		return True
	if rayTraceWorldNoEndPoints(p1, p2, worldLines) is not None:
		return False
	else:
		for point in worldPoints:
			path_Line = (p1, p2)
			if minimumDistance(path_Line, point) <= agent.getMaxRadius():
				return False
		return True

	### YOUR CODE GOES ABOVE HERE ###
	return False

