#include "init_registry.h"
#include "Tympan/models/business/TYElement.h"
#include "Tympan/models/business/infrastructure/TYTopographie.h"
#include "Tympan/models/business/infrastructure/TYInfrastructure.h"
#include "Tympan/models/business/infrastructure/TYSiteNode.h"
#include "Tympan/models/business/infrastructure/TYDalle.h"
#include "Tympan/models/business/infrastructure/TYEcran.h"
#include "Tympan/models/business/infrastructure/TYBatiment.h"
#include "Tympan/models/business/infrastructure/TYEtage.h"
#include "Tympan/models/business/infrastructure/TYMachine.h"
#include "Tympan/models/business/infrastructure/TYMur.h"
#include "Tympan/models/business/infrastructure/TYRoute.h"
#include "Tympan/models/business/infrastructure/TYMurElement.h"
#include "Tympan/models/business/infrastructure/TYReseauTransport.h"
#include "Tympan/models/business/infrastructure/TYParoi.h"
#include "Tympan/models/business/material/TYMateriauConstruction.h"
#include "Tympan/models/business/material/TYVegetation.h"
#include "Tympan/models/business/material/TYSol.h"
#include "Tympan/models/business/geometry/TYPoint.h"
#include "Tympan/models/business/geometry/TYRepere.h"
#include "Tympan/models/business/geometry/TYBox.h"
#include "Tympan/models/business/geometry/TYSegment.h"
#include "Tympan/models/business/geometry/TYGeometryNode.h"
#include "Tympan/models/business/geometry/TYPolygon.h"
#include "Tympan/models/business/geometry/TYFaceSet.h"
#include "Tympan/models/business/geometry/TYRectangle.h"
#include "Tympan/models/business/TYPalette.h"
#include "Tympan/models/business/TYPanel.h"
#include "Tympan/models/business/TYBoundaryNoiseMap.h"
#include "Tympan/models/business/TYLinearMaillage.h"
#include "Tympan/models/business/TYPointControl.h"
#include "Tympan/models/business/TYPointCalcul.h"
#include "Tympan/models/business/TYResultat.h"
#include "Tympan/models/business/TYRectangularMaillage.h"
#include "Tympan/models/business/TYProjet.h"
#include "Tympan/models/business/TYMaillage.h"
#include "Tympan/models/business/TYCalcul.h"
#include "Tympan/models/business/topography/TYAltimetrie.h"
#include "Tympan/models/business/topography/TYPlanEau.h"
#include "Tympan/models/business/topography/TYCoursEau.h"
#include "Tympan/models/business/topography/TYTerrain.h"
#include "Tympan/models/business/topography/TYCourbeNiveau.h"
#include "Tympan/models/business/acoustic/TYSource.h"
#include "Tympan/models/business/acoustic/TYDirectivity.h"
#include "Tympan/models/business/acoustic/TYSpectre.h"
#include "Tympan/models/business/acoustic/TYSourceLineic.h"
#include "Tympan/models/business/acoustic/TYSourceCheminee.h"
#include "Tympan/models/business/acoustic/TYUserSourcePonctuelle.h"
#include "Tympan/models/business/acoustic/TYSourceSurfacic.h"
#include "Tympan/models/business/acoustic/TYSourcePonctuelle.h"
#include "Tympan/models/business/acoustic/TYSourceBafflee.h"
#include "Tympan/models/business/acoustic/TYTrafic.h"
#include "Tympan/models/business/acoustic/TYAttenuateur.h"
#include "Tympan/models/business/geoacoustic/TYBoucheSurface.h"
#include "Tympan/models/business/geoacoustic/TYChemineeSurface.h"
#include "Tympan/models/business/geoacoustic/TYAcousticSurfaceNode.h"
#include "Tympan/models/business/geoacoustic/TYAcousticVolume.h"
#include "Tympan/models/business/geoacoustic/TYAcousticSurface.h"
#include "Tympan/models/business/geoacoustic/TYAcousticRectangle.h"
#include "Tympan/models/business/geoacoustic/TYAcousticSemiCylinder.h"
#include "Tympan/models/business/geoacoustic/TYAcousticRectangleNode.h"
#include "Tympan/models/business/geoacoustic/TYAcousticCylinder.h"
#include "Tympan/models/business/geoacoustic/TYAcousticBox.h"
#include "Tympan/models/business/geoacoustic/TYAcousticSemiCircle.h"
#include "Tympan/models/business/geoacoustic/TYAcousticVolumeNode.h"
#include "Tympan/models/business/geoacoustic/TYAcousticCircle.h"
#include "Tympan/models/business/geoacoustic/TYAcousticPolygon.h"
#include "Tympan/models/business/geoacoustic/TYAcousticLine.h"
#include "Tympan/models/business/geoacoustic/TYAcousticFaceSet.h"

