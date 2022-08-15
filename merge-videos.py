#!/usr/bin/python3

from glob import glob
import os
import sys
	
def main():
	# Values from CLI arguments:
	folder = os.getcwd()
	overwrite = "-n"
	scale = ""
	logging = "-hide_banner -loglevel error -stats"

	for (i, arg) in enumerate(sys.argv):
		if arg == "-f":
			folder = sys.argv[i + 1]
		elif arg == "-y":
			overwrite = arg
		elif arg == "-s":
			scale = "-filter:v scale=" +  sys.argv[i + 1]
	
	folders = glob(os.path.join(folder, '*'))
	folders.sort()

	for show in folders:
		videos = glob(os.path.join(show, 'Video', '*'))
		videos.sort()

		for video in videos:
			name = os.path.basename(video)
			audio = os.path.join(show, "Audio", name)
			output = os.path.join(show, name)

			if (os.path.exists(audio)):
				command = "ffmpeg {l} {w} -i '{v}' -i '{a}' -c copy '{o}'"

				if scale:
					command = "ffmpeg {l} {w} -i '{v}' -i '{a}' {s} -c:a copy '{o}'"

				command = command.format(l = logging, w = overwrite, v = video, a = audio, s = scale,o = output)
				print(video + ":")
				os.system(command)

if __name__ == "__main__":
	main()