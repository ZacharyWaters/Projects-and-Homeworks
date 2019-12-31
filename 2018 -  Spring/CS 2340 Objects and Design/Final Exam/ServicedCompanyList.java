package model;

import java.util.ArrayList;
import java.util.LinkedList;

public class ServicedCompanyList {
    private LinkedList<Company> companies;
    
    public ServicedCompanyList() {
    	companies = new LinkedList<Company>();
    }
    
    public int computeTotalPayroll() {
    	int result = 0;
    	for (Company comp : companies) {
    		int companyTotal = 0;
    		ArrayList<Employee> emps = comp.getEmployees();
    		for (Employee emp : emps) {
    			companyTotal += (emp.getHours() * emp.getPayAmount());
    		}
    		result += companyTotal;
    	}
    	
    	return result;
    }
    
    public int computePartialPayroll(int companySize) {
    	int result = 0;
    	for (Company comp : companies) {
    		int companyTotal = 0;
    		ArrayList<Employee> emps = comp.getEmployees();
    		if (emps.size() >= companySize) {
    			for (Employee emp : emps) {
        			companyTotal += (emp.getHours() * emp.getPayAmount());
        		}
    			result += companyTotal;
    		}
    	}
    	return result;
    }
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

	}

}
