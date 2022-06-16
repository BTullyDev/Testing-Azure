import os

#input file
fin = open("PlayBufferPCH.h", "rt")
#output file to write the result to
fout = open("Play.h", "wt")
#for each line in the input file
filelist = [ ]

print( "*****************************" )

for line in fin:
	#read replace the string and write to output file
	if line.find("#include \"Play") != -1:
		header_filename = line[ line.find("\"")+1 : line.rfind("\"") ]
		print( "Including " + header_filename )
		filelist.append(header_filename)
		fin_h = open(header_filename, "rt");
		fout.write ("\n//*******************************************************************\n");
		fout.write("// PLAYSHF BEGIN: " + header_filename + "\n")
		fout.write ("//*******************************************************************\n\n");
		for line_h in fin_h:
			if line_h.find("#pragma once") == -1:
				fout.write(line_h)
		fout.write ("\n//*******************************************************************\n");
		fout.write("// PLAYSHF END: " + header_filename + "\n" )
		fout.write ("//*******************************************************************\n\n");
		fin_h.close()
	else :
		if line.find("#pragma once") == -1:
			fout.write( line )

fin.close()

fout.write ("\n//*******************************************************************\n");
fout.write ("//*******************************************************************\n");
fout.write ("//*******************************************************************\n");
fout.write("#ifdef PLAYSHF_IMPLEMENTATION\n" )
fout.write ("//*******************************************************************\n");
fout.write ("//*******************************************************************\n");
fout.write ("//*******************************************************************\n");

#filelist.append("PlayMemory.cpp")

for header_filename in filelist:
	source_filename = header_filename.replace( ".h", ".cpp" )
	if os.path.isfile( source_filename ) :
		print( "Including " + source_filename )
		fin_cpp = open(source_filename, "rt");
		fout.write ("\n//*******************************************************************\n");
		fout.write("// PLAYSHF BEGIN: " + source_filename + "\n")
		fout.write ("//*******************************************************************\n\n");
		for line_cpp in fin_cpp:
			if line_cpp.find("#include") == -1:
					fout.write(line_cpp)
		fout.write ("\n//*******************************************************************\n");
		fout.write("// PLAYSHF END: " + source_filename + "\n" )
		fout.write ("//*******************************************************************\n\n");

#close input and output files

fout.write("#endif // PLAYSHF_IMPLEMENTATION\n" )

print( "Done" )
print( "*****************************" )

fout.close()
