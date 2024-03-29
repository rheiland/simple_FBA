/*
###############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the version #
# number, such as below:                                                      #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version x.y.z) [1].    #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# See VERSION.txt or call get_PhysiCell_version() to get the current version  #
#     x.y.z. Call display_citations() to get detailed information on all cite-#
#     able software used in your PhysiCell application.                       #
#                                                                             #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite BioFVM  #
#     as below:                                                               #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version x.y.z) [1],    #
# with BioFVM [2] to solve the transport equations.                           #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient para- #
#     llelized diffusive transport solver for 3-D biological simulations,     #
#     Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730  #
#                                                                             #
###############################################################################
#                                                                             #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)     #
#                                                                             #
# Copyright (c) 2015-2018, Paul Macklin and the PhysiCell Project             #
# All rights reserved.                                                        #
#                                                                             #
# Redistribution and use in source and binary forms, with or without          #
# modification, are permitted provided that the following conditions are met: #
#                                                                             #
# 1. Redistributions of source code must retain the above copyright notice,   #
# this list of conditions and the following disclaimer.                       #
#                                                                             #
# 2. Redistributions in binary form must reproduce the above copyright        #
# notice, this list of conditions and the following disclaimer in the         #
# documentation and/or other materials provided with the distribution.        #
#                                                                             #
# 3. Neither the name of the copyright holder nor the names of its            #
# contributors may be used to endorse or promote products derived from this   #
# software without specific prior written permission.                         #
#                                                                             #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" #
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   #
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  #
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   #
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         #
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        #
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    #
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     #
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)     #
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  #
# POSSIBILITY OF SUCH DAMAGE.                                                 #
#                                                                             #
###############################################################################
*/

#include "./custom.h"

// declare cell definitions here 

Cell_Definition motile_cell; 

