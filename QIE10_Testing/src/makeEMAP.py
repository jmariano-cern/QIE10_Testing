import sqlite3
import datetime
DB_name = "ngHF2017LMap_20170206_pre05.db"

hf_lmap_db = sqlite3.connect(DB_name)
hf_lmap_db.text_factory = str

cursor = hf_lmap_db.cursor()

today = datetime.date.today()


def printFullEmap():
    output = ''
    output += '## file created {0} by dnoonan #\n'.format(today.strftime("%d%b%Y"))
    output += '## extraced from lmap {0} #\n'.format(DB_name[:-3])
    output += '#       i  cr  sl tb dcc spigot  fiber  fibcha  subdet  ieta  iphi  depth\n'
    uHTRcrates = [22,29,32]


    for crate in uHTRcrates:
        for slot in range(12):
            values = cursor.execute("select Crate, uHTR, uHTR_FI, FI_CH, Eta, Phi, Depth, Side from ngHF_Mapping where Crate={0} and uHTR={1}".format(crate,slot+1)).fetchall()

            for v in values:
                output += ' 4200458C {0} {1}  u   0      0    {2}     {3}      HF   {4}   {5}    {6}\n'.format(str(v[0]).rjust(3),str(v[1]).rjust(3),str(v[2]).rjust(3),str(v[3]).rjust(3),str(int(v[4]*v[7])).rjust(3),str(v[5]).rjust(3),str(v[6]).rjust(3))
    outputFile = open('ngHF_emap.txt','w')
    outputFile.write(output)

def printEmapPerCrate(crateList):
    output = ''
    output += '## file created {0} by dnoonan #\n'.format(today.strftime("%d%b%Y"))
    output += '## extraced from lmap {0} #\n'.format(DB_name[:-3])
    output += '## map is only for crates '
    for crate in crateList:
        output += crate+' '
    output +=  '#\n'
    output += '#       i  cr  sl tb dcc spigot  fiber  fibcha  subdet  ieta  iphi  depth\n'
    for FEcrate in crateList:
        values = cursor.execute("select Crate, uHTR, uHTR_FI, FI_CH, Eta, Phi, Depth, Side from ngHF_Mapping where ngRBX='{0}'".format(FEcrate)).fetchall()

        for v in values:
            output += ' 4200458C {0} {1}  u   0      0    {2}     {3}      HF   {4}   {5}    {6}\n'.format(str(v[0]).rjust(3),str(v[1]).rjust(3),str(v[2]).rjust(3),str(v[3]).rjust(3),str(int(v[4]*v[7])).rjust(3),str(v[5]).rjust(3),str(v[6]).rjust(3))

    outputFile = open('ngHF_emap.txt','w')
    outputFile.write(output)

if __name__=="__main__":
    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option("-c","--crate",dest="crate",
                      default=-1,
                      help="specify crate, or list of crates (seperated by comma without spaces)"
                      )
    parser.add_option("-m","--mask",dest="mask",
                      default=-1,
                      help="specify crate, or list of crates that will be masked and not included in the emap (seperated by comma without spaces)"
                      )

    (options, args) = parser.parse_args()

    if options.crate==-1 and options.mask==-1:
        printFullEmap()
    elif not options.crate==-1:
        if not(options.crate[0]=='[' and options.crate[-1]==']'):
            options.crate = '["'+options.crate+'"]'
        fecrates = eval(options.crate.replace(',','","'))
        fecrates = list(set(fecrates))
        fecrates.sort()
        printEmapPerCrate(fecrates)
    else:
        fullcrateList = ["HFP01","HFP02","HFP03","HFP04","HFP05","HFP06","HFP07","HFP08","HFM01","HFM02","HFM03","HFM04","HFM05","HFM06","HFM07","HFM08"]
        if not(options.mask[0]=='[' and options.mask[-1]==']'):
            options.mask = '["'+options.mask+'"]'
        fecratemask = eval(options.mask.replace(',','","'))
        fecratemask = list(set(fecratemask))
        fecratemask.sort()

        fecratelist = list(set(fullcrateList)-set(fecratemask))
        fecratelist.sort()
        printEmapPerCrate(fecratelist)
        
