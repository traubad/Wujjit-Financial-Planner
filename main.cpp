// HelloApplication.h
#include "Wujjit.h"

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
	return new Wujjit(env);
}

int main(int argc, char **argv)
{
	Wt::ApplicationCreator app(&createApplication);
	return Wt::WRun(argc, argv, app);
}
