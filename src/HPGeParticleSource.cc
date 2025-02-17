#include <G4PrimaryParticle.hh>
#include <G4Event.hh>
#include <G4TransportationManager.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4IonTable.hh>
#include <G4Ions.hh>
#include <G4TrackingManager.hh>
#include <G4Track.hh>
#include <Randomize.hh>
#include "TH1.h" 
#include "TF1.h" 

#include <sstream>
#include <cmath>

using std::stringstream;

using namespace CLHEP;

#include "HPGeParticleSource.hh"

HPGeParticleSource::HPGeParticleSource()
{
	m_iNumberOfParticlesToBeGenerated = 1;
	m_pParticleDefinition = 0;
	G4ThreeVector hZero(0., 0., 0.);

	m_hParticleMomentumDirection = G4ParticleMomentum(1., 0., 0.);
	m_dParticleEnergy = 1.0*MeV;
	m_hParticlePosition = hZero;
	m_dParticleTime = 0.0;
	m_hParticlePolarization = hZero;
	m_dParticleCharge = 0.0;

	m_hSourcePosType = "Volume";
	m_hShape = "NULL";
	m_dHalfz = 0.;
	m_dRadius = 0.;
	m_hCenterCoords = hZero;
	m_bConfine = false;
	m_hVolumeNames.clear();

	m_hAngDistType = "iso";
	m_dMinTheta = 0.;
	m_dMaxTheta = pi;
	m_dMinPhi = 0.;
	m_dMaxPhi = twopi;

	m_hEnergyDisType = "Mono";
	m_dMonoEnergy = 1*MeV;

	m_iVerbosityLevel = 0;

	m_pMessenger = new HPGeParticleSourceMessenger(this);
	m_pNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
	
	
	AmBe_n = new TH1D("AmBe_n_s", "ambe_s", 157, 0.0714 -.0696/2., 0.0714+(156.5*.0696));
	 //Energy spectrum, values in MeV
	double ambe_s[157] ={
	1.004E-01,  6.349E-02,  1.031E-01,  1.359E-01,  1.315E-01,  1.201E-01,  1.343E-01,  1.238E-01,  1.143E-01, 1.304E-01	, 
	1.163E-01, 1.127E-01,  8.092E-02, 1.000E-01, 9.570E-02, 9.349E-02, 9.126E-02, 8.227E-02, 9.614E-02, 8.432E-02, 
	9.490E-02, 9.740E-02, 8.272E-02, 8.414E-02, 6.596E-02, 8.635E-02, 9.263E-02, 1.213E-01, 9.154E-02, 9.792E-02, 
	1.201E-01, 1.246E-01, 1.094E-01, 1.010E-01, 1.261E-01, 9.619E-02, 1.329E-01, 1.067E-01, 1.167E-01, 1.565E-01, 
	1.539E-01, 1.779E-01, 1.907E-01, 2.034E-01, 2.004E-01, 2.042E-01, 1.657E-01, 1.861E-01, 1.519E-01, 1.585E-01, 
	1.667E-01, 1.481E-01, 1.460E-01, 1.451E-01, 1.370E-01, 1.481E-01, 1.499E-01, 1.520E-01, 1.239E-01, 1.291E-01, 
	1.476E-01, 1.563E-01, 1.153E-01, 1.294E-01, 1.410E-01, 1.430E-01, 1.444E-01, 1.619E-01, 1.440E-01, 1.705E-01, 
	1.703E-01, 1.749E-01, 1.519E-01, 1.301E-01, 1.410E-01, 1.346E-01, 1.153E-01, 1.547E-01, 1.131E-01, 9.905E-02, 
	1.127E-01, 1.101E-01, 1.046E-01, 1.053E-01, 6.399E-02, 6.723E-02, 7.881E-02, 8.328E-02, 1.023E-01, 1.058E-01, 
	8.405E-02, 1.002E-01, 9.540E-02, 8.703E-02, 7.800E-02, 9.114E-02, 8.293E-02, 7.526E-02, 7.438E-02, 6.851E-02, 
	7.815E-02, 4.823E-02, 9.437E-02, 8.208E-02, 7.958E-02, 9.560E-02, 8.861E-02, 8.009E-02, 9.201E-02, 8.710E-02, 
	9.187E-02, 1.104E-01, 6.282E-02, 9.033E-02, 8.109E-02, 5.874E-02, 5.157E-02, 5.224E-02, 5.835E-02, 4.950E-02, 
	4.855E-02, 3.236E-02, 2.553E-02, 2.174E-02, 2.779E-02, 2.529E-02, 2.111E-02, 1.353E-02, 1.703E-02, 1.568E-02, 
	1.747E-02, 2.092E-02, 8.151E-03, 1.651E-02, 3.178E-02, 2.201E-02, 2.573E-02, 2.779E-02, 4.220E-02, 2.313E-02, 
	4.142E-02, 2.734E-02, 3.135E-02, 3.358E-02, 2.641E-02, 3.052E-02, 1.736E-02, 1.170E-02, 2.562E-02, 7.964E-03, 
	6.034E-03, 1.625E-02, 6.155E-03, 2.072E-03, 4.184E-03, 0.000E+00, 0.000E+00	};
	for(int i=1; i<=157; i++) 
	   AmBe_n->SetBinContent(i+1,ambe_s[i]);


	hflat100 = new TH1D("hflat100","",100,0,100);
	for(int i=1; i<=100; i++)
	  hflat100->SetBinContent(i,1);
	      
	Pb210_Bremsstrahlung = new TH1D("Pb210_Brems_s","pb210_brems_s",1000,0.0,1.0); // values in /second MeV
    for (int i=0; i<1000; i++) {
       double x_val = double(i)*0.001+0.0005;
       double temp_val = 514.31138 + 779.0155*x_val
                       - 68.607204*pow(x_val,2.5)
                       - 1224.3796*pow(x_val,0.5);
       Pb210_Bremsstrahlung->SetBinContent(i+1,temp_val);
    }

    func_cosmic_neutron_flux = new TF1("func_cosmic_neutron_flux","1.006e-6*exp(-0.35*log(x[0])*log(x[0])+2.1451*log(x[0]))+1.011e-3*exp(-0.4106*log(x[0])*log(x[0])-0.667*log(x[0]))",0.1,10000.0); // unit MeV
	Flag_Use_Fluka_Input_File=kFALSE; 
}

