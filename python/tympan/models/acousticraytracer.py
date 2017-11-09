"""Acoustic Ray Tracer module"""
from tympan.models import _acousticraytracer as cyAcousticRayTracer
from tympan.models._common import Point3D


class Simulation(object):
    """Main object Simulation for Ray Tracer
    """

    def __init__(self):
        """Create an empty simulation"""
        self._simulation = cyAcousticRayTracer.cySimulation()

    def add_source(self, source):
        """Add a source (with default sampler)"""
        self._simulation.addSource(source.cysource)

    @property
    def nsources(self):
        return self._simulation.getSourcesNumber()

    def add_recepteur(self, recepteur):
        """Add a receptor"""
        self._simulation.addRecepteur(recepteur.cyrecepteur)

    @property
    def nrecepteurs(self):
        return self._simulation.getReceptorsNumber()

    def set_solver(self, solver):
        """Add the solver"""
        self._simulation.setSolver(solver.cysolver)

    def set_accelerator(self):
        """Set the accelerator (by default, the ToDo one)"""
        self._simulation.setAccelerator()

    def set_engine(self):
        """Set the engine (by default, the DefaultEngine one)"""
        self._simulation.setEngine()

    def post_treatment_scene(self):
        self._simulation.postTreatmentScene()

    def launch_simulation(self):
        """Launch the ray tracer process"""
        return self._simulation.launchSimulation()

    def clean(self):
        """Clean the process"""
        self._simulation.clean()

    def configuration(self):
        """Return the ray tracer configuration"""
        return self._simulation.getConfiguration()

    def export_scene(self, filename):
        """Export a Scene to a ply file"""
        self._simulation.export_to_ply(filename)

    def import_scene(self, filename):
        """Import a Scene from a ply file"""
        self._simulation.import_from_ply(filename)

    @property
    def valid_rays(self):
        rays = list()
        for valid_ray in self._simulation.validRays:
            r = ray()
            r.cyray = valid_ray
            rays.append(r)
        return rays

    @property
    def nvalid_rays(self):
        return self._simulation.nValidRays

    @property
    def lost_rays(self):
        rays = list()
        for lost_ray in self._simulation.lostRays:
            r = ray()
            r.cyray = lost_ray
            rays.append(r)
        return rays

    def append_triangle_to_scene(self, point1, point2, point3):
        point1 = Point3D(*point1)
        point2 = Point3D(*point2)
        point3 = Point3D(*point3)
        i1 = self._simulation.add_vertex_to_scene(point1)
        i2 = self._simulation.add_vertex_to_scene(point2)
        i3 = self._simulation.add_vertex_to_scene(point3)
        self._simulation.add_triangle_to_scene(i1, i2, i3)

    def __getattr__(self, name):
        return getattr(self._simulation, name)


class ray(object):

    def __init__(self):
        self.cyray = cyAcousticRayTracer.cyRay()

    @property
    def source(self):
        s = Source()
        s.cysource = self.cyray.source
        return s

    @property
    def nevents(self):
        return self.cyray.nevents

    @property
    def ndiffractions(self):
        return self.cyray.ndiffractions

    @property
    def nreflexions(self):
        return self.cyray.nreflexions

    @property
    def length(self):
        return self.cyray.length

    @property
    def direction(self):
        return self.cyray.direction

    @property
    def events(self):
        l = list()
        for cyevent in self.cyray.events:
            e = Event()
            e.cyevent = cyevent
            l.append(e)
        return l


class Event(object):

    def __init__(self):
        self.cyevent = cyAcousticRayTracer.cyEvent()

    @property
    def type(self):
        return self.cyevent.type

    @property
    def position(self):
        return self.cyevent.position

    @property
    def incoming_direction(self):
        return self.cyevent.incoming_direction


class Source(object):
    """Source"""

    def __init__(self, x=0, y=0, z=0):
        """Create an empty source"""
        self.cysource = cyAcousticRayTracer.cySource(x, y, z)

    @property
    def position(self):
        return self.cysource.position

    @property
    def nrays_left(self):
        return self.cysource.nbRayLeft()

    def __getattr__(self, name):
        return getattr(self.cysource, name)


class Recepteur(object):
    """Receptor"""

    def __init__(self, x=0, y=0, z=0, r=0):
        """Create an empty receptor"""
        self.cyrecepteur = cyAcousticRayTracer.cyRecepteur(x, y, z, r)

    @property
    def position(self):
        return self.cyrecepteur.position

    def __getattr__(self, name):
        return getattr(self.cyrecepteur, name)


class Solver(object):
    """Solver"""

    def __init__(self):
        """Create a solver"""
        self.cysolver = cyAcousticRayTracer.cySolver()

    def __getattr__(self, name):
        return getattr(self.cysolver, name)
