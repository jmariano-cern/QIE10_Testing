import os

cfgfile_name = 'maps.cfg'
maskfile_name = 'mask.h'
emapfile_name = 'emap.txt'

cfgfile = open(cfgfile_name,'r')
lines = cfgfile.readlines()
cfgfile.close()

in_sd = 0
in_sp = 0
in_sl = 0
in_qi = 0
in_he = 0
default = 0 
line_num = 0

n_sp = {}
n_sl = {}
n_qi = {}

data = {}
data_sl = {}
data_sp = {}
    
sd = 'Null'
sp = 0
sl = 0
qi = 0

def parse_header():
    for sd in data:
        for i in range(n_sp[sd]):
            data[sd].append([])
            data_sl[sd].append([])
            data_sp[sd].append(1)
            for j in range(n_sl[sd]):
                data[sd][i].append([])
                data_sl[sd][i].append(1)
                for k in range(n_qi[sd]):
                    data[sd][i][j].append(default)

def slot_mapper(raw):
    return raw - 1

def mapper_slot(raw):
    return raw + 1

def check_slots():
    for sd in data:
        for i in range(len(data[sd])):
            for j in range(len(data[sd][i])):
                k_flag = 0
                for k in range(len(data[sd][i][j])):
                    if data[sd][i][j][k] == 1:
                        k_flag = 1
                if k_flag == 0:
                    data_sl[sd][i][j] = 0
                else:
                    data_sl[sd][i][j] = 1
        
def check_spigots():
    for sd in data:
        for i in range(len(data[sd])):
            j_flag = 0
            for j in range(len(data[sd][i])):
                k_flag = 0
                for k in range(len(data[sd][i][j])):
                    if data[sd][i][j][k] == 1:
                        k_flag = 1
                if k_flag == 1:
                    j_flag = 1
            if j_flag == 0:
                data_sp[sd][i] = 0
            else:
                data_sp[sd][i] = 1

def print_emap():
    os.system('rm ./' + emapfile_name)
    emapfile = open(emapfile_name,'w')

    emapfile.write('#i crate slot top/bottom dcc spigot fiber fiber-channel  subdet  ieta  iphi  depth\n')
    emapfile.write('\n')

    ID = '42075C'
    BE = 53
    uHTRs = [ 1 , 2 , 3 , 4, 5 , 6 , 7 , 8 ]
    fibers = 24
    channels = 4

    channel_counter = 0
    fiber_counter = 0
    uhtr_counter = 0

    for sd in data:
        for i in range(len(data[sd])):
            for j in range(len(data[sd][i])):
                for k in range(len(data[sd][i][j])):
                    if data[sd][i][j][k] == 1:                        
                        emapfile.write(str(ID) + " ")
                        emapfile.write(str(BE) + " ")
                        emapfile.write(str(uHTRs[uhtr_counter]) + " ")
                        emapfile.write("u 0 0 ")
                        emapfile.write(str(fiber_counter).rjust(2) + " ")
                        emapfile.write(str(channel_counter) + " ")
                        emapfile.write(str(sd) + " ")
                        emapfile.write(str(k+1).rjust(2) + " ")
                        emapfile.write(str(j+1).rjust(2) + " ")
                        emapfile.write(str(i+1) + "\n")
                        channel_counter = channel_counter + 1
                        if channel_counter > channels-1:
                            fiber_counter = fiber_counter + 1
                            channel_counter = 0
                            if fiber_counter > fibers-1:
                                uhtr_counter = uhtr_counter + 1
                                fiber_counter = 0
    emapfile.close()    

