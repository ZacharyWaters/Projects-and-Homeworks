package dk.itu.mario.engine.level.generator;

import java.util.*;

import dk.itu.mario.MarioInterface.Constraints;
import dk.itu.mario.MarioInterface.GamePlay;
import dk.itu.mario.MarioInterface.LevelGenerator;
import dk.itu.mario.MarioInterface.LevelInterface;
import dk.itu.mario.engine.level.Level;
import dk.itu.mario.engine.level.MyLevel;
import dk.itu.mario.engine.level.MyDNA;

import dk.itu.mario.engine.PlayerProfile;

import dk.itu.mario.engine.sprites.SpriteTemplate;
import dk.itu.mario.engine.sprites.Enemy;

public class MyLevelGenerator{

	public boolean verbose = true; //print debugging info
	
	double previousFitness = 0.0;
	int chainCount = 0;

	// MAKE ANY NEW MEMBER VARIABLES HERE

	// Called by the game engine.
	// Returns the level to be played.
	public Level generateLevel(PlayerProfile playerProfile)
	{
		// Call genetic algorithm to optimize to the player profile
		MyDNA dna = this.geneticAlgorithm(playerProfile);

		// Post process
		dna = this.postProcess(dna);

		// Convert the solution to the GA into a Level
		MyLevel level = new MyLevel(dna, LevelInterface.TYPE_OVERGROUND);

		if (this.verbose) {
			System.out.println("Solution: " + dna + " fitness: " + playerProfile.evaluateLevel(level));
		}

		return (Level)level;
	}

	// Genetic Algorithm implementation
	private MyDNA geneticAlgorithm (PlayerProfile playerProfile)
	{
		// Set the population size
		int populationSize = getPopulationSize();

		// Make the population array
		ArrayList<MyDNA> population = new ArrayList<MyDNA>();

		// Make the solution, which is initially null
		MyDNA solution = null;

		// Generate a random population
		for (int i=0; i < populationSize; i++) {
			MyDNA newIndividual = this.generateRandomIndividual();
			newIndividual.setFitness(this.evaluateFitness(newIndividual, playerProfile));
			population.add(newIndividual);
		}
		if (this.verbose) {
			System.out.println("Initial population:");
			printPopulation(population);
		}

		// Iteration counter
		int count = 0;

		// Iterate until termination criteria met
		while (!this.terminate(population, count)) {
			// Make a new, possibly larger population
			ArrayList<MyDNA> newPopulation = new ArrayList<MyDNA>();

			// Keep track of individual's parents (for this iteration only)
			Hashtable parents = new Hashtable();

			// Mutuate a number of individuals
			ArrayList<MyDNA> mutationPool = this.selectIndividualsForMutation(population);
			for (int i=0; i < mutationPool.size(); i++) {
				MyDNA parent = mutationPool.get(i);
				// Mutate
				MyDNA mutant = parent.mutate();
				// Evaluate fitness
				double fitness = this.evaluateFitness(mutant, playerProfile);
				mutant.setFitness(fitness);
				// Add mutant to new population
				newPopulation.add(mutant);
				// Create a list of parents and remember it in a hash
				ArrayList<MyDNA> p = new ArrayList<MyDNA>();
				p.add(parent);
				parents.put(mutant, p);
			}


			// Do Crossovers
			for (int i=0; i < this.numberOfCrossovers(); i++) {
				// Pick two parents
				MyDNA parent1 = this.pickIndividualForCrossover(newPopulation, null);
				MyDNA parent2 = this.pickIndividualForCrossover(newPopulation, parent1);

				if (parent1 != null && parent2 != null) {
					// Crossover produces one or more children
					ArrayList<MyDNA> children = parent1.crossover(parent2);

					// Add children to new population and remember their parents
					for (int j=0; j < children.size(); j++) {
						// Get a child
						MyDNA child = children.get(j);
						// Evaluate fitness
						double fitness = this.evaluateFitness(child, playerProfile);
						child.setFitness(fitness);
						// Add it to new population
						newPopulation.add(child);
						// Create a list of parents and remember it in a hash
						ArrayList<MyDNA> p = new ArrayList<MyDNA>();
						p.add(parent1);
						p.add(parent2);
						parents.put(child, p);
					}
				}

			}

			// Cull the population
			// There is more than one way to do it.
			if (this.competeWithParentsOnly()) {
				population = this.competeWithParents(population, newPopulation, parents);
			}
			else {
				population = this.globalCompetition(population, newPopulation);
			}

			//increment counter
			count = count + 1;

			if (this.verbose) {
				MyDNA best = this.getBestIndividual(population);
				System.out.println("" + count + ": Best: " + best + " fitness: " + best.getFitness());
			}
		}

		// Get the winner
		solution = this.getBestIndividual(population);

		return solution;
	}