void create_cell_types( void )
{
	// use the same random seed so that future experiments have the 
	// same initial histogram of oncoprotein, even if threading means 
	// that future division and other events are still not identical 
	// for all runs 
	
	SeedRandom( parameters.ints("random_seed") ); // or specify a seed here 

		initialize_default_cell_definition();
	cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment ); 
	
	// turn the default cycle model to live, 
	// so it's easier to turn off proliferation
	
	cell_defaults.phenotype.cycle.sync_to_cycle_model( live ); 
	
	// Make sure we're ready for 2D
	
	cell_defaults.functions.set_orientation = up_orientation; 
	cell_defaults.phenotype.geometry.polarity = 1.0; 
	cell_defaults.phenotype.motility.restrict_to_2D = true; 
	
	// turn off proliferation and death 
	
	int cycle_start_index = live.find_phase_index( PhysiCell_constants::live ); 
	int cycle_end_index = live.find_phase_index( PhysiCell_constants::live ); 
	
	int apoptosis_index = cell_defaults.phenotype.death.find_death_model_index( PhysiCell_constants::apoptosis_death_model ); 
	
	// cell_defaults.phenotype.cycle.data.transition_rate( cycle_start_index , cycle_end_index ) = 0.0; 
	cell_defaults.phenotype.death.rates[apoptosis_index] = 0.0; 
	
	// set uptake and secretion to zero 
	cell_defaults.phenotype.secretion.secretion_rates[0] = 0; 
	cell_defaults.phenotype.secretion.uptake_rates[0] = 0; 
	// cell_defaults.phenotype.secretion.uptake_rates[0] = 1; 
	cell_defaults.phenotype.secretion.saturation_densities[0] = 1; 

	// disable motility for now
	cell_defaults.phenotype.motility.is_motile = false; 
	// cell_defaults.phenotype.motility.is_motile = true; 
	// motile_cell.phenotype.motility.persistence_time = parameters.doubles( "motile_cell_persistence_time" ); // 15.0; 
	// cell_defaults.phenotype.motility.migration_speed = parameters.doubles( "motile_cell_migration_speed" ); // 0.25 micron/minute 
	// cell_defaults.phenotype.motility.migration_bias = 0.0;// completely random 
	// cell_defaults.phenotype.motility.migration_bias = 1.0;// completely biased 
	// cell_defaults.phenotype.motility.migration_bias_direction[0] = 1.0;
	
	// set the default cell type to no phenotype updates 
	cell_defaults.functions.update_phenotype = NULL; 

	
	// housekeeping 
	
	// initialize_default_cell_definition();
	// cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment ); 
	
	// // enable internalized substrate tracking 
	
	// // Name the default cell type 
	
	// cell_defaults.type = 0; 
	// cell_defaults.name = "tumor cell"; 
	
	// // set default cell cycle model 

	// cell_defaults.functions.cycle_model = flow_cytometry_separated_cycle_model; 
	
	// // set default_cell_functions; 
	
	// cell_defaults.functions.update_phenotype = update_cell_and_death_parameters_O2_based; 
	
	// // needed for a 2-D simulation: 
	
	// /* grab code from heterogeneity */ 
	
	// cell_defaults.functions.set_orientation = up_orientation; 
	// cell_defaults.phenotype.geometry.polarity = 1.0;
	// cell_defaults.phenotype.motility.restrict_to_2D = true; 
	
	// // make sure the defaults are self-consistent. 
	
	// cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment );
	// cell_defaults.phenotype.sync_to_functions( cell_defaults.functions ); 

	// // set the rate terms in the default phenotype 

	// // first find index for a few key variables. 
	// int apoptosis_model_index = cell_defaults.phenotype.death.find_death_model_index( "Apoptosis" );
	// int necrosis_model_index = cell_defaults.phenotype.death.find_death_model_index( "Necrosis" );
	// int oxygen_substrate_index = microenvironment.find_density_index( "oxygen" ); 

	// int G0G1_index = flow_cytometry_separated_cycle_model.find_phase_index( PhysiCell_constants::G0G1_phase );
	// int S_index = flow_cytometry_separated_cycle_model.find_phase_index( PhysiCell_constants::S_phase );
	
	// int G2_index = flow_cytometry_separated_cycle_model.find_phase_index( PhysiCell_constants::G2_phase ); 
	// int M_index = flow_cytometry_separated_cycle_model.find_phase_index( PhysiCell_constants::M_phase ); 

	// // initially no necrosis 
	// cell_defaults.phenotype.death.rates[necrosis_model_index] = 0.0; 
	
	// // set apoptosis rate 
	// cell_defaults.phenotype.death.rates[apoptosis_model_index] =  0.0025; 

	// // set a faster time scale for division 
	// cell_defaults.phenotype.cycle.data.transition_rate(G0G1_index,S_index) = 1.0 / 1.0; // 1 min wait time 
	// // let's speed up M phase 
	// cell_defaults.phenotype.cycle.data.transition_rate(M_index,G0G1_index) = 1.0 / 30.0; // 30 minutes M
	// // let's speed up G2 phase 
	// cell_defaults.phenotype.cycle.data.transition_rate(G2_index,M_index) = 1.0 / 60.0; // 60 minutes G2
	// // let's speed up S phase 
	// cell_defaults.phenotype.cycle.data.transition_rate(S_index,G2_index) = 1.0 / 90.0; // 90 minutes S
	
	// // set oxygen uptake / secretion parameters for the default cell type 
	// cell_defaults.phenotype.secretion.uptake_rates[oxygen_substrate_index] = 10; 
	// cell_defaults.phenotype.secretion.secretion_rates[oxygen_substrate_index] = 0; 
	// cell_defaults.phenotype.secretion.saturation_densities[oxygen_substrate_index] = 38; 

	// // next substrate is secreted 
	// cell_defaults.phenotype.secretion.uptake_rates[1] = 0; 
	// cell_defaults.phenotype.secretion.secretion_rates[1] = 10; 
	// cell_defaults.phenotype.secretion.saturation_densities[1] = 1; 

	// // next substrate is secreted and uptake 
	// cell_defaults.phenotype.secretion.uptake_rates[2] = 3; 
	// cell_defaults.phenotype.secretion.secretion_rates[2] = 10; 
	// cell_defaults.phenotype.secretion.saturation_densities[2] = 1; 
	

	// // Now, let's define another cell type. 
	// // It's best to just copy the default and modify it. 
	
	// // make this cell type randomly motile, less adhesive, greater survival, 
	// // and less proliferative 
	
	// motile_cell = cell_defaults; 
	// motile_cell.type = 1; 
	// motile_cell.name = "motile tumor cell"; 
	
	// // make sure the new cell type has its own reference phenotype
	
	// motile_cell.parameters.pReference_live_phenotype = &( motile_cell.phenotype ); 
	
	// // enable random motility 
	// motile_cell.phenotype.motility.is_motile = true; 
	// motile_cell.phenotype.motility.persistence_time = parameters.doubles( "motile_cell_persistence_time" ); // 15.0; 
	// motile_cell.phenotype.motility.migration_speed = parameters.doubles( "motile_cell_migration_speed" ); // 0.25 micron/minute 
	// motile_cell.phenotype.motility.migration_bias = 0.0;// completely random 
	
	// // Set cell-cell adhesion to 5% of other cells 
	// motile_cell.phenotype.mechanics.cell_cell_adhesion_strength *= parameters.doubles( "motile_cell_relative_adhesion" ); // 0.05; 
	
	// // Set apoptosis to zero 
	// motile_cell.phenotype.death.rates[apoptosis_model_index] = parameters.doubles( "motile_cell_apoptosis_rate" ); // 0.0; 
	
	// // Set proliferation to 10% of other cells. 
	// // Alter the transition rate from G0G1 state to S state
	// motile_cell.phenotype.cycle.data.transition_rate(G0G1_index,S_index) *= 
	// 	parameters.doubles( "motile_cell_relative_cycle_entry_rate" ); // 0.1; 
	
	return; 
}

