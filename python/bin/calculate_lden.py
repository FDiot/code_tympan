"""How to calculate lden from an xml file containing 3 computations for Day, Evening and Night periods"""

#
import argparse
import numpy as np
import sys
import os
import csv
import tympan.models._business as bus
from tympan.models.project import Project, Spectrum


def set_op_data(fpath, sources):
    ''' Create a Operating Conditions csv file with all sources at 100% if does not exist'''
    if not os.path.exists(fpath):
        print('CSV Operating Conditions file does not exist, creating with 100% for all sources')
        with open(fpath, 'wb') as csvfile:
             writer = csv.DictWriter(csvfile,fieldnames = ['Sources','Day','Evening','Night'],delimiter=';')
             writer.writeheader()
             for src in sources:
                 writer.writerow({'Sources': src.name, 'Day': 100, 'Evening': 100, 'Night': 100})


def get_op_data(fpath, sources):
    ''' Get Operating Conditions Data from the fpath csv file'''

    with open(fpath, 'rb') as csvfile:
        print('CSV Operating Conditions file: %s' % fpath)
        reader = csv.DictReader(csvfile, delimiter=';')
       # Check the file header format
        if reader.fieldnames != ['Sources','Day','Evening','Night'] :
           sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', 0)
           sys.exit('Error : Bad fieldnames format for Operating Conditions file (must be Day, Evening, Night) ')
        for row in reader:
          if not row['Sources'] in [src.name for src in sources]:
              print('Source named',row['Sources'],' is not in the xml file???')
              sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', 0)
              sys.exit('Error : Bad Sources Names format for Operating Conditions file ')
        csvfile.seek(0)
        csvfile.readline()
        OP = []
        for row in reader:
            OP.append([float(row['Day']),float(row['Evening']),float(row['Night'])])
    return np.array(OP)

def test_imputs(project, calculations_namelist):
    '''
         Testing coherence between:
         - Calculation list in project and the choosen ones for Day, Evening and Night periodsprint([ field for field in reader.fieldnames])
    '''

    calc_names_in_project = [comp.name for comp in project.computations]
    print('Calculation names in project: ', calc_names_in_project)
    print('Given list:                   ', calculations_namelist)
    for calc in calculations_namelist:
        if calc not in calc_names_in_project:
          sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', 0)
          sys.exit('Calculation named %s not found in the xml file' % calc)


def load_tympan_xml(tympan_xml):
    '''
    Return data read from Code_Tympan XML file and built using Code_Tympan
    data model and altimetry
    tympan_xml: filepath to xml tympan project file
    '''
    print('Opening Code_TYMPAN project from %s' % tympan_xml)
    project = Project.from_xml(tympan_xml, verbose=False, update_altimetry=False)
    return project


def get_sources_list(project, calculations_namelist):
    '''
      Build a list of sources for the calculations in the calculations_list

    '''
    sources_namelist =[]
    sources = []
    for calc in project.computations:
        if calc.name in calculations_namelist:
            project.select_computation(calc, current=False)
            result = calc.result
            for src in result.sources:
                   # print('Calcul:',calc.name,'Source:',src.name)
                   if src.name not in sources_namelist :
                    sources_namelist.append(src.name)
                    sources.append(src)

    return sources

def get_receptors_list(project, calculations_namelist):
    '''
      Build a list of receivers for the calculations in the calculations_list

    '''
    receptors_namelist =[]
    receptors = []
    for calc in project.computations:
        if calc.name in calculations_namelist:
            project.select_computation(calc, current=False)
            result =  calc.result
            for rec in result.receptors:
                   if rec.name not in receptors_namelist:
                    receptors_namelist.append(rec.name)
                    receptors.append(rec)
    return receptors

def get_results(project, sources, receptors, calculations_namelist):
    '''
       Build a spectrum matrix for each period
    '''

    sources_namelist = [src.name for src in sources]
    receptors_namelist =  [rec.name for rec in receptors]
    nsources = len(sources_namelist)
    nreceptors =len(receptors_namelist)

    for calc in project.computations:
        if calc.name in calculations_namelist:
            result = calc.result
            # Initialize contribution table to -200 for
            lp = np.ones([nsources, nreceptors, 31]) * -200
            for src in result.sources:
                ind_src = (sources_namelist).index(src.name)
                for rec in result.receptors:
                    ind_rec = (receptors_namelist).index(rec.name)
                    # print('Calculation', calc.name, ' - Source index:', ind_src, '- Recepteur index:',ind_rec)
                    lp[ind_src, ind_rec, :] = result.spectrum(rec, src).values

            if calc.name == calculations_namelist[0]:
                print('Calculation named ', calc.name, ' is taken for the Day period')
                L1 = lp.copy()
            elif calc.name == calculations_namelist[1]:
                print('Calculation named ', calc.name, ' is taken for the Evening period')
                L2 = lp.copy()
            elif calc.name == calculations_namelist[2]:
                print('Calculation named ', calc.name, ' is taken for the Night period')
                L3 = lp.copy()
    del lp
    return L1, L2, L3

