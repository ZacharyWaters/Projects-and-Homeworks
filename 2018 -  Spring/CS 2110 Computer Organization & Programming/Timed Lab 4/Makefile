all: clean test

autograde: clean test-autograde grade

test:
	lc3test timedlab4_test.xml timedlab4.asm

test-autograde:
	lc3test timedlab4_test.xml timedlab4.asm -result=timedlab4_test.json

grade:
	python3 ./grade.py > grader_output.txt

clean:
	rm -f *.json
	rm -f grader_output.txt
