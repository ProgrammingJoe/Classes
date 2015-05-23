#!/opt/bin/python3
#Author: Joe Czepil, V00774878

import sys
import os.path
import os
import re

global depends
depends = []

def depend(depends, fil):
	pstream = open(fil)
	for line in pstream:
		incl = re.findall('^#include\s+"[\w]+.h"',line)
		for j in incl:
			parts = j.split()
			thefile = parts[1]
			thefile = thefile[1:-1]
			if (thefile not in depends):
				depends.append(thefile)
				if(os.path.isfile(thefile)):
					depend(depends, thefile)
	pstream.close()

def main():
	folderpath = sys.argv[1]
	filelist = []
	os.chdir(folderpath)
	target = open("makefile", 'w')
	for files in os.listdir():
		if os.path.isfile(files):
			if (re.match("[\w_0-9]*\.(cpp$|cc$|C$|c$)", files)):
				filelist.append(files)
	
	target.write("SRCS =")
	for x in filelist:
		target.write(" " + x)
	
	target.write("\n\nOBJS = ")
	filetext = " ".join(filelist)
	filetext = re.sub(r"\.(cpp|C|cc|c)", ".o", filetext)
	target.write(filetext)

	target.write("\n\nPROG = prog.exe")
	target.write("\n\n$(PROG): $(OBJS)\n")
	target.write("\t\t$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(PROG)\n\n")

	k = 0
	while k < len(filelist):
		del depends[:]
		ofiles = filetext.split()
		target.write((ofiles[k]) + ": " + (filelist[k]) + " ")
		newfile = filelist[k]
		depend(depends, newfile)
		for items in depends:
			target.write(items + " ")
		for items in depends:
			if(os.path.isfile(items) == False):
				print(newfile + ": contains #include for missing file " + items)
			
		target.write("\n\t\t")

		if (re.match("[\w\s\d]*.cpp$", filelist[k])):
			target.write("$(CXX) $(CPPFLAGS) $(CXXLAGS) -c " + filelist[k])
		if (re.match("[\w\s\d]*.C$", filelist[k])):
			target.write("$(CC) $(CPPFLAGS) $(CFLAGS) -c " + filelist[k])
		if (re.match("[\w\s\d]*.cc$", filelist[k])):
			target.write("$(CXX) $(CPPFLAGS) $(CXXLAGS) -c " + filelist[k])
		if (re.match("[\w\s\d]*.c$", filelist[k])):
			target.write("$(CC) $(CPPFLAGS) $(CFLAGS) -c " + filelist[k])
		target.write("\n\n")
		k = k + 1


	target.write("clean: \n\t\trm -f $(OBJS)")

	target.close()


if __name__ == "__main__":
    main()
