import re
import sys
import os


def replace(search, replace, string):
	matcher = re.search(search, string)
	result = re.sub(search, replace, string)

	if matcher is not None and len(matcher.groups()) == result.count("{}"):
		result = result.format(*matcher.groups())
	
	return result
flags = {
	"simulation": False
}

paths = []
filters = []
actions = []

for (i, arg) in enumerate(sys.argv):
	if arg == "-s":
		flags["simulation"] = True
	elif arg == "--path":
		paths.append(sys.argv[i + 1])
	elif arg == "--paths":
		pass # TODO support passing in an array of paths.
	elif arg == "--recipe":
		recipe = open(sys.argv[i + 1])
		lines = recipe.readlines()
		
		for (i, line) in enumerate(lines):
			if line.startswith("path"):
				paths.append(line.split(":")[1].strip("\n"))
			elif line.startswith("exclude"):
				if lines[i + 1].lstrip().startswith("contains"):
					filters.append(("contains", lines[i + 1].split(":")[1].rstrip("\n")))
			elif line.startswith("prefix"):
				actions.append(("prefix", line.split(":")[1].strip("\n")))
			elif line.startswith("case"):
				actions.append(("case", line.split(":")[1].strip("\n")))
			elif line.startswith("regex"):
				actions.append(("regex", lines[i + 1].lstrip("\tsearch:").rstrip("\n"), lines[i + 2].lstrip("\treplace:").rstrip("\n")))
			elif line.startswith("replace"):
				actions.append(("replace", lines[i + 1].lstrip("\tsearch:").rstrip("\n"), lines[i + 2].lstrip("\treplace:").rstrip("\n")))

files = []

for path in paths:
	if not path.endswith(os.sep):
		path += os.sep
	files.extend([(path, file) for file in os.listdir(path)])

for f in filters:
	if f[0] == "contains":
		results = filter(lambda file: file[1].find(f[1]) == -1, files)
		files = [file for file in results]

results = []

for file in files:
	name = re.split(r"\.[a-zA-Z0-9]+$", file[1])[0]
	extension = re.search(r"\.[a-zA-Z0-9]+$", file[1]).group(0)

	for action in actions:
		if action[0] == "regex":
			name = replace(action[1], action[2], name)
		elif action[0] == "replace":
			name = name.replace(action[1], action[2])
		elif action[0] == "prefix":
			name = action[1] + name
		elif action[0] == "suffix":
			name = name + action[1]
		elif action[0] == "case":
			if action[1] == "upper":
				name = name.upper()
			elif action[1] == "lower":
				name = name.lower()
			elif action[1] == "title":
				name = name.title()
	
	results.append((*file, name + extension))

results.sort()

if flags["simulation"] is True:
	print("Simulation:")
	for result in results:
		print(result[0], "\n\t", result[1], "->\n\t", result[2])
else:
	for result in results:
		os.rename(result[0]+result[1], result[0]+result[2])
		print("Renamed in", result[0], "\n\t", result[1], "->\n\t", result[2])

print("\nNumber of results: ", len(results))