HPGeParticleSource::~HPGeParticleSource()
{
	delete m_pMessenger;
        delete AmBe_n;
        delete hflat100;
        delete Pb210_Bremsstrahlung;
        delete func_cosmic_neutron_flux;
        m_hVolumeNames.clear();
}

void
HPGeParticleSource::ConfineSourceToVolume(G4String hVolumeList)
{
	stringstream hStream;
	hStream.str(hVolumeList);
	G4String hVolumeName;

	// store all the volume names
	while(!hStream.eof())
	{
		hStream >> hVolumeName;
		m_hVolumeNames.insert(hVolumeName);
	}

	// checks if the selected volumes exist and store all volumes that match
	G4PhysicalVolumeStore *PVStore = G4PhysicalVolumeStore::GetInstance();
	G4bool bFoundAll = true;

	set<G4String> hActualVolumeNames;
	for(set<G4String>::iterator pIt = m_hVolumeNames.begin(); pIt != m_hVolumeNames.end(); pIt++)
	{
		G4String hRequiredVolumeName = *pIt;
		G4bool bMatch = false;

		if(bMatch = (hRequiredVolumeName.last('*') != std::string::npos))
			hRequiredVolumeName = hRequiredVolumeName.strip(G4String::trailing, '*');

		G4bool bFoundOne = false;
		for(G4int iIndex = 0; iIndex < (G4int) PVStore->size(); iIndex++)
		{
			G4String hName = (*PVStore)[iIndex]->GetName();

			if((bMatch && (hName.substr(0, hRequiredVolumeName.size())) == hRequiredVolumeName) || hName == hRequiredVolumeName)
			{
				hActualVolumeNames.insert(hName);
				bFoundOne = true;
			}
		}

		bFoundAll = bFoundAll && bFoundOne;
	}

	if(bFoundAll)
	{
		m_hVolumeNames = hActualVolumeNames;
		m_bConfine = true;

		if(m_iVerbosityLevel >= 1)
			G4cout << "Source confined to volumes: " << hVolumeList << G4endl;

		if(m_iVerbosityLevel >= 2)
		{
			G4cout << "Volume list: " << G4endl;

			for(set<G4String>::iterator pIt = m_hVolumeNames.begin(); pIt != m_hVolumeNames.end(); pIt++)
				G4cout << *pIt << G4endl;
		}
	}
	else if(m_hVolumeNames.empty())
		m_bConfine = false;
	else
	{
		G4cout << " **** Error: One or more volumes do not exist **** " << G4endl;
		G4cout << " Ignoring confine condition" << G4endl;
		m_hVolumeNames.clear();
		m_bConfine = false;
	}
}

