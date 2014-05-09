#!/usr/bin/env python
# All work authored by Jordan Bayles
# Written for ECE 478: Network Security
# All rights reserved

import hashlib
import sys

valid_chars = "abcdefghijklmnopqrstuvwyxz"
valid_ints = "0123456789"
truncate = 4 * 2 # characters

def generate_fields(element_list, max_len):
    li = list(element_list)
    index = [0,0]

    for _ in range(1, max_len):
        index = [index[1], len(li)]
        for string in li[index[0]:]:
            for character in element_list:
                li.append(string + character)

    return li

def generate_hashes(hhmm, max_len):

    # timestamp format DDMMYYYYHHmm
    user_timestamp = "09052014" + hhmm

    usernames = generate_fields(valid_chars, max_len)
    timestamps = generate_fields(valid_ints, max_len)

    # generate the hashes with timestamp=08052014hhmm
    user_hashes = [None]*len(usernames)
    for index, value in enumerate(usernames):
        data = value + "." + user_timestamp
        user_hashes[index] = hashlib.md5(data.encode('utf-8')).hexdigest()[:truncate]

    # generate the hashes with username=admin
    admin_hashes = [None]*len(timestamps)
    for index, value in enumerate(timestamps):
        data = "admin." + value
        admin_hashes[index] = hashlib.md5(data.encode('utf-8')).hexdigest()[:truncate]

    return usernames, timestamps, user_hashes, admin_hashes

def find_collisions(hhmm, max_len):
    users, timestamps, usersh, adminsh = generate_hashes(hhmm, max_len)

    # run through and check for collisions
    for i, userh in enumerate(usersh):
        for j, adminh in enumerate(adminsh):
            if userh == adminh:
                print("Found collision")
                print(users[i], timestamps[j])
                return users[i], timestamps[j]

    return None

def main():
    find_collisions(sys.argv[1], int(sys.argv[2]))
    return 0

if __name__ == "__main__":
    main()
