import sys

def isRotated(s1, s2):
	if (len(s1) == len(s2)):
		for r in range(len(s1)):
			for i in range(len(s1)):
				if s1[i] != s2[(i + r) % len(s1)]: break
				elif i == len(s1) - 1: return 1
	return 0

if len(sys.argv) < 3:
	print("Requires 2 strings as arguments.")
	exit(1)

print("true" if isRotated(sys.argv[1], sys.argv[2]) else "false")
print("\n")