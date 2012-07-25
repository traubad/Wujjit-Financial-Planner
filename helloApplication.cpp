/*
 * helloApplication.cpp
 *
 *  Created on: Jul 10, 2012
 *      Author: adamtraub
 */

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WPushButton>
#include "helloApplication.h"

class Foo{public: Foo(){}};

class TestWidget : public Wt::WContainerWidget
{
	private:
		Foo* arg;
	public:
		TestWidget(Foo* _arg){
			arg = _arg;
		}

		~TestWidget(){
			delete arg;
		}
};


HelloApplication::HelloApplication(const Wt::WEnvironment& env)
:	Wt::WApplication(env)
{
	TestWidget* myWidget = new TestWidget(new Foo());
	root()->addWidget(myWidget);

	root()->addWidget(
			new Wt::WText("<h1>Hello, World!</h1>"));
	Wt::WPushButton *button = new Wt::WPushButton("Quit");
	button->clicked().connect(this, &Wt::WApplication::quit);
	root()->addWidget(button);
}
