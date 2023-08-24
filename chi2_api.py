from scipy.stats import chi2_contingency

file = open("data.txt","r")
lines = file.read().split(",")
for i in range(len(lines)):
    lines[i] = int(lines[i])
file.close()

cols = 3 # this is a length of a single range[] element

data = [lines[x:x+3] for x in range(0, len(lines),3)]

stat,p,dof,expected = chi2_contingency(data)

alpha = 0.05
print("p-value:", p)