void setup_microenvironment( void )
{
	// set domain parameters
	
/*
	// in the XML now 
	default_microenvironment_options.X_range = {-750, 750}; 
	default_microenvironment_options.Y_range = {-750, 750}; 
	// default_microenvironment_options.Z_range = {-750, 750}; 
*/
	if( default_microenvironment_options.simulate_2D == false )
	{
		std::cout << "WARNING: overriding from 3-D to 2-D" << std::endl; 
		default_microenvironment_options.simulate_2D = true; 
	}
	
	// gradients are needed for this example 
	
	default_microenvironment_options.calculate_gradients = true; 
	
	// add other substrates besides oxygen
	microenvironment.add_density( "glu", "dimensionless" ); 
	microenvironment.diffusion_coefficients[1] = 0; 
	microenvironment.decay_rates[1] = 0; 	
	
	microenvironment.add_density( "lac", "dimensionless" ); 
	microenvironment.diffusion_coefficients[2] = 0; 
	microenvironment.decay_rates[2] = 0; 	
	
	// let BioFVM use oxygen as the default 
	
	default_microenvironment_options.use_oxygen_as_first_field = true; 

	// set Dirichlet conditions 
	std::vector<double> bc_vector( 3 , 42  ); // 21% o2
	bc_vector[1] = 0.0; 
	bc_vector[2] = 1.0; 
	
	// default_microenvironment_options.outer_Dirichlet_conditions = true;
	default_microenvironment_options.outer_Dirichlet_conditions = false;
	default_microenvironment_options.Dirichlet_condition_vector[0] = 38; // physioxic conditions 
	default_microenvironment_options.Dirichlet_condition_vector[1] = 0; 
	default_microenvironment_options.Dirichlet_condition_vector[2] = 1; 
	
	default_microenvironment_options.Dirichlet_activation_vector[1] = false;  
	default_microenvironment_options.Dirichlet_activation_vector[2] = false;  
	
	// set initial conditions 
	default_microenvironment_options.initial_condition_vector = { 38.0 , 0.0, 1.0 }; 


	default_microenvironment_options.track_internalized_substrates_in_each_agent = true; 
			
	initialize_microenvironment(); 	

	microenvironment.decay_rates[0] = 0; 	 // no decay
	microenvironment.diffusion_coefficients[0] = 0; 	// no diffusion

	microenvironment.decay_rates[1] = 0; 	 // no decay
	microenvironment.diffusion_coefficients[1] = 0; 	// no diffusion

	double factor = 1.0;
	for( unsigned int n=0; n < microenvironment.number_of_voxels() ; n++ )
	{
		factor = 1.0 - ((n%20)/20.);
		bc_vector[0] = factor*38.;
		bc_vector[1] = 0.;
		bc_vector[2] = 1.;
		microenvironment(n) = bc_vector; 
	}	

	return; 
}	