void
HPGeParticleSource::GeneratePointSource()
{
	// Generates Points given the point source.
	if(m_hSourcePosType == "Point")
		m_hParticlePosition = m_hCenterCoords;
	else if(m_iVerbosityLevel >= 1)
		G4cout << "Error SourcePosType is not set to Point" << G4endl;
}

void
HPGeParticleSource::GeneratePointsInVolume()
{
	G4ThreeVector RandPos;
	G4double x = 0., y = 0., z = 0.;

	if(m_hSourcePosType != "Volume" && m_iVerbosityLevel >= 1)
		G4cout << "Error SourcePosType not Volume" << G4endl;

	if(m_hShape == "Sphere")
	{
		x = m_dRadius * 2.;
		y = m_dRadius * 2.;
		z = m_dRadius * 2.;
		while(((x * x) + (y * y) + (z * z)) > (m_dRadius * m_dRadius))
		{
			x = G4UniformRand();
			y = G4UniformRand();
			z = G4UniformRand();

			x = (x * 2. * m_dRadius) - m_dRadius;
			y = (y * 2. * m_dRadius) - m_dRadius;
			z = (z * 2. * m_dRadius) - m_dRadius;
		}
	}

	else if(m_hShape == "Cylinder")
	{
		x = m_dRadius * 2.;
		y = m_dRadius * 2.;
		while(((x * x) + (y * y)) > (m_dRadius * m_dRadius))
		{
			x = G4UniformRand();
			y = G4UniformRand();
			z = G4UniformRand();
			x = (x * 2. * m_dRadius) - m_dRadius;
			y = (y * 2. * m_dRadius) - m_dRadius;
			z = (z * 2. * m_dHalfz) - m_dHalfz;
		}
	}

    else if(m_hShape == "Box")
    {

      x = 2*(G4UniformRand()-0.5)*m_dHalfx;
      y = 2*(G4UniformRand()-0.5)*m_dHalfy;
      z = 2*(G4UniformRand()-0.5)*m_dHalfz;
    }

	else
		G4cout << "Error: Volume Shape Does Not Exist" << G4endl;

	RandPos.setX(x);
	RandPos.setY(y);
	RandPos.setZ(z);
	m_hParticlePosition = m_hCenterCoords + RandPos;

}

/*
 following is to generate downwards comic neutron
 select a random point on the x-y surface at z=65cm, x from -50 to 50, y from -50 to 50cm
*/
 
void 
HPGeParticleSource::GenerateCosmicNeutron()
{
//---> vertex
// the BEGe detector center position is at x=-45.5mm, y 0.0mm
// now generate neutrons right on top of BEGe 10x10cm2
	G4ThreeVector RandPos;
	G4double x, y, z;
    z=65.0*cm;
    x=(-5.0+G4UniformRand()*10.0-4.55)*cm;
    y=(-5.0+G4UniformRand()*10.0)*cm;
	RandPos.setX(x);
	RandPos.setY(y);
	RandPos.setZ(z);
	m_hParticlePosition = RandPos;
}


void 
HPGeParticleSource::GenerateCosmicNeutronDirection()
{
//---> Momentum
    m_hParticleMomentumDirection.setX(0.0);
    m_hParticleMomentumDirection.setY(0.0);
    m_hParticleMomentumDirection.setZ(-1.0);
//---> kinetic Energy
//    m_dParticleEnergy = func_cosmic_neutron_flux->GetRandom()*MeV;
//    G4cout<<" particle energy "<<m_dParticleEnergy<<G4endl;
}

