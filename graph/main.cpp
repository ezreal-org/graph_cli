#include "display.h"
#include "poi.h"
#include "lbs_user.h"
#include<cstdlib>
#include<ctime>
using namespace graph; //�Լ��������ռ�

[STAThreadAttribute]

int main()
{
	freopen("lpp_algorithm\\out1.txt", "w", stdout);
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	display ^dp = gcnew display();
	dp->Name = "display";
	Application::Run(dp);

	return 0;
}
