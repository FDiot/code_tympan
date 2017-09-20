"""
Created on 15 nov. 2016

@author: t.legall

Export spectrums from all sources in the tympan project, included machines and buildings.
"""
from __future__ import print_function
from tympan.models.solver import Model
from _util import input_string, run_calculations, ask_xml_file
import numpy as np


def write_results(valeurs, src_ids, dict_id_name, fichier_txt):
    """
    Write third octave spectrum values to a txt file.
    Objects used to handle the values are numpy arrays.
    
    valeurs type : ndarray
    src_ids type : list
    """
    # numpy array containing frequency values
    freq_tiers_oct = np.array([[16.0, 20.0, 25.0, 31.5, 40.0,
                               50.0, 63.0, 80.0, 100.0, 125.0,
                               160.0, 200.0, 250.0, 315.0, 400.0,
                               500.0, 630.0, 800.0, 1000.0, 1250.0, 1600.0,
                               2000.0, 2500.0, 3150.0, 4000.0, 5000.0,
                               6300.0, 8000.0, 10000.0, 12500.0, 16000.0]])

    # Convert IDs to Names:
    names = []
    for i in range(len(src_ids)):
        names.append(dict_id_name[src_ids[i]])

    # Create final array to be written in the txt file
    noms = np.array(names)[:, np.newaxis]
    freq = np.array(['Frequence'])[:, np.newaxis]
    result_array = np.hstack((noms, valeurs))
    # Sort alphabetically by name to have stable comparisons:
    result_array = np.sort(result_array, axis = 0)
    freq_array = np.hstack((freq, freq_tiers_oct))
    final_array = np.append(freq_array, result_array, axis=0)
    final_array = final_array.transpose()
    
    # save in txt file
    with open(fichier_txt, 'wb') as f:
        np.savetxt(f, final_array, delimiter=', ', fmt='%s')

def calc_surf_spectrum(list_src):
    """
    Get sources list contained in the model (list_src = model.sources) and return surface
    spectrum for sources that are not a point source. If the source is a point source return
    spectrum of the point source only.
    """
    # Definition of variables
    src_ids = list()
    indices_fin = list()

    # Add the face_id (id of the source) to the list src_ids
    # Add the end indices to indices_fin
    src_ids.append(list_src[0].face_id)
    
    for i in range(len(list_src)-1):
        if list_src[i+1].face_id != list_src[i].face_id:
            src_ids.append(list_src[i+1].face_id)
            indices_fin.append(i)

    # convert indices_fin to a numpy array
    temp_array = np.asarray(indices_fin)
    del(indices_fin)
    indices_fin = temp_array
    del(temp_array)
    
    # Define indices_debut and finalize the lists
    indices_debut = np.append([0], indices_fin+1)
    indices_fin = np.append(indices_fin, len(list_src)-1)

    spectrum_values = None
    for i in range(len(list_src)):
        values = [list_src[i].spectrum.values]
        spectrum_values = values if i == 0 else np.append(spectrum_values, values, axis=0)

    # Creation of n-dimensional array for each real "surface" source, not each point source
    # There is a different calculus if the "surface" source is a point source or a surface
    values_final = np.empty([len(indices_debut), 31])

    for i in range(len(indices_debut)):
        if indices_fin[i]-indices_debut[i] == 0:
            resultat = spectrum_values[indices_debut[i],:]
        else:
            resultat = spectrum_values[indices_debut[i]]*((indices_fin[i]-indices_debut[i])+1)
            
        values_final[i,:] = resultat

    return (values_final, src_ids)