G4bool
HPGeParticleSource::IsSourceConfined()
{
	// Method to check point is within the volume specified
	if(m_bConfine == false)
		G4cout << "Error: Confine is false" << G4endl;
	G4ThreeVector null(0., 0., 0.);
	G4ThreeVector *ptr;

	ptr = &null;

	// Check m_hParticlePosition is within a volume in our list
	G4VPhysicalVolume *theVolume;

	theVolume = m_pNavigator->LocateGlobalPointAndSetup(m_hParticlePosition, ptr, true);
	G4String theVolName = theVolume->GetName();

	set<G4String>::iterator pIt;
	if((pIt = m_hVolumeNames.find(theVolName)) != m_hVolumeNames.end())
	{
		if(m_iVerbosityLevel >= 1)
			G4cout << "Particle is in volume " << *pIt << G4endl;
		return (true);
	}
	else
		return (false);
}

void
HPGeParticleSource::GenerateIsotropicFlux()
{
	G4double rndm, rndm2;
	G4double px, py, pz;

	G4double sintheta, sinphi, costheta, cosphi;

	rndm = G4UniformRand();
	costheta = std::cos(m_dMinTheta) - rndm * (std::cos(m_dMinTheta) - std::cos(m_dMaxTheta));
	sintheta = std::sqrt(1. - costheta * costheta);

	rndm2 = G4UniformRand();
	m_dPhi = m_dMinPhi + (m_dMaxPhi - m_dMinPhi) * rndm2;
	sinphi = std::sin(m_dPhi);
	cosphi = std::cos(m_dPhi);

	px = -sintheta * cosphi;
	py = -sintheta * sinphi;
	pz = -costheta;

	G4double ResMag = std::sqrt((px * px) + (py * py) + (pz * pz));

	px = px / ResMag;
	py = py / ResMag;
	pz = pz / ResMag;

	m_hParticleMomentumDirection.setX(px);
	m_hParticleMomentumDirection.setY(py);
	m_hParticleMomentumDirection.setZ(pz);

	// m_hParticleMomentumDirection now holds unit momentum vector.
	if(m_iVerbosityLevel >= 2)
		G4cout << "Generating isotropic vector: " <<
			m_hParticleMomentumDirection << G4endl;
}

void
HPGeParticleSource::GenerateMonoEnergetic()
{
	m_dParticleEnergy = m_dMonoEnergy;
}

//Get energy base on the AmBe neutron energy histogram
void
HPGeParticleSource::GenerateAmBe(){



	m_dParticleEnergy = AmBe_n->GetRandom() * MeV;
//	G4cout << "Particle energy -> " << m_dParticleEnergy/keV << G4endl;
}

void
HPGeParticleSource::GeneratePb210Bremsstrahlung(){
    m_dParticleEnergy = Pb210_Bremsstrahlung->GetRandom() * MeV;
}

void
HPGeParticleSource::SetParticleDefinition(G4ParticleDefinition * aParticleDefinition)
{
	m_pParticleDefinition = aParticleDefinition;
	m_dParticleCharge = m_pParticleDefinition->GetPDGCharge();
}

