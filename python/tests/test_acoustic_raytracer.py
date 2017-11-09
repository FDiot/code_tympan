import unittest
import tempfile
from utils import TympanTC
from tympan.models.acousticraytracer import Simulation, Source, Recepteur, Solver


class TestAcousticRayTracer(TympanTC):
    """Class for AcousticRayTracer tests"""

    def setup_1source_1receptor(self, simulation, nb_rays_per_source=0, accelerator=2, pos_source=(5, 0, 0), pos_rec=(15, 0, 0)):
        config = simulation.configuration()
        config.setNbRaysPerSource(nb_rays_per_source)
        config.setAccelerator(accelerator)
        config.setKeepDebugRay(True)

        source = Source(*pos_source)
        simulation.add_source(source)

        pos_rec = pos_rec + (3,)
        receptor = Recepteur(*pos_rec)
        simulation.add_recepteur(receptor)

        solver = Solver()
        simulation.set_solver(solver)
        simulation.post_treatment_scene()
        simulation.set_accelerator()
        simulation.set_engine()

    def test_basic_changing_parameter(self):
        """Test: changing some parameters"""
        simulation = Simulation()
        simulation.configuration().setNbRaysPerSource(51)
        self.assertEqual(simulation.configuration().getNbRaysPerSource(), 51)

    def test_basic_checking_scene(self):
        """Test: checking the creation of a Scene"""
        simulation = Simulation()
        self.assertEqual(simulation._simulation.getSceneName(), "MyScene")

    def test_basic_change_source_sampler(self):
        """Test: adding a source"""
        simulation = Simulation()
        # Set UniformSphericSampler2 sampler
        simulation.configuration().Discretization = 2
        source = Source()
        simulation.add_source(source)
        # Check the number of sources
        self.assertEqual(simulation._simulation.getSourcesNumber(), 1)

    def test_basic_read_write_ply_files(self):
        """Test: import/export a Scene"""
        simulation = Simulation()
        ply_file = self.datapath('Scene.ply')
        simulation.import_scene(ply_file)
        # Check the number of vertices
        self.assertEqual(simulation._simulation.getSceneVerticesNumber(), 119)
        with tempfile.NamedTemporaryFile(delete=False) as file:
            simulation.export_scene(file.name)

    def test_complete_running_simulation_with_no_scene(self):
        simulation = Simulation()
        self.setup_1source_1receptor(simulation)
        simulation.launch_simulation()
        self.assertEqual(len(simulation.valid_rays), 1)
        self.assertEqual(len(simulation.lost_rays), 1)
        simulation.clean()

    def test_complete_running_simulation_with_triangles(self):
        simulation = Simulation()
        simulation.append_triangle_to_scene((-1, -1, 0), (1, 1, 1), (1, 1, -1))
        simulation.append_triangle_to_scene(
            (-1, 9, 0), (1, 11, -1), (1, 11, 1))
        simulation.append_triangle_to_scene(
            (9, 11, 0), (11, 9, -1), (11, 9, 1))
        self.setup_1source_1receptor(simulation, 0, 2, (-10, 0, 0), (10, 0, 0))
        simulation.launch_simulation()
        vrays = simulation.valid_rays
        lrays = simulation.lost_rays
        self.assertEqual(len(vrays), 1)
        self.assertEqual(len(lrays), 1)
        ray = vrays[0]
        self.assertEqual(ray.ndiffractions, 0)
        self.assertEqual(ray.nreflexions, 3)
        self.assertEqual(ray.direction, (0, -1, 0))
        self.assertEqual(ray.length, 40)
        events = ray.events
        self.assertEqual(len(events), 3)
        event = events[0]
        self.assertEqual(event.type, 'SPECULAR REFLEXION')
        self.assertEqual(event.position, (0, 0, 0))
        self.assertEqual(event.incoming_direction, (1, 0, 0))
        simulation.clean()

    def test_complete_running_simulation(self):
        """Test: complete running of a simulation"""
        # Create the RayTracer
        simulation = Simulation()
        # Read a Scene from a PLY file
        ply_file = self.datapath('Scene.ply')
        simulation.import_scene(self.datapath(ply_file))
        # Change some parameters
        simulation.configuration().Discretization = 3
        simulation.configuration().NbRaysPerSource = 11
        # Add somes sources
        source = Source(1, 1, 1)
        simulation.add_source(source)
        source = Source(2, 2, 2)
        simulation.add_source(source)
        # Add a receptor
        receptor = Recepteur(10, 10, 10, 0.4)
        simulation.add_recepteur(receptor)
        # Set a solver
        solver = Solver()
        simulation.set_solver(solver)
        # Set accelerators
        simulation.configuration().Accelerator = 2
        simulation.set_accelerator()
        # Set the engine
        simulation.set_engine()
        # Launch the simulation
        success = simulation.launch_simulation()
        # Check success
        self.assertEqual(success, True)
        # Clean the process
        simulation.clean()

if __name__ == '__main__':
    unittest.main()
