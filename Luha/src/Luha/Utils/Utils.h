#pragma once

namespace Luha {

	enum class AppColorTheme
	{
		Dark = 1, Classic, Light
	};
	enum class PlotColorTheme
	{
		Auto = 1, Dark, Classic, Light,
	};

	enum class PlotColorMap {
		Deep = 0,      // a.k.a. seaborn deep             
		Dark = 1,      // a.k.a. matplotlib "Set1"        
		Pastel = 2,    // a.k.a. matplotlib "Pastel1"     
		Paired = 3,    // a.k.a. matplotlib "Paired"      
		Viridis = 4,   // a.k.a. matplotlib "viridis"     
		Plasma = 5,    // a.k.a. matplotlib "plasma"      
		Hot = 6,       // a.k.a. matplotlib/MATLAB "hot"  
		Cool = 7,      // a.k.a. matplotlib/MATLAB "cool" 
		Pink = 8,      // a.k.a. matplotlib/MATLAB "pink" 
		Jet = 9,       // a.k.a. MATLAB "jet"             
		Twilight = 10, // a.k.a. matplotlib "twilight"    
		RdBu = 11,     // red/blue, Color Brewer          
		BrBG = 12,     // brown/blue-green, Color Brewer  
		PiYG = 13,     // pink/yellow-green, Color Brewer 
		Spectral = 14, // color spectrum, Color Brewer    
		Greys = 15,    // white/black                     
	};

	// TODO load all fonts in font dir instead of hardcoding them
	enum class AppFont
	{
		Roboto = 1, OpenSans, Oswald, Montserrat
	};

}