void
HPGeParticleSource::GeneratePrimaryVertex(G4Event * evt)
{
//---> use FLUKA input
    if (Flag_Use_Fluka_Input_File) {
  //--> check whether the input file is open
      if (!(fFlukaInputFile.is_open())) {
         G4cout << "Fluka File must be set!" << G4endl;
         return;
      }
      G4int nEntries=0;
      G4int nEvent=0;
      G4int fluka_code_of_simulated_muon_above_hill = 0;
      G4double energy_of_simulated_muon_above_hill = 0.0;
      G4double theta_of_simulated_muon_above_hill = 0.0;
      G4double time_of_individual_particle=0.0;
      G4double px, py ,pz , particle_position_x, particle_position_y, particle_position_z, tmp_x_position, tmp_y_position, tmp_px, tmp_py;
      G4int fluka_code  = 0;
      G4String Geant4_particleName;
   //--> read in 
     if (fFlukaInputFile.eof()) {
        fFlukaInputFile.close();
        G4cout<< "File over: not enough events!" << G4endl;
        return;
     }
     fFlukaInputFile >> nEvent  >> nEntries >> fluka_code_of_simulated_muon_above_hill >> energy_of_simulated_muon_above_hill >> theta_of_simulated_muon_above_hill;
     if (nEntries>100) G4cout<<" FLUKA number of vertexes to be generated "<<nEntries<<G4endl;
     for (G4int i=0;i<nEntries;i++) { 
		 //loop of the number of particles per event
    
      // set all variables to strange values so u can see if there is a mistake
      fluka_code = 0;
      Geant4_particleName ="";
      px = -1e6;
      py = -1e6;
      pz = -1e6;
      particle_position_x = -1e16;
      particle_position_y = -1e16;
      particle_position_z = -1e16;
      time_of_individual_particle = -1e16;

	  // tonis x is our y and his y is our x (and we have to change the sign for the y values)
      fFlukaInputFile >> fluka_code >> time_of_individual_particle  >> particle_position_y >> particle_position_x >> particle_position_z >> py >> px >> pz;
      particle_position_y = -particle_position_y;
      py = -py;


	  // this is new for run5 (added to adjust to the existing stuff from the previous runs) as the experiment was rotated by 90° but in the gdml file we did not rotate AND we move the sphere a bit to include the detectors nicely into the sphere

	  //this part is for the different sphere position
	  particle_position_x = particle_position_x + 9.9; // so we shift the input data by 9.9cm towards positive x-direction
      particle_position_z = particle_position_z + 11.5; // so we shift the input data by 11.5cm towards positive z-direction
	  
	  // this part is for the rotated room
	  tmp_x_position = particle_position_x;
      tmp_px = px;      
      tmp_y_position = particle_position_y;
      tmp_py = py;      
      particle_position_x = tmp_y_position;
      px = tmp_py;      
      particle_position_y = -tmp_x_position;
      py = -tmp_px;
      
      Geant4_particleName = ConvertFlukaCodeToGeant4ParticleName(fluka_code);// the Geant4 code is NOT the same as the PDG coding which we use for the simulation output !!!!
      if(!Geant4_particleName.empty()) // if it is empty we do not know the particle and so it will not be added to the starting vertices 
      {
        // fluka uses as standard: s, GeV and cm !!!!!!!!!!!!!!!!!!!! so this means that the input of toni has this units and therefore need here to be put in as this units
                // so here we have to really input seconds !!!! and GeV and cm as this comes directly from TONI and not from me !!!!!
      particle_position =  G4ThreeVector(particle_position_x,particle_position_y,particle_position_z); // now here its our position 
      G4PrimaryVertex* vertex = new G4PrimaryVertex (particle_position*cm, time_of_individual_particle*s); // 
      G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle(Geant4_particleName);
      G4PrimaryParticle* particle = new G4PrimaryParticle(particleDefinition,px*GeV,py*GeV,pz*GeV); // the Geant4 code is NOT the same as the PDG coding which we use for the simulation output !!!!
      //put the partcile which is here one of the entries of the event we look at into its vertex 
      vertex->SetPrimary(particle);
      evt->AddPrimaryVertex(vertex);
      } // end of the request in which we only simulated particles that we know 
      else // if it is empty we wanna display on the screen the fluka number of the particle so we can include it later
      {
        G4cout<< "Fluka code for particle we could not read in: " << fluka_code << G4endl;
      }
     }
    }
//--> NOT using FLUKA
    else {
	if(m_pParticleDefinition == 0)
	{
		G4cout << "No particle has been defined!" << G4endl;
		return;
	}

	// Position
	G4bool srcconf = false;
	G4int LoopCount = 0;

	while(srcconf == false)
	{
		if(m_hSourcePosType == "Point")
			GeneratePointSource();
		else if(m_hSourcePosType == "Volume")
			GeneratePointsInVolume();
		else if(m_hSourcePosType == "CosmicNeutron")
                        GenerateCosmicNeutron();
		else
		{
			G4cout << "Error: SourcePosType undefined" << G4endl;
			G4cout << "Generating point source" << G4endl;
			GeneratePointSource();
		}

		if(m_bConfine == true)
		{
			srcconf = IsSourceConfined();
			// if source in confined srcconf = true terminating the loop
			// if source isnt confined srcconf = false and loop continues
		}
		else if(m_bConfine == false)
			srcconf = true;		// terminate loop

		LoopCount++;
		if(LoopCount == 1000000)
		{
			G4cout << "*************************************" << G4endl;
			G4cout << "LoopCount = 1000000" << G4endl;
			G4cout << "Either the source distribution >> confinement" << G4endl;
			G4cout << "or any confining volume may not overlap with" << G4endl;
			G4cout << "the source distribution or any confining volumes" << G4endl;
			G4cout << "may not exist" << G4endl;
			G4cout << "If you have set confine then this will be ignored" << G4endl;
			G4cout << "for this event." << G4endl;
			G4cout << "*************************************" << G4endl;
			srcconf = true;		//Avoids an infinite loop
		}
	}

	// Angular stuff
	if(m_hAngDistType == "iso")
		GenerateIsotropicFlux();
	else if(m_hAngDistType == "direction")
		SetParticleMomentumDirection(m_hParticleMomentumDirection);
	else if(m_hAngDistType == "CosmicNeutronDirection")
        GenerateCosmicNeutronDirection();
	else
		G4cout << "Error: AngDistType has unusual value" << G4endl;
	// Energy stuff
	if(m_hEnergyDisType == "Mono")
		GenerateMonoEnergetic();
	else if(m_hEnergyDisType == "AmBe")
		GenerateAmBe();
	else if(m_hEnergyDisType == "Pb210Bremss")
		GeneratePb210Bremsstrahlung();
	else if(m_hEnergyDisType == "Flat100")
	  m_dParticleEnergy = hflat100->GetRandom() * keV;
	else if(m_hEnergyDisType == "CosmicNeutronEnergy")
       m_dParticleEnergy = func_cosmic_neutron_flux->GetRandom()*MeV;
	else
		G4cout << "Error: EnergyDisType has unusual value" << G4endl;

	// create a new vertex
	G4PrimaryVertex *vertex = new G4PrimaryVertex(m_hParticlePosition, m_dParticleTime);

	if(m_iVerbosityLevel >= 2)
		G4cout << "Creating primaries and assigning to vertex" << G4endl;
	// create new primaries and set them to the vertex
	G4double mass = m_pParticleDefinition->GetPDGMass();
	G4double energy = m_dParticleEnergy + mass;
	G4double pmom = std::sqrt(energy * energy - mass * mass);
	G4double px = pmom * m_hParticleMomentumDirection.x();
	G4double py = pmom * m_hParticleMomentumDirection.y();
	G4double pz = pmom * m_hParticleMomentumDirection.z();

	if(m_iVerbosityLevel >= 1)
	{
		G4cout << "Particle name: " << m_pParticleDefinition->GetParticleName() << G4endl;
		G4cout << "       Energy: " << m_dParticleEnergy << G4endl;
		G4cout << "     Position: " << m_hParticlePosition << G4endl;
		G4cout << "    Direction: " << m_hParticleMomentumDirection << G4endl;
		G4cout << " NumberOfParticlesToBeGenerated: " << m_iNumberOfParticlesToBeGenerated << G4endl;
	}

	for(G4int i = 0; i < m_iNumberOfParticlesToBeGenerated; i++)
	{
		G4PrimaryParticle *particle = new G4PrimaryParticle(m_pParticleDefinition, px, py, pz);
		particle->SetMass(mass);
		particle->SetCharge(m_dParticleCharge);
		particle->SetPolarization(m_hParticlePolarization.x(), m_hParticlePolarization.y(), m_hParticlePolarization.z());
		vertex->SetPrimary(particle);
	}
	evt->AddPrimaryVertex(vertex);
	if(m_iVerbosityLevel > 1)
		G4cout << " Primary Vetex generated " << G4endl;
    } // end of NON-Fluka case
}

