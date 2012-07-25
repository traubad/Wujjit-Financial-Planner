/* Each cell is going to be a stacked widget, which will contain a
 * text_display (the value the user sees) and an input_field (a form field)
 * Stacked Widgets only show one item at a time.
 * The cell needs to flip-flop its state when clicked.
 */

#include <Wt/WStackedWidget>
#include <Wt/WText>
#include <Wt/WLineEdit>
#include "EditableCell.h"

EditableCell::EditableCell(std::string original_value, Wt::WContainerWidget* parent)
: Wt::WStackedWidget (parent), _stage_for_update(this)
{
    addWidget(new Wt::WText(original_value)); // set the text for the widget
    addWidget(new Wt::WLineEdit(original_value)); // get the input field ready

    /* When this is clicked, call the change state method */

    /*Look up the signals available in WInteractWidget, use the ones that are actually for focus/blur */
    this ->clicked().connect(this, &EditableCell::get_input);
    this ->escapePressed().connect(this, &EditableCell::cancel_update);
}

Wt::Signal<EditableCell*>& EditableCell::updatedByUser()
{
    /* This method does not actually stage anything for updating. It just connects the listener
     * to the signal. --TAT
     */
	std::cout<<"STAGED FOR UPDATE!"<<std::endl;
    return _stage_for_update;
}

void EditableCell::get_input()
{
    /* When the cell is clicked, we'll change its state to get input from the user
     * by passing the currently displayed value into the input field
     */
	_stage_for_update.emit(this);
	updatedByUser();//wrong?

	Wt::WText* text_display = (Wt::WText*) widget(0); // get the display object
	Wt::WLineEdit* input_field = (Wt::WLineEdit*) widget(1); // get the input object

	/*check how to extract a value from a WText widget  in the docs */
	std::string current_value = text_display->text().toUTF8();
	input_field ->setText(current_value);
	setCurrentWidget(input_field); //switch to the input field
	input_field ->setFocus();
}


void EditableCell::display_input()
{
     Wt::WLineEdit* input_field = (Wt::WLineEdit*) widget(1); // get the field object
     std::string new_value = input_field->text().toUTF8(); // convert the value to string
     Wt::WText* text_display = (Wt::WText*) widget(0); // get the display object
     /* Look up how to change text in an existing WText widget */
     text_display->setText(new_value); //set the value
     setCurrentWidget(text_display); //switch back to the text_display widget
}

void EditableCell::cancel_update()
{
	setCurrentWidget((Wt::WText*) widget(0));
}

