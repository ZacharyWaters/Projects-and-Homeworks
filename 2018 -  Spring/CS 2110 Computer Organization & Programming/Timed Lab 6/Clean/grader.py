import json
import subprocess

test_list_filename = "tests.json"

with open(test_list_filename, 'r') as f:
    tests = json.load(f)
    tests_dict = {}
    for row in tests:
        tests_dict[row["test_name"]] = row

# run valgrind on all tests
for row in tests:
    test_name = row["test_name"]
    # print(test_name)
    print("#", end='', flush=True)
    try:
        status = subprocess.run(
            ["valgrind", "--error-exitcode=122", "--leak-check=full", "--xml=yes", "--xml-file=/dev/null",
             "./tl06tests_single", test_name],
            timeout=5,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE)

        # test pass
        tests_dict[test_name]["pass"] = status.returncode == 0
        tests_dict[test_name]["message"] = ""

        # test pass
        if status.returncode == 0:
            tests_dict[test_name]["pass"] = True
            tests_dict[test_name]["message"] = ""

        # valgrind failed
        if status.returncode == 122:
            tests_dict[test_name]["pass"] = False
            tests_dict[test_name]["message"] = "valgrind failed"

        # any other kind of failure
        elif status.returncode != 0:
            tests_dict[test_name]["pass"] = False
            tests_dict[test_name]["message"] = status.stdout.decode("utf-8") + status.stderr.decode("utf-8")

    # test failed via timeout
    except subprocess.TimeoutExpired:
        tests_dict[test_name]["pass"] = False
        tests_dict[test_name]["message"] = "test timeout"

# print out test results
print("\nBEGIN_GRADELOG")
total_score = 0
total_out_of = 0
for row in tests:
    test_entry = tests_dict[row["test_name"]]

    score = test_entry["test_score"] if test_entry["pass"] else 0
    out_of = test_entry["test_score"]

    total_score += score
    total_out_of += out_of

    print("(%d/%d) [%s] %s %s" % (
        score,
        out_of,
        "PASS" if test_entry["pass"] else "FAIL",
        row["test_name"],
        (" - " if len(test_entry["message"]) > 0 else "") + test_entry["message"]))

print("\n")
print("--------------------------\n|  (%d/%d) FINAL SCORE |\n--------------------------\n" % (total_score, total_out_of))

print(json.dumps({"scores": {"Correctness": total_out_of * float(total_score) / float(total_out_of)}}))