void
HPGeParticleSource::GeneratePrimaryVertexFromTrack(G4Track *pTrack, G4Event *pEvent)
{
	G4double dPX = pTrack->GetMomentum().x();
	G4double dPY = pTrack->GetMomentum().y();
	G4double dPZ = pTrack->GetMomentum().z();

	G4PrimaryVertex *pVertex = new G4PrimaryVertex(pTrack->GetPosition(), m_dParticleTime);

	G4PrimaryParticle *pPrimary = new G4PrimaryParticle(pTrack->GetDefinition(), dPX, dPY, dPZ);
	pPrimary->SetMass(pTrack->GetDefinition()->GetPDGMass());
	pPrimary->SetCharge(pTrack->GetDefinition()->GetPDGCharge());

	pVertex->SetPrimary(pPrimary);

	pEvent->AddPrimaryVertex(pVertex);
}

void HPGeParticleSource::OpenFlukaInputFile()
{
  fFlukaInputFile.open(fFlukaInputFileName);
  if (!(fFlukaInputFile.is_open())) {//not open correctly
    G4cout << "Fluka File "<<fFlukaInputFileName<<" not valid!" << G4endl;
  }
  G4cout << "Our Input file with the filename " << fFlukaInputFileName << " seems to work" << G4endl;
}

