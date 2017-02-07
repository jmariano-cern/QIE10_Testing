import os

emap_file = "../cfg/ngHF_emap.txt"
ACheader_file = "./ActiveChannels.h"

emap = open(emap_file,"r")
emap_lines = emap.readlines();
emap.close()

os.system("rm ActiveChannels.h")
os.system("touch ActiveChannels.h")

ACheader = open(ACheader_file,"w")

emap_header = "#       i  cr  sl tb dcc spigot  fiber  fibcha  subdet  ieta  iphi  depth"
data = 0;
channel_array = []
for line in emap_lines:
  if (data == 1):
    eta = line.split()[9];
    phi = line.split()[10];
    dep = line.split()[11];
    channel_array.append([eta,phi,dep])
  if (line.strip() == emap_header):
    data = 1

outline = "static const int numActiveChannels = "
outline += str(len(channel_array))
outline += ";\n"
ACheader.write(outline)

ACheader.write("int activeChannels[numActiveChannels][3]{\n")
for channel in channel_array:
    outline = "  { "
    outline += channel[0]
    outline += " , "
    outline += channel[1]
    outline += " , "
    outline += channel[2]
    outline += " },\n"
    ACheader.write(outline)
ACheader.write("};\n")
