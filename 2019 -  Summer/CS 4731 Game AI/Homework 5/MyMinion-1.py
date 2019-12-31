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
from moba import *

class MyMinion(Minion):
	
	def __init__(self, position, orientation, world, image = NPC, speed = SPEED, viewangle = 360, hitpoints = HITPOINTS, firerate = FIRERATE, bulletclass = SmallBullet):
		Minion.__init__(self, position, orientation, world, image, speed, viewangle, hitpoints, firerate, bulletclass)
		self.states = [Idle]
		### Add your states to self.states (but don't remove Idle)
		### YOUR CODE GOES BELOW HERE ###
		self.states.extend([MoveToEnemyStructure])
		self.states.extend([AttackTargetStructure])
		self.states.extend([MoveToGuardTower])
		self.states.extend([WaitingForBackup])
		self.states.extend([RecalculateAndMove])
		### YOUR CODE GOES ABOVE HERE ###

	def start(self):
		Minion.start(self)
		self.changeState(Idle)





############################
### Idle
###
### This is the default state of MyMinion. The main purpose of the Idle state is to figure out what state to change to and do that immediately.

class Idle(State):

	def enter(self, oldstate):
		State.enter(self, oldstate)
		# stop moving
		self.agent.stopMoving()

	def execute(self, delta = 0):
		State.execute(self, delta)
		### YOUR CODE GOES BELOW HERE ###

		def getClosest(array, current_Loc):
			closest_distance = float("inf")
			target_Item = None
			for structure_tuple in enumerate(array):
				structure_Item = structure_tuple[1]
				structure_Location = structure_Item.getLocation()
				structure_Distance = distance(current_Loc, structure_Location)
				if structure_Distance < closest_distance:
					target_Item = structure_Item
					closest_distance = structure_Item
			return target_Item

		my_team = self.agent.getTeam()
		my_location = self.agent.getLocation()
		friendly_Towers = self.agent.world.getTowersForTeam(my_team)
		enemyBases = self.agent.world.getEnemyBases(my_team)
		enemyTowers = self.agent.world.getEnemyTowers(my_team)
		guard_distance = 300
		if(friendly_Towers  == None or len(friendly_Towers) == 0):
			guard_distance = 200;
		if enemyTowers != None and len(enemyTowers) > 0:
			closestEnemyTower = getClosest(enemyTowers, my_location)
			self.agent.changeState(MoveToGuardTower, closestEnemyTower, guard_distance)
		elif enemyBases != None and len(enemyBases) > 0:
			closestEnemyBase = getClosest(enemyBases, my_location)
			self.agent.changeState(MoveToGuardTower, closestEnemyBase, guard_distance)
		# else:
		# 		print "Total minions spawned: ", self.agent.world.getBaseForTeam(self.agent.team).numSpawned


		# enemyTowers = self.agent.world.getEnemyTowers(my_team)
		# enemyBases = self.agent.world.getEnemyBases(my_team)
		# if enemyTowers != None and len(enemyTowers) > 0:
		# 	closestEnemyTower = getClosest(enemyTowers, my_location)
		# 	self.agent.changeState(MoveToEnemyStructure, closestEnemyTower)
		# elif enemyBases != None and len(enemyBases) > 0:
		# 	closestEnemyBase = getClosest(enemyBases, my_location)
		# 	self.agent.changeState(MoveToEnemyStructure, closestEnemyBase)
		### YOUR CODE GOES ABOVE HERE ###
		return None

##############################
### Taunt
###
### This is a state given as an example of how to pass arbitrary parameters into a State.
### To taunt someome, Agent.changeState(Taunt, enemyagent)

class Taunt(State):

	def parseArgs(self, args):
		self.victim = args[0]

	def execute(self, delta = 0):
		if self.victim is not None:
			print "Hey " + str(self.victim) + ", I don't like you!"
		self.agent.changeState(Idle)

##############################
### YOUR STATES GO HERE:

class MoveToEnemyStructure(State):
	def parseArgs(self, args):
		self.moveTarget = args[0]

	def enter(self, oldstate):
		self.agent.navigateTo(self.moveTarget.getLocation())

	def execute(self, delta = 0):
		self.agent.shoot()
		my_team = self.agent.getTeam()
		my_location = self.agent.getLocation()
		enemyTowers = self.agent.world.getEnemyTowers(my_team)
		enemyBases = self.agent.world.getEnemyBases(my_team)
		bullet_range = 150
		for tower in enemyTowers:
			towerDistance = distance(my_location, tower.getLocation())
			if tower in self.agent.getVisible() and towerDistance <= bullet_range:
				self.agent.changeState(AttackTargetStructure, tower)
		for base in enemyBases:
			baseDistance = distance(my_location, base.getLocation())
			if base in self.agent.getVisible() and baseDistance <= bullet_range:
				self.agent.changeState(AttackTargetStructure, base)



class AttackTargetStructure(State):

	def parseArgs(self,args):
		self.attackTarget = args[0]

	def enter(self, oldstate):
		self.agent.stopMoving()

	def execute(self, delta = 0):
		if not (self.attackTarget in self.agent.getVisible()):
			self.agent.changeState(RecalculateAndMove)

		my_location = self.agent.getLocation()
		target_location = self.attackTarget.getLocation()
		target_distance = distance(my_location, target_location)
		bullet_range = 150

		if target_distance > bullet_range:
			self.agent.changeState(RecalculateAndMove)
		else:
			self.agent.turnToFace(target_location)
			self.agent.shoot()



class MoveToGuardTower(State):
	def parseArgs(self, args):
		self.moveTarget = args[0]
		self.guardDistance = args[1]

	def enter(self, oldstate):
		self.agent.navigateTo(self.moveTarget.getLocation())

	def execute(self, delta = 0):
		self.agent.shoot()
		my_team = self.agent.getTeam()
		my_location = self.agent.getLocation()
		friendly_Base = self.agent.world.getBaseForTeam(my_team)
		friendly_Base_Location = friendly_Base.getLocation()
		range = distance(my_location, friendly_Base_Location)
		if range >= self.guardDistance:
			self.agent.changeState(WaitingForBackup)

class WaitingForBackup(State):

	def enter(self, oldstate):
		self.agent.stopMoving()

	def execute(self, delta=0):
		self.agent.shoot()

		def countOfNearbyTeamates(friendlyNPCsArray, currentLocation):
			count = 0
			for teamate in friendlyNPCsArray:
				teamate_Position = teamate.getLocation()
				nearby_distance = distance(currentLocation, teamate_Position)
				if(nearby_distance < 10):
					count = count + 1
			return count

		my_team = self.agent.getTeam()
		my_location = self.agent.getLocation()
		friendlyNPCs = self.agent.world.getNPCsForTeam(my_team)
		backupNumber = countOfNearbyTeamates(friendlyNPCs, my_location)
		if(backupNumber >= 5):
			self.agent.changeState(RecalculateAndMove)
		enemyTowers = self.agent.world.getEnemyTowers(my_team)
		if(len(enemyTowers)==0 or enemyTowers == None):
			self.agent.changeState(RecalculateAndMove)

class RecalculateAndMove(State):

	def enter(self, oldstate):
		State.enter(self, oldstate)
		# stop moving
		self.agent.stopMoving()

	def execute(self, delta = 0):
		State.execute(self, delta)
		### YOUR CODE GOES BELOW HERE ###

		def getClosest(array, current_Loc):
			closest_distance = float("inf")
			target_Item = None
			for structure_tuple in enumerate(array):
				structure_Item = structure_tuple[1]
				structure_Location = structure_Item.getLocation()
				structure_Distance = distance(current_Loc, structure_Location)
				if structure_Distance < closest_distance:
					target_Item = structure_Item
					closest_distance = structure_Item
			return target_Item

		my_team = self.agent.getTeam()
		my_location = self.agent.getLocation()
		enemyTowers = self.agent.world.getEnemyTowers(my_team)
		enemyBases = self.agent.world.getEnemyBases(my_team)
		if enemyTowers != None and len(enemyTowers) > 0:
			closestEnemyTower = getClosest(enemyTowers, my_location)
			self.agent.changeState(MoveToEnemyStructure, closestEnemyTower)

		elif enemyBases != None and len(enemyBases) > 0:
			closestEnemyBase = getClosest(enemyBases, my_location)
			self.agent.changeState(MoveToEnemyStructure, closestEnemyBase)
		# else:
		# 	print "Total minions spawned: ", self.agent.world.getBaseForTeam(self.agent.team).numSpawned
		return None


        ### YOUR CODE GOES ABOVE HERE ###

