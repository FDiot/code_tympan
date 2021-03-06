"""solver models for Code_TYMPAN, headers
"""
import cython as cy
from cython.operator cimport dereference as deref
from tympan.models cimport _common as tycommon

cdef class cySimulation:
    """Simulation Cython class"""

    def __cinit__(self):
        self.thisptr = shared_ptr[Simulation](new Simulation())
        self.configuration = cyAcousticRaytracerConfiguration()
        self.configuration.thisptr = self.thisptr.get().getConfiguration()
        '''Naming the Scene'''
        scene = self.thisptr.get().getScene()
        scene.setName("MyScene")

    @cy.locals(s=cySource, source=Source)
    def addSource(self, s):
        """Add a source"""
        '''Set the sampler'''
        nbRaysPerSource = self.getConfiguration().NbRaysPerSource
        s.setSampler(self.getConfiguration().Discretization, nbRaysPerSource)
        source = s.thisptr
        self.thisptr.get().addSource(source)

    def getSourcesNumber(self):
        """Get the sources number"""
        return self.thisptr.get().getSources().size()

    @cy.locals(r=cyRecepteur, receptor=Recepteur)
    def addRecepteur(self, r):
        """Add a receptor"""
        receptor = r.thisptr
        self.thisptr.get().addRecepteur(receptor)

    def getReceptorsNumber(self):
        """Get the receptors number"""
        return self.thisptr.get().getRecepteurs().size()

    @cy.locals(s=cySolver)
    def setSolver(self, s):
        """Set the solver"""
        self.thisptr.get().setSolver(s.thisptr)

    @cy.locals(point=tycommon.Point3D)
    def add_vertex_to_scene(self, point):
        cpp_vertex = cy.declare(base_vec3[float],
                                base_vec3[float](point.x, point.y, point.z))
        cdef unsigned int index
        self.thisptr.get().getScene().addVertex(cpp_vertex, index)
        return index

    def add_triangle_to_scene(self, idx1, idx2, idx3):
        mat = new Material()
        self.thisptr.get().getScene().addTriangle(idx1, idx2, idx3, mat, False)

    def getSceneName(self):
        scene = self.thisptr.get().getScene()
        return scene.getName().decode('utf-8')

    def export_to_ply(self, filename):
        scene = self.thisptr.get().getScene()
        scene.export_to_ply(filename.encode('utf-8'))

    def import_from_ply(self, filename):
        scene = self.thisptr.get().getScene()
        scene.import_from_ply(filename.encode('utf-8'))

    def setAccelerator(self):
        """Set the accelerator"""
        scene = self.thisptr.get().getScene()
        '''Select the accelerator from the configuration'''
        accelerator_id = self.getConfiguration().Accelerator
        scene.finish(accelerator_id, FIRST)
        receptors_scene = self.thisptr.get().get_receptors_landscape()
        receptors_scene.finish(accelerator_id, ALL)

    def setEngine(self):
        """Set the engine"""
        '''DefaultEngine by default'''
        self.thisptr.get().setEngine()

    def postTreatmentScene(self):
        '''Post-process the scene'''
        self.thisptr.get().getSolver().postTreatmentScene(self.thisptr.get().getScene(),
                                                          self.thisptr.get().getSources(), self.thisptr.get().getRecepteurs())

    def launchSimulation(self):
        """Run the process"""
        return self.thisptr.get().launchSimulation()

    def clean(self):
        """Clean the process"""
        self.thisptr.get().clean()

    def getConfiguration(self):
        """Get the ray tracer configuration"""
        return self.configuration

    def getSceneVerticesNumber(self):
        """Get the number of Scene vertices"""
        scene = self.thisptr.get().getScene()
        return scene.getVertices().size()

    @property
    def nValidRays(self):
        """Number of valid rays found by the simulation"""
        return self.thisptr.get().getSolver().getValidRays().size()

    @property
    def validRays(self):
        crays_pointer = cy.declare(cy.pointer(deque[cy.pointer(Ray)]),
                                   self.thisptr.get().getSolver().getValidRays())
        crays = cy.declare(deque[cy.pointer(Ray)], deref(crays_pointer))
        cray = cy.declare(cy.pointer(Ray))
        rays_list = list()
        for cray in crays:
            cyray = cyRay()
            cyray.thisptr = cray
            rays_list.append(cyray)
        return rays_list

    @property
    def lostRays(self):
        crays_pointer = cy.declare(cy.pointer(deque[cy.pointer(Ray)]),
                                   self.thisptr.get().getSolver().getDebugRays())
        crays = cy.declare(deque[cy.pointer(Ray)], deref(crays_pointer))
        cray = cy.declare(cy.pointer(Ray))
        rays_list = list()
        for cray in crays:
            cyray = cyRay()
            cyray.thisptr = cray
            rays_list.append(cyray)
        return rays_list

