package model;

public class Employee {
	private String name;
	private int payAmount;
	private int hours;
	
	public Employee(String s, int p) {
		name  = s;
		payAmount = p;
		hours = 0;
	}
	
	public void setHours(int h) {
		hours = h;
	}
	
	public String getName() {
		return name;
	}
	
	public int getPayAmount() {
		return payAmount;
	}

	public int getHours() {
		return hours;
	}

}