void HPGeParticleSource::ChangeFlukaInputFileName(G4String newFileName)
{
   //Use_Fluka_Input_File=kTRUE;
   if (fFlukaInputFileName != newFileName) //check if the new file is equal to the other
    {
      if (fFlukaInputFile.is_open()) fFlukaInputFile.close(); //close the old file
      fFlukaInputFileName = newFileName;
      this->OpenFlukaInputFile(); //open the new one
    }
}

G4String HPGeParticleSource::ConvertFlukaCodeToGeant4ParticleName(G4int fluka_code)
{
  if (fluka_code == 1)
    return "proton";
  else if (fluka_code == 2)
    return "anti_proton";
  else if (fluka_code == 3)
    return "e-";
  else if (fluka_code == 4)
    return "e+";
  else if (fluka_code == 5)
    return "nu_e";
  else if (fluka_code == 6)
    return "anti_nu_e";
  else if (fluka_code == 7)
    return "gamma";
  else if (fluka_code == 8)
    return "neutron";
  else if (fluka_code == 9)
    return "anti_neutron";
  else if (fluka_code == 10)
    return "mu+";
  else if (fluka_code == 11)
    return "mu-";
  else if (fluka_code == 12)
    return "kaon0L";
  else if (fluka_code == 13)
    return "pi+";
  else if (fluka_code == 14)
    return "pi-";
  else if (fluka_code == 15)
    return "kaon+";
  else if (fluka_code == 16)
    return "kaon-";
  else if (fluka_code == 17)
    return "lambda";
  else if (fluka_code == 18)
    return "anti_lambda";
  else if (fluka_code == 19)
    return "kaon0S";
  else if (fluka_code == 20)
    return "sigma-";
  else if (fluka_code == 21)
    return "sigma+";
  else if (fluka_code == 22)
    return "sigma0";
  else if (fluka_code == 23)
    return "pi0";
  else if (fluka_code == 24)
    return "kaon0";
  else if (fluka_code == 25)
    return "anti_kaon0";
  else if (fluka_code == 27)
    return "nu_mu";
  else if (fluka_code == 28)
    return "anti_num_mu";
  else if (fluka_code == 31)
    return "anti_sigma-";
  else if (fluka_code == 32)
    return "anti_sigma0";
  else if (fluka_code == 33)
    return "anti_sigma+";
  else if (fluka_code == 34)
    return "xi0";
  else if (fluka_code == 35)
    return "anti_xi0";
  else if (fluka_code == 36)
    return "xi-";
  else if (fluka_code == 37)
    return "anti_xi-";
  else if (fluka_code == 38)
    return "omega-";
  else if (fluka_code == 39)
    return "anti_omega-";
  else if (fluka_code == 41)
    return "tau+";
  else if (fluka_code == 42)
    return "tau-";
  else if (fluka_code == 43)
    return "nu_tau";
  else if (fluka_code == 44)
    return "anti_nu_tau";
  else if (fluka_code == 45)
    return "D+";
  else if (fluka_code == 46)
    return "D-";
  else if (fluka_code == 47)
    return "D0";
  else if (fluka_code == 48)
    return "anti_D0";
  else if (fluka_code == 49)
    return "Ds+";
  else if (fluka_code == 50)
    return "Ds-";
  else if (fluka_code == 51)
    return "lambda_c+";
  else if (fluka_code == 52)
    return "xi_c+";
  else if (fluka_code == 53)
    return "xi_c0";
  else if (fluka_code == 56) // 54 and 55 are not in geant4 list
    return "omega_c0";
  else if (fluka_code == 57)
    return "anti_lambda_c+";
  else if (fluka_code == 58)
    return "anti_xi_c+";
  else if (fluka_code == 59)
    return "anti_xi_c0";
  else if (fluka_code == 62) // 60 61 ?
    return "anti_omega_c0";
  else if (fluka_code == -3)
    return "deuteron";
  else if (fluka_code == -4)
    return "triton";
  else if (fluka_code == -5)
    return "He3";
  else if (fluka_code == -6)
    return "alpha";
  else {
    G4cout << "We got no in my code defined particle for the code from fluka -> this particle will be neglected" << G4endl;
    return "";
  }
}