def calc_vol_spectrum(list_src):
    """
    Get the sources list contained in the model (list_src = model.sources) and return volume
    spectrum for sources that are not a point source. If the source is a point source return
    spectrum of the point source only.
    """
    # Definition of variables
    src_ids = list()
    indices_fin = list()

    # Add the volume_id (id of the source) to the list src_ids
    # Add the end indices to indices_fin
    src_ids.append(list_src[0].volume_id)
    
    for i in range(len(list_src)-1):
        if list_src[i+1].volume_id != list_src[i].volume_id:
            src_ids.append(list_src[i+1].volume_id)
            indices_fin.append(i)
            
    # convert indices_fin into a numpy array
    temp_array = np.asarray(indices_fin)
    del(indices_fin)
    indices_fin = temp_array
    del(temp_array)
    
    # Define indices_debut and finalize the lists
    indices_debut = np.append([0], indices_fin+1)
    indices_fin = np.append(indices_fin, len(list_src)-1)

    spectrum_values = None
    for i in range(len(list_src)):
        values = [list_src[i].spectrum.values]
        spectrum_values = values if i == 0 else np.append(spectrum_values, values, axis=0)

    # Creation of n-dimensional array for each real "volume" source, not each point source
    # There is a different calculus if the "volume" source is a point source or a surface
    values_final = np.empty([len(indices_debut), 31])
    
    for i in range(len(indices_debut)):
        if indices_fin[i]-indices_debut[i] == 0:
            resultat = spectrum_values[indices_debut[i],:]
        else:
            resultat = spectrum_values[indices_debut[i]]*(indices_fin[i]-indices_debut[i])
            
        values_final[i,:] = resultat
    
    return (values_final, src_ids)


def main(fichier_xml, fichier_txt):
    # Get model from project
    project = run_calculations(fichier_xml)
    model = Model.from_project(project)
    
    # Get sources list
    list_src = model.sources

    # Loop on engines
#    for item in project.site.engines:
#        print("Engine:", item.name," ID=",item.getID)

    # Build a dictionnary to convert ID to name
    dict_id_name = {}

    # Loop on acoustic surfaces to print infos
    print("----------------:")
    print("Punctual sources:")
    print("----------------:")
    for src in project.site.user_sources:
        print(src.name+" ID="+src.getID)
        # Update dictionnary for punctual sources
        dict_id_name[src.getID] = src.name

#    print("----------------:")
#    print("Solver sources:")
#    print("----------------:")
#    for src in model.sources:
#        print(" ID="+src.volume_id)

    print("")
    print("-----------------:")
    print("Acoustic surfaces:")
    print("-----------------:")
    for ac in project.site.acoustic_surfaces:
        face_name = ac.surface_node_name()
        volume_name = ac.volume_name()
        print("Face \""+face_name+"\" from volume \""+volume_name+"\" is","radiant." if ac.getIsRayonnant else "non radiant.")
        # Update dictionnary for acoustic surfaces:
        dict_id_name[ac.volume_id()] = volume_name
        dict_id_name[ac.surface_node_id()] = volume_name+" ("+face_name+")"

    # Get spectrums from the different sources PER SURFACE
    (values_final, src_ids) = calc_surf_spectrum(list_src)
    # Transform the power value into a dB value
    P0 = 1e-12
    values_final_db = 10*np.log10(values_final/P0)

    # Write the values into a file using write_results
    write_results(values_final_db, src_ids, dict_id_name, fichier_txt+'_parSurface.txt')

    # Get spectrums from the different sources PER VOLUME
    (values_final, src_ids) = calc_vol_spectrum(list_src)
    # Transform the power value into a dB value
    P0 = 1e-12
    values_final_db = 10*np.log10(values_final/P0)

    # Write the values into a file using write_results
    write_results(values_final_db, src_ids, dict_id_name, fichier_txt+'_parVolume.txt')

    # IMPORTANT COMMENT : The result values present some round problems or precision problems
    # moreover, in the "surface" case it seems better to multiply the spectrum value by :
    # ((indices_fin[i]-indices_debut[i])+1)
    # Whereas in the "volume" case the results seem better when multiplied by :
    # (indices_fin[i]-indices_debut[i])
    
if __name__ == '__main__':
    # Get xml file name and verify user input
    fichier_xml = ask_xml_file("Enter xml file name (with xml extension) : ")

    # Get txt file name where results have to be written
    fichier_txt = input_string("Enter txt file name (without extension) : ")
     
    # launch main 
    main(fichier_xml, fichier_txt)
    
    
    
    
    
    
    
    
