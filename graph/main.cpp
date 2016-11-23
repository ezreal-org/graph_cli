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

	display ^dp = gcnew display();
	dp->Name = "display";
	Application::Run(dp);
	return 0;
}
