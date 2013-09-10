/**
 * @file SolverDataModelBuilder.cpp
 *
 * @brief Implementation of how to build a solver data model from a site.
 *
 * @author Anthony Truchet <anthony.truchet@logilab.fr>
 *
 */

#include <boost/foreach.hpp>

#include <cassert>
#include "Tympan/MetierSolver/DataExchange/SolverDataModelBuilder.hpp"

namespace tympan
{

UuidAdapter::UuidAdapter(const TYUUID& rhs)
{
    uuid.s.data1 = rhs.quid.data1;
    uuid.s.data2 = rhs.quid.data2;
    uuid.s.data3 = rhs.quid.data3;
    for (unsigned int i = 0; i < 8; ++i)
    {
        uuid.s.data4[i] = rhs.quid.data4[i];
    }
}

UuidAdapter::operator TYUUID() const
{
    TYUUID element_uid;
    element_uid.quid.data1 = uuid.s.data1;
    element_uid.quid.data2 = uuid.s.data2;
    element_uid.quid.data3 = uuid.s.data3;
    for (unsigned int i = 0; i < 8; ++i)
    {
        element_uid.quid.data4[i] = uuid.s.data4[i];
    }
    return element_uid;
}


SolverDataModelBuilder::SolverDataModelBuilder(SolverModel& model_):
		model(model_)
{
    // We store a simple reference to the model this builder is responsible to update.
}

SolverDataModelBuilder::~SolverDataModelBuilder()
{
    // The simple reference to the model is forgotten but we DO NOT touch to the model
    // as the whole point of the builder is to... build it !
}


void SolverDataModelBuilder::walkTroughtSite(LPTYSiteNode site_ptr)
{
    // TODO Implement it.

    // Acoustic geometries.
    setGeometricEntities(site_ptr);
    // Acoustic source.
    setAcousticSource(site_ptr);
}


void SolverDataModelBuilder::setGeometricEntities(LPTYSiteNode site_ptr)
{

    // Get \c AcousticTriangle.
    // Have to link to \c AcousticTriangle with relation \c surface_has_node
    unsigned int nb_building_faces = 0;
    // XXX Maybe swith to 'true'.
    bool with_ecran = false;
    TYTabAcousticSurfaceGeoNode face_list;
    std::vector<bool> estUnIndexDeFaceEcran;
    site_ptr->getListFaces(with_ecran, face_list, nb_building_faces,
                           estUnIndexDeFaceEcran);

    // Faces d'infrastructure
    TYAcousticSurface* pSurf = NULL;

    std::cout << " # Nb of building faces: " << nb_building_faces << std::endl;

    // 'face_list' can contain topography elements. Not relevant here.
    for (unsigned int i = 0 ; i < nb_building_faces ; i++)
    {
    	if (pSurf = TYAcousticSurface::safeDownCast(face_list[i]->getElement()))
    		setAcousticTriangle(pSurf);
    	else
    		continue; // type de face non identifiee

        // XXX Refactor all this big switch by calling a (virtual) method in the
        // base TYSurfaceInterface class
        /*

        if (pPoly = TYAcousticPolygon::safeDownCast(face_list[i]->getElement()))
        {
            // List of points of the surface.
            TYTabPoint tabPoint = pPoly->getContour();
            // Get the volume (and its ID) which contains the surface.
            TYElement* element = pPoly->getParent();
            uid = element->getStringID().toStdString();
            setAcousticTriangle(uid, tabPoint);
            // Get building material.
            LPTYMateriauConstruction material_ptr = pPoly->getMateriau();
            setAcousticBuildMaterial(material_ptr);
        }
        else if (pRect = TYAcousticRectangle::safeDownCast(face_list[i]->getElement()))
        {
            TYTabPoint tabPoint = pPoly->getContour();
            TYElement* element = pPoly->getParent();
            uid = element->getStringID().toStdString();
            setAcousticTriangle(uid, tabPoint);
            // Get building material.
            LPTYMateriauConstruction material_ptr = pPoly->getMateriau();
            setAcousticBuildMaterial(material_ptr);
        }
        else if (pCircle = TYAcousticCircle::safeDownCast(face_list[i]->getElement()))
        {
            TYTabPoint tabPoint = pPoly->getContour();
            TYElement* element = pPoly->getParent();
            uid = element->getStringID().toStdString();
            setAcousticTriangle(uid, tabPoint);
            // Get building material.
            LPTYMateriauConstruction material_ptr = pPoly->getMateriau();
            setAcousticBuildMaterial(material_ptr);
        }
        else if (pSemiCircle = TYAcousticSemiCircle::safeDownCast(face_list[i]->getElement()))
        {
            TYTabPoint tabPoint = pPoly->getContour();
            TYElement* element = pPoly->getParent();
            uid = element->getStringID().toStdString();
            setAcousticTriangle(uid, tabPoint);
            // Get building material.
            LPTYMateriauConstruction material_ptr = pPoly->getMateriau();
            setAcousticBuildMaterial(material_ptr);
        }
        else
        {
            continue; // type de face non identifiee
        }
        */
    }
}

/*
Node::pointer SolverDataModelBuilder::node_for(const TYPoint& point)
{
	const TYUUID uid = point.getID();
	QHash<TYUUID, Node::pointer>::iterator i = pointsUID_to_Nodes.find(uid);
	if (i != pointsUID_to_Nodes.end() )
	{
		// The point already exists
		return i.value();
	}
	else
	{
		// First time we encounter this point
		Node::pointer node = Node::pointer( new tympan::Node(point));
		pointsUID_to_Nodes.insert(uid, node);
		return node;
	}
	assertConsistency_pointsUID_to_Nodes(point);
}
*/
/*
#ifndef NDEBUG
void SolverDataModelBuilder::assertConsistency_pointsUID_to_Nodes(const TYPoint& point)
{
	const TYUUID uid = point.getID();
	bool found = false;
	// For each entry in the map...
	QHashIterator<TYUUID, Node::pointer> i(pointsUID_to_Nodes);
	while (i.hasNext()) {
		i.next();
		const Node::pointer node = i.value();
		if (i.key() == uid) // The current entry matches the point
		{
			// We check the point is not present multiple times
			assert( found==false && "The point's UID has been found multiple times" );
			found=true;
			// We check the coordinates match
			assert(node==point);
		}
		else
		{
			// We check that the coordinates do NOT match
			assert(!(node==point));
		}
	}
}
#endif // NDEBUG not defined
*/

//XXX
void SolverDataModelBuilder::setAcousticTriangle(const TYAcousticSurface* pSurf)
{
    TYElement* element = pSurf->getParent();
    // The uid for SiteElement (parent of an acoustic surface, i.e. the
    // geometric volume).
    TYUUID ty_uid(element->getID());
    UuidAdapter element_uid(ty_uid);
//    if (!elementsUID_to_SiteElement.contains(ty_uid))
//    {
//        SiteElement::pointer element_ptr(new SiteElement(element_uid.getUuid()));
//        elementsUID_to_SiteElement.insert(ty_uid, element_ptr);
//    }

    // Get building material.
    LPTYMateriauConstruction material_ptr = pSurf->getMateriau();
    setAcousticBuildMaterial(material_ptr);

    std::deque<OPoint3D> points;
    std::deque<OTriangle> triangles;
    pSurf->exportMesh(points, triangles);
    // TODO Use the triangulating interface of TYSurfaceInterface to get triangles
    // and convert them to Nodes and AcousticTriangles (beware of mapping
    // TYPoints to Node in the correct way.)


    // Walk trough the node of a triangle.
    // Create all nodes related to the triangle.
    std::vector<node_idx> map_to_model(points.size(), 0);
    size_t i = 0;
    BOOST_FOREACH(const Point& point, points)
    {
    	map_to_model[i] = model.make_node(point);
    }

    BOOST_FOREACH(const OTriangle& tri, triangles)
    {
        // Assert consistency of the OPoint3D given in the mesh
    	assert(tri._A == points[tri._p1]);
    	assert(tri._B == points[tri._p2]);
    	assert(tri._C == points[tri._p3]);
    	triangle_idx tri_idx = model.make_triangle(
    			map_to_model[tri._p1],
    			map_to_model[tri._p2],
    			map_to_model[tri._p3]);

        // Associate the triangle with the UUID of the element it belongs to
        model.triangle(tri_idx).uuid = element_uid.getUuid();
    }

    // XXX What is still supposed to be implement
    assert(false && "Not implemented yet");
}


void SolverDataModelBuilder::setFrequencyTab(const OTabFreq& freq_tab)
{
//    OTabFreq::const_iterator freq_it = freq_tab.begin();
//    for (; freq_it != freq_tab.end(); ++freq_it)
//    {
//        double value = *freq_it;
//        Frequency::pointer freq_ptr(new Frequency(value));
//        // Fill the dedicated container.
//        frequencies.push_back(freq_ptr);
//    }
}


//void SolverDataModelBuilder::updateSpectrumRelations(
//    const TYSpectre* ty_spectrum_ptr,
//    const AcousticSpectrum::pointer acoustic_spectrum_ptr)
//{
//    typedef deque<boost::shared_ptr<Frequency> > frequency_container;
//
//    // Freq. range (static table in Tympan). \note frequencies table is unique
//    // for a single computation (and static in a \c OSpectre instance). That is
//    // why there is the 'is_frequency' attr.
//    OTabFreq freq_tab = ty_spectrum_ptr->getTabFreqExact();
//    if (!is_frequency)
//    {
//        setFrequencyTab(freq_tab);
//        is_frequency = true;
//    }
//
//    // Loop on 'tab_modulus'. Get 'modules' and type of spectrum sample.
//    const double* tab_modulus = ty_spectrum_ptr->getTabValReel();
//    // XXX Clarify the different available 'types' of spectrum.
//    string type = "third-octave";
//    for (int i = 0; i < ty_spectrum_ptr->getNbValues(); ++i)
//    {
//        double modulus = tab_modulus[i];
//        SpectrumSample::pointer
//            sample_ptr(new SpectrumSample(modulus, type));
//
//        // Link the \c tympan::SpectrumSample with all frequencies via
//        // \c tympan::frequency_rdef.
//        frequency_container::const_iterator freq_it = frequencies.begin();
//        for (; freq_it != frequencies.end(); ++freq_it) {
//            sample_ptr->add<frequency_rdef>(*freq_it);
//        }
//
//        // Link the spectrum sample with the single \c tympan::AcousticSpectrum via
//        // \c tympan::sample_of_rdef.
//        sample_ptr->add<sample_of_rdef>(acoustic_spectrum_ptr);
//    }
//}


void SolverDataModelBuilder::setAcousticBuildMaterial(LPTYMateriauConstruction material_ptr)
{
//    std::cout << " # setAcousticBuildMaterial #" << std::endl;
//    // Building material.
//    AcousticBuildingMaterial::pointer
//        build_mat_ptr(new AcousticBuildingMaterial());
//
//    // Spectrum from the \c TYMateriauConstruction.
//    const TYSpectre& ty_spectrum = material_ptr->getSpectreAbso();
//
//    // Create the entity \c tympan::AcousticSpectrum
//    AcousticSpectrum::pointer acoustic_spectrum_ptr(new AcousticSpectrum());
//
//    // Link AcousticBuildingMaterial with a \c AcousticSpectrum.
//    build_mat_ptr->add<reflection_spectrum_rdef>(acoustic_spectrum_ptr);
//
//    // Update relations between freq. & samples.
//    updateSpectrumRelations(&ty_spectrum, acoustic_spectrum_ptr);
}


void SolverDataModelBuilder::setAcousticSource(LPTYSiteNode site_ptr)
{
//    // Need project and calcul to get all source points.
//    LPTYInfrastructure infrastructure_ptr = site_ptr->getInfrastructure();
//    LPTYProjet project_ptr = site_ptr->getProjet();
//    LPTYCalcul current_calcul = project_ptr->getCurrentCalcul();
//
//    // Get all localised acoustic points. key: TYelement*; value: table of
//    // points (i.e. TYTabSourcePonctuelleGeoNode, aka a table of TYGeometryNode).
//    TYMapElementTabSources source_point_map;
//    infrastructure_ptr->getAllSrcs(current_calcul, source_point_map);
//
//    // XXX Missing the geolocation of an AcousticSource (via attribute or
//    // relation?).
//
//    // Walk trough all source points and create entity.
//    TYMapElementTabSources::const_iterator map_it;
//    for (map_it = source_point_map.begin(); map_it != source_point_map.end();
//         ++map_it)
//    {
//        TYUUID ty_uid(map_it->first->getID());
//        // Stored the TYElement related to an acoustic source.
//        if (!elementsUID_to_SiteElement.contains(ty_uid))
//        {
//            UuidAdapter element_uid(ty_uid);
//            SiteElement::pointer element_ptr(new SiteElement(element_uid.getUuid()));
//            elementsUID_to_SiteElement.insert(ty_uid, element_ptr);
//        }
//
//        BOOST_FOREACH(const SmartPtr<TYGeometryNode>& source, map_it->second)
//        {
//            AcousticSource::pointer source_ptr(new AcousticSource());
//            SiteElement::pointer element_ptr =
//                elementsUID_to_SiteElement[ty_uid];
//            acoustic_sources.push_back(source_ptr);
//            // Link to a \c tympan::SiteElement
//            source_ptr->add<from_element_rdef>(element_ptr);
//
//
//            // Get TYSource in order to get spectrum.
//            TYSource* ty_source_ptr = NULL;
//            if (ty_source_ptr = TYSource::safeDownCast(source->getElement()))
//            {
//                TYSpectre* ty_spectrum_ptr = ty_source_ptr->getCurrentSpectre();
//                if (!ty_spectrum_ptr)
//                    assert(false && "NULL spectrum pointer from TYSource.");
//                // Create the entity \c tympan::AcousticSpectrum
//                AcousticSpectrum::pointer acoustic_spectrum_ptr(new AcousticSpectrum());
//                source_ptr->add<emission_spectrum_rdef>(acoustic_spectrum_ptr);
//                updateSpectrumRelations(ty_spectrum_ptr,
//                                        acoustic_spectrum_ptr);
//            }
//            else
//    		assert(false && "Should be a TYSource at least.");
//        }
//    }
}

void SolverDataModelBuilder::setAcousticReceptor(LPTYSiteNode site_ptr)
{
//    // Need project and calcul to get all receptors.
//    LPTYInfrastructure infrastructure_ptr = site_ptr->getInfrastructure();
//    LPTYProjet project_ptr = site_ptr->getProjet();
//    LPTYCalcul current_calcul = project_ptr->getCurrentCalcul();
//
//    TYTabPointCalculGeoNode receptor_table;
//    current_calcul->getAllRecepteurs(receptor_table);
//
//    BOOST_FOREACH(const SmartPtr<TYGeometryNode>& receptor, receptor_table)
//    {
//        TYElement* ty_element = receptor->getElement();
//        TYUUID ty_uid(ty_element->getID());
//        // Stored the TYElement related to an acoustic source.
//        if (!elementsUID_to_SiteElement.contains(ty_uid))
//        {
//            UuidAdapter element_uid(ty_uid);
//            SiteElement::pointer element_ptr(new SiteElement(element_uid.getUuid()));
//            elementsUID_to_SiteElement.insert(ty_uid, element_ptr);
//        }
//
//        // Get the coordinate of the receptor, create the entity and link with
//        // \c tympan::SiteElement.
//        TYPointCalcul* ty_point_calcul_ptr = NULL;
//        if (ty_point_calcul_ptr = TYPointCalcul::safeDownCast(ty_element))
//        {
//            Point point(*ty_point_calcul_ptr);
//            AcousticReceptor::pointer receptor_ptr(new AcousticReceptor(point));
//            SiteElement::pointer element_ptr =
//                elementsUID_to_SiteElement[ty_uid];
//            // Link to a \c tympan::SiteElement
//            receptor_ptr->add<from_user_receptor_rdef>(element_ptr);
//            acoustic_receptors.push_back(receptor_ptr);
//        }
//        else
//            assert(false && "Should be a TYPointCalcul at least.");
//    }
}


} /* namespace tympan */

// bool operator == (const TYPoint& point, const tympan::Node::pointer& node)
