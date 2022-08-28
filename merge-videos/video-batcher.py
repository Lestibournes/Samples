#!/usr/bin/python3

from glob import glob
import os
import sys

def sanitize(string):
	subs = [
		['"', '\\"']
	]

	for sub in subs:
		string = string.replace(sub[0], sub[1])
	
	return string

def merge(video_folder, audio_folder, output_folder, overwrite, scale, logging, conversion, extension):
	videos = os.listdir(video_folder)
	videos.sort()

	if not os.path.exists(output_folder):
		os.mkdir(output_folder)

	for i, video in enumerate(videos):
		video = sanitize(video)
		video = os.path.join(video_folder, video)

		(name, video_extension) = os.path.splitext(os.path.basename(video))
		audio = ""
		
		for file in os.listdir(audio_folder):
			if file.startswith(name):
				audio = os.path.join(audio_folder, file)

		audio_extension = ""

		if audio:
			(name, audio_extension) = os.path.splitext(os.path.basename(audio))

		if extension:
			output = os.path.join(output_folder, name + extension)
		elif audio_extension and video_extension == audio_extension:
			output = os.path.join(output_folder, name + "." + video_extension)
		else:
			output = os.path.join(output_folder, name + ".mkv")

		command = 'ffmpeg {l} {w} -i "{v}" -i "{a}" -c copy "{o}"'

		command = 'ffmpeg {l} {w} -i "{v}"'

		if audio: command += ' -i "{a}"'
		
		if scale: command += ' {s} -c:a copy "{o}"'
		elif conversion:
			command += ' {c} "{o}"'

		command = command.format(l = logging, w = overwrite, v = video, a = audio, s = scale, o = output, c = conversion)
		print("\nvideo", str(i + 1) + "/" + str(len(videos)) + ":", name + ":")
		os.system(command)
	print()
	
def main():
	# Values from CLI arguments:
	root_folder = os.getcwd()
	video_folder = ""
	audio_folder = ""
	output_folder = ""

	multi_show = False

	#	ffmpeg options:
	overwrite = "-n"
	scale = ""
	conversion = ""
	extension = ""
	logging = "-hide_banner -loglevel error -stats"

	for (i, arg) in enumerate(sys.argv):
		if arg == "-f":
			root_folder = sys.argv[i + 1]
		elif arg == "-m":
			multi_show = True
		elif arg == "-v":
			video_folder = sys.argv[i + 1]
		elif arg == "-a":
			audio_folder = sys.argv[i + 1]
		elif arg == "-o":
			output_folder = sys.argv[i + 1]
		elif arg == "-y":
			overwrite = arg
		elif arg == "-s":
			scale = "-filter:v scale=" + sys.argv[i + 1]
		elif arg == "-c":
			conversion = sys.argv[i + 1]
		elif arg == "-e":
			extension = "." + sys.argv[i + 1]
	
	if scale and conversion:
		print("You can either use the scale option or specify full free-form output conversion, not both.")

	if not video_folder:
		video_folder = root_folder

	if not multi_show and not root_folder:
		merge(video_folder, audio_folder, output_folder, overwrite, scale, logging, conversion, extension)
	elif root_folder and not multi_show:
		videos = os.path.join(root_folder, video_folder)
		audios = os.path.join(root_folder, audio_folder)
		outputs = os.path.join(root_folder, output_folder)
		merge(videos, audios, outputs, overwrite, scale, logging, conversion, extension)
	else:
		folders = glob(os.path.join(root_folder, '*'))
		folders.sort()

		for i, show in enumerate(folders):
			print("show", str(i + 1) + "/" + str(len(folders)) + ":", show + ":")
			print()
			videos = os.path.join(show, video_folder)
			audios = os.path.join(show, audio_folder)
			outputs = os.path.join(show, output_folder)
			merge(videos, audios, outputs, overwrite, scale, logging, conversion, extension)
			print()
			

if __name__ == "__main__":
	main()