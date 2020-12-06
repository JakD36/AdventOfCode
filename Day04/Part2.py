import io
import re

eyeCols = ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"]

def IsEcl(x : str) -> bool:
    return x in eyeCols

def IsPid(x : str) -> bool:
    return len(x) == 9 and x.isdigit()

def IsEyr(x : str) -> bool:
    try:
        val = int(x)
        return 2020 <= val <= 2030
    except:
        return False

def IsHcl(x : str) -> bool:
    return re.match("#[0-9a-f]{6}",x) is not None and len(x) == 7

def IsByr(x : str) -> bool:
    try:
        val = int(x)
        return 1920 <= val <= 2002
    except:
        return False

def IsIyr(x : str) -> bool:
    try:
        val = int(x)
        return 2010 <= val <= 2020
    except:
        return False

def IsHgt(x : str) -> bool:
    if x.endswith("in"):
        try:
            val = int(x[:-2])
            return 59 <= val <= 76
        except:
            return False
    elif x.endswith("cm"):
        try:
            val = int(x[:-2])
            return 150 <= val <= 193
        except:
            return False

    return False

requiredFields = {"ecl": IsEcl,
                  "pid": IsPid,
                  "eyr": IsEyr,
                  "hcl": IsHcl,
                  "byr": IsByr,
                  "iyr": IsIyr,
                  "hgt": IsHgt}

def IsValid(passport : dict) -> bool:
    for key,validate in requiredFields.items():
        if key not in passport.keys():
            return False
        else:
            if not validate(passport[key]):
                return False
    return True



f = open("input.txt",'r')
lines = f.readlines()
passport = {}

count = 0
start = 0
for i in range(len(lines)):
    if lines[i].isspace():
        #build dict
        batch = lines[start:i]
        for line in batch:
            keyVals = line.strip().split(' ')
            for j in range(len(keyVals)):
                key,val = keyVals[j].split(':')
                passport.update({key:val})
        if IsValid(passport):
            count += 1
        passport.clear()
        start = i + 1

batch = lines[start:]
for line in batch:
    keyVals = line.strip().split(' ')
    for j in range(len(keyVals)):
        key,val = keyVals[j].split(':')
        passport.update({key:val})
if IsValid(passport):
    count += 1

print(count)