	// Create a random individual.
	private MyDNA generateRandomIndividual ()
	{
		MyDNA individual = new MyDNA();
		// YOUR CODE GOES BELOW HERE
		Random rand = new Random();
		int individuals_length = rand.nextInt(50) + 50;
		//int individuals_length = 60
		if((individuals_length%2)!=0) {
			individuals_length = individuals_length + 1;
		}
		
		char[] numConstants = new char[] {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
		char[] charConstants = new char[] {'G', 'S', 'C', 'H', 'M'};
		
		String individualString = "";
		int counter = 0;
		
		char newChar;
		while (counter < individuals_length) {
			if((counter%2)==0) {
				// even, should be a letter character
				int charConstantsIndex = rand.nextInt(charConstants.length);
				newChar = charConstants[charConstantsIndex];
				individualString = individualString + newChar;
			} else{
				// odd, should be a numeric character
				int numConstantsIndex = rand.nextInt(numConstants.length);
				newChar = numConstants[numConstantsIndex];
			individualString = individualString + newChar;
			}
			counter = counter + 1;
			
		}
		individual.setChromosome(individualString);
		// YOUR CODE GOES ABOVE HERE
		return individual;
	}

	// Returns true if the genetic algorithm should terminate.
	private boolean terminate (ArrayList<MyDNA> population, int count)
	{
		boolean decision = false;
		// YOUR CODE GOES BELOW HERE
		
		double bestValue = this.getBestIndividual(population).getFitness();
		// Track convergence
		if (bestValue == previousFitness) {
			chainCount++;
		}
		else {
			previousFitness = bestValue;
			chainCount = 0;
		}
		
		if ((bestValue >= 0.8)){
			decision = true;
		}
		if(count > 5000){
			decision = true;
		}
		// YOUR CODE GOES ABOVE HERE
		return decision;
	}

	// Return a list of individuals that should be copied and mutated.
	private ArrayList<MyDNA> selectIndividualsForMutation (ArrayList<MyDNA> population)
	{
		ArrayList<MyDNA> selected = new ArrayList<MyDNA>();
		// YOUR CODE GOES BELOW HERE
		
		
		Random rand = new Random();
		int popLength = population.size();
		int numberOfMutations = rand.nextInt(popLength / 2);
		int modifier = chainCount;
		if(modifier > popLength - numberOfMutations){
				modifier = popLength - numberOfMutations;
			}
		numberOfMutations = numberOfMutations + modifier;
		
		int modifier2 = chainCount;
		if(modifier2 > 50){
				modifier2 = 50;
			}
		double upperbound = ((double)((double)50 + (double)modifier2))/((double)100);
		
		int mutCounter = 0;
		int x = rand.nextInt(2); 
        //Start at front
		if(x == 0){
			int index = 0;
			while((mutCounter < numberOfMutations) && (index < popLength)){
				//x = rand.nextInt(2); 
				double y = rand.nextDouble();
				if(y < upperbound){
					selected.add(population.get(index));
					mutCounter = mutCounter + 1;
				}
				index = index + 1;
			}
		} 
		//Start at back
		else{
			int index = popLength - 1;
			while((mutCounter < numberOfMutations) && (index > -1)){
				//x = rand.nextInt(2); 
				double y = rand.nextDouble();
				if(y < upperbound){
					selected.add(population.get(index));
					mutCounter = mutCounter + 1;
				}
				index = index - 1;
			}
		}
		// YOUR CODE GOES ABOVE HERE
		return selected;
	}

	// Returns the size of the population.
	private int getPopulationSize ()
	{
		int num = 1; // Default needs to be changed
		// YOUR CODE GOES BELOW HERE
		num = 400;
		// YOUR CODE GOES ABOVE HERE
		return num;
	}

	// Returns the number of times crossover should happen per iteration.
	private int numberOfCrossovers ()
	{
		int num = 0; // Default is no crossovers
		// YOUR CODE GOES BELOW HERE
		num = 200;
		// YOUR CODE GOES ABOVE HERE
		return num;

	}

	// Pick one of the members of the population that is not the same as excludeMe
	private MyDNA pickIndividualForCrossover (ArrayList<MyDNA> population, MyDNA excludeMe)
	{
		MyDNA picked = null;
		// YOUR CODE GOES BELOW HERE
		int popSize = population.size();
		if(popSize == 1){
			return null;
		}
		
		double highestFitness = Double.NEGATIVE_INFINITY;
	
		if(!population.isEmpty()) {
			Random rand = new Random();
			//int x = rand.nextInt(2);
			double y = rand.nextDouble();
			int modifier = chainCount;
			if(chainCount > 25){
				modifier = 25;
			}
			double lowerBound = ((double)(75 + modifier))/(double)100;
			//System.out.println(lowerBound);
			if((y > lowerBound)) {
				for(int i = 0; i < popSize; i++){
					MyDNA member = population.get(i);
					if (member != excludeMe) {
						if (member.getFitness() > highestFitness) {
							highestFitness = member.getFitness();
							picked = member;
						}
					}
				}
			} else {
				int x = rand.nextInt(popSize);
				picked = population.get(x);
				while((picked == excludeMe)){
					x = rand.nextInt(popSize);
					picked = population.get(x);
					}
			}
			
		} else {
			for(int i = 0; i < popSize; i++){
				MyDNA member = population.get(i);
				if (member != excludeMe) {
					if (member.getFitness() > highestFitness) {
						highestFitness = member.getFitness();
						picked = member;
					}
				}
			}
		}
		
		// YOUR CODE GOES ABOVE HERE
		if (picked == excludeMe) {
			return null;
		}
		else {
			return picked;
		}
	}

	// Returns true if children compete to replace parents.
	// Retursn false if the the global population competes.
	private boolean competeWithParentsOnly ()
	{
		boolean doit = false;
		// YOUR CODE GOES BELOW HERE

		// YOUR CODE GOES ABOVE HERE
		return doit;
	}

	// Determine if children are fitter than parents and keep the fitter ones.
	private ArrayList<MyDNA> competeWithParents (ArrayList<MyDNA> oldPopulation, ArrayList<MyDNA> newPopulation, Hashtable parents)
	{
		ArrayList<MyDNA> finalPopulation = new ArrayList<MyDNA>();
		// YOUR CODE GOES BELOW HERE

		// YOUR CODE GOES ABOVE HERE
		if (finalPopulation.size() != this.getPopulationSize()) {
			throw new IllegalStateException("Population not the correct size.");
		}
		return finalPopulation;
	}

	// Combine the old population and the new population and return the top fittest individuals.
	private ArrayList<MyDNA> globalCompetition (ArrayList<MyDNA> oldPopulation, ArrayList<MyDNA> newPopulation)
	{
		ArrayList<MyDNA> finalPopulation = new ArrayList<MyDNA>();
		// YOUR CODE GOES BELOW HERE
		ArrayList<MyDNA> bothOldandNew = new ArrayList<>();
        bothOldandNew.addAll(oldPopulation);
        bothOldandNew.addAll(newPopulation);
        Collections.sort(bothOldandNew);
		int index =  bothOldandNew.size() - 1;
		while(index > -1){
			if (finalPopulation.size() < this.getPopulationSize()) {
				finalPopulation.add(bothOldandNew.get(index));
			}
			index = index - 1;
		}


		// YOUR CODE GOES ABOVE HERE
		if (finalPopulation.size() != this.getPopulationSize()) {
			throw new IllegalStateException("Population not the correct size.");
		}
		return finalPopulation;
	}

	// Return the fittest individual in the population.
	private MyDNA getBestIndividual (ArrayList<MyDNA> population)
	{
		MyDNA best = population.get(0);
		double bestFitness = Double.NEGATIVE_INFINITY;
		for (int i=0; i < population.size(); i++) {
			MyDNA current = population.get(i);
			double currentFitness = current.getFitness();
			if (currentFitness > bestFitness) {
				best = current;
				bestFitness = currentFitness;
			}
		}
		return best;
	}

	// Changing this function is optional.
	private double evaluateFitness (MyDNA dna, PlayerProfile playerProfile)
	{
		double fitness = 0.0;
		// YOUR CODE GOES BELOW HERE
		MyLevel level = new MyLevel(dna, LevelInterface.TYPE_OVERGROUND);
		fitness = playerProfile.evaluateLevel(level);
		// YOUR CODE GOES ABOVE HERE
		return fitness;
	}

	private MyDNA postProcess (MyDNA dna)
	{
		// YOUR CODE GOES BELOW HERE

		// YOUR CODE GOES ABOVE HERE
		return dna;
	}

	//for this to work, you must implement MyDNA.toString()
	private void printPopulation (ArrayList<MyDNA> population)
	{
		for (int i=0; i < population.size(); i++) {
			MyDNA dna = population.get(i);
			System.out.println("Individual " + i + ": " + dna + " fitness: " + dna.getFitness());
		}
	}

	// MAKE ANY NEW MEMBER FUNCTIONS HERE

}