void setup_microenvironment_OLD( void )
{
	// set domain parameters 
	
/* now this is in XML 
	default_microenvironment_options.X_range = {-1000, 1000}; 
	default_microenvironment_options.Y_range = {-1000, 1000}; 
	default_microenvironment_options.simulate_2D = true; 
*/
	// make sure to override and go back to 2D 
	if( default_microenvironment_options.simulate_2D == false )
	{
		std::cout << "Warning: overriding XML config option and setting to 2D!" << std::endl; 
		default_microenvironment_options.simulate_2D = true; 
	}
	
	// no gradients need for this example?
	// default_microenvironment_options.calculate_gradients = false; 
	// default_microenvironment_options.calculate_gradients = true; 
	
	// set Dirichlet conditions off?
	// default_microenvironment_options.outer_Dirichlet_conditions = true;
	default_microenvironment_options.outer_Dirichlet_conditions = false;

	// enable tracking (requires 1.5.0 or later)
	default_microenvironment_options.track_internalized_substrates_in_each_agent = true; 
	
	// use this for default Dirichlet conditions. 
	// std::vector<double> bc_vector( 1 , 160  ); // 21% o2
	std::vector<double> zero_vector( 1 , 0.0  ); 
	// std::vector<double> bc_vector( 1 , 3.42  ); // made up
	std::vector<double> bc_vector( 3 , 42  ); // 21% o2
	bc_vector[1] = 0.0; 
	bc_vector[2] = 1.0; 
	default_microenvironment_options.Dirichlet_condition_vector = bc_vector;
	// default_microenvironment_options.Dirichlet_condition_vector = zero_vector;
	
	// this is for initial conditions 
	// default_microenvironment_options.initial_condition_vector = {160.0, 0.5, 0.5}; 
	default_microenvironment_options.initial_condition_vector = {3.42}; 
	default_microenvironment_options.initial_condition_vector = {160.0}; 
	
	// initialize BioFVM 
	
	initialize_microenvironment(); 	
	
	// set the oxygen decay rate back to zero 
	microenvironment.decay_rates[0] = 0; 	 // no decay
	microenvironment.diffusion_coefficients[0] = 0; 	// no diffusion
		
	// now, let's set all the substrates to the bc_vector value 
	float factor = 1.0;
	for( unsigned int n=0; n < microenvironment.number_of_voxels() ; n++ )
	{
		factor = 1.0 - ((n%20)/20.);
		bc_vector[0] = factor*160.;
		microenvironment(n) = bc_vector; 
		/*
		if (!(n % 20))
			microenvironment(n) = bc_vector; 
		else
			microenvironment(n) = zero_vector; 
		*/
	}	
	
	return; 
}

void setup_tissue( void )
{
	// create some cells near boundary corners
	Cell* pC;
	double box_radius = 100.;  // rf. config file
	double xmin = -box_radius;
	// xmin = parameters.doubles("x_min");   // no, only the <user_parameters> are in parameters!
	// std::cout << "x_min=" << xmin << std::endl;  // --> 15 (the first user_parameter value!)
	double ymin = -box_radius;
	double xmax = box_radius;
	double ymax = box_radius;
	double delta = 4.0;
	
	pC = create_cell(); 
	pC->assign_position( -50., 50.0, 0.0 ); 

	pC = create_cell(); 
	pC->assign_position( 50., -50.0, 0.0 ); 
	// pC->flag_for_division();

	return; 
}
	
std::vector<std::string> my_coloring_function( Cell* pCell )
{
	// start with flow cytometry coloring 
	
	std::vector<std::string> output = false_cell_coloring_cytometry(pCell); 
	output[0] = "red"; 
	output[1] = "red"; 
	output[2] = "red"; 
		
	if( pCell->phenotype.death.dead == false && pCell->type == 1 )
	{
		 output[0] = "black"; 
		 output[2] = "black"; 
	}
	
	return output; 
}

// std::vector<double> integrate_total_substrates( void )
// {
// 	// start with 0 vector 
// 	std::vector<double> out( microenvironment.number_of_densities() , 0.0 ); 

// 	// integrate extracellular substrates 
// 	for( unsigned int n = 0; n < microenvironment.number_of_voxels() ; n++ )
// 	{
// 		// out = out + microenvironment(n) * dV(n) 
// 		axpy( &out , microenvironment.mesh.voxels[n].volume , microenvironment(n) ); 
// 	}

// 	// inte
// 	for( unsigned int n=0; n < (*all_cells).size(); n++ )
// 	{
// 		Cell* pC = (*all_cells)[n];
// 		out += pC->phenotype.molecular.internalized_substrates;
// 	}
	
// 	return out; 
// }