def calc_lden(l1, l2, l3, op):
     '''
     '''
     [nsources, nreceptors, nfreq] =np.shape(l1)

     # Day
     P = np.matrix(op)[:,0]
     P[np.where(P<=0.)]=1e-20 # to allow the log10 following operation
     MP = np.tile(P,(1,nfreq))
     LD=np.ones([nsources,nreceptors,31])*-200
     for irec in range(nreceptors):
         LD[:,irec,:] =10.*np.log10(np.multiply(np.power(10.,l1[:,irec,:]/10.),MP/100.))

    # Evening
     P = np.matrix(op)[:,1]
     P[np.where(P<=0.)]=1e-20 # to allow the log10 following operation
     MP = np.tile(P,(1,nfreq))
     LE=np.ones([nsources,nreceptors,31])*-200
     for irec in range(nreceptors):
         LE[:,irec,:] =10.*np.log10(np.multiply(np.power(10.,l2[:,irec,:]/10.),MP/100.))

    # Night
     P = np.matrix(op)[:,2]
     P[np.where(P<=0.)]=1e-20 # to allow the log10 following operation
     MP = np.tile(P,(1,nfreq))
     LN=np.ones([nsources,nreceptors,31])*-200
     for irec in range(nreceptors):
         LN[:,irec,:] =10.*np.log10(np.multiply(np.power(10.,l3[:,irec,:]/10.),MP/100.))


     LDEN = 10.*np.log10(12./24.*np.power(10.,LD/10.)
                                     +4./24.*np.power(10.,(LE+5.)/10.)
                                     + 8./24.*np.power(10.,(LN+10.)/10.) )

     return LD, LE, LN, LDEN

def main(tympan_xml, calculations_namelist, operating_conditions_file, debug):
    '''
    load &
    print('Starting LDEN')

    '''
    process_id = None
    if debug != 0:
        process_id = os.getpid()
        print ("bin/solve_tympan_project.py : l'application est prete pour le debuggage. Entrez 'run' une fois que "
               "ce processus python (id: %d) aura ete rattache au debugger.\n"
               "Attention, ce mode de debuggage n'est disponible que si "
               "solve_tympan_project.py est appele directement depuis une console interactive." % process_id)
        import pdb
        pdb.set_trace()


    # Load project
    project = load_tympan_xml(tympan_xml)

    # Test inputs
    test_imputs(project, calculations_namelist)

    # Get the list of the source included in the project (caculations may have differents sources list)
    S = get_sources_list(project, calculations_namelist)
    print('Sources: %s' % [src.name for src in S])

    # Get the list of the receptors included in the project (caculations may have differents receptors list)
    R = get_receptors_list(project, calculations_namelist)
    print('Receptors: %s' % [rec.name for rec in R])

    # Operating conditions
    set_op_data(operating_conditions_file, S) # Check if the csv file exist and create it if not
    OP = get_op_data(operating_conditions_file, S)

    # Get results for each period
    L1, L2, L3 = get_results(project, S, R, calculations_namelist)

    # LDEN
    _,_,_,L_DEN = calc_lden(L1, L2, L3, OP)
    # print(L_DEN)

    # Add new computation
    project.add_computation()
    project.current_computation.set_name('LDEN')
    for src in S:
        project.current_computation.result.add_source(src)
    for rec in R:
        project.current_computation.result.add_receptor(rec)

    # build the result matrix
    project.current_computation.result.build_matrix()

    # fill the matrix
    isrc = 0
    for src in S:
        irec = 0
        for rec in R:
            spectre = Spectrum(L_DEN[isrc,irec,:])
            project.current_computation.set_spectrum(rec, spectre)
            irec = irec + 1
        isrc = isrc + 1

     # Affect LT i.e. the total LDEN to each receptor to allow the result table to show the global LDEN

    [nsources, nreceptors, nfreq] =np.shape(L_DEN)
    irec = 0
    for rec in R:
        lt = np.zeros(31)
        isrc = 0
        for src in S:
            lt = lt + np.power(10.,L_DEN[isrc,irec,:]/10.)
            isrc = isrc + 1
        LT = 10.*np.log10(lt)
        spectrum_lt = Spectrum(LT)
        project.current_computation.set_spectrum(bus.elemen2receptor(rec), spectrum_lt)
        irec = irec + 1

    #   Mask Lw column in the result table
    project.current_computation.result.not_use_LW()

    # Save project to a temp xml file
    print('Writing results to LDEN_Included.xml')
    project.to_xml('LDEN_Included.xml')


    #  To avoid console output due to the loading of the xml  file (verbose=False not functional)
    sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', 0)
    sys.exit('End of LDEN calculation ')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='LDEN CALCULATION - The result is saved in a new tympan xml file named LDEN_Included.xml')
    parser.add_argument(
        'tympan_xml_file',
        help='the Code_TYMPAN XML file')
    parser.add_argument(
        '--day', default='Day' ,metavar='',
       help='Name of the Day calculation')
    parser.add_argument(
        '--evening', default='Evening' ,metavar='',
       help='Name of the Evening calculation')
    parser.add_argument(
        '--night', default='Night' ,metavar='',
       help='Name of the Night calculation')
    parser.add_argument(
        '--operating_conditions_file','--ocf', default='Default_Operating_Conditions.csv' ,metavar='',
       help='csv file name (full path) for operating conditions - use point for decimal values !!!')
    parser.add_argument(
        '--debug',metavar='D', default=0, type=int,
        help='is debugging on ? D=1 : Yes / D=0 otherwise (default)')
    args = parser.parse_args()
    #if len(args.calculations_list) != 3:
        # raise ValueError('calculations list must contain 3 calculation names!')
    main(args.tympan_xml_file,[args.day,args.evening,args.night],args.operating_conditions_file, args.debug)