namespace tympan
{

void init_registry()
{
    // MetierSolver/DataManagerCore
    OPrototype::add_factory("TYElement", std::move(build_factory<TYElement>()));
    // Metier/Solver/DataManagerMetier/Site
    OPrototype::add_factory("TYTopographie", std::move(build_factory<TYTopographie>()));
    OPrototype::add_factory("TYInfrastructure", std::move(build_factory<TYInfrastructure>()));
    OPrototype::add_factory("TYSiteNode", std::move(build_factory<TYSiteNode>()));
    // models/business/infrastructure
    OPrototype::add_factory("TYDalle", std::move(build_factory<TYDalle>()));
    OPrototype::add_factory("TYEcran", std::move(build_factory<TYEcran>()));
    OPrototype::add_factory("TYBatiment", std::move(build_factory<TYBatiment>()));
    OPrototype::add_factory("TYEtage", std::move(build_factory<TYEtage>()));
    OPrototype::add_factory("TYMachine", std::move(build_factory<TYMachine>()));
    OPrototype::add_factory("TYMur", std::move(build_factory<TYMur>()));
    OPrototype::add_factory("TYRoute", std::move(build_factory<TYRoute>()));
    OPrototype::add_factory("TYMurElement", std::move(build_factory<TYMurElement>()));
    OPrototype::add_factory("TYReseauTransport", std::move(build_factory<TYReseauTransport>()));
    OPrototype::add_factory("TYParoi", std::move(build_factory<TYParoi>()));
    // models/business/material
    OPrototype::add_factory("TYMateriauConstruction", std::move(build_factory<TYMateriauConstruction>()));
    OPrototype::add_factory("TYVegetation", std::move(build_factory<TYVegetation>()));
    OPrototype::add_factory("TYSol", std::move(build_factory<TYSol>()));
    // models/business/geometry
    OPrototype::add_factory("TYPoint", std::move(build_factory<TYPoint>()));
    OPrototype::add_factory("TYRepere", std::move(build_factory<TYRepere>()));
    OPrototype::add_factory("TYBox", std::move(build_factory<TYBox>()));
    OPrototype::add_factory("TYSegment", std::move(build_factory<TYSegment>()));
    OPrototype::add_factory("TYGeometryNode", std::move(build_factory<TYGeometryNode>()));
    OPrototype::add_factory("TYPolygon", std::move(build_factory<TYPolygon>()));
    OPrototype::add_factory("TYFaceSet", std::move(build_factory<TYFaceSet>()));
    OPrototype::add_factory("TYRectangle", std::move(build_factory<TYRectangle>()));
    // models/business/Commun
    OPrototype::add_factory("TYPalette", std::move(build_factory<TYPalette>()));
    OPrototype::add_factory("TYPanel", std::move(build_factory<TYPanel>()));
    OPrototype::add_factory("TYBoundaryNoiseMap", std::move(build_factory<TYBoundaryNoiseMap>()));
    OPrototype::add_factory("TYLinearMaillage", std::move(build_factory<TYLinearMaillage>()));
    OPrototype::add_factory("TYPointControl", std::move(build_factory<TYPointControl>()));
    OPrototype::add_factory("TYPointCalcul", std::move(build_factory<TYPointCalcul>()));
    OPrototype::add_factory("TYResultat", std::move(build_factory<TYResultat>()));
    OPrototype::add_factory("TYRectangularMaillage", std::move(build_factory<TYRectangularMaillage>()));
    OPrototype::add_factory("TYProjet", std::move(build_factory<TYProjet>()));
    OPrototype::add_factory("TYMaillage", std::move(build_factory<TYMaillage>()));
    OPrototype::add_factory("TYCalcul", std::move(build_factory<TYCalcul>()));
    // models/business/topography
    OPrototype::add_factory("TYAltimetrie", std::move(build_factory<TYAltimetrie>()));
    OPrototype::add_factory("TYPlanEau", std::move(build_factory<TYPlanEau>()));
    OPrototype::add_factory("TYCoursEau", std::move(build_factory<TYCoursEau>()));
    OPrototype::add_factory("TYTerrain", std::move(build_factory<TYTerrain>()));
    OPrototype::add_factory("TYCourbeNiveau", std::move(build_factory<TYCourbeNiveau>()));
    // models/business/acoustique
    OPrototype::add_factory("TYSource", std::move(build_factory<TYSource>()));
    OPrototype::add_factory("TYDirectivity", std::move(build_factory<TYDirectivity>()));
    OPrototype::add_factory("TYUserDefinedDirectivity", std::move(build_factory<TYUserDefinedDirectivity>()));
    OPrototype::add_factory("TYComputedDirectivity", std::move(build_factory<TYComputedDirectivity>()));
    OPrototype::add_factory("TYSpectre", std::move(build_factory<TYSpectre>()));
    OPrototype::add_factory("TYSourceLineic", std::move(build_factory<TYSourceLineic>()));
    OPrototype::add_factory("TYSourceCheminee", std::move(build_factory<TYSourceCheminee>()));
    OPrototype::add_factory("TYUserSourcePonctuelle", std::move(build_factory<TYUserSourcePonctuelle>()));
    OPrototype::add_factory("TYSourceSurfacic", std::move(build_factory<TYSourceSurfacic>()));
    OPrototype::add_factory("TYSourcePonctuelle", std::move(build_factory<TYSourcePonctuelle>()));
    OPrototype::add_factory("TYSourceBafflee", std::move(build_factory<TYSourceBafflee>()));
    OPrototype::add_factory("TYTrafic", std::move(build_factory<TYTrafic>()));
    OPrototype::add_factory("TYAttenuateur", std::move(build_factory<TYAttenuateur>()));
    // models/business/geoacoustic
    OPrototype::add_factory("TYBoucheSurface", std::move(build_factory<TYBoucheSurface>()));
    OPrototype::add_factory("TYChemineeSurface", std::move(build_factory<TYChemineeSurface>()));
    OPrototype::add_factory("TYAcousticSurfaceNode", std::move(build_factory<TYAcousticSurfaceNode>()));
    OPrototype::add_factory("TYAcousticVolume", std::move(build_factory<TYAcousticVolume>()));
    OPrototype::add_factory("TYAcousticSurface", std::move(build_factory<TYAcousticSurface>()));
    OPrototype::add_factory("TYAcousticRectangle", std::move(build_factory<TYAcousticRectangle>()));
    OPrototype::add_factory("TYAcousticSemiCylinder", std::move(build_factory<TYAcousticSemiCylinder>()));
    OPrototype::add_factory("TYAcousticRectangleNode", std::move(build_factory<TYAcousticRectangleNode>()));
    OPrototype::add_factory("TYAcousticCylinder", std::move(build_factory<TYAcousticCylinder>()));
    OPrototype::add_factory("TYAcousticBox", std::move(build_factory<TYAcousticBox>()));
    OPrototype::add_factory("TYAcousticSemiCircle", std::move(build_factory<TYAcousticSemiCircle>()));
    OPrototype::add_factory("TYAcousticVolumeNode", std::move(build_factory<TYAcousticVolumeNode>()));
    OPrototype::add_factory("TYAcousticCircle", std::move(build_factory<TYAcousticCircle>()));
    OPrototype::add_factory("TYAcousticPolygon", std::move(build_factory<TYAcousticPolygon>()));
    OPrototype::add_factory("TYAcousticLine", std::move(build_factory<TYAcousticLine>()));
    OPrototype::add_factory("TYAcousticFaceSet", std::move(build_factory<TYAcousticFaceSet>()));

    // These classes weren't registered initially. Make sure they really don't need
    // to be.
//    OPrototype::add_factory("TYUserSrcRegime", std::move(build_factory<TYUserSrcRegime>()));
//    OPrototype::add_factory("TYRay", std::move(build_factory<TYRay>()));
//    OPrototype::add_factory("TYRegime", std::move(build_factory<TYRegime>()));
}

}
