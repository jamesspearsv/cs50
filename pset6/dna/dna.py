from csv import reader, DictReader
from sys import argv, exit


if len(argv) != 3:
    print("Usage error: dna.py <database.csv> <sequence.txt>")
    exit(1)

# Opens DNA sequences and stores as string
with open(argv[2], "r") as txt:
    DNA_sequence = txt.read()

#Opens database.csv and stores keys as dictionary
with open(argv[1], "r") as csv:
    database_reader = reader(csv)
    for row in database_reader:
        keys = row
        keys.pop(0)
        break

sequence_keys = {}
for item in keys:
    sequence_keys[item] = 1


#Calculate STR counts
for key in sequence_keys:
    l = len(key)
    temp = 0
    tempMax = 0
    for i in range(len(DNA_sequence)):
        while temp > 0:
            temp -= 1
            continue
        tempSequence = DNA_sequence[i : i + l]
        if DNA_sequence[i : i + l] == key:
            while DNA_sequence[i - l : i] == DNA_sequence[i : i + l]:
                temp += 1
                i += l
            if temp > tempMax:
                tempMax = temp

    sequence_keys[key] += tempMax


#Compare STR counts to database
with open(argv[1], "r") as csv:
    people_database = DictReader(csv, )
    for person in people_database:
        match = 0
        for key in sequence_keys:
            if sequence_keys[key] == int(person[key]):
                match += 1

            if match == len(sequence_keys):
                print(person['name'])
                exit(0)
    print("No match")