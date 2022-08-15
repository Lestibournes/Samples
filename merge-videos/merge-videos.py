#!/usr/bin/python3

from glob import glob
import os
import sys

def merge(video_folder, audio_folder, output_folder, overwrite, scale, logging):
	videos = glob(os.path.join(video_folder, '*'))
	videos.sort()

	for video in videos:
		(name, video_extension) = os.path.splitext(os.path.basename(video))
		audio = ""
		
		for file in os.listdir(audio_folder):
			if file.startswith(name):
				audio = os.path.join(audio_folder, file)


		if (audio):
			(name, audio_extension) = os.path.splitext(os.path.basename(audio))

			if (video_extension == audio_extension):
				output = os.path.join(output_folder, name + "." + video_extension)
			else:
				output = os.path.join(output_folder, name + ".mkv")

			command = "ffmpeg {l} {w} -i '{v}' -i '{a}' -c copy '{o}'"

			if scale:
				command = "ffmpeg {l} {w} -i '{v}' -i '{a}' {s} -c:a copy '{o}'"

			command = command.format(l = logging, w = overwrite, v = video, a = audio, s = scale,o = output)
			print(video + ":")
			os.system(command)
	
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
			scale = "-filter:v scale=" +  sys.argv[i + 1]
	
	if not multi_show and not root_folder:
		merge(video_folder, audio_folder, output_folder, overwrite, scale, logging)
	elif root_folder and not multi_show:
		videos = os.path.join(root_folder, video_folder)
		audios = os.path.join(root_folder, audio_folder)
		outputs = os.path.join(root_folder, output_folder)
		merge(videos, audios, outputs, overwrite, scale, logging)
	else:
		folders = glob(os.path.join(root_folder, '*'))
		folders.sort()

		for show in folders:
			videos = os.path.join(show, video_folder)
			audios = os.path.join(show, audio_folder)
			outputs = os.path.join(show, output_folder)
			merge(videos, audios, outputs, overwrite, scale, logging)
			

if __name__ == "__main__":
	main()