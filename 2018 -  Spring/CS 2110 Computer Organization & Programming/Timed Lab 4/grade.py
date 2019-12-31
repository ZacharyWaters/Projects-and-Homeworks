import json

tests = [
    {"test": "timedlab4_test", "points": 100},
]

total_score = 0

for test in tests:
    try:
        with open(test['test'] + ".json", 'r') as f:
            jtests = json.load(f)

            jtest_total = 0
            jtest_out_of = 0

            for jtest in jtests['tests']:
                num_warnings = float(jtest['num_warnings'])
                if num_warnings > 0:
                    print("test \"%s\" contained %d warnings, score set to 0" % (jtest['name'], num_warnings))
                else:
                    jtest_total += float(jtest['score'])
                jtest_out_of += float(jtest['max_score'])

            test_out_of = float(test['points'])
            normalized_score = jtest_total / jtest_out_of * test_out_of

            print("%s [actual: %d/%d] [normalized: %d/%d]" % (
                test['test'], jtest_total, jtest_out_of, normalized_score, test_out_of))

            total_score += normalized_score

    except FileNotFoundError:
        print("Could not read result file \"%s\", marking as 0" % (test['test']))

print(json.dumps({"scores":{"Correctness": total_score}}))



