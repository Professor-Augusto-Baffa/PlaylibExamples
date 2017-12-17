#ifndef __SaveState__
#define __SaveState__

#include "Defines.h"
#include "Graphics.h"
using namespace PlayLib;

class SaveState
{
	private:
		bool file_good;

		bool temples [6];
		float hearts;
		bool heart_container [17];
		float mp;
		float max_mp;
		float max_stamina;
		int rupees;
		bool spell[3];
		bool easter_egg[7];
		int phase;

		int casting_Dins_Fire;
		int casting_Nayrus_Love;
		int casting_Farores_Wind;

	
	public:
	
		SaveState();
	
		//void save_file();
		void read_file();
	
		bool get_temple (int t);
		float get_hearts();
		int get_heart_containers();
		float get_mp();
		float get_max_mp();
		int get_rupees();
		float get_max_stamina();

		int get_phase();
		void tweak_phase();
	
		bool forest();
		bool fire();
		bool water();
		bool spirit();
		bool shadow();
		bool light();
		void tweak_temple (int i);

		void set_easter_egg (int i, bool b);
		bool get_easter_egg (int i);
		void tweak_easter_egg (int i);
		
		void alter_hearts (float h);
		void set_hearts (float h);

		void gain_heart_container (int h);
		void lose_heart_container (int h);

		void alter_rupees (int r);
		void alter_mp (float p);
		void alter_max_mp (float p);

		void print_report();
		void print_table();
		void graphics_print_table (Graphics g);

		int get_DF();
		int get_NL();
		int get_FW();

		void set_DF (int n);
		void set_NL (int n);
		void set_FW (int n);

		void alter_DF (int n);
		void alter_NL (int n);
		void alter_FW (int n);

		int first_heart_container (bool b);
};

#endif /* defined(__SaveState__) */