package model;

import java.util.ArrayList;
import java.util.List;

public class Company {
    private ArrayList<Employee> employees;
	private String name;
     
     public Company(String s) {
    	 name = s;
    	 employees = new ArrayList<Employee>();
     }

	public ArrayList<Employee> getEmployees() {
		return employees;
	}
}