def print_mask():
    os.system('rm ./' + maskfile_name)
    maskfile = open(maskfile_name,'w')

    maskfile.write('static const int HF_num = ' + str(len(data[[sd for sd in data][0]])) + ';\n')
    maskfile.write('static const int SL_num = ' + str(len(data[[sd for sd in data][0]][0])) + ';\n')
    maskfile.write('static const int QI_num = ' + str(len(data[[sd for sd in data][0]][0][0])) + ';\n')
    maskfile.write('\n')

    for sd in data:
        maskfile.write('bool lv0_mask[HF_num] = {\n')
        for i in range(len(data[sd])):
            if data_sp[sd][i] == 1:
                if i != len(data[sd])-1:
                    maskfile.write('  1 , // HF ' + str(i+1) + '\n')
                else:
                    maskfile.write('  1   // HF ' + str(i+1) + '\n')
            if data_sp[sd][i] == 0:
                if i != len(data[sd])-1:
                    maskfile.write('  0 , // HF ' + str(i+1) + '\n')
                else:
                    maskfile.write('  0   // HF ' + str(i+1) + '\n')
        maskfile.write('};\n')
        maskfile.write('\n')

    for sd in data:
        maskfile.write('bool lv1_mask[HF_num][SL_num] = {\n')
        maskfile.write('//{')
        for j in range(len(data[sd][0])):
            if j != len(data[sd][0]) - 1:
                maskfile.write(' ' + str(mapper_slot(j)).rjust(2,'S') + ' ,')
            else:
                maskfile.write(' ' + str(mapper_slot(j)).rjust(2,'S') + ' ')            
        maskfile.write('}\n')
        for i in range(len(data[sd])):
            maskfile.write('  { ')
            for j in range(len(data[sd][0])):
                if data_sl[sd][i][j] == 0:
                    if j != len(data[sd][0])-1:
                        maskfile.write(' 0 , ')
                    else:
                        maskfile.write(' 0 ')
                if data_sl[sd][i][j] == 1:
                    if j != len(data[sd][0])-1:
                        maskfile.write(' 1 , ')
                    else:
                        maskfile.write(' 1 ')
            if i != len(data[sd])-1:
                maskfile.write('} , // HF ' + str(i+1) + '\n')
            else:
                maskfile.write('}   // HF ' + str(i+1) + '\n')
        maskfile.write('};\n')
        maskfile.write('\n')

    for sd in data:
        maskfile.write('bool lv2_mask[HF_num][SL_num][QI_num] = {\n')
        for i in range(len(data[sd])):
            maskfile.write('  { // HF ' + str(i+1) + '\n')
            maskfile.write('  //{')
            for k in range(len(data[sd][0][0])):
                if k != len(data[sd][0][0]) - 1:
                    maskfile.write(' ' + str(k+1).rjust(2,'Q') + ' ,')
                else:
                    maskfile.write(' ' + str(k+1).rjust(2,'Q') + ' ')            
            maskfile.write('}\n')
            for j in range(len(data[sd][0])):
                maskfile.write('    { ')
                for k in range(len(data[sd][0][0])):
                    if data[sd][i][j][k] == 1:
                        if k != len(data[sd][0][0])-1:
                            maskfile.write(' 1 , ')
                        else:
                            maskfile.write(' 1 ')
                    else:
                        if k != len(data[sd][0][0])-1:
                            maskfile.write(' 0 , ')
                        else:
                            maskfile.write(' 0 ')
                if j != len(data[sd][0])-1:
                    maskfile.write('} , // Slot ' + str(mapper_slot(j)).zfill(2)  + '\n')                
                else:
                    maskfile.write('}   // Slot ' + str(mapper_slot(j)).zfill(2)  + '\n')                                
            if i != len(data[sd])-1:
                maskfile.write('  } ,\n')
            else:
                maskfile.write('  }\n')
        maskfile.write('};\n')
        maskfile.write('\n')
    
    maskfile.close()

