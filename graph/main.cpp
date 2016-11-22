#include "display.h"
#include "poi.h"
#include "lbs_user.h"
#include<cstdlib>
#include<ctime>
using namespace graph; //自己的命名空间

[STAThreadAttribute]

int main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	
	srand((unsigned)time(NULL));
	for (int i = 0; i < 10; i++) {
		int rand_val = rand() % 20;
		if (rand_val < 5) {
			cout << rand_val << endl;
		}
		else {
			cout << (rand_val-5) / 3 + 5 << endl;
		}
	}
		
	cout << endl;


	display ^dp = gcnew display();
	dp->Name = "display";
	Application::Run(dp);
	return 0;
}
