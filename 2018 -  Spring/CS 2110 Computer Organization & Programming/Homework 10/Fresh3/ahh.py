import os
import subprocess

types = {
    "application/gzip; charset=binary": "tar.gz",        # tar.gz
    "text/x-c; charset=us-ascii": "c",                  # c
    "application/x-tar; charset=binary": "tar",       # tar
    "application/zip; charset=binary" : "zip",        # zip
    "text/plain; charset=us-ascii": "e"             # python
}

def process_filename(fname):
    res = subprocess.run(["file", "-b", "-i", fname], stdout=subprocess.PIPE)
    type = res.stdout.decode("utf-8").strip()

    type2 = types[type]
    print("file: \"%s\", type: \"%s\"" % (fname, type2))

    # tar.gz
    if type2 == "tar.gz":

        results = subprocess.run(["tar", "-tf", fname], stdout=subprocess.PIPE)
        stuff2 = results.stdout.decode("utf-8")
        archive_files = stuff2.split()

        if "array_list.c" in archive_files:
            res1 = subprocess.run(["tar", "-xzf", fname, "array_list.c"])
        if "circ_list.c" in archive_files:
            res2 = subprocess.run(["tar", "-xzf", fname, "circ_list.c"])

    # C file
    if type2 == "c":
        res = subprocess.run(["cp", fname, "array_list.c"])

    # tar
    if type2 == "tar":
        results = subprocess.run(["tar", "-tf", fname], stdout=subprocess.PIPE)
        stuff2 = results.stdout.decode("utf-8")
        archive_files = stuff2.split()

        if "array_list.c" in archive_files:
            res1 = subprocess.run(["tar", "-xf", fname, "array_list.c"])
        if "circ_list.c" in archive_files:
            res2 = subprocess.run(["tar", "-xf", fname, "circ_list.c"])

    # zip
    if type == "zip":
        print("lol I'm not dealing with this")


# process_filename("zwaters3@gatech.edu_2_hw10") # tar.gz
# process_filename("zfarooq3@gatech.edu_1_hw10") # c
# process_filename("tlobl3@gatech.edu_2_hw10") # tar
# process_filename("drammohan3@gatech.edu_1_hw10") # zip
process_filename("hw10")