for line in lines:

    if ((line.strip() != '') and (line.strip()[0] != '#')):

        if line.find('<default_state>') != -1:
            if line.split('default_state>')[1].strip() == 'enable':
                for i in range(n_sp[sd]):
                    data_sp[sd][i] = 1
                    for j in range(n_sl[sd]):
                        data_sl[sd][i][j] = 0
                        for k in range(n_sq[sd]):
                            data[sd][i][j][k] = 1
            if line.split('default_state>')[1].strip() == 'disable':
                for i in range(n_sp[sd]):
                    data_sp[sd][i] = 0
                    for j in range(n_sl[sd]):
                        data_sl[sd][i][j] = 0
                        for k in range(n_qi[sd]):
                            data[sd][i][j][k] = 0
 
        if line.find('<header>') != -1:
            in_he = 1
        if (line.find('</header>') != -1):
            if (in_he == 1):
                in_he = 0
                parse_header()
            else:
                print 'ERROR: </header> without matching <header>' + ': line ' + str(line_num)

        if (in_he == 1) and (in_sd == 1):
            if line.find('<num_spigots>') != -1:
                n_sp[sd] = int(line.split('=')[1].strip())
            if line.find('<num_slots>') != -1:
                n_sl[sd] = int(line.split('=')[1].strip())
            if line.find('<num_qies>') != -1:
                n_qi[sd] = int(line.split('=')[1].strip())

        if line.find('<subdetector>') != -1:
            in_sd = 1
        if (line.find('</subdetector>') != -1):
            if (in_sd == 1):
                in_sd = 0
            else:
                print 'ERROR: </subdetector> without matching <subdetector>' + ': line ' + str(line_num)

        if line.find('<spigot>') != -1:
            if (in_sd == 1):
                in_sp = 1
            else:
                print 'ERROR: <spigot> declared outside of subdetector block' + ': line ' + str(line_num)
        if (line.find('</spigot>') != -1):
            if (in_sp == 1):
                in_sp = 0
            else:
                print 'ERROR: </spigot> without matching <spigot>' + ': line ' + str(line_num)

        if line.find('<slot>') != -1:
            if (in_sp == 1):
                in_sl = 1
            else:
                print 'ERROR: <slot> declared outside of spigot block' + ': line ' + str(line_num)
        if (line.find('</slot>') != -1):
            if (in_sl == 1):
                in_sl = 0
            else:
                print 'ERROR: </slot> without matching <slot>' + ': line ' + str(line_num)

        if line.find('<qie>') != -1:
            if (in_sl == 1):
                in_qi = 1
            else:
                print 'ERROR: <qie> declared outside of slot block' + ': line ' + str(line_num)
        if (line.find('</qie>') != -1):
            if (in_qi == 1):
                in_qi = 0
            else:
                print 'ERROR: </qie> without matching <qie>' + ': line ' + str(line_num)

        if (in_sd == 1) and (in_sp != 1) and (in_sl != 1) and (in_qi != 1):
            if line.split('=')[0].strip() == 'add_subdetector':
                sd = line.split('=')[1].strip()
                if sd not in data:
                    data[sd] = []
                    data_sp[sd] = []
                    data_sl[sd] = []
            if line.split('=')[0].strip() == 'state':
                if line.split('=')[1].strip() == 'enable':
                    for d_sp in range(len(data[sd])):
                        for d_sl in range(len(data[sd][d_sp])):
                            for d_qi in range(len(data[sd][d_sp][d_sl])):
                                data[sd][d_sp][d_sl][d_qi] = 1
                if line.split('=')[1].strip() == 'disable':
                    for d_sp in range(len(data[sd])):
                        for d_sl in range(len(data[sd][d_sp])):
                            for d_qi in range(len(data[sd][d_sp][d_sl])):
                                data[sd][d_sp][d_sl][d_qi] = 0

        if (in_sd == 1) and (in_sp == 1) and (in_sl != 1) and (in_qi != 1):
            if line.split('=')[0].strip() == 'add_spigot':
                if line.split('=')[1].find('A') != -1:
                    sp = range(n_sp[sd])
                elif line.split('=')[1].find('(') != -1:
                    sp = []
                    for each in line.split('=')[1].split('(')[1].split(')')[0].split(','):
                        sp.append(int(each.strip()))
                else:
                    sp = [int(line.split('=')[1].strip())]
            if line.split('=')[0].strip() == 'state':
                if line.split('=')[1].strip() == 'enable':
                    for d_sp in sp:
                        for d_sl in range(len(data[sd][d_sp])):
                            for d_qi in range(len(data[sd][d_sp][d_sl])):
                                data[sd][d_sp-1][d_sl][d_qi] = 1
                if line.split('=')[1].strip() == 'disable':
                    for d_sp in sp:
                        for d_sl in range(len(data[sd][d_sp])):
                            for d_qi in range(len(data[sd][d_sp][d_sl])):
                                data[sd][d_sp-1][d_sl][d_qi] = 0

        if (in_sd == 1) and (in_sp == 1) and (in_sl == 1) and (in_qi != 1):
            if line.split('=')[0].strip() == 'add_slot':
                if line.split('=')[1].find('A') != -1:
                    sl = range(n_sl[sd])
                elif line.split('=')[1].find('(') != -1:
                    sl = []
                    for each in line.split('=')[1].split('(')[1].split(')')[0].split(','):
                        sl.append(int(each.strip()))
                else:
                    sl = [int(line.split('=')[1].strip())]
            if line.split('=')[0].strip() == 'state':
                if line.split('=')[1].strip() == 'enable':
                    for d_sp in sp:
                        for d_sl in sl:
                            for d_qi in range(len(data[sd][d_sp][d_sl])):
                                data[sd][d_sp-1][slot_mapper(d_sl)][d_qi] = 1
                if line.split('=')[1].strip() == 'disable':
                    for d_sp in sp:
                        for d_sl in sl:
                            for d_qi in range(len(data[sd][d_sp][d_sl])):
                                data[sd][d_sp-1][slot_mapper(d_sl)][d_qi] = 0


        if (in_sd == 1) and (in_sp == 1) and (in_sl == 1) and (in_qi == 1):
            if line.split('=')[0].strip() == 'add_qie':
                if line.split('=')[1].find('A') != -1:
                    qi = range(n_qi[sd])
                elif line.split('=')[1].find('(') != -1:
                    qi = []
                    for each in line.split('=')[1].split('(')[1].split(')')[0].split(','):
                        qi.append(int(each.strip()))
                else:
                    qi = [int(line.split('=')[1].strip())]
            if line.split('=')[0].strip() == 'state':
                if line.split('=')[1].strip() == 'enable':
                    for d_sp in sp:
                        for d_sl in sl:
                            for d_qi in qi:
                                data[sd][d_sp-1][slot_mapper(d_sl)][d_qi-1] = 1
                if line.split('=')[1].strip() == 'disable':
                    for d_sp in sp:
                        for d_sl in sl:
                            for d_qi in qi:
                                data[sd][d_sp-1][slot_mapper(d_sl)][d_qi-1] = 0

        line_num = line_num + 1
check_spigots()
check_slots()
print_mask()
print_emap()