cdef class cyRay:
    def __cinit__(self):
        self.thisptr = new Ray()

    @property
    def source(self):
        source = cySource(0, 0, 0)
        source.thisptr = deref(self.thisptr.getSource())
        return source

    @property
    def nevents(self):
        return self.thisptr.getNbEvents()

    @property
    def ndiffractions(self):
        return self.thisptr.getDiff()

    @property
    def nreflexions(self):
        return self.thisptr.getReflex()

    @property
    def length(self):
        self.thisptr.computeLongueur()
        return self.thisptr.getLongueur()

    @property
    def direction(self):
        cdirection = cy.declare(base_vec3[float], self.thisptr.getDirection())
        x = cdirection.get_x()
        y = cdirection.get_y()
        z = cdirection.get_z()
        return (x, y, z)

    @property
    def events(self):
        cevents_ptr = cy.declare(cy.pointer(vector[shared_ptr[Event]]),
                                 self.thisptr.getEvents())
        cevents = cy.declare(vector[shared_ptr[Event]], deref(cevents_ptr))
        cevent = cy.declare(shared_ptr[Event])
        events_list = list()
        for cevent in cevents:
            event = cyEvent()
            event.thisptr = cevent.get()
            events_list.append(event)
        return events_list

cdef class cyEvent:
    TYPE_EVENT = {0: 'SPECULAR REFLEXION',
                  1: 'DIFFRACTION',
                  2: 'NOTHING'}

    def __cinit__(self):
        self.thisptr = new Event()

    @property
    def type(self):
        return self.TYPE_EVENT[self.thisptr.getType()]

    @property
    def position(self):
        position = self.thisptr.getPosition()
        x = position.get_x()
        y = position.get_y()
        z = position.get_z()
        return (x, y, z)

    @property
    def incoming_direction(self):
        direction = self.thisptr.getIncomingDirection()
        x = direction.get_x()
        y = direction.get_y()
        z = direction.get_z()
        return (x, y, z)

cdef class cyRecepteur:
    """Cython class for Receptor"""
    @cy.locals(vec=base_vec3[float])
    def __cinit__(self, x, y, z, r):
        """Create a receptor"""
        vec = base_vec3[float](x, y, z)
        self.thisptr = Recepteur(vec, r)

    @property
    def position(self):
        position = self.thisptr.getPosition()
        x = position.get_x()
        y = position.get_y()
        z = position.get_z()
        return (x, y, z)

cdef class cySource:
    """Cython class for Source"""
    SamplerList = ['RandomSphericSampler', 'UniformSphericSampler',
                   'UniformSphericSampler2', 'Latitude2DSampler']

    def __cinit__(self, x, y, z):
        """Create a source"""
        vec = new base_vec3[float](x, y, z)
        self.thisptr.setPosition(vec)

    @property
    def position(self):
        position = self.thisptr.getPosition()
        x = position.get_x()
        y = position.get_y()
        z = position.get_z()
        return (x, y, z)

    @property
    def nbRaysLeft(self):
        return self.thisptr.getNbRayLeft()

    def setSampler(self, SamplerID, nbRaysPerSource):
        """Change the Source Sampler"""
        '''Method based on TYANIME3DAcousticPathFinder::appendSourceToSimulation'''
        '''which should be moved in AcousticRayTracer library ?'''
        '''assert s in self.SamplerList, ('%r : unknown sampler type. Possible values: %r' % (s, self.SamplerList))'''
        '''print("Provisoire Sampler selected:"+self.SamplerList[SamplerID])'''
        sampler = cySampler()
        if SamplerID == 0:
            sampler.thisptr = new RandomSphericSampler(nbRaysPerSource)
            realNbRaysPerSource = nbRaysPerSource
        elif SamplerID == 1:
            sampler.thisptr = new UniformSphericSampler(nbRaysPerSource)
            realNbRaysPerSource = ( < UniformSphericSampler*>sampler.thisptr).getRealNbRays()
        elif SamplerID == 2:
            sampler.thisptr = new UniformSphericSampler2(nbRaysPerSource)
            realNbRaysPerSource = ( < UniformSphericSampler2*>sampler.thisptr).getRealNbRays()
        elif SamplerID == 3:
            sampler.thisptr = new Latitude2DSampler(nbRaysPerSource)
            realNbRaysPerSource = nbRaysPerSource
            ( < Latitude2DSampler*>sampler.thisptr).setStartPhi(0.)
            ( < Latitude2DSampler*>sampler.thisptr).setEndPhi(360.)
            ( < Latitude2DSampler*>sampler.thisptr).setStartTheta(0.)
            ( < Latitude2DSampler*>sampler.thisptr).setEndTheta(0.)
        self.thisptr.setSampler(sampler.thisptr)
        self.thisptr.setInitialRayCount(realNbRaysPerSource)

