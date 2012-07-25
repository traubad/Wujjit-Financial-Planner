/*
 * Nav.cpp
 *
 *  Created on: Jul 10, 2012
 *      Author: adamtraub
 */


#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WPushButton>
#include <Wt/WAnchor>
#include "Nav.h"

Nav::Nav(const Wt::WEnvironment& env) : Wt::WApplication(env){

	Wt::WPushButton *profileBtn;
	Wt::WPushButton *calculatorsBtn;
	Wt::WPushButton *resourcesBtn;


	profileBtn =
			new Wt::WPushButton("Profile");

	calculatorsBtn =
			new Wt::WPushButton("Calculators");

	resourcesBtn =
			new Wt::WPushButton("Resources");

	//TODO add clicked functionality

	root()->addWidget(profileBtn);
	root()->addWidget(calculatorsBtn);
	root()->addWidget(resourcesBtn);

}
