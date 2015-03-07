from sys import *
import os

def run():
	if argv[1] == "engine":
		os.system("make MODULE=Engine BUILD_TYPE=ENGINE")

	elif argv[1] == "editor":
		os.system("make MODULE=FrontEd BUILD_TYPE=EDITOR");
	else:
		print "Option not supported"
		print "Usage: python build.py <engine|editor>"

run()