cdef class cySolver:
    """Cython class for Solver"""

    def __cinit__(self):
        """Create a solver"""
        self.thisptr = new BasicSolver()

cdef class cyAcousticRaytracerConfiguration:
    """Cython class for ray tracer configuration"""

    def setNbRaysPerSource(self, value):
        self.thisptr.NbRaysPerSource = value

    def getNbRaysPerSource(self):
        return self.thisptr.NbRaysPerSource
    NbRaysPerSource = property(getNbRaysPerSource, setNbRaysPerSource)

    def setDiscretization(self, value):
        self.thisptr.Discretization = value

    def getDiscretization(self):
        return self.thisptr.Discretization
    Discretization = property(getDiscretization, setDiscretization)

    def setAccelerator(self, value):
        self.thisptr.Accelerator = value

    def getAccelerator(self):
        return self.thisptr.Accelerator
    Accelerator = property(getAccelerator, setAccelerator)

    def setMaxTreeDepth(self, value):
        self.thisptr.MaxTreeDepth = value

    def getMaxTreeDepth(self):
        return self.thisptr.MaxTreeDepth
    MaxTreeDepth = property(getMaxTreeDepth, setMaxTreeDepth)

    def setNbRayWithDiffraction(self, value):
        self.thisptr.NbRayWithDiffraction = value

    def getNbRayWithDiffraction(self):
        return self.thisptr.NbRayWithDiffraction
    NbRayWithDiffraction = property(
        getNbRayWithDiffraction, setNbRayWithDiffraction)

    def setMaxProfondeur(self, value):
        self.thisptr.MaxProfondeur = value

    def getMaxProfondeur(self):
        return self.thisptr.MaxProfondeur
    MaxProfondeur = property(getMaxProfondeur, setMaxProfondeur)

    def setMaxReflexion(self, value):
        self.thisptr.MaxReflexion = value

    def getMaxReflexion(self):
        return self.thisptr.MaxReflexion
    MaxReflexion = property(getMaxReflexion, setMaxReflexion)

    def setMaxDiffraction(self, value):
        self.thisptr.MaxDiffraction = value

    def getMaxDiffraction(self):
        return self.thisptr.MaxDiffraction
    MaxDiffraction = property(getMaxDiffraction, setMaxDiffraction)

    def setMaxLength(self, value):
        self.thisptr.MaxLength = value

    def getMaxLength(self):
        return self.thisptr.MaxLength
    MaxLength = property(getMaxLength, setMaxLength)

    def setSizeReceiver(self, value):
        self.thisptr.SizeReceiver = value

    def getSizeReceiver(self):
        return self.thisptr.SizeReceiver
    SizeReceiver = property(getSizeReceiver, setSizeReceiver)

    def setAngleDiffMin(self, value):
        self.thisptr.AngleDiffMin = value

    def getAngleDiffMin(self):
        return self.thisptr.AngleDiffMin
    AngleDiffMin = property(getAngleDiffMin, setAngleDiffMin)

    def setCylindreThick(self, value):
        self.thisptr.CylindreThick = value

    def getCylindreThick(self):
        return self.thisptr.CylindreThick
    CylindreThick = property(getCylindreThick, setCylindreThick)

    def setMaxPathDifference(self, value):
        self.thisptr.MaxPathDifference = value

    def getMaxPathDifference(self):
        return self.thisptr.MaxPathDifference
    MaxPathDifference = property(getMaxPathDifference, setMaxPathDifference)

    def setInitialAnglePhi(self, value):
        self.thisptr.InitialAnglePhi = value

    def getInitialAnglePhi(self):
        return self.thisptr.InitialAnglePhi
    InitialAnglePhi = property(getInitialAnglePhi, setInitialAnglePhi)

    def setFinalAnglePhi(self, value):
        self.thisptr.FinalAnglePhi = value

    def getFinalAnglePhi(self):
        return self.thisptr.FinalAnglePhi
    FinalAnglePhi = property(getFinalAnglePhi, setFinalAnglePhi)

    def setInitialAngleTheta(self, value):
        self.thisptr.InitialAngleTheta = value

    def getInitialAngleTheta(self):
        return self.thisptr.InitialAngleTheta
    InitialAngleTheta = property(getInitialAngleTheta, setInitialAngleTheta)

    def setFinalAngleTheta(self, value):
        self.thisptr.FinalAngleTheta = value

    def getFinalAngleTheta(self):
        return self.thisptr.FinalAngleTheta
    FinalAngleTheta = property(getFinalAngleTheta, setFinalAngleTheta)

    def setUseSol(self, value):
        self.thisptr.UseSol = value

    def getUseSol(self):
        return self.thisptr.UseSol
    UseSol = property(getUseSol, setUseSol)

    def setUsePathDifValidation(self, value):
        self.thisptr.UsePathDifValidation = value

    def getUsePathDifValidation(self):
        return self.thisptr.UsePathDifValidation
    UsePathDifValidation = property(
        getUsePathDifValidation, setUsePathDifValidation)

    def setDiffractionFilterRayAtCreation(self, value):
        self.thisptr.DiffractionFilterRayAtCreation = value

    def getDiffractionFilterRayAtCreation(self):
        return self.thisptr.DiffractionFilterRayAtCreation
    DiffractionFilterRayAtCreation = property(
        getDiffractionFilterRayAtCreation, setDiffractionFilterRayAtCreation)

    def setDiffractionUseDistanceAsFilter(self, value):
        self.thisptr.DiffractionUseDistanceAsFilter = value

    def getDiffractionUseDistanceAsFilter(self):
        return self.thisptr.DiffractionUseDistanceAsFilter
    DiffractionUseDistanceAsFilter = property(
        getDiffractionUseDistanceAsFilter, setDiffractionUseDistanceAsFilter)

    def setDiffractionUseRandomSampler(self, value):
        self.thisptr.DiffractionUseRandomSampler = value

    def getDiffractionUseRandomSampler(self):
        return self.thisptr.DiffractionUseRandomSampler
    DiffractionUseRandomSampler = property(
        getDiffractionUseRandomSampler, setDiffractionUseRandomSampler)

    def setDiffractionDropDownNbRays(self, value):
        self.thisptr.DiffractionDropDownNbRays = value

    def getDiffractionDropDownNbRays(self):
        return self.thisptr.DiffractionDropDownNbRays
    DiffractionDropDownNbRays = property(
        getDiffractionDropDownNbRays, setDiffractionDropDownNbRays)

    def setUsePostFilters(self, value):
        self.thisptr.UsePostFilters = value

    def getUsePostFilters(self):
        return self.thisptr.UsePostFilters
    UsePostFilters = property(getUsePostFilters, setUsePostFilters)

    def setDebugUseCloseEventSelector(self, value):
        self.thisptr.DebugUseCloseEventSelector = value

    def getDebugUseCloseEventSelector(self):
        return self.thisptr.DebugUseCloseEventSelector
    DebugUseCloseEventSelector = property(
        getDebugUseCloseEventSelector, setDebugUseCloseEventSelector)

    def setDebugUseDiffractionAngleSelector(self, value):
        self.thisptr.DebugUseDiffractionAngleSelector = value

    def getDebugUseDiffractionAngleSelector(self):
        return self.thisptr.DebugUseDiffractionAngleSelector
    DebugUseDiffractionAngleSelector = property(
        getDebugUseDiffractionAngleSelector, setDebugUseDiffractionAngleSelector)

    def setDebugUseDiffractionPathSelector(self, value):
        self.thisptr.DebugUseDiffractionPathSelector = value

    def getDebugUseDiffractionPathSelector(self):
        return self.thisptr.DebugUseDiffractionPathSelector
    DebugUseDiffractionPathSelector = property(
        getDebugUseDiffractionPathSelector, setDebugUseDiffractionPathSelector)

    def setDebugUseFermatSelector(self, value):
        self.thisptr.DebugUseFermatSelector = value

    def getDebugUseFermatSelector(self):
        return self.thisptr.DebugUseFermatSelector
    DebugUseFermatSelector = property(
        getDebugUseFermatSelector, setDebugUseFermatSelector)

    def setDebugUseFaceSelector(self, value):
        self.thisptr.DebugUseFaceSelector = value

    def getDebugUseFaceSelector(self):
        return self.thisptr.DebugUseFaceSelector
    DebugUseFaceSelector = property(
        getDebugUseFaceSelector, setDebugUseFaceSelector)

    def setKeepDebugRay(self, value):
        self.thisptr.KeepDebugRay = value

    def getKeepDebugRay(self):
        return self.thisptr.KeepDebugRay
    KeepDebugRay = property(getKeepDebugRay, setKeepDebugRay)
