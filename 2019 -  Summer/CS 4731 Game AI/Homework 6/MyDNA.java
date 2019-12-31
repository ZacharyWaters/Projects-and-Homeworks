package dk.itu.mario.engine.level;

import java.util.Random;
import java.util.*;

//Make any new member variables and functions you deem necessary.
//Make new constructors if necessary
//You must implement mutate() and crossover()


public class MyDNA extends DNA
{

	public int numGenes = 25; //number of genes

	// Use these constants to make your DNA strings.

	// Represents a gap in the floor that Mario can fall through and die.
	public static final char GAP_CHAR = 'G';
	// Represents a straight, flat section of ground.
	public static final char STRAIGHT_CHAR = 'S';
	// Represents ground with coins above it.
	public static final char COINS_CHAR = 'C';
	// Represents a set of stairs that Mario needs to jump over.
	public static final char HILL_CHAR = 'H';
	// Represents ground with monsters over it (e.g., goombas, koopas).
	public static final char MONSTERS_CHAR = 'M';

	// Return a new DNA that differs from this one in a small way.
	// Do not change this DNA by side effect; copy it, change the copy, and return the copy.
	public MyDNA mutate ()
	{
		MyDNA copy = new MyDNA();
		//YOUR CODE GOES BELOW HERE
		Random rand = new Random();

		char[] oldChromosome = this.getChromosome().toCharArray();
		int oldChromosomeIndex = rand.nextInt(this.getLength());
		char oldChar = oldChromosome[oldChromosomeIndex];
		
		char newChar;
		
		if((oldChromosomeIndex%2)==0) {
			// even, should be a letter character
			char[] charConstants = new char[] {'G', 'S', 'C', 'H', 'M'};
			int charConstantsIndex = rand.nextInt(charConstants.length);
			newChar = charConstants[charConstantsIndex];
			if(oldChar == newChar){
				charConstantsIndex = rand.nextInt(charConstants.length);
				newChar = charConstants[charConstantsIndex];
			}
			
		} else {
			// odd, should be a numeric character
			char[] numConstants = new char[] {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
			int numConstantsIndex = rand.nextInt(numConstants.length);
			newChar = numConstants[numConstantsIndex];
			if(oldChar == newChar){
				numConstantsIndex = rand.nextInt(numConstants.length);
				newChar = numConstants[numConstantsIndex];
			}
		}
		oldChromosome[oldChromosomeIndex] = newChar;
		copy.setChromosome(String.valueOf(oldChromosome));
		//YOUR CODE GOES ABOVE HERE
		return copy;
	}

	// Do not change this DNA by side effect
	public ArrayList<MyDNA> crossover (MyDNA mate)
	{
		ArrayList<MyDNA> offspring = new ArrayList<MyDNA>();
		//YOUR CODE GOES BELOW HERE
		String fatherString = this.getChromosome().toString();
		String motherString = mate.getChromosome().toString();
		
		int fatherLength = fatherString.length();
		int motherLength = motherString.length();
		
		
		MyDNA childOneDNA = new MyDNA();
		MyDNA childTwoDNA = new MyDNA();
		String childOneChromosome = "";
		String childTwoChromosome = "";
		
		int fatherHalf = fatherLength/2;
		if((fatherHalf%2)!=0){
			fatherHalf = fatherHalf - 1;
		}
		
		int motherHalf = motherLength/2;
		if((motherHalf%2)!=0){
			motherHalf = motherHalf - 1;
		}
		
		String fatherFrontHalf = fatherString.substring(0, fatherHalf);
		String fatherBackHalf = fatherString.substring(fatherHalf, fatherLength);
		String motherFrontHalf = motherString.substring(0, motherHalf);
		String motherBackHalf = motherString.substring(motherHalf, motherLength);
		
		childOneChromosome = fatherFrontHalf + motherBackHalf;
		childTwoChromosome = motherFrontHalf + motherBackHalf;
		
		//One Will always be added
		if(childOneChromosome.length()/2 > numGenes){
				childOneDNA.setChromosome(childOneChromosome);
				offspring.add(childOneDNA);;
		}
		
		//Two Will always be added
		if(childTwoChromosome.length()/2 > numGenes){
				childTwoDNA.setChromosome(childTwoChromosome);
				offspring.add(childTwoDNA);;
		}
		
		int maxLength = numGenes * 3;
		Random rand=new Random();
		//Makes the midpoints random, 1/5 chance of happening
		int x = rand.nextInt(4); 
		if(x == 0){
			fatherHalf = rand.nextInt(fatherLength);
			motherHalf = rand.nextInt(motherLength);
			if((fatherHalf%2)!=0){
				fatherHalf = fatherHalf - 1;
			}
		
			if((motherHalf%2)!=0){
				motherHalf = motherHalf - 1;
			}
		
			fatherFrontHalf = fatherString.substring(0, fatherHalf);
			fatherBackHalf = fatherString.substring(fatherHalf, fatherLength);
			motherFrontHalf = motherString.substring(0, motherHalf);
			motherBackHalf = motherString.substring(motherHalf, motherLength);
		}
		
		//Three
		x = rand.nextInt(2); 
		if(x == 1){
			MyDNA childThreeDNA = new MyDNA();
			String childThreeChromosome = "";
			childThreeChromosome = motherBackHalf + fatherFrontHalf;
			if((childThreeChromosome.length()/2 > numGenes) && (childThreeChromosome.length() < maxLength)){
				childThreeDNA.setChromosome(childThreeChromosome);
				offspring.add(childThreeDNA);
			}
		}
		//Four
		x = rand.nextInt(2); 
		if(x == 1){
			MyDNA childFourDNA = new MyDNA();
			String childFourChromosome = "";
			childFourChromosome = fatherBackHalf + motherFrontHalf;
			if((childFourChromosome.length()/2 > numGenes) && (childFourChromosome.length() < maxLength)){
				childFourDNA.setChromosome(childFourChromosome);
				offspring.add(childFourDNA);
			}
		}
		//Five
		x = rand.nextInt(2); 
		if(x == 1){
			MyDNA childFiveDNA = new MyDNA();
			String childFiveChromosome = "";
			childFiveChromosome = fatherFrontHalf + fatherFrontHalf;
			if((childFiveChromosome.length()/2 > numGenes) && (childFiveChromosome.length() < maxLength)){
				childFiveDNA.setChromosome(childFiveChromosome);
				offspring.add(childFiveDNA);
			}
		}
		//Six
		x = rand.nextInt(2); 
		if(x == 1){
			MyDNA childSixDNA = new MyDNA();
			String childSixChromosome = "";
			childSixChromosome = motherFrontHalf + motherFrontHalf;
			if((childSixChromosome.length()/2 > numGenes) && (childSixChromosome.length() < maxLength)){
				childSixDNA.setChromosome(childSixChromosome);
				offspring.add(childSixDNA);
			}
		}
		//Seven
		x = rand.nextInt(2); 
		if(x == 1){
			MyDNA childSevenDNA = new MyDNA();
			String childSevenChromosome = "";
			childSevenChromosome = fatherBackHalf + fatherBackHalf;
			if((childSevenChromosome.length()/2 > numGenes) && (childSevenChromosome.length() < maxLength)){
				childSevenDNA.setChromosome(childSevenChromosome);
				offspring.add(childSevenDNA);
			}
		}
		//Eight
		x = rand.nextInt(2); 
		if(x == 1){
			MyDNA childEightDNA = new MyDNA();
			String childEightChromosome = "";
			childEightChromosome = motherBackHalf + motherBackHalf;
			if((childEightChromosome.length()/2 > numGenes) && (childEightChromosome.length() < maxLength)){
				childEightDNA.setChromosome(childEightChromosome);
				offspring.add(childEightDNA);
			}
		}

		
		//YOUR CODE GOES ABOVE HERE
		return offspring;
	}

	// Optional, modify this function if you use a means of calculating fitness other than using the fitness member variable.
	// Return 0 if this object has the same fitness as other.
	// Return -1 if this object has lower fitness than other.
	// Return +1 if this objet has greater fitness than other.
	public int compareTo(MyDNA other)
	{
		int result = super.compareTo(other);
		//YOUR CODE GOES BELOW HERE

		//YOUR CODE GOES ABOVE HERE
		return result;
	}


	// For debugging purposes (optional)
	public String toString ()
	{
		String s = super.toString();
		//YOUR CODE GOES BELOW HERE

		//YOUR CODE GOES ABOVE HERE
		return s;
	}

	public void setNumGenes (int n)
	{
		this.numGenes = n;
	}

}

